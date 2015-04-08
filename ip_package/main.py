import sys
import time

def ip_to_hex(s):
  f1, f2, f3, f4 = s.split('.')
  return (int(f1) << 24) | \
         (int(f2) << 16) | \
         (int(f3) << 8)  | \
          int(f4)
 
def hex_to_ip(n):
  f1 = (n >> 24) & 255
  f2 = (n >> 16) & 255
  f3 = (n >> 8) & 255
  f4 = n & 255
  return '{}.{}.{}.{}'.format(f1, f2, f3, f4)
  
def add_carry(a, b):
  c = a + b
  return (c & 0xffff) + (c >> 16)
    
def parse_ip(p, new_s, new_t):
  version = (int(p[0], 16) >> 4) & 0xf
  ihl = int(p[0], 16) & 0xf
  
  dscp = (int(p[1], 16) >> 2) & 63
  ecn = int(p[1], 16) & 3
  
  total_len = int(''.join(p[2:4]), 16)
  ident = int(''.join(p[4:6]), 16)
  
  byte6 = int(p[6], 16)
  flag0 = (byte6 >> 7) & 1;
  flag1 = (byte6 >> 6) & 1;
  flag2 = (byte6 >> 5) & 1;
  
  fragment_offset = byte6 & 31;
  ttl = int(p[7], 16)
  proto = int(p[8], 16)
 
  cks = int(''.join(p[9:11]), 16)
  
  source = int(''.join(p[11:15]), 16)
  target = int(''.join(p[15:19]), 16)

  # print(hex(ip_to_hex(new_s) >> 16))
  # print(hex(ip_to_hex(new_s) & 0xffff))
  
  new_s = ip_to_hex(new_s)
  new_t = ip_to_hex(new_t)
  
  # print('version : {0}\n' \
        # 'ihl     : {1}\n' \
        # 'dscp    : {2}\n' \
        # 'ecn     : {3}\n' \
        # 'len     : {4}\n' \
        # 'ident   : {5}\n' \
        # 'flag 0  : {6}\n' \
        # 'flag 1  : {7}\n' \
        # 'flag 2  : {8}\n' \
        # 'foffset : {9}\n' \
        # 'ttl     : {10}\n' \
        # 'proto   : {11}\n' \
        # 'chksum  : {12}\n' \
        # 'source  : {13} ({15})\n' \
        # 'target  : {14} ({16})\n' \
        # .format(version, ihl, dscp, ecn, total_len,
                # ident, flag0, flag1, flag2, fragment_offset,
                # ttl, proto, cks, source, target, hex_to_ip(source), 
                # hex_to_ip(target)))
  
  new_check_sum = 0
  
  new_check_sum = add_carry(new_check_sum, int(''.join(p[0:2]), 16))
  new_check_sum = add_carry(new_check_sum, int(''.join(p[2:4]), 16))
  new_check_sum = add_carry(new_check_sum, int(''.join(p[4:6]), 16))
  new_check_sum = add_carry(new_check_sum, int(''.join(p[6:8]), 16))
  new_check_sum = add_carry(new_check_sum, int(''.join(p[8:10]), 16))
  new_check_sum = add_carry(new_check_sum, new_s >> 16)
  new_check_sum = add_carry(new_check_sum, new_s & 0xffff)
  new_check_sum = add_carry(new_check_sum, new_t >> 16)
  new_check_sum = add_carry(new_check_sum, new_t & 0xffff)

  new_check_sum = ~new_check_sum & 0xffff
  
  # print('new checksum = {}'.format(hex(new_check_sum)))
  
  print(' '.join(p[:10]), end='')
  print(' {0} {1} '.format(format(new_check_sum >> 8, '02x'), format(new_check_sum & 0xff, '02x')), end='')
  print('{0} {1} {2} {3} '.format(format(new_s >> 24, '02x'), format((new_s >> 16) & 0xff, '02x'), 
                                  format((new_s >> 8) & 0xff, '02x'), format(new_s& 0xff, '02x')), end='')
  print('{0} {1} {2} {3}'.format(format(new_t >> 24, '02x'), format((new_t >> 16) & 0xff, '02x'), 
                                 format((new_t >> 8) & 0xff, '02x'), format(new_t& 0xff, '02x')))
  
def main():
  with open(sys.argv[1]) as f:
    for line in f:
      cont = line.strip().split(' ')
      source_ip, dest_ip, package = cont[0], cont[1], cont[2:]
      parse_ip(package, source_ip, dest_ip)
  
if __name__ == '__main__':
  start = time.time()
  main()
  print('Elapsed: {0}'.format(time.time() - start))