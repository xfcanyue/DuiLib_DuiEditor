-- add_requires("cmake::angelscript", {alias = "angelscript", configs = {moduledirs = "angelscript/angelscript/projects/cmake"}})

-- package("angelscript")
--     set_sourcedir("/angelscript/angelscript")
    
--     on_install(function (package)
--         import("package.tools.cmake").install(package) 
--     end)

-- 定义一个名为 asm 的规则 
rule("asm") 
    -- 设置规则的扩展名为.asm 
    set_extensions(".asm") 
 
    -- 定义构建命令文件时的回调函数 
    on_buildcmd_file(function (target, batchcmds, sourcefile) 
        -- 获取目标文件路径，这里将.asm 文件编译后生成同名的.o 文件 
        local objectfile = target:objectfile(sourcefile) 
 
        -- 根据目标平台获取对应的汇编器命令 
        local as = target:tool("as") or "as" 
 
        -- 添加汇编命令到命令列表中 
        batchcmds:add({ 
            cmd = "", 
            commands = { 
                -- 构建汇编命令，使用汇编器将.asm 文件编译成.o 文件 
                "$(as) -o ".. objectfile.. " ".. sourcefile 
            }, 
            envs = { 
                -- 设置汇编器环境变量 
                as = as 
            } 
        }) 
    end) 
 
-- rule("asm")
--     set_extensions(".asm")
--     on_buildcmd_file(function (target, batchcmds, sourcefile)
--         local objfile = path.join(target:autogendir(), path.basename(sourcefile) .. ".obj")
--         batchcmds:show_progress("${color.build.object}compiling.assembly %s", sourcefile)
--         batchcmds:mkdir(path.directory(objfile))
--         batchcmds:compile("ml.exe", "/c", "/Fo" .. objfile, sourcefile)
--     end)

-- Add target
target("DuiScript")
    set_kind("shared")
    --dll/lib输出文件名
    local target_file_name = get_target_file_name("DuiScript")
    add_rules("asm")

    add_includedirs(".")
    add_includedirs("./angelscript/angelscript/include")

    add_files("*.cpp")
    -- 添加cmake的angelscript
    -- add_packages("angelscript")
    add_files("angelscript/angelscript/source/as_callfunc_arm_msvc.asm")
    add_files("angelscript/angelscript/source/as_callfunc_x64_msvc_asm.asm")
    add_files("angelscript/angelscript/source/as_callfunc_arm64_msvc.asm")
    add_files("angelscript/angelscript/source/*.cpp")
    add_files("angelscript/add_on/scriptstdtime/*.cpp")
    add_files("angelscript/add_on/scriptbuilder/*.cpp")


    set_pcxxheader("./stdafx.h")

    set_basename(target_file_name)
    set_targetdir("$(buildir)/$(plat)/$(arch)/$(mode)")

    if is_plat("windows") then
        add_defines("WIN32","_WIN32", "WINDOWS")
        
        --unicode
        if has_config("unicode", "true") then
            add_defines("UNICODE", "_UNICODE")
        end
        
        if is_kind("static") then
            add_defines("UISCRIPT_EXPORTS", "ANGELSCRIPT_EXPORT")
        else
            add_defines("UISCRIPT_EXPORTS","ANGELSCRIPT_EXPORT","_USRDLL")
        end
    end

    add_deps("DuiLib")

    -- add_linkdirs("../lib")
    -- add_links("DuiLib_us.lib")

    -- print(get_target_file_name("DuiLib").. ".lib")
