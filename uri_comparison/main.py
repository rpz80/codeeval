import sys
import re

uri_re = re.compile(r'(\S+)://([^/]+)/*(\S+)*')

def process(s):
  left, right = s.strip().split(';')
  m1, m2 = uri_re.search(left), uri_re.search(right)

  # scheme
  # print(m1.group(1), m1.group(2), m1.group(3))

  if m1.group(1).upper() != m2.group(1).upper():
    print('False')
    return

  # base
  base1, base2 = m1.group(2), m2.group(2)
  
  if ':' in base1:
    host1, port1 = m1.group(2).split(':')
  else:
    host1, port1 = base1, None
  if ':' in base2:  
    host2, port2 = m2.group(2).split(':')
  else:
   host2, port2 = base2, None
  
  if host1.upper() != host2.upper():
    print('False')
    return

  if port1:
    if port2:
      if port1 != port2:
        print('False')
        return
    else:
      if port1 != '80':
        print('False')
        return
  else:
    if port2:
      if port2 != '80':
        print('False')
        return

  #uri
  if m1.group(3) is None and m2.group(3) is None:
    print('True')
    return

  if (m1.group(3) is None and m2.group(3) is not None) or \
     (m2.group(3) is None and m1.group(3) is not None):
    print('False')
    return

  uri1, uri2 = url_decode(m1.group(3)), url_decode(m2.group(3))
  # print(uri1, uri2)
  if uri1 != uri2:
    print('False')
    return

  print('True')


def url_decode(s):
  spos = 0
  
  while 1:
    pos = s.find('%', spos)
    if pos == -1:
      return s
    if s[pos+1].isalpha() or s[pos+1].isdigit():
      s = s[:pos] + chr(int(s[pos+1:pos+3], 16)) + s[pos+3:]
    else:
      spos = pos+1
      continue

def main():
  with open(sys.argv[1]) as f:
    for line in f:
      process(line)

if __name__ == '__main__':
  main()