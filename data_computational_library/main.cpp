//#include <iostream>
//#include "Series.h"
//#include <chrono>
//using namespace project;
//int main() {
//	auto start = std::chrono::high_resolution_clock::now();
//	Series<int> s("large_test_file.csv");
//	auto end = std::chrono::high_resolution_clock::now();
//	//std::cout << s;
//	// Calculate the duration
//	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
//
//	std::cout << "Time taken: " << duration.count() << " microseconds" << std::endl;
//	return 0;
//}
#include <iostream>
#include "Series.h"
#include <chrono>

using namespace project;

int main() {
    // Record the start time
    auto start = std::chrono::high_resolution_clock::now();

    // Create a Series object by reading the CSV file
    Series<int> s("large_test_file.csv");

    // Record the end time
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the duration in seconds with decimal points
    std::chrono::duration<double> duration = end - start;

    // Output the duration
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;

    return 0;
}
