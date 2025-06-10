cmake_minimum_required(VERSION 3.10)
project(Optimization_levels)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



add_executable(OP_level_1 main.cpp)
add_executable(OP_level_2 main.cpp)

if(MSVC)
    # Windows/MSVC specific flags
    target_compile_options(OP_level_1 PRIVATE /Od)
    # target_compile_options(OP_level_2 PRIVATE /O2)
else()
    # Linux/GCC/Clang specific flags
    target_compile_options(OP_level_1 PRIVATE -O0)
    target_compile_options(OP_level_2 PRIVATE -O2)
endif()


# Post-build analysis per platform
if(MSVC)
    add_custom_target(dumpbin_info ALL
        COMMAND ${CMAKE_COMMAND} -E echo "Dumping PE + OBJ info (MSVC)"
        
        # Dumpbin on obj file
        COMMAND cmd /c "dumpbin /DISASM \"${CMAKE_BINARY_DIR}\\OP_level_1.dir\\Release\\main.obj\" > \"${CMAKE_BINARY_DIR}\\analysis\\assembly.txt\""
        DEPENDS OP_level_1
        COMMAND cmd /c "dumpbin /DISASM \"${CMAKE_BINARY_DIR}\\OP_level_2.dir\\Release\\main.obj\" > \"${CMAKE_BINARY_DIR}\\analysis\\assembly.txt\""
        DEPENDS OP_level_2
    )
else()
    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/analysis/assembly.txt
        COMMAND ${CMAKE_COMMAND} -E echo "Dumping ELF info (Unix)"
        COMMAND objdump -d -M intel "$<TARGET_FILE:OP_level_1>" > "${CMAKE_BINARY_DIR}/analysis/assembly.txt"
        DEPENDS OP_level_1
        COMMAND objdump -d -M intel "$<TARGET_FILE:OP_level_2>" > "${CMAKE_BINARY_DIR}/analysis/assembly.txt"
        DEPENDS OP_level_2

    )
endif()

