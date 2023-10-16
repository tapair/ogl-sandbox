set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ogls_dependencies)

#=========================== glbindings ===========================#

set(OPTION_BUILD_TOOLS OFF)
set(OPTION_BUILD_EXAMPLES OFF)
set(OPTION_BUILD_OWN_KHR_HEADERS ON)
add_subdirectory(${ogls_dependencies_dir}/glbindings)

list(APPEND ogls_dependencies glbinding::glbinding)

#============================== SDL2 ==============================#

set(OPTION_TEST OFF)
set(SDL2_DISABLE_SDL2MAIN OFF)
add_subdirectory(${ogls_dependencies_dir}/SDL2)

list(APPEND ogls_dependencies SDL2::SDL2-static)
list(APPEND ogls_definitions SDL_MAIN_HANDLED=1)

#======================== ogl::dependencies =======================#

add_library(ogls_dependencies_lib INTERFACE)
add_library(ogls::dependencies ALIAS ogls_dependencies_lib)
target_link_libraries(ogls_dependencies_lib INTERFACE ${ogls_dependencies})
target_compile_definitions(ogls_dependencies_lib INTERFACE ${ogls_definitions} )

unset(ogls_dependencies)
