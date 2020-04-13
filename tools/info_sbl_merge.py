import sys
import os
from intelhex import IntelHex
import struct

fh_i = open(sys.argv[1],'rb')
flash_base = int(sys.argv[2],16)
sbl_data = fh_i.read()

test_word0 = 0xa5a53c3c
test_word1 = 0x5a5ac3c3
cap_delay = 4
spi_clk = 0
cfg_info = 0xffffffff
image_crypt = 0xffffffff
sbl_code_start = flash_base+0x1000
sbl_code_length = len(sbl_data)

info_head = struct.pack('LLHHLLLL',test_word0,test_word1,cap_delay,spi_clk,cfg_info,image_crypt,sbl_code_start,sbl_code_length)

ih = IntelHex()
ih.puts(flash_base,info_head)
ih.puts(sbl_code_start,sbl_data)
ih.write_hex_file(sys.argv[3])

