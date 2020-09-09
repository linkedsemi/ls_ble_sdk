@echo OFF
echo python %2\tools\info_sbl_merge.py %2\dev\soc\arm_cm\le501x\bin\bram.bin le501x .\UVBuild\info_sbl.hex
 python %2\tools\info_sbl_merge.py %2\dev\soc\arm_cm\le501x\bin\bram.bin le501x .\UVBuild\info_sbl.hex
echo fromelf --i32 --output=.\UVBuild\%1.hex .\UVBuild\%1.axf
 fromelf --i32 --output=.\UVBuild\%1.hex .\UVBuild\%1.axf
echo fromelf -c -a -d -e -v -o .\UVBuild\%1.asm .\UVBuild\%1.axf
 fromelf -c -a -d -e -v -o .\UVBuild\%1.asm .\UVBuild\%1.axf
