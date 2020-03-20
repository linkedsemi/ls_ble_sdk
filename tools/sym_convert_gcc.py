import re
import sys
import os
fh_i = open(sys.argv[1],'r')
fh_o = open(sys.argv[2],'w')
for line in fh_i:
    match = re.match(r'(\w{8})\s\w\s(\w+)',line)
    addr_str = match.group(1)
    sym_name = match.group(2)
    fh_o.write(sym_name+' = 0x'+ addr_str + ';\n')

fh_i.close()
fh_o.close()

