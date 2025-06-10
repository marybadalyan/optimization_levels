#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <string>

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

int main() {
    
    auto start = std::chrono::high_resolution_clock::now();
    run();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;


    std::cout << "Time: " << elapsed.count() << " seconds\n";

    std::ifstream asmFile("analysis\\assembly.txt");
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

    std::cout << "Total instructions in run() function compiled: " << count << "\n";

    return 0;
}
