@echo OFF
if "%1"=="sbl" ( 
 @fromelf --bin --output=..\..\..\..\build\examples\%1.bin .\UVBuild\%1.axf
 @python ..\..\..\..\tools\info_sbl_merge.py ..\..\..\..\build\examples\%1.bin 0x18000000 ..\..\..\..\build\examples\info_sbl.hex
) else (
 @fromelf --i32 --output=.\UVBuild\%1.hex .\UVBuild\%1.axf
 @fromelf -c -a -d -e -v -o .\UVBuild\%1.asm .\UVBuild\%1.axf
)
