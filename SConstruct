
    
tools = ['arm-gcc']
#tools.append('armcc')
toolpath = ['tools']
objdump_asm = Action('$OBJDUMP -d -x $TARGET > ${TARGET.base}.asm')
objcopy_hex = Action('$OBJCOPY -O ihex $OBJCOPY_HEX_OPTION $TARGET ${TARGET.base}.hex')
env = Environment(tools=tools,toolpath=toolpath)
env['CPPPATH'] = ['#inc','#inc/cmsis']
env['CFLAGS'] = '-O2 -Os -mabi=aapcs -mthumb -mcpu=cortex-m0 -std=c11 -g -ffunction-sections -fdata-sections -fstrict-volatile-bitfields'
env['LINKFLAGS'] = '-O2 -Os -mabi=aapcs -mthumb -mcpu=cortex-m0 -std=c11 -g'
SConscript(dirs='stack',exports=['env','objdump_asm','objcopy_hex'])