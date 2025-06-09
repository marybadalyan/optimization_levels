#include <iostream>
#include <vector>
#include <chrono>

int main() {
    const size_t N = 100'000'000; // 100 million elements
    std::vector<double> data(N, 1.0); // initialize all elements to 1.0

    double sum = 0.0;

    auto start = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < N; ++i) {
        sum += data[i];
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Sum: " << sum << "\n";
    std::cout << "Time: " << elapsed.count() << " seconds\n";

    return 0;
}
