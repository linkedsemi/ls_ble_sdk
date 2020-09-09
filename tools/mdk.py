import mdk_xml_schema
import pyxb
import os
import SCons.Tool
import SCons.Builder
import subprocess

def mdk_builder(target,source,env):
    prj = mdk_xml_schema.Project()
    prj.ToolsetName = 'ARM'
    prj.Device = 'ARMCM0'
    prj.useUlib = 1
#    target_path = target[0].path[0: target[0].path.rfind("\\")+1]
#    target_name = target[0].path[target[0].path.rfind("\\")+1: len(target[0].path)]
    prj_dir = os.path.join(env['PROJ_DIR'].srcnode().abspath,'mdk')
    if os.path.exists(prj_dir)==False:
        os.mkdir(prj_dir)
    inc_path_str = ""
    for node_path in env.Dir(env['CPPPATH']):
        inc_path_str = inc_path_str + os.path.relpath(node_path.abspath, prj_dir) + ";"
    prj.IncludePath = inc_path_str
    
    scat_file_str = os.path.relpath((env['LINKSCRIPT']).srcnode().abspath, prj_dir)
    prj.ScatterFile = scat_file_str
    
    prj.CDefines = "" 
    prj.COptions = "--c99 -O2 --wchar32" 
    prj.LinkOptions = "--datacompressor=off"
    beforecompile1 = mdk_xml_schema.UserAction('')
    beforecompile2 = mdk_xml_schema.UserAction('')
    beforebuild1 = mdk_xml_schema.UserAction('')
    beforebuild2 = mdk_xml_schema.UserAction('')
    afterbuild1 = mdk_xml_schema.UserAction(os.path.relpath(env.Dir("#").abspath, prj_dir) + '\\tools\\disassemble.bat @L ' + os.path.relpath(env.Dir("#").abspath, prj_dir))
    afterbuild2 = mdk_xml_schema.UserAction('')
    prj.User = pyxb.BIND(BeforeCompile1=beforecompile1,BeforeCompile2=beforecompile2,BeforeBuild1=beforebuild1,BeforeBuild2=beforebuild2,AfterBuild1=afterbuild1,AfterBuild2=afterbuild2)
    
    c_list = []
    asm_list = []
    obj_list = []
    lib_list = []
    for build_src in source:
        src = build_src.srcnode().path
        filepath,extension = os.path.splitext(src)
        if extension == '.c':
            c_list.append(os.path.relpath(src,prj_dir))
        elif extension == '.s':
            asm_list.append(os.path.relpath(src,prj_dir))
        else:
            obj_list.append(os.path.relpath(src,prj_dir))
    if 'LIBS' in env:
        for lib in env['LIBS']:
            libpath,libext = os.path.splitext(lib.path)
            if libext == '.o':
                obj_list.append(os.path.relpath(lib.path,prj_dir))
            else:
                lib_list.append(os.path.relpath(lib.path,prj_dir))

    file_list = file_list_build(c_list,1) + file_list_build(asm_list,2) + file_list_build(obj_list,3) + file_list_build(lib_list,4)
    prj.Groups = pyxb.BIND(pyxb.BIND('Group1',pyxb.BIND(*file_list)))

    filename = target[0].abspath + '.xml'
    with open(filename,'wb') as file_obj:
        file_obj.write(prj.toxml("utf-8"))

    #print(os.path.isfile(os.path.join(prj_dir,os.path.basename(target[0].path)+'.uvprojx')))
    if os.path.isfile(os.path.join(prj_dir,os.path.basename(target[0].path)+'.uvprojx'))==True:
        os.remove(os.path.join(prj_dir,os.path.basename(target[0].path)+'.uvprojx'))
    if os.path.isfile(os.path.join(prj_dir,os.path.basename(target[0].path)+'.uvoptx'))==True:
        os.remove(os.path.join(prj_dir,os.path.basename(target[0].path)+'.uvoptx'))    
    
    proj_path = os.path.join(prj_dir,os.path.basename(target[0].path)+'.uvprojx')
    proj_gen = env['UV'] + ' ' + proj_path + ' -i ' + filename
    proj_build = env['UV'] + ' -b ' + proj_path
    complete = subprocess.run(proj_gen,shell=True)
    return complete.returncode

def generate(env):
    env['BUILDERS']['Program'] = SCons.Builder.Builder(action = mdk_builder)
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

    

