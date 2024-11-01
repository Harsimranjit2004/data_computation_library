////#endif // HARSIMRAN_SERIES_H
#pragma once
#ifndef HARSIMRAN_SERIES_H
#define HARSIMRAN_SERIES_H
//
//#include <iostream>
//#include <vector>
//#include <string>
//#include <fstream>
//#include <charconv>    // For std::from_chars (C++17)
//#include <string_view>
//#include <algorithm>
//#include <cctype>
//
//namespace project {
//
//    template<typename T>
//    class Series {
//        struct Entry {
//            T value;
//            bool isNull;
//        };
//        std::vector<Entry> m_data;
//        
//    public:
//        Series() = default;
//        Series(const std::string& fileName, char delimiter = ',', bool hasIndex = false );
//
//        std::ostream& print(std::ostream& ostr = std::cout) const;
//        size_t size() const { return m_data.size(); }
//        
//
//
//
//
//
//    };
//
//
//
//    template<typename T>
//    std::ostream& operator<<(std::ostream& ostr, const Series<T>& series) {
//        return series.print(ostr);
//    }
//
//
//    // Implementation of the print function
//    template<typename T>
//    std::ostream& Series<T>::print(std::ostream& ostr) const {
//        for (const auto& entry : m_data) {
//            if (entry.isNull) {
//                ostr << "NULL" << std::endl;
//            }
//            else {
//                ostr << entry.value << std::endl;
//            }
//        }
//        return ostr;
//    }
//
//    // Helper function to trim whitespace (if needed)
//    inline std::string_view trim(std::string_view sv) {
//        while (!sv.empty() && std::isspace(static_cast<unsigned char>(sv.front()))) {
//            sv.remove_prefix(1);
//        }
//        while (!sv.empty() && std::isspace(static_cast<unsigned char>(sv.back()))) {
//            sv.remove_suffix(1);
//        }
//        return sv;
//    }
//
//    // Function to split the data into lines without copying
//    inline std::vector<std::string_view> splitLines(const char* data, size_t size) {
//        std::vector<std::string_view> lines;
//        const char* ptr = data;
//        const char* end = data + size;
//
//        while (ptr < end) {
//            const char* lineEnd = static_cast<const char*>(memchr(ptr, '\n', end - ptr));
//            if (!lineEnd) {
//                lineEnd = end;
//            }
//            std::string_view line(ptr, lineEnd - ptr);
//
//            // Remove '\r' if present (Windows line endings)
//            if (!line.empty() && line.back() == '\r') {
//                line.remove_suffix(1);
//            }
//
//            lines.emplace_back(line);
//            ptr = lineEnd + 1;
//        }
//        return lines;
//    }
//
//    template<typename T>
//    Series<T>::Series(const std::string& fileName, char delimiter, bool hasIndex) {
//        // Open the file
//        std::ifstream file(fileName, std::ios::binary | std::ios::ate);
//        if (!file.is_open()) {
//            std::cerr << "File failed to open: " << fileName << std::endl;
//            return;
//        }
//
//        // Read the entire file into a buffer
//        std::streamsize fileSize = file.tellg();
//        file.seekg(0, std::ios::beg);
//
//        // Read file into buffer
//        std::vector<char> buffer(static_cast<size_t>(fileSize));
//        if (!file.read(buffer.data(), fileSize)) {
//            std::cerr << "Error reading file: " << fileName << std::endl;
//            return;
//        }
//        file.close();
//
//        // Split buffer into lines
//        std::vector<std::string_view> lines = splitLines(buffer.data(), buffer.size());
//
//        // Skip header if present
//        size_t startLine = 0;
//        if (!lines.empty() && !lines[0].empty() && lines[0].find_first_not_of(", \t\r\n") != std::string_view::npos) {
//            startLine = 1; // Assuming first line is header
//        }
//
//        size_t totalLines = lines.size() - startLine;
//        m_data.reserve(totalLines);
//
//        for (size_t i = startLine; i < lines.size(); ++i) {
//            const std::string_view& line = lines[i];
//            Entry entry;
//            entry.isNull = false;
//
//            std::string_view valueStr = line;
//
//            if (hasIndex) {
//                // Find the delimiter
//                size_t delimPos = line.find(delimiter);
//                if (delimPos == std::string_view::npos) {
//                    // Missing delimiter, treat as NULL
//                    entry.isNull = true;
//                    m_data.push_back(entry);
//                    continue;
//                }
//                valueStr = line.substr(delimPos + 1);
//            }
//
//            // Trim whitespace
//            valueStr = trim(valueStr);
//
//            // Check for NULL
//            if (valueStr.empty() || valueStr == "NULL") {
//                entry.isNull = true;
//            }
//            else {
//                if constexpr (std::is_arithmetic_v<T>) {
//                    // Numeric types
//                    auto [p, ec] = std::from_chars(valueStr.data(), valueStr.data() + valueStr.size(), entry.value);
//                    if (ec != std::errc()) {
//                        // Conversion failed, treat as NULL
//                        entry.isNull = true;
//                    }
//                }
//                else if constexpr (std::is_same_v<T, std::string>) {
//                    // For std::string, just assign the value
//                    entry.value = std::string(valueStr);
//                }
//                else {
//                    // For other types, attempt to construct from string
//                    std::istringstream iss(std::string(valueStr));
//                    if (!(iss >> entry.value)) {
//                        // Conversion failed, treat as NULL
//                        entry.isNull = true;
//                    }
//                }
//            }
//
//            m_data.push_back(std::move(entry));
//        }
//    }
//    /// hi
//
//} // namespace project
//
//#endif // HARSIMRAN_SERIES_H
////#include <iostream>
////#include <vector>
////#include <string>
////#include <fstream>
////#include <charconv>  // For std::from_chars (C++17)
////#include <string_view>
////#include <algorithm>
////#include <cctype>
////#include <thread>
////#include <future>
////#include <mutex>
////
////namespace project {
////
////    template<typename T>
////    class Series {
////        struct Entry {
////            T value;
////            bool isNull;
////        };
////        std::vector<Entry> m_data;
////        std::mutex data_mutex; // Mutex to protect shared data
////
////    public:
////        Series() = default;
////        Series(const std::string& fileName, char delimiter = ',', bool hasIndex = false, size_t numThreads = 3);
////
////        std::ostream& print(std::ostream& ostr = std::cout) const;
////        size_t size() const { return m_data.size(); }
////    };
////
////    template<typename T>
////    std::ostream& operator<<(std::ostream& ostr, const Series<T>& series) {
////        return series.print(ostr);
////    }
////
////    template<typename T>
////    std::ostream& Series<T>::print(std::ostream& ostr) const {
////        for (const auto& entry : m_data) {
////            if (entry.isNull) {
////                ostr << "NULL" << std::endl;
////            }
////            else {
////                ostr << entry.value << std::endl;
////            }
////        }
////        return ostr;
////    }
////
////    inline std::string_view trim(std::string_view sv) {
////        while (!sv.empty() && std::isspace(static_cast<unsigned char>(sv.front()))) {
////            sv.remove_prefix(1);
////        }
////        while (!sv.empty() && std::isspace(static_cast<unsigned char>(sv.back()))) {
////            sv.remove_suffix(1);
////        }
////        return sv;
////    }
////
////    inline std::vector<std::string_view> splitLines(const char* data, size_t size) {
////        std::vector<std::string_view> lines;
////        const char* ptr = data;
////        const char* end = data + size;
////
////        while (ptr < end) {
////            const char* lineEnd = static_cast<const char*>(memchr(ptr, '\n', end - ptr));
////            if (!lineEnd) {
////                lineEnd = end;
////            }
////            std::string_view line(ptr, lineEnd - ptr);
////
////            if (!line.empty() && line.back() == '\r') {
////                line.remove_suffix(1);
////            }
////
////            lines.emplace_back(line);
////            ptr = lineEnd + 1;
////        }
////        return lines;
////    }
////
////    template<typename T>
////    void processChunk(const std::vector<std::string_view>& lines, size_t start, size_t end, char delimiter, bool hasIndex, std::vector<typename Series<T>::Entry>& chunkData, std::mutex& data_mutex) {
////        for (size_t i = start; i < end; ++i) {
////            const std::string_view& line = lines[i];
////            typename Series<T>::Entry entry;
////            entry.isNull = false;
////
////            std::string_view valueStr = line;
////            if (hasIndex) {
////                size_t delimPos = line.find(delimiter);
////                if (delimPos == std::string_view::npos) {
////                    entry.isNull = true;
////                    std::lock_guard<std::mutex> lock(data_mutex);
////                    chunkData.push_back(entry);
////                    continue;
////                }
////                valueStr = line.substr(delimPos + 1);
////            }
////
////            valueStr = trim(valueStr);
////            if (valueStr.empty() || valueStr == "NULL") {
////                entry.isNull = true;
////            }
////            else {
////                if constexpr (std::is_arithmetic_v<T>) {
////                    auto [p, ec] = std::from_chars(valueStr.data(), valueStr.data() + valueStr.size(), entry.value);
////                    if (ec != std::errc()) {
////                        entry.isNull = true;
////                    }
////                }
////                else if constexpr (std::is_same_v<T, std::string>) {
////                    entry.value = std::string(valueStr);
////                }
////                else {
////                    std::istringstream iss(std::string(valueStr));
////                    if (!(iss >> entry.value)) {
////                        entry.isNull = true;
////                    }
////                }
////            }
////            std::lock_guard<std::mutex> lock(data_mutex);
////            chunkData.push_back(std::move(entry));
////        }
////    }
////
////    template<typename T>
////    Series<T>::Series(const std::string& fileName, char delimiter, bool hasIndex, size_t numThreads) {
////        // Open the file
////        std::ifstream file(fileName, std::ios::binary | std::ios::ate);
////        if (!file.is_open()) {
////            std::cerr << "File failed to open: " << fileName << std::endl;
////            return;
////        }
////
////        // Read the entire file into a buffer
////        std::streamsize fileSize = file.tellg();
////        file.seekg(0, std::ios::beg);
////
////        std::vector<char> buffer(static_cast<size_t>(fileSize));
////        if (!file.read(buffer.data(), fileSize)) {
////            std::cerr << "Error reading file: " << fileName << std::endl;
////            return;
////        }
////        file.close();
////
////        // Split buffer into lines
////        std::vector<std::string_view> lines = splitLines(buffer.data(), buffer.size());
////
////        // Skip header if present
////        size_t startLine = 0;
////        if (!lines.empty() && !lines[0].empty() && lines[0].find_first_not_of(", \t\r\n") != std::string_view::npos) {
////            startLine = 1; // Assuming the first line is a header
////        }
////
////        size_t totalLines = lines.size() - startLine;
////        m_data.reserve(totalLines);
////
////        // Calculate the chunk size
////        size_t chunkSize = (totalLines + numThreads - 1) / numThreads;
////        std::vector<std::future<void>> futures;
////
////        // Process each chunk in a separate thread
////        for (size_t i = 0; i < numThreads; ++i) {
////            size_t start = startLine + i * chunkSize;
////            size_t end = std::min(startLine + (i + 1) * chunkSize, lines.size());
////
////            // Lambda function for processing each chunk
////            futures.push_back(std::async(std::launch::async, [&, start, end]() {
////                for (size_t j = start; j < end; ++j) {
////                    const std::string_view& line = lines[j];
////                    Entry entry;
////                    entry.isNull = false;
////
////                    std::string_view valueStr = line;
////                    if (hasIndex) {
////                        size_t delimPos = line.find(delimiter);
////                        if (delimPos == std::string_view::npos) {
////                            entry.isNull = true;
////                            std::lock_guard<std::mutex> lock(data_mutex);
////                            m_data.push_back(entry);
////                            continue;
////                        }
////                        valueStr = line.substr(delimPos + 1);
////                    }
////
////                    valueStr = trim(valueStr);
////                    if (valueStr.empty() || valueStr == "NULL") {
////                        entry.isNull = true;
////                    }
////                    else {
////                        if constexpr (std::is_arithmetic_v<T>) {
////                            auto [p, ec] = std::from_chars(valueStr.data(), valueStr.data() + valueStr.size(), entry.value);
////                            if (ec != std::errc()) {
////                                entry.isNull = true;
////                            }
////                        }
////                        else if constexpr (std::is_same_v<T, std::string>) {
////                            entry.value = std::string(valueStr);
////                        }
////                        else {
////                            std::istringstream iss(std::string(valueStr));
////                            if (!(iss >> entry.value)) {
////                                entry.isNull = true;
////                            }
////                        }
////                    }
////
////                    // Safely add the entry to m_data
////                    {
////                        std::lock_guard<std::mutex> lock(data_mutex);
////                        m_data.push_back(std::move(entry));
////                    }
////                }
////                }));
////        }
////
////        // Wait for all threads to complete
////        for (auto& future : futures) {
////            future.get();
////        }
////    }
////
////
////} // namespace project
////
////#endif // HARSIMRAN_SERIES_H
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <charconv>  // For std::from_chars (C++17)
#include <string_view>
#include <algorithm>
#include <cctype>
#include <thread>
#include <future>
#include <mutex>

namespace project {

    template<typename T>
    class Series {
        struct Entry {
            T value;
            bool isNull;
        };
        std::vector<Entry> m_data;

    public:
        Series() = default;
        Series(const std::string& fileName, char delimiter = ',', bool hasIndex = false, size_t numThreads = 4);

        std::ostream& print(std::ostream& ostr = std::cout) const;
        size_t size() const { return m_data.size(); }
    };

    template<typename T>
    std::ostream& operator<<(std::ostream& ostr, const Series<T>& series) {
        return series.print(ostr);
    }

    template<typename T>
    std::ostream& Series<T>::print(std::ostream& ostr) const {
        for (const auto& entry : m_data) {
            if (entry.isNull) {
                ostr << "NULL" << std::endl;
            }
            else {
                ostr << entry.value << std::endl;
            }
        }
        return ostr;
    }

    inline std::string_view trim(std::string_view sv) {
        while (!sv.empty() && std::isspace(static_cast<unsigned char>(sv.front()))) {
            sv.remove_prefix(1);
        }
        while (!sv.empty() && std::isspace(static_cast<unsigned char>(sv.back()))) {
            sv.remove_suffix(1);
        }
        return sv;
    }

    inline std::vector<std::string_view> splitLines(const char* data, size_t size) {
        std::vector<std::string_view> lines;
        const char* ptr = data;
        const char* end = data + size;

        while (ptr < end) {
            const char* lineEnd = static_cast<const char*>(memchr(ptr, '\n', end - ptr));
            if (!lineEnd) {
                lineEnd = end;
            }
            std::string_view line(ptr, lineEnd - ptr);

            if (!line.empty() && line.back() == '\r') {
                line.remove_suffix(1);
            }

            lines.emplace_back(line);
            ptr = lineEnd + 1;
        }
        return lines;
    }

    template<typename T>
    void processChunk(const std::vector<std::string_view>& lines, size_t start, size_t end, char delimiter, bool hasIndex, std::vector<typename Series<T>::Entry>& localBuffer) {
        for (size_t i = start; i < end; ++i) {
            const std::string_view& line = lines[i];
            typename Series<T>::Entry entry;
            entry.isNull = false;

            std::string_view valueStr = line;
            if (hasIndex) {
                size_t delimPos = line.find(delimiter);
                if (delimPos == std::string_view::npos) {
                    entry.isNull = true;
                    localBuffer.push_back(entry);
                    continue;
                }
                valueStr = line.substr(delimPos + 1);
            }

            valueStr = trim(valueStr);
            if (valueStr.empty() || valueStr == "NULL") {
                entry.isNull = true;
            }
            else {
                if constexpr (std::is_arithmetic_v<T>) {
                    auto [p, ec] = std::from_chars(valueStr.data(), valueStr.data() + valueStr.size(), entry.value);
                    if (ec != std::errc()) {
                        entry.isNull = true;
                    }
                }
                else if constexpr (std::is_same_v<T, std::string>) {
                    entry.value = std::string(valueStr);
                }
                else {
                    std::istringstream iss(std::string(valueStr));
                    if (!(iss >> entry.value)) {
                        entry.isNull = true;
                    }
                }
            }
            localBuffer.push_back(std::move(entry));
        }
    }

    template<typename T>
    Series<T>::Series(const std::string& fileName, char delimiter, bool hasIndex, size_t numThreads) {
        // Open the file
        std::ifstream file(fileName, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            std::cerr << "File failed to open: " << fileName << std::endl;
            return;
        }

        // Read the entire file into a buffer
        std::streamsize fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> buffer(static_cast<size_t>(fileSize));
        if (!file.read(buffer.data(), fileSize)) {
            std::cerr << "Error reading file: " << fileName << std::endl;
            return;
        }
        file.close();

        // Split buffer into lines
        std::vector<std::string_view> lines = splitLines(buffer.data(), buffer.size());

        // Skip header if present
        size_t startLine = 0;
        if (!lines.empty() && !lines[0].empty() && lines[0].find_first_not_of(", \t\r\n") != std::string_view::npos) {
            startLine = 1; // Assuming the first line is a header
        }

        size_t totalLines = lines.size() - startLine;
        m_data.reserve(totalLines);

        // Calculate the chunk size
        size_t chunkSize = (totalLines + numThreads - 1) / numThreads;
        std::vector<std::future<std::vector<Entry>>> futures;

        // Process each chunk in a separate thread
        for (size_t i = 0; i < numThreads; ++i) {
            size_t start = startLine + i * chunkSize;
            size_t end = std::min(startLine + (i + 1) * chunkSize, lines.size());

            // Lambda function for processing each chunk
            futures.push_back(std::async(std::launch::async, [&, start, end]() -> std::vector<Entry> {
                std::vector<Entry> localBuffer;
                localBuffer.reserve(end - start);

                for (size_t j = start; j < end; ++j) {
                    const std::string_view& line = lines[j];
                    Entry entry;
                    entry.isNull = false;

                    std::string_view valueStr = line;
                    if (hasIndex) {
                        size_t delimPos = line.find(delimiter);
                        if (delimPos == std::string_view::npos) {
                            entry.isNull = true;
                            localBuffer.push_back(entry);
                            continue;
                        }
                        valueStr = line.substr(delimPos + 1);
                    }

                    valueStr = trim(valueStr);
                    if (valueStr.empty() || valueStr == "NULL") {
                        entry.isNull = true;
                    }
                    else {
                        if constexpr (std::is_arithmetic_v<T>) {
                            auto [p, ec] = std::from_chars(valueStr.data(), valueStr.data() + valueStr.size(), entry.value);
                            if (ec != std::errc()) {
                                entry.isNull = true;
                            }
                        }
                        else if constexpr (std::is_same_v<T, std::string>) {
                            entry.value = std::string(valueStr);
                        }
                        else {
                            std::istringstream iss(std::string(valueStr));
                            if (!(iss >> entry.value)) {
                                entry.isNull = true;
                            }
                        }
                    }
                    localBuffer.push_back(std::move(entry));
                }
                return localBuffer;
                }));
        }

        // Collect results from all threads
        for (auto& future : futures) {
            auto localBuffer = future.get();
            m_data.insert(m_data.end(), std::make_move_iterator(localBuffer.begin()), std::make_move_iterator(localBuffer.end()));
        }
    }


} // namespace project

#endif // HARSIMRAN_SERIES_H
