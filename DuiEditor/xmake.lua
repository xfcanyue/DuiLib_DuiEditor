-- Add target
target("DuiEditor")
    set_kind("binary")
    --dll/lib输出文件名
    local target_file_name = get_target_file_name("DuiEditor")

    add_includedirs(".")
    add_files("**.cpp")
    set_pcxxheader("./stdafx.h")
    
    set_basename(target_file_name)
    set_targetdir("$(buildir)/$(plat)/$(arch)/$(mode)")

    add_deps("DuiLib")
    add_deps("DuiPlugins")
    add_deps("DuiPreviewer")
    add_deps("DuiScript")
