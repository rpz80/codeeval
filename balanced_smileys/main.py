import sys
import re

emo_re = re.compile(r':[\(\)]')

def check_parensis(s):
  lcount = 0
  for c in s:
    if c == '(':
      lcount += 1
    if c == ')':
      if lcount <= 0:
        return False
      else:
        lcount -= 1
  if lcount != 0:
    return False
  return True

def rec_check(s, flag):
  if flag[0]:
    return
  if check_parensis(s):
    flag[0] = True
    return
  else:
    for m in emo_re.finditer(s):
      s1 = s[:m.start()] + '1' + s[m.start()+2:]
      rec_check(s1, flag)

def main():
  with open(sys.argv[1]) as f:
    for line in f:
      flag = [False]
      rec_check(line, flag)
      if flag[0]:
        print('YES')
      else:
        print('NO')
        
if __name__ == '__main__':
  main()