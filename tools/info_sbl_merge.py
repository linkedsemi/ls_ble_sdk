import sys
import os
from intelhex import IntelHex
import struct

fh_i = open(sys.argv[1],'rb')
ic = sys.argv[2]
sbl_data = fh_i.read()
test_word0 = 0xa5a53c3c
test_word1 = 0x5a5ac3c3
if ic == 'le501x':
    flash_base = 0x18000000
    cap_delay = 2
    spi_clk = 0
    cfg_info = 0xffffffff
    image_crypt = 0xffffffff
    sbl_code_start = flash_base+0x1000
    sbl_code_length = len(sbl_data)
    info_head = struct.pack('LLHHLLLL',test_word0,test_word1,cap_delay,spi_clk,cfg_info,image_crypt,sbl_code_start,sbl_code_length)
else:
    flash_base = 0x00800000
    code_offset = 0x1000
    sbl_code_length = len(sbl_data)
    sbl_code_start = flash_base+code_offset
    sbl_code_exec_addr = 0x400100
    feature_mask = 0xffffffff
    info_head = struct.pack('LLLLLL',test_word0,test_word1,code_offset,sbl_code_length,sbl_code_exec_addr,feature_mask)
    

mac_addr_base = flash_base + 0x20
mac_addr = bytes([0xff,0xff,0xff,0xff,0xff,0xff]) # ff:ff:ff:ff:ff:ff is not a valid address

ih = IntelHex()
ih.puts(flash_base,info_head)
ih.puts(mac_addr_base,mac_addr)
ih.puts(sbl_code_start,sbl_data)
ih.write_hex_file(sys.argv[3])

