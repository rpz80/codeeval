package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
	"time"
)

const (
	GOING = iota
	IN    = iota
	DONE  = iota
)

const (
	UP   = iota
	DOWN = iota
	NO   = iota
)

type action struct {
	code      uint8
	timeLeft  uint32
	where     string
	done      bool
	direction uint8
	timeSpent uint32
}

type agent struct {
	id        uint8
	actions   []action
	totalTime uint32
	startTime uint32
	serving   bool
}

func (a *agent) printActions() {
	for _, v := range a.actions {
		fmt.Printf("code: %v, time: %d, where: %s\n", v.code, v.timeLeft, v.where)
	}
}

func (a *agent) findCurAction() *action {
	for i := 0; i < len(a.actions); i++ {
		if !a.actions[i].done {
			return &a.actions[i]
		}
	}
	return nil
}

func (a *agent) prepareActions(acts []string) {
	acts = append(acts, "0100", "0")
	a.actions = make([]action, 0, 32)
	if acts[0][:2] != "01" {
		a.actions = append(a.actions, action{GOING, 10, "e1", false, NO, 0})
	} else {
		a.actions = append(a.actions, action{GOING, 10, acts[0], false, NO, 0})
	}
	i := 0
	// fmt.Println(acts)

for_:
	for {
		if i >= len(acts) {
			break
		}
		prev := &a.actions[len(a.actions)-1]
		cur := action{done: false, direction: NO}
		room := acts[i]

		if roomTime, err := strconv.ParseInt(acts[i+1], 10, 32); err != nil {
			fmt.Fprintln(os.Stderr, err)
			return
		} else {
			switch prev.code {
			case GOING:
				if prev.where[0] == 'e' {
					cur.code = IN
					cur.where = prev.where
					cur.done = false
					if dstFloor, err := strconv.ParseInt(room[:2], 10, 32); err != nil {
						fmt.Fprintln(os.Stderr, err, "Destination floor number parse failed")
						return
					} else {
						if curFloor, err := strconv.ParseInt(cur.where[1:], 10, 32); err != nil {
							fmt.Fprintln(os.Stderr, err, "Current floor number parse failed: ", cur.where)
							return
						} else {
							var floorDelta uint32
							if dstFloor > curFloor {
								floorDelta = uint32(dstFloor - curFloor)
								cur.direction = UP
							} else {
								floorDelta = uint32(curFloor - dstFloor)
								cur.direction = DOWN
							}
							cur.timeLeft = 10 * floorDelta
						}
					}
					a.actions = append(a.actions, cur)

					cur.code = GOING
					cur.where = room
					cur.timeLeft = 10
					a.actions = append(a.actions, cur)

					cur.code = IN
					cur.where = room
					cur.timeLeft = uint32(roomTime)
					a.actions = append(a.actions, cur)
				} else {
					cur.code = IN
					cur.where = prev.where
					cur.timeLeft = uint32(roomTime)
					a.actions = append(a.actions, cur)
				}
				break
			case IN:
				if curFloor, err := strconv.ParseInt(prev.where[:2], 10, 32); err != nil {
					fmt.Fprintln(os.Stderr, err)
					return
				} else {
					cur.code = GOING
					var nextFloor uint32
					nf, err := strconv.ParseInt(acts[i][:2], 10, 32)

					if err != nil {
						fmt.Fprintln(os.Stderr, err)
						return
					}
					nextFloor = uint32(nf)
					if uint32(curFloor) == nextFloor {
						cur.where = acts[i]
						cur.timeLeft = 10
						a.actions = append(a.actions, cur)
					} else {
						cur.where = "e" + strconv.Itoa(int(curFloor))
						cur.timeLeft = 10
						a.actions = append(a.actions, cur)
					}
					continue for_
				}
			}
		}
		i += 2
	}
}

func prepareData(s string, agents []agent) []agent {
	data := strings.Split(s, " ")
	if sTime, err := time.Parse("15:04:05", data[1]); err != nil {
		fmt.Fprintln(os.Stderr, err, "Time parse failed")
		return nil
	} else {
		intTime := sTime.Hour()*3600 + sTime.Minute()*60 + sTime.Second()
		a := agent{id: uint8([]byte(data[0])[0]), startTime: uint32(intTime), totalTime: 0}
		a.prepareActions(data[2:])
		// a.printActions()
		agents = append(agents, a)
	}
	return agents
}

func copyAgents(dst, src []agent) {
	for i := 0; i < len(src); i++ {
		dst[i].id = src[i].id
		dst[i].totalTime = src[i].totalTime
		dst[i].startTime = src[i].startTime
		dst[i].serving = src[i].serving
		dst[i].actions = make([]action, len(src[i].actions))
		copy(dst[i].actions, src[i].actions)
	}
}

func calcTime(agents []agent) {
	var done bool
	for i := 3600 * 9; ; i += 5 {
		done = true
		agentsCopy := make([]agent, len(agents))
		copyAgents(agentsCopy, agents)

		for j := 0; j < len(agents); j++ {
			if agents[j].startTime <= uint32(i) {
				curAction := agents[j].findCurAction()
				curActionCopy := agentsCopy[j].findCurAction()
				// fmt.Printf("%s[ %02d ]: %-4v  ", string(agents[j].id), agents[j].totalTime, curAction)
				if curAction != nil {
					done = false
					if curAction.timeLeft != 0 {
						if curAction.code == GOING || (curAction.code == IN && curAction.where == "0100") {
							curActionCopy.timeLeft -= 5
							agentsCopy[j].totalTime += 5
						} else {
							if !agents[j].serving {
								isTop := true

							innerFor_:
								for k := 0; k < len(agents); k++ {
									if k != j {
										kCurAction := agents[k].findCurAction()
										// fmt.Println("\n --- ", string(agents[j].id), string(agents[k].id), curAction, kCurAction, agents[j].serving, agents[k].serving)

										if agents[k].serving && curAction.where[0] != 'e' && curAction.where == kCurAction.where {
											isTop = false
											break innerFor_
										} else {
											if curAction.where[0] == 'e' {
												if kCurAction != nil {
													// fmt.Println("\n --- ", string(agents[j].id), string(agents[k].id), curAction, kCurAction)
													if kCurAction.code == IN && kCurAction.where == curAction.where &&
														kCurAction.direction == curAction.direction && kCurAction.timeSpent == 0 {
														if agents[k].id < agents[j].id {
															// fmt.Println(string(agents[j].id), string(agents[k].id))
															isTop = false
															break innerFor_
														}
													}
												}
											} else {
												if kCurAction != nil {
													if kCurAction.code == IN && kCurAction.where == curAction.where {
														if agents[k].id < agents[j].id {
															isTop = false
															break innerFor_
														}
													}
												}
											}
										}
									}
								}
								if isTop {
									agentsCopy[j].serving = true
									curActionCopy.timeLeft -= 5
									curActionCopy.timeSpent += 5
									agentsCopy[j].totalTime += 5
								} else {
									// fmt.Println("isTop == false")
									agentsCopy[j].totalTime += 5
								}
							} else {
								curActionCopy.timeLeft -= 5
								agentsCopy[j].totalTime += 5
								curActionCopy.timeSpent += 5
							}
						}
					}
					if curActionCopy.timeLeft == 0 {
						curActionCopy.done = true
						agentsCopy[j].serving = false
					}
					// fmt.Println(string(agents[j].id), ": ", curAction.where)
				}
			}
		}
		copyAgents(agents, agentsCopy)
		// fmt.Println("")
		if done {
			break
		}
	}

	for i := 0; i < len(agents); i++ {
		sh, sm, ss := durToTime(agents[i].startTime)
		eh, em, es := durToTime(agents[i].startTime + agents[i].totalTime)
		fmt.Printf("%02d:%02d:%02d %02d:%02d:%02d\n", sh, sm, ss, eh, em, es)
	}
}

func durToTime(d uint32) (uint32, uint32, uint32) {
	h := d / 3600
	m := (d - h*3600) / 60
	s := d - h*3600 - m*60
	return h, m, s
}

func main() {
	start := time.Now()
	if len(os.Args) != 2 {
		fmt.Fprintf(os.Stderr, "Usage: vhq <file-name>")
		return
	}
	if f, err := os.Open(os.Args[1]); err != nil {
		fmt.Fprintln(os.Stderr, err)
		return
	} else {
		defer f.Close()

		agents := make([]agent, 0, 32)

		sc := bufio.NewScanner(f)
		for sc.Scan() {
			agents = prepareData(sc.Text(), agents)
		}
		calcTime(agents)
	}
	end := time.Now()
	fmt.Println("Elapsed: ", end.Sub(start))
}
