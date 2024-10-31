#pragma once
#ifndef HARSIMRAN_UTILS_H
#define HARSIMRAN_UTILS_H
#include <type_traits>
#include <string>
namespace project {
	class Utils {
	public:
		template<typename T> 
		T convertStringToType(const std::string& str) {
			if constexpr (std::is_same<T, int>::value) {
				return std::stoi(str);
			}
			else if constexpr (std::is_same<T, float>::value) {
				return std::stof(str);
			}
			else if constexpr (std::is_same<T, double>::value) {
				return std::stod(str);
			}
			else {
				return str;
			}
		}

	};
	extern Utils utils;
}
#endif