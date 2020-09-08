import os
SetOption('warn', ['no-duplicate-environment'] + GetOption('warn'))

tool = ARGUMENTS.get('toolchain','arm-gcc')
toolpath = ['tools']
VariantDir('build', '.',duplicate=0)
env = Environment(ENV = os.environ,tools=[tool],toolpath=toolpath)
print(env['TOOLS'])

env['IC'] = ARGUMENTS.get('ic','le501x')
if env['IC'] == 'le501x':
    env['CPU'] = 'cortex-m0'
else:
    env['CPU'] = 'cortex-m3'

if 'mdk' in env['TOOLS']:
    env['COMPILER'] = 'armcc'
    
else:
    env['COMPILER'] = 'gnu'
    env['CFLAGS'] = '-O2 -Os -mabi=aapcs -mthumb -mcpu=$CPU -std=c11 -g -g3 -ffunction-sections -fdata-sections -fstrict-volatile-bitfields -fno-common -mno-unaligned-access -fno-optimize-sibling-calls -Wall'
    env['ASFLAGS'] = ' -g'
    env['LINKFLAGS'] = '-O2 -Os -mabi=aapcs -mthumb -mcpu=$CPU -std=c11 -g -g3 -specs=nano.specs -specs=nosys.specs -T ${LINKSCRIPT} -Wl,-Map=${TARGET.base}.map -Wl,--cref'
    env['GC_OPTION'] = ' -Wl,--gc-sections '
env['CPPPATH'] = ['#inc','#inc/cmsis','#inc/prf']
dev_env = env.Clone()

if not 'mdk' in env['TOOLS']:
    stack = SConscript('build/stack/SConscript',exports=['env'],must_exist=False)
else:
    stack = None

if env['IC']!= 'le501x':
    target_lib = stack
    target_fw = stack
else:
    if stack is None:
        target_fw = File('#dev/soc/arm_cm/le501x/bin/fw.hex')
        target_lib = File('#dev/soc/arm_cm/le501x/bin/libfw.o')
    else:
        target_fw = stack[0]
        target_lib = stack[1]


SConscript('build/dev/SConscript',exports=['dev_env','target_fw','target_lib'])