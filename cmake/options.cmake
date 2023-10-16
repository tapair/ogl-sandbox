set(ogls_target triangle_demo CACHE STRING "Select the target for bild")
set_property(CACHE ogls_target PROPERTY STRINGS
	triangle_demo
)

set(ogls_definitions)
set(ogls_dependencies_dir ${ogls_root}/dependencies CACHE PATH "Dependence directory")
set(ogls_target_dir ${ogls_root}/code/${ogls_target} CACHE PATH "Target directory for build")
set(ogls_binary_dir ${ogls_root}/bin/${ogls_target} CACHE PATH "Binary directory for output")
mark_as_advanced(ogls_dependencies_dir ogls_target_dir ogls_binary_dir)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache) # Less useful to do it for linking, see edit2
endif(CCACHE_FOUND)

make_directory(${ogls_binary_dir})