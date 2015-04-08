import sys, math

def get_center(s):
  x = float(s[s.find('(') + 1 :s.find(',')])
  y = float(s[s.find(',') + 1 :s.find(';') - 1].strip())
  return x, y

def get_radius(s):
  r_pos = s.find('Radius:') + len('Radius') + 1
  s = s[r_pos:].strip()
  r_end_pos = s.find(';')
  return float(s[:r_end_pos].strip())
 
def get_point(s):
  r_pos = s.find('Point:') + len('Point:') + 1
  s = s[r_pos:].strip()
  x = float(s[s.find('(') + 1 : s.find(',')])
  y = float(s[s.find(',') + 1 :s.find(')')].strip())
  return (x,y)

def calc_dist(p1, p2):
  return math.sqrt(abs(p2[0] - p1[0])**2 + abs(p2[1] - p1[1])**2)

if __name__ == '__main__':
  with open(sys.argv[1]) as f:
    for line in f:
      if(get_radius(line) > calc_dist(get_center(line), get_point(line))):
        print('true')
      else:
        print('false')
      