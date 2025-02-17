-- Add target
target("DuiPlugins")
    set_kind("shared")
    --dll/lib输出文件名
    local target_file_name = get_target_file_name("DuiPlugins")

    add_includedirs(".")
    add_files("**.cpp")
    -- add_files("DuiLib/*.cpp")
    -- add_files("MFCGridCtrl/**.cpp")
    -- add_files("pugixml/*.cpp")

    set_pcxxheader("./stdafx.h")

    add_defines("DUIPLUGINS_EXPORTS")

    -- add_deps("DuiLib")

    add_linkdirs("$(projectdir)/lib")
    -- add_links("DuiLib_u")

    set_basename(target_file_name)
    set_targetdir("$(buildir)/$(plat)/$(arch)/$(mode)")


    -- print(get_target_file_name("DuiLib").. ".lib")