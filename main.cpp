#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <string>

namespace Color {
    const char* RESET = "\033[0m";
    const char* BOLD_CYAN = "\033[1;36m";
    const char* BOLD_YELLOW = "\033[1;33m";
    const char* BOLD_GREEN = "\033[1;32m";
    const char* BOLD_MAGENTA = "\033[1;35m";
}

int run() {
    const size_t N = 100'000'000;
    std::vector<double> data(N, 1.0);
    double sum = 0.0;
    for (size_t i = 0; i < N; ++i) {
        sum += data[i];
    }
    std::cout << "Sum: " << sum << std::endl;
    return 0;
}

int main(int argc, char* argv[]) {
    // --- 1. Argument Handling (FIXED) ---
    if (argc < 3) { // Check for 2 arguments + program name
        std::cerr << "Error: Please provide the assembly file path AND optimization level as arguments.\n";
        return 1;
    }
    std::string assemblyFilePath = argv[1];
    std::string optLevel = argv[2];

    // --- 2. Timed Execution ---
    auto start = std::chrono::high_resolution_clock::now();
    run();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // --- 3. Analysis and Colored Output (FIXED) ---
    std::cout << Color::BOLD_CYAN << "Assembly Path:      " << assemblyFilePath << Color::RESET << std::endl;
    std::cout << Color::BOLD_MAGENTA << "Optimization Level: " << optLevel <<   Color::RESET << std::endl;
    std::cout << Color::BOLD_YELLOW << "Time Taken:         " << elapsed.count() << " seconds" << Color::RESET <<  std::endl;

    std::ifstream asmFile(assemblyFilePath);
    if (!asmFile) {
        std::cerr << "Failed to open file\n";
        return 1;
    }

    // --- 4. Platform-Independent Parsing (FIXED) ---
    #if defined(_MSC_VER)
        const std::string mangledRunName = "?run@@YAHXZ"; // MSVC
    #else
        const std::string mangledRunName = "_Z3runv";     // GCC/Clang
    #endif

    // ... inside main(), after opening the file ...

    std::string line;
    bool inRunFunc = false;
    int count = 0;
    while (std::getline(asmFile, line)) {
        if (!inRunFunc) {
            if (line.find(mangledRunName) != std::string::npos) {
                inRunFunc = true;
                std::cout << "\n--- Assembly for run() ---" << std::endl; // Header for the assembly
            }
        } else {
            // Check for the end of the function
            if (line.empty() || line.find("ENDP") != std::string::npos || line.find(".cfi_endproc") != std::string::npos) {
                break;
            }

            // Simple check to only count and print lines with instructions (indented)
            if (!line.empty() && (line[0] == '\t' || line[0] == ' ')) {
                 std::cout << line << std::endl; // <<< THIS LINE IS NOW RE-ADDED
                 count++;
            }
        }
    }
    asmFile.close();

    std::cout << "--- End of Assembly ---\n" << std::endl;

    std::cout << Color::BOLD_GREEN << "Assembly Lines:     " << count <<  " instructions" << Color::RESET <<  std::endl;

    return 0;
}