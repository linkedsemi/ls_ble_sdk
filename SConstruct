import os
SetOption('warn', ['no-duplicate-environment'] + GetOption('warn'))
VariantDir('build', '.',duplicate=0)
plf ={
    'le501x': ('arm_cm','cortex-m0'),
    'sagi': ('arm_cm','cortex-m3'),
    'taurus': ('rv32','e902'),
}
ic = ARGUMENTS.get('ic','le501x')
base_arch,cpu = plf[ic]
default_toolchain = {
    'arm_cm': 'arm-gcc',
    'rv32': 'riscv-gcc',
}
tool = ARGUMENTS.get('toolchain',default_toolchain[base_arch])
env = Environment(ENV = os.environ,tools=[tool],toolpath=['tools'])
print(env['TOOLS'])
env['IC'] = ic
env['CPU'] = cpu
env['BASE_ARCH'] = base_arch

if env['IC'] == 'le501x':
    env['CPU'] = 'cortex-m0'
    env['STACK_BIN_OUTPUT_DIR'] = Dir('#dev/soc/arm_cm/le501x/bin/')
elif env['IC'] == 'sagi':
    env['CPU'] = 'cortex-m3'
elif env['IC'] == 'taurus':
    env['ARCH'] = 'rv32emac'
    env['ABI'] = 'ilp32e'

if env['BASE_ARCH'] == 'rv32':
    env['ARCH_FLAGS'] = ' -mabi=$ABI -march=$ARCH -mstrict-align -msave-restore '
    env['AS_ARCH_FLAGS'] = ' -mabi=$ABI -march=$ARCH '
else:
    env['ARCH_FLAGS'] = ' -mabi=aapcs -mthumb -mcpu=$CPU -mno-unaligned-access '

if 'mdk' in env['TOOLS']:
    env['COMPILER'] = 'armcc'
    
else:
    env['COMPILER'] = 'gnu'
    env['CFLAGS'] = '${ARCH_FLAGS} -O2 -Os -std=c11 -g -g3 -ffunction-sections -fdata-sections -fstrict-volatile-bitfields -fno-common -fno-optimize-sibling-calls -Wall'
    env['ASFLAGS'] = '${AS_ARCH_FLAGS} -g '
    env['LINKFLAGS'] = '${ARCH_FLAGS} -O2 -Os -std=c11 -g -g3 -specs=nano.specs -specs=nosys.specs -T ${LINKSCRIPT} -Wl,-Map=${TARGET.base}.map -Wl,--cref'
    env['GC_OPTION'] = ' -Wl,--gc-sections '
env['CPPPATH'] = ['#inc','#inc/cmsis','#inc/prf']
dev_env = env.Clone()

if not 'mdk' in env['TOOLS']:
    stack = SConscript('build/stack/SConscript',exports=['env'],must_exist=False)
else:
    stack = None

if env['IC']!= 'le501x':
    target_ble_lib = stack
    target_fw = stack
    target_mesh_lib = stack
    target_mesh_fw = stack
else:
    if stack is None:
        target_fw = File('#dev/soc/arm_cm/le501x/bin/fw.hex')
        target_ble_lib = File('#dev/soc/arm_cm/le501x/bin/libfw.o')
        target_mesh_lib = File('#dev/soc/arm_cm/le501x/bin/libmesh.o')
        target_mesh_fw = File('#dev/soc/arm_cm/le501x/bin/fw_with_mesh.hex')
    else:
        target_fw = stack[0]
        target_ble_lib = stack[1]
        target_mesh_lib = stack[2]
        target_mesh_fw = stack[3]

SConscript('build/dev/SConscript',exports=['dev_env','target_fw','target_ble_lib','target_mesh_lib','target_mesh_fw'])