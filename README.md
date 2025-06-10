## C++ Compiler Optimization Analyzer
This project demonstrates the tangible impact of compiler optimization levels on a simple C++ program. It compiles the same source code twice—once with optimizations disabled and once with a high level of optimization—and then analyzes the differences in both execution speed and the resulting machine code (assembly).

### What it Does
The core of this project is a C++ program that performs a computationally intensive (but simple) task: it initializes a large vector of doubles and calculates their sum. The program is designed to:

- Measure Execution Time: It times how long the main computational task takes to complete.

- Analyze Assembly Code: After being compiled, the program reads the assembly file that the compiler generated for itself.

- Count Instructions: It parses the assembly code to count the number of instructions specifically within the main computational function (run()).

- Display Results: It prints a clear, color-coded comparison showing the execution time and the number of assembly instructions, highlighting the effects of the compiler's optimization level.

This provides a direct and practical look at how optimizations like loop unrolling and instruction reordering can lead to faster and more efficient code.

### How it Works
The project uses CMake to automate the build and analysis process across different platforms (Linux, Windows) and compilers (GCC, Clang, MSVC).
It compiles the same source code with two different optimization levels: one with no optimizations (`-O0` or `/Od`) and another with high optimizations (`-O2` or `/O2`). After compilation, it generates assembly files for both versions, which are then analyzed by the C++ program itself.

The project uses a two-stage process orchestrated by `CMake` and the C++ executable itself to provide a focused analysis.

1.  **CMake Configuration & Assembly Generation (`CMakeLists.txt`)**:
    * It defines two separate executables, `OP_level_1` and `OP_level_2`, from the same `main.cpp` source.
    * It applies different compiler flags to each: `OP_level_1` gets **no optimizations** (`-O0` or `/Od`), while `OP_level_2` gets **high optimizations** (`-O2`).
    * Crucially, it adds a **post-build command**. After compilation, CMake automatically runs a platform-specific tool (`objdump` on Linux, `dumpbin` on Windows) to create a full disassembly of the executable. This entire assembly output is saved to a text file (e.g., `assembly_O0.txt`).

2.  **C++ Program & Focused Analysis (`main.cpp`)**:
    * The compiled C++ program is designed to analyze the assembly file generated in the previous step. It takes the path to this file as a command-line argument.
    * It first executes the core `run()` function and times its execution.
    * It then opens the corresponding assembly file and **parses its content**. The program intelligently locates the start of the `run()` function, correctly handling the different "mangled" names that compilers (like GCC, Clang, and MSVC) assign to functions.
    * Once it finds the function, it reads line by line, **counting only the assembly instructions within that specific function block**, until it detects the end of the function.
    * Finally, it prints the measured time and the precise instruction count for just the `run()` function.
This two-step process allows the project to focus on the performance and assembly characteristics of the `run()` function, providing a clear and direct comparison between the two optimization levels without cluttering the output with unrelated assembly code.
### Project Structure

### 3. GitHub Actions CI (cmake-multi-platform.yml):

- The repository includes a CI workflow that automatically builds and runs the analysis on Ubuntu (with GCC and Clang) and Windows (with MSVC).

- It configures, builds, and then executes both executables, passing the correct arguments to ensure the analysis runs correctly in the CI environment. This validates that the project works as expected across different systems.

### Example Output
When you run the project, the workflow will execute both the unoptimized and optimized versions. The output provides a clear comparison between the two.

```
Running OP_level_1...
Sum: 1e+08
Assembly Path:      analysis/assembly_O0.txt
Optimization Level: -O0
Time Taken:         0.541914 seconds

--- Assembly for run() ---
    22c0:	55                   	push   rbp
    22c1:	48 89 e5             	mov    rbp,rsp
    22c4:	48 81 ec 80 00 00 00 	sub    rsp,0x80
    22cb:	48 c7 45 d8 00 e1 f5 	mov    QWORD PTR [rbp-0x28],0x5f5e100
    22d2:	05 
    22d3:	48 b8 00 00 00 00 00 	movabs rax,0x3ff0000000000000
    22da:	00 f0 3f 
    22dd:	48 89 45 b8          	mov    QWORD PTR [rbp-0x48],ra
    234d:	48 83 c0 01          	add    rax,0x1
    2351:	48 89 45 90          	mov    QWORD PTR [rbp-0x70],rax
    2355:	e9 c6 ff ff ff       	jmp    2320 <_Z3runv+0x60>
    235a:	48 89 c1             	mov    rcx,rax
    235d:	89 d0                	mov    eax,edx
    235f:	48 89 4d a8          	mov    QWORD PTR [rbp-0x58],rcx
    2363:	89 45 a4             	mov    DWORD PTR [rbp-0x5c],eax
    2366:	48 8d 45 b7          	lea    rax,[rbp-0x49]
    236a:	48 89 45 f0          	mov    QWORD PTR [rbp-0x10],rax
    236e:	e9 71 00 00 00       	jmp    23e4 <_Z3runv+0x124>
    23db:	48 8d 7d c0          	lea    rdi,[rbp-0x40]
    23df:	e8 cc 08 00 00       	call   2cb0 <_ZNSt6vectorIdSaIdEED2Ev>
    23e4:	48 8b 7d a8          	mov    rdi,QWORD PTR [rbp-0x58]
    23e8:	e8 b3 fd ff ff       	call   21a0 <_Unwind_Resume@plt>
    23ed:	0f 1f 00             	nop    DWORD PTR [rax]
--- End of Assembly ---

Assembly Lines:     72 instructions
--------------------------------------------------
Running OP_level_2...
Sum: 1e+08
Assembly Path:      analysis/assembly_O2.txt
Optimization Level: -O2
Time Taken:         0.183425 seconds

--- Assembly for run() ---
    22b0:	41 57                	push   r15
    22b2:	41 56                	push   r14
    22b4:	53                   	push   rbx
    22b5:	48 83 ec 10          	sub    rsp,0x10
    22b9:	bf 00 08 af 2f       	mov    edi,0x2faf0800
    22be:	e8 1d fe ff ff       	call   20e0 <_Znwm@plt>
    22c3:	48 89 c3             	mov    rbx,rax
    22c6:	b8 12 00 00 00       	mov    eax,0x12
    22cb:	66 0f 28 05 3d 0d 00 	movapd xmm0,XMMWORD PTR [rip+0xd3d]        # 3010 <_IO_stdin_used+0x10>
    22d2:	00 
    22d3:	66 66 66 66 2e 0f 1f 	data16 data16 data16 cs nop WORD PTR [rax+rax*1+0x0]
    22da
    2319:	66 0f 11 04 c3       	movupd XMMWORD PTR [rbx+rax*8],xmm0
    231e:	48 83 c0 14          	add    rax,0x14
    2322:	48 3d 12 e1 f5 05    	cmp    rax,0x5f5e112
    2328:	75 b6                	jne    22e0 <_Z3runv+0x30>
    232a:	66 0f 57 c0          	xorpd  xmm0,xmm0
    232e:	b8 07 00 00 00       	mov    eax,0x7
    2333:	66 66 66 66 2e 0f 1f 	data16 data16 data16 cs nop WORD PTR [rax+rax*1+0x0]
    233a:	84 00 00 00 00 00 
    2340:	f2 0f 58 44 c3 c8    	addsd  xmm0,QWORD PTR [rbx+rax*8-0x38]
    2346:	f2 0f 58 44 c3 d0    	addsd  xmm0,QWORD PTR [rbx+rax*8-0x30]
    234c:	f2 0f 58 44 c3 d8    	addsd  xmm0,QWORD PTR [rbx+rax*8-0x28]
    2352:	f2 0f 58 44 c3 e0    	addsd  xmm0,QWORD PTR [rbx+rax*8-0x20]
    2358:	f2 0f 58 44 c3 e8    	addsd  xmm0,QWORD PTR [rbx+rax*8-0x18]
    235e:	f2 0f 58 44 c3 f0    	addsd  xmm0,QWORD PTR [rbx+rax*8-0x10]
    2364:	
    23f1:	48 89 c7             	mov    rdi,rax
    23f4:	e8 a7 fc ff ff       	call   20a0 <_ZNSo5flushEv@plt>
    23f9:	48 89 df             	mov    rdi,rbx
    23fc:	e8 cf fc ff ff       	call   20d0 <_ZdlPv@plt>
    2401:	31 c0                	xor    eax,eax
    2403:	48 83 c4 10          	add    rsp,0x10
    2407:	5b                   	pop    rbx
    2408:	41 5e                	pop    r14
    240a:	41 5f                	pop    r15
    240c:	c3                   	ret
    240d:	e8 1e fd ff ff       	call   2130 <_ZSt16__throw_bad_castv@plt>
    2412:	49 89 c6             	mov    r14,rax
    2415:	48 89 df             	mov    rdi,rbx
    2418:	e8 b3 fc ff ff       	call   20d0 <_ZdlPv@plt>
    241d:	4c 89 f7             	mov    rdi,r14
    2420:	e8 5b fd ff ff       	call   2180 <_Unwind_Resume@plt>
    2425:	66 66 2e 0f 1f 84 00 	data16 cs nop WORD PTR [rax+rax*1+0x0]
    242c:	00 00 00 00 
--- End of Assembly ---

Assembly Lines:     87 instructions
```

Note: The instruction count for -O2 may sometimes be higher due to techniques like loop unrolling, which increases code size but dramatically decreases execution time by reducing loop overhead and enabling parallel processing (SIMD).

### How to Build and Run
#### Prerequisites
- A C++ compiler (GCC, Clang, or MSVC)
- CMake (version 3.10 or higher)
- git (to clone the repository)

#### Steps
Clone the repository:

```bash
git clone <your-repository-url>
cd <repository-directory>
```

Configure the project using CMake:
This will create a build directory and generate the build files for your system.

```bash
cmake -B build
```
Build the executables:
This will compile main.cpp twice with the different optimization flags and run the post-build commands to generate the assembly files.

```bash
cmake --build build
```

Run the analysis:
The final executables need to be run from the build directory. You must provide the path to the assembly file and the optimization level string as arguments.

On Linux (or macOS):

# Run the unoptimized version

```bash
./build/OP_level_1 ./build/analysis/assembly_O0.txt "-O0"
```
# Run the optimized version
```bash
./build/OP_level_2 ./build/analysis/assembly_O2.txt "-O2"
```
On Windows (in a command prompt):

# Run the unoptimized version
```bash
.\build\Release\OP_level_1.exe .\build\analysis\assembly_Od.txt "/Od"

# Run the optimized version
.\build\Release\OP_level_2.exe .\build\analysis\assembly_O2.txt "/O2"
```
You will see the colored output in your terminal, comparing the performance and code size of the two optimization levels.