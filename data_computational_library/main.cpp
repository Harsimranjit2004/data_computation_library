#include <iostream>
#include "Series.h"

using namespace project;
int main() {
	Series<int> s("series_data_no_index.csv");
	std::cout << s << std::endl;
	return 0;
}