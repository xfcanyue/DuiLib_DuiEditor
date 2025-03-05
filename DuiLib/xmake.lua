-- 公共的安装文件配置
function setup_install_files()
    add_installfiles("compat.h", {prefixdir = "include/DuiLib"})
    add_installfiles("DuiLib.h", {prefixdir = "include/DuiLib"})
    add_installfiles("UIlib.h", {prefixdir = "include/DuiLib"})
    add_installfiles("Control/*.h", {prefixdir = "include/DuiLib/Control"})
    add_installfiles("Core/*.h", {prefixdir = "include/DuiLib/Core"})
    add_installfiles("Layout/*.h", {prefixdir = "include/DuiLib/Layout"})
    add_installfiles("Render/*.h", {prefixdir = "include/DuiLib/Render"})
    add_installfiles("Utils/*.h", {prefixdir = "include/DuiLib/Utils"})

    after_uninstall(function (target)
        os.rm(target:installdir()..  "/include/DuiLib/")
    end)
end



-----------------------------------------------------------------------------------------------
-- 定义工程目标
target("DuiLib")
    -- 设置目标编译类型 phony,binary,static,shared,object,headeronly
    set_kind("$(kind)")
	
    -- 根据编译参数调整输出文件名
    local target_file_name = get_target_file_name("DuiLib")
	
	-- 设置目标文件名
	set_basename(target_file_name)
		
    -- 添加源代码文件
    add_files("**.cpp")

    -- 从前面的源代码文件列表中删除指定文件
    remove_files("Utils/unzip.cpp")
        
    -- 添加头文件搜索目录
    add_includedirs(".", {public = true})

    -- 设置 C++ 预编译头文件
    set_pcxxheader("./StdAfx.h") 
	
	--通用配置
	GeneralConfig()

	--添加子工程目标依赖
	--
	
	-- 添加规则
	--
	
	--不同平台的特殊操作
    if is_plat("linux", "macosx") then
        setup_gtk_includedirs()
        setup_install_files()
    elseif is_plat("windows") then	
        if is_kind("static") then
            add_defines("UILIB_EXPORTS", "UILIB_STATIC")
        else
            add_defines("UILIB_EXPORTS")
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
        





