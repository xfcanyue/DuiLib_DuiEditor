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

-- Add target
target("DuiScript")
    -- 设置目标编译类型 phony,binary,static,shared,object,headeronly
    set_kind("$(kind)")
	
	-- 根据编译参数调整输出文件名
    local target_file_name = get_target_file_name("DuiScript")
	
	-- 设置目标文件名
	set_basename(target_file_name)
	
	-- 添加规则
    --add_rules("asm")

    -- 添加源代码文件
    add_files("*.cpp")
    if is_arch("x64") then
		add_asflags("-fwin64")
		add_files("angelscript/angelscript/source/as_callfunc_x64_msvc_asm.asm")
	end
    add_files("angelscript/angelscript/source/*.cpp")
    add_files("angelscript/add_on/scriptstdtime/*.cpp")
    add_files("angelscript/add_on/scriptbuilder/*.cpp")
	
    -- 从前面的源代码文件列表中删除指定文件
	--
	
	-- 添加头文件搜索目录
    add_includedirs(".", {public = true})
    add_includedirs("./angelscript/angelscript/include")
	
    -- 设置 C++ 预编译头文件
    --set_pcxxheader("./StdAfx.h") 
	
	--通用配置
	GeneralConfig()
	set_optimize("fast") --在vs2010编译时，fastest会非常慢。
	
	--添加子工程目标依赖
    add_deps("DuiLib")
	
	--不同平台的特殊操作
	if is_plat("linux", "macosx") then
        setup_gtk_includedirs()
    elseif is_plat("windows") then	
        if is_kind("shared") then
            add_defines("UISCRIPT_EXPORTS","ANGELSCRIPT_EXPORT")
        end

        --编译结束时，把.dll拷贝到bin目录, 把.lib拷贝到Lib目录
        if is_kind("shared") then
            after_build(function (target)
                local destfile = os.projectdir() .. "/bin/" .. target:filename()
                os.cp(target:targetfile(), destfile)
                
                local destfile = target:scriptdir() .. "/Lib/" .. target_file_name .. ".lib"
                local srcfile = target:targetdir() .. "/" .. target_file_name .. ".lib"
                os.cp(srcfile, destfile)
            end)
        elseif is_kind("static") then
			after_build(function (target)
				local destfile = target:scriptdir() .. "/Lib/" .. target:filename()
                os.cp(target:targetfile(), destfile)
            end)
        elseif is_kind("binary") then
			after_build(function (target)
				local destfile = os.projectdir() .. "/bin/" .. target:filename()
                os.cp(target:targetfile(), destfile)
            end)
        end        
    end  