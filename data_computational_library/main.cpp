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
////}
//#include <iostream>
//#include "Series.h"
//#include <chrono>
//
//using namespace project;
//
//int main() {
//    // Record the start time
//    auto start = std::chrono::high_resolution_clock::now();
//
//    // Create a Series object by reading the CSV file
//    Series<double> s("large_test_file.csv", ',',true);
//
//    // Record the end time
//    auto end = std::chrono::high_resolution_clock::now();
//
//    // Calculate the duration in seconds with decimal points
//    std::chrono::duration<double> duration = end - start;
//    //std::cout << s;
//    // Output the duration
//    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
//
//    return 0;
//}
//
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <random>
//#include <iomanip>
//
//void generateLargeFile(const std::string& fileName, size_t numLines, size_t numColumns = 2) {
//    std::ofstream file(fileName);
//    if (!file.is_open()) {
//        std::cerr << "Failed to open file for writing: " << fileName << std::endl;
//        return;
//    }
//
//    // Random number generator for numeric values
//    std::random_device rd;
//    std::mt19937 gen(rd());
//    std::uniform_real_distribution<> dis(1.0, 1000.0);
//
//    // Write header
//    file << "Index";
//    for (size_t i = 1; i <= numColumns - 1; ++i) {
//        file << ",Value" << i;
//    }
//    file << "\n";
//
//    // Generate lines with random values
//    for (size_t i = 1; i <= numLines; ++i) {
//        file << i;  // Index column
//        for (size_t j = 1; j <= numColumns - 1; ++j) {
//            file << "," << std::fixed << std::setprecision(2) << dis(gen);  // Random value with 2 decimal places
//        }
//        file << "\n";
//
//        // Optionally print progress for large files
//        if (i % 1000000 == 0) {
//            std::cout << "Generated " << i << " lines..." << std::endl;
//        }
//    }
//
//    file.close();
//    std::cout << "File generation complete: " << fileName << std::endl;
//}
//
//int main() {
//    std::string fileName = "large_test_file.csv";
//    size_t numLines = 20000000;  // Number of lines to generate
//    size_t numColumns = 2;       // Number of columns (including index)
//
//    generateLargeFile(fileName, numLines, numColumns);
//
//    return 0;
//}
