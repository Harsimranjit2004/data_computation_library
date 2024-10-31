#pragma once
#ifndef HARSIMRAN_SERIES_H
#define HARSIMRAN_SERIES_H
#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <fstream>
#include "Utils.h"
namespace project {
	template<typename T> 
	class Series {
		std::vector<std::optional<T>> m_data{}; // can we present or empty (null values)
		size_t m_size{};
	public:
		std::ostream& print(std::ostream & = std::cout) const;
		Series() = default;
		Series(const std::string fileName , char delimiter = ',' , bool hasIndex = false);
	};




	template<typename T> 
	std::ostream& operator<<(std::ostream& ostr, const Series<T>& series) {
		return series.print(ostr) ;
	}
	template<typename T>
	inline std::ostream& Series<T>::print(std::ostream& ostr) const
	{
		for (size_t i = 0; i < m_size; i++) {
			if (m_data[i].has_value()) {
				std::cout << *m_data[i] << std::endl;
			}
			else {
				std::cout << "NULL" << std::endl;
			}
		}
		return ostr;
	}
	template<typename T>
	inline Series<T>::Series(const std::string fileName, char delimiter, bool hasIndex)
	{
		std::ifstream file(fileName);

		if (!file.is_open()) {
			std::cerr << "File failed to open" << std::endl;
			return;
		}
		if (!hasIndex) {
			std::string line;
			while (std::getline(file, line)) {
				if (line.empty() || line == "NULL" || line == ",") {
					m_data.emplace_back(std::nullopt); 
				}
				else {
					m_data.emplace_back(utils.convertStringToType<T>(line)); 
				}
				m_size++;
			};
		}
		file.close();
		
	}
}
#endif