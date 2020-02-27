import mdk
import pyxb
import os

def file_list_build(list,type):
    file_list = []
    for src in list:
        file_name = os.path.basename(src)
        dir = os.path.dirname(src)
        file_list.append(pyxb.BIND(file_name, type , dir))
    return file_list

def import_xml_gen(include_path,scatter_file,c_list,asm_list,lib_list,xml_path):
    prj = mdk.Project()
    prj.ToolsetName = 'ARM'
    prj.Device = 'ARMCM0'
    #prj.Output = pyxb.BIND()
    #beforecompile1 = mdk.UserAction('')
    #beforecompile2 = mdk.UserAction('')
    #beforebuild1 = mdk.UserAction('')
    #beforebuild2 = mdk.UserAction('')
    #afterbuild1 = mdk.UserAction('')
    #afterbuild2 = mdk.UserAction('')
    #prj.User = pyxb.BIND(BeforeCompile1=beforecompile1,BeforeCompile2=beforecompile2,BeforeBuild1=beforebuild1,BeforeBuild2=beforebuild2,AfterBuild1=afterbuild1,AfterBuild2=afterbuild2)
    #prj.IncludePath = include_path
    #prj.ScatterFile = scatter_file
    #prj.SFDFile = ''
    #prj.MiscControls = '' 
    #prj.CDefines = ''
    #prj.AsmDefines = ''
    #prj.COptions = ''
    #prj.AsmOptions = ''
    #prj.LinkOptions = ''
    prj.useUlib = 1
    #prj.Simulator = pyxb.BIND(UseSimulator = 0,LoadApplicationAtStartup =0,RunToMain =0,InitializationFile='')
    #prj.Target = pyxb.BIND(UseTarget = 1,LoadApplicationAtStartup =0,RunToMain =0,InitializationFile='',Driver='')
    file_list = file_list_build(c_list,1) + file_list_build(asm_list,2) + file_list_build(lib_list,4)
    prj.Groups = pyxb.BIND(pyxb.BIND('Group1',pyxb.BIND(*file_list)))
    
    filename = xml_path
    with open(filename,'wb') as file_obj:
        file_obj.write(prj.toxml("utf-8"))
