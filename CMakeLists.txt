cmake_minimum_required(VERSION 3.10)
project(Optimization_levels)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create two separate executables from the same source
add_executable(OP_level_1 main.cpp)
add_executable(OP_level_2 main.cpp)

# Define the output directory for the analysis files
set(ANALYSIS_DIR "${CMAKE_BINARY_DIR}/analysis")
file(MAKE_DIRECTORY ${ANALYSIS_DIR})

if(MSVC)
    # Windows/MSVC specific flags
    # NOTE: I changed /O0 to /Od, which is the standard "Disable Optimizations" flag for MSVC
    target_compile_options(OP_level_1 PRIVATE /Od) 
    target_compile_options(OP_level_2 PRIVATE /O2)
else()
    # Linux/GCC/Clang specific flags
    target_compile_options(OP_level_1 PRIVATE -O0)
    target_compile_options(OP_level_2 PRIVATE -O2)
endif()


# --- CORRECTED POST-BUILD ANALYSIS ---

if(MSVC)
    # Create a separate target for each analysis file

    # Target to generate assembly for OP_level_1 (/Od)
    add_custom_target(dump_asm_O0 ALL
        COMMAND ${CMAKE_COMMAND} -E echo "Dumping assembly for /Od..."
        COMMAND cmd /c "dumpbin /DISASM \"${CMAKE_BINARY_DIR}\\OP_level_1.dir\\Release\\main.obj\" > \"${ANALYSIS_DIR}\\assembly_Od.txt\""
        DEPENDS OP_level_1
    )

    # Target to generate assembly for OP_level_2 (/O2)
    add_custom_target(dump_asm_O2 ALL
        COMMAND ${CMAKE_COMMAND} -E echo "Dumping assembly for /O2..."
        COMMAND cmd /c "dumpbin /DISASM \"${CMAKE_BINARY_DIR}\\OP_level_2.dir\\Release\\main.obj\" > \"${ANALYSIS_DIR}\\assembly_O2.txt\""
        DEPENDS OP_level_2
    )
else()
    # Create a separate custom command for each analysis file

    # Command to generate assembly for OP_level_1 (-O0)
    add_custom_command(
        OUTPUT ${ANALYSIS_DIR}/assembly_O0.txt
        COMMAND objdump -d -M intel "$<TARGET_FILE:OP_level_1>" > "${ANALYSIS_DIR}/assembly_O0.txt"
        DEPENDS OP_level_1
        COMMENT "Generating assembly for -O0..."
    )
    # A target is needed to drive the command
    add_custom_target(generate_asm_O0 ALL DEPENDS ${ANALYSIS_DIR}/assembly_O0.txt)


    # Command to generate assembly for OP_level_2 (-O2)
    add_custom_command(
        OUTPUT ${ANALYSIS_DIR}/assembly_O2.txt
        COMMAND objdump -d -M intel "$<TARGET_FILE:OP_level_2>" > "${ANALYSIS_DIR}/assembly_O2.txt"
        DEPENDS OP_level_2
        COMMENT "Generating assembly for -O2..."
    )
    # A target is needed to drive the command
    add_custom_target(generate_asm_O2 ALL DEPENDS ${ANALYSIS_DIR}/assembly_O2.txt)
endif()