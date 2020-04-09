import re
import sys
import os

sweep_list = [
    'memset',
    'memcpy',
]
raw_file = sys.argv[1]
fh_i = open(raw_file,'r')
filename = os.path.basename(raw_file)
out_folder = sys.argv[2]
fh_armcc = open(os.path.join(out_folder,filename+'.armcc'),'w')
fh_gnu = open(os.path.join(out_folder,filename+'.gnu'),'w')
for line in fh_i:
    match = re.match(r'(\w{8})\s\w\s(\w+)',line)
    addr_str = match.group(1)
    sym_name = match.group(2)
    if  sym_name  not in sweep_list:
        fh_armcc.write(line)
        fh_gnu.write(sym_name+' = 0x'+ addr_str + ';\n')

fh_i.close()
fh_armcc.close()
fh_gnu.close()

