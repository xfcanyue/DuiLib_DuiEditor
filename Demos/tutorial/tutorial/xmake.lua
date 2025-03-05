-- Add target
target("tutorial")
	-- 设置目标编译类型 phony,binary,static,shared,object,headeronly
    set_kind("binary")
	
	-- 根据编译参数调整输出文件名
    local target_file_name = get_target_file_name("tutorial")
	
	-- 设置目标文件名
	set_basename(target_file_name)
	
	-- 添加规则
    if is_kind("static") then
		add_rules("win.sdk.mfc.static_app")
	else
	   add_rules("win.sdk.mfc.shared_app")
	end

    -- 添加源代码文件
    add_files("**.cpp", "tutorial.rc")
	
    -- 从前面的源代码文件列表中删除指定文件
	--
	
	-- 添加头文件搜索目录
    add_includedirs(".", {public = true})
	
    -- 设置 C++ 预编译头文件
    set_pcxxheader("./StdAfx.h") 
	
	--通用配置
	GeneralConfig()
	
	--添加子工程目标依赖
    add_deps("DuiLib")
    add_deps("DuiScript")
    add_deps("DuiPlugins")
	
	--不同平台的特殊操作
	if is_plat("windows") then	
        if is_kind("static") then
			add_defines("UILIB_STATIC")
		end

        --编译结束时，把.dll拷贝到bin目录, 把.lib拷贝到Lib目录
		after_build(function (target)
			local destfile = os.projectdir() .. "/bin/" .. target:filename()
				os.cp(target:targetfile(), destfile)
        end) 
    end  