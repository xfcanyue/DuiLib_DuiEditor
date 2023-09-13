-- Set the project information
set_project("DuiLib")
set_version("1.0.0")

-- Add target
target("DuiLib")
	set_languages("cxx14")
    -- Set the kind of target through configuration
    set_kind("$(kind)")

    if is_kind("static") then
        add_defines("UILIB_EXPORTS", "UILIB_STATIC")
    elseif is_kind("binary") then
        add_defines("UILIB_EXPORTS")
    end
    -- Set the target kind (executable, shared library, or static library)
    -- set_kind("binary")
    set_kind("static")
    -- Add source files
    add_files("*.cpp")
    
    -- Add header files search paths
    add_includedirs(".")

    -- Add precompiled header
    set_pcxxheader("./StdAfx.h")

    -- Add preprocessor definitions
    add_defines("UNICODE", "_UNICODE")
	add_defines("WIN32","WINDOWS")
    if is_plat("windows") then
        add_defines("WIN32","WINDOWS")
	else
	
    end
	add_rules("mode.debug", "mode.release")
    -- Set the build modes (debug and release)
    if is_mode("debug") then
        -- Handle debug configuration
        add_defines("DEBUG","_DEBUG")
        set_symbols("debug")
		set_targetdir("$(buildir)/lib/$(kind)/$(mode)")
		set_basename("DuiLibd")
    elseif is_mode("release") then
        -- Handle release configuration
        set_optimize("fastest")
        set_strip("all")
        add_defines("NDEBUG")
		set_targetdir("$(buildir)/lib/$(kind)/$(mode)")
		set_basename("DuiLib")
    end
