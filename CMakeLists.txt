cmake_minimum_required(VERSION 3.0)
project(log LANGUAGES C)

set(PROJECT_VERSION 1.0)
set(PROJECT_DESCRIPTION "Simple C logging framework for platform independent demos and games")

include(GNUInstallDirs)
include(FindPkgConfig)

set( INCLUDE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/log/log.h )
set( EXAMPLE_SOURCE_FILES examples/example.c )

add_compile_options(-march=native -O3)
add_compile_options(-DHAVE_GLCOREARB_H=1)
add_compile_options(-D_FILE_OFFSET_BITS=64)
add_compile_options(-fdata-sections)
add_compile_options(-fpermissive)
add_compile_options(-ffunction-sections)
add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Wpedantic)
add_compile_options(-Wnarrowing)
add_link_options(-Wl,--gc-sections)
add_link_options(-Wl,--print-gc-sections)
add_link_options(-Wl,-s)

set(install_prefix "${CMAKE_INSTALL_PREFIX}")
set(install_libdir "${CMAKE_INSTALL_LIBDIR}")
set(install_includedir "${CMAKE_INSTALL_INCLUDEDIR}")

configure_file(${PROJECT_NAME}.pc.in ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc @ONLY)

set(ENV{PKG_CONFIG_PATH} "${PROJECT_BINARY_DIR}:$ENV{PKG_CONFIG_PATH}")

add_executable( example ${EXAMPLE_SOURCE_FILES} )
target_include_directories(example PUBLIC
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}> )
set_target_properties( example PROPERTIES RUNTIME_OUTPUT_DIRECTORY
	${CMAKE_BINARY_DIR} )
add_test(example example)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/log DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT headers)

