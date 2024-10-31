//#pragma once
//#ifndef HARSIMRAN_SERIES_H
//#define HARSIMRAN_SERIES_H
//#include <iostream>
//#include <vector>
//#include <string>
//#include <optional>
//#include <fstream>
//#include <unordered_map>
//#include "Utils.h"
//namespace project {
//	template<typename T>
//	class Series {
//		std::vector<std::optional<T>> m_data{}; // can be present or empty (null values)
//		//std::vector<std::string> m_index{};\
//		//std::unordered_map<std::string, size_t> m_orderedMap{}; // for easy access
//		size_t m_size{};
//	public:
//		std::ostream& print(std::ostream & = std::cout) const;
//		Series() = default;
//		Series(const std::string fileName, char delimiter = ',', bool hasIndex = false); // 
//	};
//
//
//
//
//	template<typename T>
//	std::ostream& operator<<(std::ostream& ostr, const Series<T>& series) {
//		return series.print(ostr);
//	}
//	template<typename T>
//	inline std::ostream& Series<T>::print(std::ostream& ostr) const
//	{
//		for (size_t i = 0; i < m_size; i++) {
//			if (m_data[i].has_value()) {
//				std::cout << *m_data[i] << std::endl;
//			}
//			else {
//				std::cout << "NULL" << std::endl;
//			}
//		}
//		return ostr;
//	}
//	template<typename T>
//	inline Series<T>::Series(const std::string fileName, char delimiter, bool hasIndex) {
//		std::ifstream file(fileName);
//		if (!file.is_open()) {
//			std::cerr << "File failed to open: " << fileName << std::endl;
//			return;
//		}
//
//		std::string line;
//		std::string heading;
//		std::getline(file, heading); // Assuming the first line is a header, but not used.
//
//		while (std::getline(file, line)) {
//			m_size++;
//
//			//line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
//			//line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
//
//			if (line.empty()) {
//				m_data.emplace_back(std::nullopt);
//				//m_index.emplace_back(std::to_string(m_size - 1));
//				//m_orderedMap[m_index.back()] = m_size - 1;
//				continue;
//			}
//			size_t pos = hasIndex ? line.find(delimiter) : std::string::npos;
//			if (hasIndex && pos == std::string::npos) {
//				std::cerr << "Missing delimiter at line " << m_size << std::endl;
//				continue; // Skip this line
//			}
//
//			//std::string indexLabel = hasIndex ? line.substr(0, pos) : std::to_string(m_data.size());
//			std::string valueStr = hasIndex ? line.substr(pos + 1) : line;
//
//			//indexLabel.erase(0, indexLabel.find_first_not_of(" \t\n\r\f\v"));
//			//indexLabel.erase(indexLabel.find_last_not_of(" \t\n\r\f\v") + 1);
//			//valueStr.erase(0, valueStr.find_first_not_of(" \t\n\r\f\v"));
//			//valueStr.erase(valueStr.find_last_not_of(" \t\n\r\f\v") + 1);
//
//			if (valueStr.empty() || valueStr == "NULL") {
//				m_data.emplace_back(std::nullopt);
//			}
//			else {
//				try {
//					m_data.emplace_back(utils.convertStringToType<T>(valueStr));
//				}
//				catch (const std::exception& e) {
//					std::cerr << "Error converting value to Type T: " << e.what() << std::endl;
//					m_data.emplace_back(std::nullopt);
//				}
//			}
//
//			//if (m_orderedMap.find(indexLabel) != m_orderedMap.end()) {
//			//	std::cerr << "Duplicate index label '" << indexLabel << "' at line " << m_size << std::endl;
//			//}
//			//else {
//				//m_index.emplace_back(indexLabel);
//				//m_orderedMap[indexLabel] = m_data.size() - 1;
//			//}
//		}
//
//		file.close(); // Close the file
//	}
//}
//#endif
//
//
//#pragma once
//#ifndef HARSIMRAN_SERIES_H
//#define HARSIMRAN_SERIES_H
//
//#include <iostream>
//#include <vector>
//#include <string>
//#include <fstream>
//#include <charconv> // For std::from_chars (C++17)
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
//        Series(const std::string& fileName, char delimiter = ',', bool hasIndex = false);
//
//        std::ostream& print(std::ostream& ostr = std::cout) const;
//        size_t size() const { return m_data.size(); }
//        // Other member functions as needed...
//    };
//
//    // Overload the << operator for easy printing
//    template<typename T>
//    std::ostream& operator<<(std::ostream& ostr, const Series<T>& series) {
//        return series.print(ostr);
//    }
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
//    // Implementation of the constructor
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
//        std::vector<char> buffer(static_cast<size_t>(fileSize));
//        if (!file.read(buffer.data(), fileSize)) {
//            std::cerr << "Error reading file: " << fileName << std::endl;
//            return;
//        }
//        file.close();
//
//        // Reserve space to prevent reallocations
//        size_t estimatedLines = static_cast<size_t>(fileSize) / 20; // Adjust based on average line length
//        m_data.reserve(estimatedLines);
//
//        const char* ptr = buffer.data();
//        const char* end = ptr + buffer.size();
//
//        // Skip the header line if present
//        const char* lineStart = ptr;
//        const char* lineEnd = std::find(ptr, end, '\n');
//        ptr = lineEnd + 1; // Move to the start of the next line
//
//        while (ptr < end) {
//            lineStart = ptr;
//            lineEnd = std::find(ptr, end, '\n');
//
//            if (lineStart == lineEnd) {
//                // Empty line, treat as NULL
//                m_data.push_back(Entry{ T{}, true });
//                ptr = lineEnd + 1;
//                continue;
//            }
//
//            std::string_view line(lineStart, lineEnd - lineStart);
//
//            // If the line ends with '\r', remove it (handling Windows line endings)
//            if (!line.empty() && line.back() == '\r') {
//                line.remove_suffix(1);
//            }
//
//            std::string_view valueStr = line;
//
//            if (hasIndex) {
//                // Find the delimiter
//                size_t delimPos = line.find(delimiter);
//                if (delimPos == std::string_view::npos) {
//                    // Missing delimiter, skip this line
//                    ptr = lineEnd + 1;
//                    continue;
//                }
//                valueStr = line.substr(delimPos + 1);
//            }
//
//            // Trim whitespace if necessary
//            valueStr = trim(valueStr);
//
//            // Check for NULL
//            if (valueStr == "NULL" || valueStr.empty()) {
//                m_data.push_back(Entry{ T{}, true });
//            }
//            else {
//                Entry entry;
//                entry.isNull = false;
//
//                // Convert to value of type T
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
//
//                m_data.push_back(std::move(entry));
//            }
//
//            ptr = (lineEnd < end) ? lineEnd + 1 : end;
//        }
//    }
//
//} // namespace project
//
//#endif // HARSIMRAN_SERIES_H
#pragma once
#ifndef HARSIMRAN_SERIES_H
#define HARSIMRAN_SERIES_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <charconv>    // For std::from_chars (C++17)
#include <string_view>
#include <algorithm>
#include <cctype>

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
        Series(const std::string& fileName, char delimiter = ',', bool hasIndex = false);

        std::ostream& print(std::ostream& ostr = std::cout) const;
        size_t size() const { return m_data.size(); }
        // Other member functions as needed...
    };

    // Overload the << operator for easy printing
    template<typename T>
    std::ostream& operator<<(std::ostream& ostr, const Series<T>& series) {
        return series.print(ostr);
    }

    // Implementation of the print function
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

    // Helper function to trim whitespace (if needed)
    inline std::string_view trim(std::string_view sv) {
        while (!sv.empty() && std::isspace(static_cast<unsigned char>(sv.front()))) {
            sv.remove_prefix(1);
        }
        while (!sv.empty() && std::isspace(static_cast<unsigned char>(sv.back()))) {
            sv.remove_suffix(1);
        }
        return sv;
    }

    // Function to split the data into lines without copying
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

            // Remove '\r' if present (Windows line endings)
            if (!line.empty() && line.back() == '\r') {
                line.remove_suffix(1);
            }

            lines.emplace_back(line);
            ptr = lineEnd + 1;
        }
        return lines;
    }

    // Implementation of the constructor without multithreading
    template<typename T>
    Series<T>::Series(const std::string& fileName, char delimiter, bool hasIndex) {
        // Open the file
        std::ifstream file(fileName, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            std::cerr << "File failed to open: " << fileName << std::endl;
            return;
        }

        // Read the entire file into a buffer
        std::streamsize fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // Read file into buffer
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
            startLine = 1; // Assuming first line is header
        }

        size_t totalLines = lines.size() - startLine;
        m_data.reserve(totalLines);

        for (size_t i = startLine; i < lines.size(); ++i) {
            const std::string_view& line = lines[i];
            Entry entry;
            entry.isNull = false;

            std::string_view valueStr = line;

            if (hasIndex) {
                // Find the delimiter
                size_t delimPos = line.find(delimiter);
                if (delimPos == std::string_view::npos) {
                    // Missing delimiter, treat as NULL
                    entry.isNull = true;
                    m_data.push_back(entry);
                    continue;
                }
                valueStr = line.substr(delimPos + 1);
            }

            // Trim whitespace
            valueStr = trim(valueStr);

            // Check for NULL
            if (valueStr.empty() || valueStr == "NULL") {
                entry.isNull = true;
            }
            else {
                if constexpr (std::is_arithmetic_v<T>) {
                    // Numeric types
                    auto [p, ec] = std::from_chars(valueStr.data(), valueStr.data() + valueStr.size(), entry.value);
                    if (ec != std::errc()) {
                        // Conversion failed, treat as NULL
                        entry.isNull = true;
                    }
                }
                else if constexpr (std::is_same_v<T, std::string>) {
                    // For std::string, just assign the value
                    entry.value = std::string(valueStr);
                }
                else {
                    // For other types, attempt to construct from string
                    std::istringstream iss(std::string(valueStr));
                    if (!(iss >> entry.value)) {
                        // Conversion failed, treat as NULL
                        entry.isNull = true;
                    }
                }
            }

            m_data.push_back(std::move(entry));
        }
    }

} // namespace project

#endif // HARSIMRAN_SERIES_H
