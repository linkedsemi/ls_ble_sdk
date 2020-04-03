import re
import sys
import os

sweep_list = [
    'memset',
    'memcpy',
]

strip_list = [
    '__aeabi_idiv'            ,
    '__aeabi_idivmod'         ,
    '__aeabi_uidiv'           ,
    '__aeabi_uidivmod'        ,
    '__clzsi2'                ,
    '__gnu_thumb1_case_shi'   ,
    '__gnu_thumb1_case_sqi'   ,
    '__gnu_thumb1_case_uhi'   ,
    '__gnu_thumb1_case_uqi'   ,
    '__stack_bss_end__'       ,
    '__stack_bss_start__'     ,
    '__stack_data_end__'      ,
    '__stack_data_lma__'      ,
    '__stack_data_start__'    ,
    'main_task'               ,
    'max_activity_num'        ,
    'max_profile_num'         ,
    'max_ral_num'             ,
    'max_user_task_num'       ,
    'memcmp'                  ,
    'memcpy'                  ,
    'memmove'                 ,
    'memset'                  ,
    'one_bits'                ,
    'rand_fn'                 ,
    'rand_init_fn'            ,
]

raw_file = sys.argv[1]
fh_i = open(raw_file,'r')
filename = os.path.basename(raw_file)
out_folder = sys.argv[2]
fh_armcc = open(os.path.join(out_folder,filename+'.armcc'),'w')
fh_gnu = open(os.path.join(out_folder,filename+'.gnu'),'w')
fh_strip = open(os.path.join(out_folder,filename+'.strip'),'w')
for line in fh_i:
    match = re.match(r'(\w{8})\s\w\s(\w+)',line)
    addr_str = match.group(1)
    sym_name = match.group(2)
    if  sym_name  not in sweep_list:
        fh_armcc.write(line)
        fh_gnu.write(sym_name+' = 0x'+ addr_str + ';\n')
    if sym_name not in strip_list:
        fh_strip.write(sym_name + '\n')
fh_i.close()
fh_armcc.close()
fh_gnu.close()

