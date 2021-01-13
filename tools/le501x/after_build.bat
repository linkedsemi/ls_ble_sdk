%2\tools\srec_cat.exe -Output .\UVBuild\info_sbl.hex -Intel %2\dev\soc\arm_cm\le501x\bin\bram.bin -Binary -Offset 0x18000300 -CRC32_Little_Endian -Maximum-Address %2\dev\soc\arm_cm\le501x\bin\bram.bin -Binary -Offset 0x18000300 %2\tools\le501x\info.bin -Binary -Offset 0x18000000
fromelf --i32 --output=.\UVBuild\%1.hex .\UVBuild\%1.axf
%2\tools\srec_cat.exe -Output .\UVBuild\%1_production.hex -Intel .\UVBuild\info_sbl.hex -Intel .\UVBuild\%1.hex -Intel %2\%3 -Intel 
fromelf -c -a -d -e -v -o .\UVBuild\%1.asm .\UVBuild\%1.axf


