#pragma once
#ifndef HARSIMRAN_DATAFRAME_H
#define HARSIMRAN_DATAFRAME_H
#include <string>
namespace project {
	class DataFrame {
		
	public:

		DataFrame() = default;
		DataFrame(const std::string fileName);
	};
}
#endif
