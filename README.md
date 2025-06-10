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

#### 1. CMake Configuration (CMakeLists.txt):

- It defines two separate executables, OP_level_1 and OP_level_2, both built from the same main.cpp source file.

- It applies different compiler flags to each target:

- OP_level_1 is compiled with no optimizations (-O0 on GCC/Clang or /Od on MSVC).

- OP_level_2 is compiled with high optimizations (-O2 on GCC/Clang or /O2 on MSVC).

- It adds a post-build step that uses platform-specific tools (objdump on Linux, dumpbin on Windows) to disassemble the executables and save the assembly code to text files (assembly_O0.txt and assembly_O2.txt).

### 2. C++ Program (main.cpp):

- When an executable is run, it accepts its corresponding assembly file path and optimization level as command-line arguments.

- It executes the run() function, which contains the vector summation loop, and times its execution.

- It then opens the specified assembly file, finds the run() function within it (handling compiler-specific name mangling), and counts the assembly instructions.

- Finally, it prints the measured time and instruction count.

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
    22dd:	48 89 45 b8          	mov    QWORD PTR [rbp-0x48],rax
    22e1:	48 8d 4d b7          	lea    rcx,[rbp-0x49]
    22e5:	48 89 4d e0          	mov    QWORD PTR [rbp-0x20],rcx
    22e9:	48 8b 45 e0          	mov    rax,QWORD PTR [rbp-0x20]
    22ed:	48 89 45 f8          	mov    QWORD PTR [rbp-0x8],rax
    22f1:	48 8d 7d c0          	lea    rdi,[rbp-0x40]
    22f5:	be 00 e1 f5 05       	mov    esi,0x5f5e100
    22fa:	48 8d 55 b8          	lea    rdx,[rbp-0x48]
    22fe:	e8 0d 09 00 00       	call   2c10 <_ZNSt6vectorIdSaIdEEC2EmRKdRKS0_>
    2303:	e9 00 00 00 00       	jmp    2308 <_Z3runv+0x48>
    2308:	48 8d 45 b7          	lea    rax,[rbp-0x49]
    230c:	48 89 45 e8          	mov    QWORD PTR [rbp-0x18],rax
    2310:	0f 57 c0             	xorps  xmm0,xmm0
    2313:	f2 0f 11 45 98       	movsd  QWORD PTR [rbp-0x68],xmm0
    2318:	48 c7 45 90 00 00 00 	mov    QWORD PTR [rbp-0x70],0x0
    231f:	00 
    2320:	48 81 7d 90 00 e1 f5 	cmp    QWORD PTR [rbp-0x70],0x5f5e100
    2327:	05 
    2328:	0f 83 45 00 00 00    	jae    2373 <_Z3runv+0xb3>
    232e:	48 8b 75 90          	mov    rsi,QWORD PTR [rbp-0x70]
    2332:	48 8d 7d c0          	lea    rdi,[rbp-0x40]
    2336:	e8 55 09 00 00       	call   2c90 <_ZNSt6vectorIdSaIdEEixEm>
    233b:	f2 0f 10 00          	movsd  xmm0,QWORD PTR [rax]
    233f:	f2 0f 58 45 98       	addsd  xmm0,QWORD PTR [rbp-0x68]
    2344:	f2 0f 11 45 98       	movsd  QWORD PTR [rbp-0x68],xmm0
    2349:	48 8b 45 90          	mov    rax,QWORD PTR [rbp-0x70]
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
    2373:	48 8b 3d 46 5c 00 00 	mov    rdi,QWORD PTR [rip+0x5c46]        # 7fc0 <_ZSt4cout@GLIBCXX_3.4>
    237a:	48 8d 35 b4 2c 00 00 	lea    rsi,[rip+0x2cb4]        # 5035 <_IO_stdin_used+0x35>
    2381:	e8 aa fd ff ff       	call   2130 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
    2386:	48 89 45 88          	mov    QWORD PTR [rbp-0x78],rax
    238a:	e9 00 00 00 00       	jmp    238f <_Z3runv+0xcf>
    238f:	48 8b 7d 88          	mov    rdi,QWORD PTR [rbp-0x78]
    2393:	f2 0f 10 45 98       	movsd  xmm0,QWORD PTR [rbp-0x68]
    2398:	e8 13 fe ff ff       	call   21b0 <_ZNSolsEd@plt>
    239d:	48 89 45 80          	mov    QWORD PTR [rbp-0x80],rax
    23a1:	e9 00 00 00 00       	jmp    23a6 <_Z3runv+0xe6>
    23a6:	48 8b 7d 80          	mov    rdi,QWORD PTR [rbp-0x80]
    23aa:	48 8b 35 ff 5b 00 00 	mov    rsi,QWORD PTR [rip+0x5bff]        # 7fb0 <_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@GLIBCXX_3.4>
    23b1:	e8 9a fd ff ff       	call   2150 <_ZNSolsEPFRSoS_E@plt>
    23b6:	e9 00 00 00 00       	jmp    23bb <_Z3runv+0xfb>
    23bb:	48 8d 7d c0          	lea    rdi,[rbp-0x40]
    23bf:	e8 ec 08 00 00       	call   2cb0 <_ZNSt6vectorIdSaIdEED2Ev>
    23c4:	31 c0                	xor    eax,eax
    23c6:	48 81 c4 80 00 00 00 	add    rsp,0x80
    23cd:	5d                   	pop    rbp
    23ce:	c3                   	ret
    23cf:	48 89 c1             	mov    rcx,rax
    23d2:	89 d0                	mov    eax,edx
    23d4:	48 89 4d a8          	mov    QWORD PTR [rbp-0x58],rcx
    23d8:	89 45 a4             	mov    DWORD PTR [rbp-0x5c],eax
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
    22da:	84 00 00 00 00 00 
    22e0:	66 0f 11 84 c3 70 ff 	movupd XMMWORD PTR [rbx+rax*8-0x90],xmm0
    22e7:	ff ff 
    22e9:	66 0f 11 44 c3 80    	movupd XMMWORD PTR [rbx+rax*8-0x80],xmm0
    22ef:	66 0f 11 44 c3 90    	movupd XMMWORD PTR [rbx+rax*8-0x70],xmm0
    22f5:	66 0f 11 44 c3 a0    	movupd XMMWORD PTR [rbx+rax*8-0x60],xmm0
    22fb:	66 0f 11 44 c3 b0    	movupd XMMWORD PTR [rbx+rax*8-0x50],xmm0
    2301:	66 0f 11 44 c3 c0    	movupd XMMWORD PTR [rbx+rax*8-0x40],xmm0
    2307:	66 0f 11 44 c3 d0    	movupd XMMWORD PTR [rbx+rax*8-0x30],xmm0
    230d:	66 0f 11 44 c3 e0    	movupd XMMWORD PTR [rbx+rax*8-0x20],xmm0
    2313:	66 0f 11 44 c3 f0    	movupd XMMWORD PTR [rbx+rax*8-0x10],xmm0
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
    2364:	f2 0f 58 44 c3 f8    	addsd  xmm0,QWORD PTR [rbx+rax*8-0x8]
    236a:	f2 0f 58 04 c3       	addsd  xmm0,QWORD PTR [rbx+rax*8]
    236f:	48 83 c0 08          	add    rax,0x8
    2373:	48 3d 07 e1 f5 05    	cmp    rax,0x5f5e107
    2379:	75 c5                	jne    2340 <_Z3runv+0x90>
    237b:	f2 0f 11 44 24 08    	movsd  QWORD PTR [rsp+0x8],xmm0
    2381:	48 8b 3d 30 2c 00 00 	mov    rdi,QWORD PTR [rip+0x2c30]        # 4fb8 <_ZSt4cout@GLIBCXX_3.4>
    2388:	48 8d 35 be 0c 00 00 	lea    rsi,[rip+0xcbe]        # 304d <_IO_stdin_used+0x4d>
    238f:	ba 05 00 00 00       	mov    edx,0x5
    2394:	e8 77 fd ff ff       	call   2110 <_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@plt>
    2399:	48 8b 3d 18 2c 00 00 	mov    rdi,QWORD PTR [rip+0x2c18]        # 4fb8 <_ZSt4cout@GLIBCXX_3.4>
    23a0:	f2 0f 10 44 24 08    	movsd  xmm0,QWORD PTR [rsp+0x8]
    23a6:	e8 95 fd ff ff       	call   2140 <_ZNSo9_M_insertIdEERSoT_@plt>
    23ab:	49 89 c6             	mov    r14,rax
    23ae:	48 8b 00             	mov    rax,QWORD PTR [rax]
    23b1:	48 8b 40 e8          	mov    rax,QWORD PTR [rax-0x18]
    23b5:	4d 8b bc 06 f0 00 00 	mov    r15,QWORD PTR [r14+rax*1+0xf0]
    23bc:	00 
    23bd:	4d 85 ff             	test   r15,r15
    23c0:	74 4b                	je     240d <_Z3runv+0x15d>
    23c2:	41 80 7f 38 00       	cmp    BYTE PTR [r15+0x38],0x0
    23c7:	74 07                	je     23d0 <_Z3runv+0x120>
    23c9:	41 0f b6 47 43       	movzx  eax,BYTE PTR [r15+0x43]
    23ce:	eb 16                	jmp    23e6 <_Z3runv+0x136>
    23d0:	4c 89 ff             	mov    rdi,r15
    23d3:	e8 48 fd ff ff       	call   2120 <_ZNKSt5ctypeIcE13_M_widen_initEv@plt>
    23d8:	49 8b 07             	mov    rax,QWORD PTR [r15]
    23db:	4c 89 ff             	mov    rdi,r15
    23de:	be 0a 00 00 00       	mov    esi,0xa
    23e3:	ff 50 30             	call   QWORD PTR [rax+0x30]
    23e6:	0f be f0             	movsx  esi,al
    23e9:	4c 89 f7             	mov    rdi,r14
    23ec:	e8 3f fc ff ff       	call   2030 <_ZNSo3putEc@plt>
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