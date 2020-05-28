@echo OFF
if "%1"=="sbl" ( 
echo fromelf --bin --output=.\UVBuild\%1.bin .\UVBuild\%1.axf
echo python ..\..\..\..\tools\info_sbl_merge.py .\UVBuild\%1.bin 0x18000000 .\UVBuild\info_sbl.hex
) else (
echo fromelf --i32 --output=.\UVBuild\%1.hex .\UVBuild\%1.axf
echo fromelf -c -a -d -e -v -o .\UVBuild\%1.asm .\UVBuild\%1.axf
)
