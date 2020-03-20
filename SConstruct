import os

tools = ['arm-gcc']
#tools.append('armcc')
toolpath = ['tools']
VariantDir('build', '.',duplicate=0)
env = Environment(ENV = os.environ,tools=tools,toolpath=toolpath)
if 'armcc' in tools:
    env['COMPILER'] = 'armcc'
    
else:
    env['COMPILER'] = 'gnu'
    env['CFLAGS'] = '-O2 -Os -mabi=aapcs -mthumb -mcpu=cortex-m0 -std=c11 -g -ffunction-sections -fdata-sections -fstrict-volatile-bitfields'
    env['ASFLAGS'] = ' -g'
    env['LINKFLAGS'] = '-O2 -Os -mabi=aapcs -mthumb -mcpu=cortex-m0 -std=c11 -g -specs=nano.specs -specs=nosys.specs -T ${LINKSCRIPT} -Wl,-Map=${TARGET.base}.map -Wl,--cref'
    env['GC_OPTION'] = ' -Wl,--gc-sections '
env['CPPPATH'] = ['#inc','#inc/cmsis','#inc/prf']
dev_env = env.Clone()
stack = SConscript('build/stack/SConscript',exports=['env'],must_exist=False)

if stack is None:
    target_fw = '#bin/fw.hex'
    target_lib = '#bin/libfw_export.strip.a'
else:
    target_fw = stack[0]
    target_lib = stack[1]


SConscript('build/dev/SConscript',exports=['dev_env','target_fw','target_lib'])