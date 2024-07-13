#include "cpu_tests.h"
#include <fstream>
#include <iostream>


namespace tests
{
	void load(uint8_t* memory, const std::string& filename)
	{
		std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
		if (!ifs)
			throw std::runtime_error("Unable to read " + filename);
		auto end = ifs.tellg();
		ifs.seekg(0, std::ios::beg);
		auto filesize = std::size_t(end - ifs.tellg());

		if (!ifs.read((char*)memory, filesize))
			throw std::runtime_error("Error during reading " + filename);
	}
}
