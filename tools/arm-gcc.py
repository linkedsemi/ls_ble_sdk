from SCons.Tool import cc,ar,link

def generate(env):

    cc.generate(env)
    ar.generate(env)
    link.generate(env)
    env['TOOLCHAIN_PATH'] = 'D:/gcc-arm-none-eabi-9-2019-q4-major-win32/bin'
    env['CC'] = '$TOOLCHAIN_PATH/arm-none-eabi-gcc'
    env['AR'] = '$TOOLCHAIN_PATH/arm-none-eabi-ar'
    env['OBJDUMP'] = '$TOOLCHAIN_PATH/arm-none-eabi-objdump'
    env['OBJCOPY'] = '$TOOLCHAIN_PATH/arm-none-eabi-objcopy'
    env['NM'] = '$TOOLCHAIN_PATH/arm-none-eabi-nm'
    env['PROGSUFFIX'] = '.elf'    
    
def exists(env):
    return True