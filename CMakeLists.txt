cmake_minimum_required(VERSION 3.10)
project(Optimization_levels)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/O2)
else()
    add_compile_options(-O2)
endif()

set(DUMPBIN_EXE "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.38.33130/bin/Hostx64/x64/dumpbin.exe")

add_executable(Optimization_levels main.cpp)

# Post-build analysis per platform
if(MSVC)
    add_custom_target(dumpbin_info ALL
        COMMAND ${CMAKE_COMMAND} -E echo "Dumping PE + OBJ info (MSVC)"
        
        # Dumpbin on executable
        COMMAND cmd /c "dumpbin /DISASM \"$<TARGET_FILE:Optimization_levels>\" > \"${CMAKE_BINARY_DIR}/analysis/Optimization_levels_headers.txt\""
        DEPENDS Optimization_levels
    )
else()
    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/analysis/Optimization_levels_headers.txt
               ${CMAKE_BINARY_DIR}/analysis/Optimization_levels_symbols.txt
               ${CMAKE_BINARY_DIR}/analysis/Optimization_levels_size.txt
        COMMAND ${CMAKE_COMMAND} -E echo "Dumping ELF info (Unix)"
        COMMAND -d -M intel "$<TARGET_FILE:Optimization_levels>" > "${CMAKE_BINARY_DIR}/analysis/Optimization_levels_headers.txt"
        DEPENDS Optimization_levels

    )
endif()
