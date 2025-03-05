-- set the project information
set_project("DuiLib")
set_version("1.0.0")

-- set xmake minimum version
set_xmakever("2.7.9")

-- set language:cxx14
if is_plat("linux", "macosx") then
set_languages("cxx14")
end

option("unicode")
option_end()

-- help clangd
add_rules("plugin.compile_commands.autoupdate", { outputdir = "." })

-- define target name rule
function get_target_file_name(target_base_name)
    if target_base_name == nil then
        target_base_name = "Default"
    end
    local arch_suffix = is_arch("x64") and "64" or ""
    local unicode_suffix = has_config("unicode", "true") and "u" or ""
    local static_suffix = is_kind("static") and "s" or ""
    local debug_suffix = is_mode("debug") and "d" or ""

    return target_base_name.. "_".. arch_suffix.. unicode_suffix.. static_suffix.. debug_suffix
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

function GeneralConfig()
	if is_plat("windows") then
        add_defines("WIN32","_WIN32", "WINDOWS")  

		if is_arch("x64") then
			add_defines("_WIN64")
		end		
       
        if has_config("unicode", "true") then
            add_defines("UNICODE", "_UNICODE")
        end
	end
		
    -- set the build modes (debug and release)
    if is_mode("debug") then
        set_optimize("none")
        add_defines("DEBUG","_DEBUG")
        set_symbols("debug")
    elseif is_mode("release") then
        set_optimize("fastest")
        set_strip("all")
        add_defines("NDEBUG")
    end 
end

--include project sources
includes("DuiLib")
includes("DuiScript")
if is_plat("windows") then
	includes("DuiPlugins")
	includes("DuiPreviewer")
	includes("DuiEditor")
	includes("Demos/tutorial/tutorial")
end
