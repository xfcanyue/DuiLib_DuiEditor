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

-- include project sources
includes("DuiLib")
