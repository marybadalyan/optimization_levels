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

int run(){
    const size_t N = 100'000'000; // 100 million elements
    std::vector<double> data(N, 1.0); // initialize all elements to 1.0

    double sum = 0.0;
    for (size_t i = 0; i < N; ++i) {
            sum += data[i];
        }

    std::cout << "Sum: " << sum << "\n";

    return 0;
}

int main(int argc, char* argv[]) {
    
    auto start = std::chrono::high_resolution_clock::now();
    run();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;


    if (argc < 2) {
        std::cerr << "Error: Please provide the path to the assembly file as an argument.\n";
        return 1;
    }
    std::string assemblyFilePath = argv[1];
    std::string optLevel = argv[2]; 
    std::cout << Color::BOLD_CYAN << "Assembly Path:      " << assemblyFilePath << Color::RESET << std::endl;
    std::cout << Color::BOLD_MAGENTA << "Optimization Level: " << optLevel << Color::RESET << std::endl;
    std::cout << Color::BOLD_YELLOW << "Time: " << elapsed.count() << " seconds\n" << Color::RESET;

    std::ifstream asmFile(assemblyFilePath);
    if (!asmFile) {
        std::cerr << "Failed to open file\n";
        return 1;
    }

    std::string line;
    bool inRunFunc = false;
    int count = 0;
    while (std::getline(asmFile, line)) {
        if (!inRunFunc) {
            if (line.find("?run@@YAHXZ") != std::string::npos) { //?run@@YAHXZ
                inRunFunc = true;
                std::cout << "Found run() function assembly:\n";
                continue;
            }
        } else {
            if (line.empty() || line.find("???") != std::string::npos) {
                // end of function assembly
                break;
            }

            // print or parse the instruction line
            std::cout << line << "\n";
            count++;
        }
    }
    asmFile.close();

    std::cout << Color::BOLD_GREEN << "Total instructions in run() function compiled: " << count << "\n" << Color::RESET;

    return 0;
}
