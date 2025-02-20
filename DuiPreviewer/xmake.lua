-- Add target
target("DuiPreviewer")
    set_kind("binary")
    --dll/lib输出文件名
    local target_file_name = get_target_file_name("DuiPreviewer")

    add_includedirs(".")
    add_files("**.cpp")
    set_pcxxheader("./stdafx.h")
    
    set_basename(target_file_name)
    set_targetdir("$(buildir)/$(plat)/$(arch)/$(mode)")

    add_deps("DuiLib")

    -- add_linkdirs("../lib")
    -- add_links("DuiLib_us.lib")

    -- print(get_target_file_name("DuiLib").. ".lib")