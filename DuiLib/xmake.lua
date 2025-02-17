-- set the project information
set_project("DuiLib")
set_version("1.0.0")

-- set xmake minimum version
set_xmakever("2.7.9")

-- set language:cxx14
set_languages("cxx14")

option("unicode")
option_end()

-- 公共的安装文件配置
function setup_install_files()
    add_installfiles("compat.h", {{prefixdir = "include/DuiLib"}})
    add_installfiles("DuiLib.h", {{prefixdir = "include/DuiLib"}})
    add_installfiles("UIlib.h", {{prefixdir = "include/DuiLib"}})
    add_installfiles("Control/*.h", {{prefixdir = "include/DuiLib/Control"}})
    add_installfiles("Core/*.h", {{prefixdir = "include/DuiLib/Core"}})
    add_installfiles("Layout/*.h", {{prefixdir = "include/DuiLib/Layout"}})
    add_installfiles("Render/*.h", {{prefixdir = "include/DuiLib/Render"}})
    add_installfiles("Utils/*.h", {{prefixdir = "include/DuiLib/Utils"}})

    after_uninstall(function (target)
        os.rm(target:installdir()..  "/include/DuiLib/")
    end)
end

-- 公共的 GTK 相关包含目录配置
function setup_gtk_includedirs()
    local base_dir = "/usr/include"
    if is_plat("macosx") then
        base_dir = "/usr/local/include"
    end

    add_includedirs(base_dir.. "/gtk-3.0")
    add_includedirs(base_dir.. "/cairo")
    add_includedirs(base_dir.. "/harfbuzz")
    add_includedirs(base_dir.. "/gdk-pixbuf-2.0")
    add_includedirs(base_dir.. "/../lib/x86_64-linux-gnu/glib-2.0/include")
    add_includedirs(base_dir.. "/glib-2.0/glib")
    add_includedirs(base_dir.. "/glib-2.0")
    add_includedirs(base_dir.. "/pango-1.0")
    add_includedirs(base_dir.. "/atk-1.0")
end

-- 定义目标文件名
function get_target_file_name()
    local target_file_name = "DuiLib"
    local arch_suffix = is_arch("x64") and "64" or ""
    local unicode_suffix = has_config("unicode", "true") and "u" or ""
    local static_suffix = is_kind("static") and "s" or ""
    local debug_suffix = is_mode("debug") and "d" or ""

    return target_file_name.. "_".. arch_suffix.. unicode_suffix.. static_suffix.. debug_suffix
end

-----------------------------------------------------------------------------------------------
-- Add target
target("DuiLib")
--dll/lib输出文件名
local target_file_name = get_target_file_name()

if is_plat("linux", "macosx") then
    setup_gtk_includedirs()
    setup_install_files()
elseif is_plat("windows") then
	add_defines("WIN32","_WIN32", "WINDOWS")
	
	--unicode
	if has_config("unicode", "true") then
		add_defines("UNICODE", "_UNICODE")
	end
	
	if is_kind("static") then
		add_defines("UILIB_EXPORTS", "UILIB_STATIC")
	else
		add_defines("UILIB_EXPORTS")
	end

	--编译结束时，把.dll拷贝到bin目录, 把.lib拷贝到Lib目录
	if is_kind("shared") then
		after_build(function (target)
			local destfile = target:scriptdir() .. "/../bin/" .. target:filename()
        	os.cp(target:targetfile(), destfile)
			
			local destfile = target:scriptdir() .. "/Lib/" .. target_file_name .. ".lib"
			local srcfile = target:targetdir() .. "/" .. target_file_name .. ".lib"
			os.cp(srcfile, destfile)
		end)
	else
		after_build(function (target)
		local destfile = target:scriptdir() .. "/Lib/" .. target:filename()
        	os.cp(target:targetfile(), destfile)
		end)
	end
	
end
	
-- set the kind of target through configuration
set_kind("$(kind)")


-- set the build modes (debug and release)
if is_mode("debug") then
    -- Handle debug configuration
	-- Prohibit optimization
	set_optimize("none")
    add_defines("DEBUG","_DEBUG")
    set_symbols("debug")
elseif is_mode("release") then
    -- Handle release configuration
    set_optimize("fastest")
    set_strip("all")
    add_defines("NDEBUG")
end
	
set_targetdir("$(buildir)/$(plat)/$(arch)/$(mode)")

set_basename(target_file_name)

-- add source files
add_files("**.cpp")

-- remove source files from add_files.
remove_files("Utils/unzip.cpp")
    
-- add header files search paths
add_includedirs(".")

-- add precompiled header
set_pcxxheader("./StdAfx.h") 


