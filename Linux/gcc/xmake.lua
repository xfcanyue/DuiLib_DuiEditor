

-- set the project information
set_project("TestDuiLib")
set_version("1.0.0")

-- set xmake minimum version
set_xmakever("2.7.9")

-- set language:cxx14
set_languages("cxx14")

option("unicode")
option_end()

-----------------------------------------------------------------------------------------------
-- Add target
target("TestDuiLib")
local target_file_name = "TestDuiLib"

if is_plat("linux") then
	add_includedirs("/usr/local/include/DuiLib")
	
	add_links("DuiLib")
	add_links("gtk-3") 
	add_links("gobject-2.0")
	add_links("glib-2.0")
	add_links("gdk-3")
	add_links("cairo")
	add_links("gdk_pixbuf-2.0")
	add_links("pango-1.0")
	add_links("pangocairo-1.0")
	
	add_ldflags("-Wl,--hash-style=sysv,-Bsymbolic,-rpath=./ -no-pie")
	
	--编译结束时，把TestDuiLib拷贝到当前目录
	after_build(function (target)
		local destfile = target:scriptdir() .. "/" .. target:filename()
        	os.cp(target:targetfile(), destfile)
    	end)
	
elseif is_plat("macosx") then
	add_includedirs("/usr/local/include/DuiLib")
	
	add_linkdirs("/usr/local/lib")
	add_links("DuiLib")
	add_links("gtk-3") 
	add_links("gobject-2.0")
	add_links("glib-2.0")
	add_links("gdk-3")
	add_links("cairo")
	add_links("gdk_pixbuf-2.0")
	add_links("pango-1.0")
	add_links("pangocairo-1.0")
	
	--编译结束时，把TestDuiLib拷贝到当前目录
	after_build(function (target)
		local destfile = target:scriptdir() .. "/" .. target:filename()
        	os.cp(target:targetfile(), destfile)
    	end)
elseif is_plat("windows") then
		
end
	
-- set the kind of target through configuration
set_kind("binary")


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

	

