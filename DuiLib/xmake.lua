

-- set the project information
set_project("DuiLib")
set_version("1.0.0")

-- set xmake minimum version
set_xmakever("2.7.9")

-- set language:cxx14
set_languages("cxx14")

option("unicode")
option_end()

-----------------------------------------------------------------------------------------------
-- Add target
target("DuiLib")
local target_file_name = "DuiLib"

if is_plat("linux") then
	add_includedirs("/usr/include/gtk-3.0")
	add_includedirs("/usr/include/cairo")
	add_includedirs("/usr/include/harfbuzz")
	add_includedirs("/usr/include/gdk-pixbuf-2.0")
	add_includedirs("/usr/lib/x86_64-linux-gnu/glib-2.0/include")
	add_includedirs("/usr/include/glib-2.0/glib")
	add_includedirs("/usr/include/glib-2.0")
	add_includedirs("/usr/include/pango-1.0")
	add_includedirs("/usr/include/atk-1.0")
	
	add_installfiles("compat.h", {prefixdir = "include/DuiLib"})
	add_installfiles("DuiLib.h", {prefixdir = "include/DuiLib"})
	add_installfiles("UIlib.h", {prefixdir = "include/DuiLib"})
	add_installfiles("Control/*.h", {prefixdir = "include/DuiLib/Control"})
	add_installfiles("Core/*.h", {prefixdir = "include/DuiLib/Core"})
	add_installfiles("Layout/*.h", {prefixdir = "include/DuiLib/Layout"})
	add_installfiles("Render/*.h", {prefixdir = "include/DuiLib/Render"})
	add_installfiles("Utils/*.h", {prefixdir = "include/DuiLib/Utils"})
	
	after_uninstall(function (target)
        	os.rm(target:installdir() .. "/include/DuiLib/")
    	end)
elseif is_plat("macosx") then
	add_includedirs("/usr/local/include/gtk-3.0")
	add_includedirs("/usr/local/include/cairo")
	add_includedirs("/usr/local/include/harfbuzz")
	add_includedirs("/usr/local/include/gdk-pixbuf-2.0")
	add_includedirs("/usr/local/lib/glib-2.0/include")
	add_includedirs("/usr/local/lib/glib-2.0/include")
	add_includedirs("/usr/local/include/glib-2.0/glib")
	add_includedirs("/usr/local/include/glib-2.0")
	add_includedirs("/usr/local/include/pango-1.0")
	add_includedirs("/usr/local/include/atk-1.0")
	
	add_installfiles("compat.h", {prefixdir = "include/DuiLib"})
	add_installfiles("DuiLib.h", {prefixdir = "include/DuiLib"})
	add_installfiles("UIlib.h", {prefixdir = "include/DuiLib"})
	add_installfiles("Control/*.h", {prefixdir = "include/DuiLib/Control"})
	add_installfiles("Core/*.h", {prefixdir = "include/DuiLib/Core"})
	add_installfiles("Layout/*.h", {prefixdir = "include/DuiLib/Layout"})
	add_installfiles("Render/*.h", {prefixdir = "include/DuiLib/Render"})
	add_installfiles("Utils/*.h", {prefixdir = "include/DuiLib/Utils"})
	
	after_uninstall(function (target)
        	os.rm(target:installdir() .. "/include/DuiLib/")
    	end)
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

	--dll/lib输出文件名
	if is_arch("x64") then
		if has_config("unicode", "true") then
			if is_kind("static") then
				if is_mode("debug") then
					target_file_name = target_file_name .. "_64usd"
				else
					target_file_name = target_file_name .. "_64us"
				end
			else
				if is_mode("debug") then
					target_file_name = target_file_name .. "_64ud"
				else
					target_file_name = target_file_name .. "_64u"
				end
			end
		else
			if is_kind("static") then
				if is_mode("debug") then
					target_file_name = target_file_name .. "_64sd"
				else
					target_file_name = target_file_name .. "_64s"
				end
			else
				if is_mode("debug") then
					target_file_name = target_file_name .. "_64d"
				else
					target_file_name = target_file_name .. "_64"
				end
			end
		end
	else
		if has_config("unicode", "true") then
			if is_kind("static") then
				if is_mode("debug") then
					target_file_name = target_file_name .. "_usd"
				else
					target_file_name = target_file_name .. "_us"
				end
			else
				if is_mode("debug") then
					target_file_name = target_file_name .. "_ud"
				else
					target_file_name = target_file_name .. "_u"
				end
			end
		else
			if is_kind("static") then
				if is_mode("debug") then
					target_file_name = target_file_name .. "_sd"
				else
					target_file_name = target_file_name .. "_s"
				end
			else
				if is_mode("debug") then
					target_file_name = target_file_name .. "_d"
				else
					target_file_name = target_file_name .. ""
				end
			end
		end
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
	

