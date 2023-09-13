-- Set the project information
set_project("DuiLib")
set_version("1.0.0")

-- Add target
target("DuiLib")
	set_languages("cxx14")
	
	local target_file_name = "DuiLib"
	
    -- Set the kind of target through configuration
    set_kind("$(kind)")

    if is_kind("static") then
		target_file_name = target_file_name .. "_s"
        add_defines("UILIB_EXPORTS", "UILIB_STATIC")
    elseif is_kind("shared") then
        add_defines("UILIB_EXPORTS")
    end
    
    -- Add source files
    add_files("*.cpp")
    
    -- Add header files search paths
    add_includedirs(".")

    -- Add precompiled header
    set_pcxxheader("./StdAfx.h")

    -- Add preprocessor definitions
    add_defines("UNICODE", "_UNICODE")
	
	--  Compile architecture configuration
	if is_arch("x86_64") then
		-- x64
		
	elseif is_arch("i386") then
		-- x86
	end
	
	-- Platform configuration
    if is_plat("windows") then
        add_defines("WIN32","WINDOWS")
	else
	
    end
	
    -- Set the build modes (debug and release)
    if is_mode("debug") then
        -- Handle debug configuration
		-- Prohibit optimization
		set_optimize("none")
		target_file_name = target_file_name .. "d"
        add_defines("DEBUG","_DEBUG")
        set_symbols("debug")
		set_targetdir("$(buildir)/lib/$(kind)/$(mode)")

    elseif is_mode("release") then
        -- Handle release configuration
        set_optimize("fastest")
        set_strip("all")
        add_defines("NDEBUG")
		set_targetdir("$(buildir)/lib/$(kind)/$(mode)")
    end
	set_basename(target_file_name)
