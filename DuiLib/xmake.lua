-- set the project information
set_project("DuiLib")
set_version("1.0.0")

-- set xmake minimum version
set_xmakever("2.7.9")

-- set language:cxx14
set_languages("cxx14")

-- Add target
target("DuiLib")
	local target_file_name = "DuiLib"
	
    -- set the kind of target through configuration
    set_kind("$(kind)")

	-- compile architecture configuration
	if is_arch("x64") then
		-- x64
		target_file_name = target_file_name .. "_64"
	elseif is_arch("win32") then
		-- x86
		target_file_name = target_file_name .. "_"
	end
	
	-- default character set
	if is_mode("unicode") then
		-- unicode
		add_defines("UNICODE", "_UNICODE")
		target_file_name = target_file_name .. "u"
	else
		-- other
	end

	if is_kind("static") then
		-- static library
		target_file_name = target_file_name .. "s"
        add_defines("UILIB_EXPORTS", "UILIB_STATIC")
    elseif is_kind("shared") then
		-- dynamic library
        add_defines("UILIB_EXPORTS")
    end
	
	-- Platform configuration
    if is_plat("windows") then
        add_defines("WIN32","WINDOWS")
		
		if is_kind("shared") then
			-- export all symbols for windows/dll
			-- add_rules("utils.symbols.export_all")
			add_rules("utils.symbols.export_all", {export_classes = true})
		end
	elseif is_plat("linux") then
	
	elseif is_plat("macosx") then
	
    end
	
	-- set the build modes (debug and release)
    if is_mode("debug") then
        -- Handle debug configuration
		-- Prohibit optimization
		set_optimize("none")
		target_file_name = target_file_name .. "d"
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
    
    -- add header files search paths
    add_includedirs(".")

    -- add precompiled header
    set_pcxxheader("./StdAfx.h") 
	

