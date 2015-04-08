import re
import sys
import colorsys

HSLRe = re.compile("HSL\\((\\d+),(\\d+),(\\d+)\\)")
HSVRe = re.compile("HSV\\((\\d+),(\\d+),(\\d+)\\)")
CMYKRe = re.compile("\\((\\d\\.\\d+),(\\d\\.\\d+),(\\d\\.\\d+),(\\d\\.\\d+)\\)")
HEXRe = re.compile("#(.+)")

def from_cmyk(c):
  c, m, y, k = float(c[0]), float(c[1]), float(c[2]), float(c[3])
  r = 255 * (1-c) * (1-k)
  g = 255 * (1-m) * (1-k)
  b = 255 * (1-y) * (1-k)
  return round(r), round(g), round(b)

def from_hex(h):
  h = h[0]
  return round(int(h[:2], 16)), int(h[2:4], 16), int(h[4:], 16)

def main():
  with open(sys.argv[1]) as f:
    for line in f:
      m1, m2, m3, m4 = HSLRe.search(line), HSVRe.search(line), \
                       CMYKRe.search(line),  HEXRe.search(line) 
      if m1:   
        r, g, b = map(lambda x : round(x*255), 
                      colorsys.hls_to_rgb(float(m1.groups()[0])/360, 
                                          float(m1.groups()[2])/100, 
                                          float(m1.groups()[1])/100))
      elif m2:
        r, g, b = map(lambda x : round(x*255), 
                      colorsys.hsv_to_rgb(float(m2.groups()[0])/360, 
                                          float(m2.groups()[1])/100, 
                                          float(m2.groups()[2])/100))
      elif m3:
        r, g, b = from_cmyk(m3.groups())
      elif m4:
        r, g, b = from_hex(m4.groups())
      print('RGB({},{},{})'.format(r, g, b))

if __name__ == '__main__':
  main()