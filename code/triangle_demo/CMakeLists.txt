cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp")
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "*.hpp")

add_executable(${ogls_target} ${sources} ${headers})
set_target_properties(${ogls_target} PROPERTIES
	C_STANDARD 11
	C_STANDARD_REQUIRED ON
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON

	RUNTIME_OUTPUT_DIRECTORY ${ogls_binary_dir}
	LIBRARY_OUTPUT_DIRECTORY ${ogls_binary_dir}
	ARCHIVE_OUTPUT_DIRECTORY ${ogls_binary_dir}
)

#add_compile_definitions(${ogls_target} PUBLIC ${ogls_definitions})
#target_link_libraries((${ogls_target} PUBLIC ogls_dependendencies)