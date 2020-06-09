cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project("onlyoneheader" DESCRIPTION "Create only one header from multiple headers" VERSION 0.0.1 LANGUAGES CXX)

include(CTest)

set(EXENAME onlyoneheader)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
	add_definitions(-D_UNICODE -DUNICODE -DWIN32_LEAN_AND_MEAN -DNOMINMAX -DWINVER=0x0A00 -D_WIN32_WINNT=0x0A00)
	add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
	add_definitions(-D_ATL_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)
endif()
		
file(GLOB_RECURSE PROJ_SRC_FILES
	CONFIGURE_DEPENDS
	src/*.c* src/*.h*
)

#To properly set the files in the treeview for MSVC
foreach(source IN LISTS PROJ_SRC_FILES)
	get_filename_component(source_path "${source}" PATH)
	string(REPLACE "${PROJECT_SOURCE_DIR}/" "" source_path_msvc "${source_path}")
	source_group("${source_path_msvc}" FILES "${source}")
endforeach()

add_executable(
	${EXENAME}
	${PROJ_SRC_FILES}
)

target_compile_options(${EXENAME}
 PRIVATE
	$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
		-Werror -Wall -Wextra>
	$<$<CXX_COMPILER_ID:MSVC>:
		/W4>
)

find_package(docopt COMPONENTS CXX REQUIRED)
include_directories(${DOCOPT_INCLUDE_DIRS})
target_link_libraries(${EXENAME} docopt)


enable_testing()

file(GLOB_RECURSE TEST_FILES 
	CONFIGURE_DEPENDS 
	${PROJECT_SOURCE_DIR}/tests/*.cpp ${PROJECT_SOURCE_DIR}/tests/*.h
)
 
add_executable(tests
	${TEST_FILES}
)

target_include_directories(tests
	PUBLIC
		tests/
		src/
)

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(tests 
	PRIVATE 
		Catch2::Catch2
		${LIBNAME}
)

