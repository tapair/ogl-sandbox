cmake_minimum_required(VERSION 3.20)
set(ogls_root ${CMAKE_CURRENT_SOURCE_DIR})

project( ogl-sandbox
	VERSION 0.1.0
	DESCRIPTION "OpenGL Sandbox"
	HOMEPAGE_URL htpps://github.com/tapair/ogl-sandbox
	LANGUAGES C CXX
)

include(${ogls_root}/cmake/options.cmake)

add_subdirectory(${ogls_dependencies_dir})
add_subdirectory(${ogls_target_dir})