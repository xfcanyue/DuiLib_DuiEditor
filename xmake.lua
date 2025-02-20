-- set the project information
set_project("DuiLib")
set_version("1.0.0")

-- set xmake minimum version
set_xmakever("2.7.9")

-- set language:cxx14
set_languages("cxx14")

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

-- include project sources
includes("DuiLib")
includes("DuiPlugins")
-- includes("DuiPreviewer")
-- includes("DuiScript")
-- includes("DuiEditor")
