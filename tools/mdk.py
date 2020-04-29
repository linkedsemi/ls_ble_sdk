import mdk_xml_schema
import pyxb
import os
import SCons.Tool
import SCons.Builder
import subprocess

def mdk_generator(target,source,env,for_signature):
    prj = mdk_xml_schema.Project()
    prj.ToolsetName = 'ARM'
    prj.Device = 'ARMCM0'
    prj.useUlib = 1
#    target_path = target[0].path[0: target[0].path.rfind("\\")+1]
#    target_name = target[0].path[target[0].path.rfind("\\")+1: len(target[0].path)]
    prj_dir = env['PROJ_DIR'].srcnode()
    inc_path_str = ""
    for node_path in env.Dir(env['CPPPATH']):
        inc_path_str = inc_path_str + os.path.relpath(node_path.abspath, prj_dir.abspath) + ";"
    prj.IncludePath = inc_path_str
    
    scat_file_str = os.path.relpath((env['LINKSCRIPT']).srcnode().abspath, prj_dir.abspath)
    prj.ScatterFile = scat_file_str
    
    prj.CDefines = "" 
    prj.COptions = "--c99 -O2 --wchar32" 
    beforecompile1 = mdk_xml_schema.UserAction('')
    beforecompile2 = mdk_xml_schema.UserAction('')
    beforebuild1 = mdk_xml_schema.UserAction('')
    beforebuild2 = mdk_xml_schema.UserAction('')
    afterbuild1 = mdk_xml_schema.UserAction(os.path.relpath(env.Dir("#").abspath, prj_dir.abspath) + '\\tools\\disassemble.bat @L')
    afterbuild2 = mdk_xml_schema.UserAction('')
    prj.User = pyxb.BIND(BeforeCompile1=beforecompile1,BeforeCompile2=beforecompile2,BeforeBuild1=beforebuild1,BeforeBuild2=beforebuild2,AfterBuild1=afterbuild1,AfterBuild2=afterbuild2)
    
    c_list = []
    asm_list = []
    obj_list = []
    lib_list = []
#    prj_dir = env['PROJ_DIR'].srcnode()
    for build_src in source:
        src = build_src.srcnode().path
        filepath,extension = os.path.splitext(src)
        if extension == '.c':
            c_list.append(os.path.relpath(src,prj_dir.path))
        elif extension == '.s':
            asm_list.append(os.path.relpath(src,prj_dir.path))
        else:
            obj_list.append(os.path.relpath(src,prj_dir.path))
    if 'LIBS' in env:
        for lib in env['LIBS']:
            libpath,libext = os.path.splitext(lib.path)
            if libext == '.o':
                obj_list.append(os.path.relpath(lib.path,prj_dir.path))
            else:
                lib_list.append(os.path.relpath(lib.path,prj_dir.path))

    file_list = file_list_build(c_list,1) + file_list_build(asm_list,2) + file_list_build(obj_list,3) + file_list_build(lib_list,4)
    prj.Groups = pyxb.BIND(pyxb.BIND('Group1',pyxb.BIND(*file_list)))

    filename = target[0].abspath + '.xml'
    with open(filename,'wb') as file_obj:
        file_obj.write(prj.toxml("utf-8"))
    
    proj_path = os.path.join(prj_dir.abspath,os.path.basename(target[0].path)+'.uvprojx')
    proj_gen = '$UV ' + proj_path + ' -i ' + filename
    proj_build = '$UV -b ' + proj_path
    return proj_gen

def generate(env):
    env['BUILDERS']['Program'] = SCons.Builder.Builder(generator = mdk_generator)
    env['UV'] = 'C:/Keil_v5/UV4/UV4.exe'
    SCons.Tool.createProgBuilder(env)

def exists(env):
    return True

def file_list_build(list,type):
    file_list = []
    for src in list:
        file_name = os.path.basename(src)
        file_list.append(pyxb.BIND(file_name, type , src))
    return file_list

def import_xml_gen(include_path,scatter_file,c_list,asm_list,lib_list,xml_path):
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

    #prj.Simulator = pyxb.BIND(UseSimulator = 0,LoadApplicationAtStartup =0,RunToMain =0,InitializationFile='')
    #prj.Target = pyxb.BIND(UseTarget = 1,LoadApplicationAtStartup =0,RunToMain =0,InitializationFile='',Driver='')

    

