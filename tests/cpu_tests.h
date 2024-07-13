#pragma once
#include <cstdint>
#include <string>
#include <chrono>
#include <iostream>


namespace tests
{
	// ROM loaders
	void load(std::uint8_t*, const std::string&);

	// Test Supervisor
	class Supervisor
	{
	protected:
		std::string _testname{ "" };
		std::uint64_t _expected_cycles{ 0 };
		std::uint64_t _expected_instructions{ 0 };
		std::chrono::steady_clock::time_point _start;
		std::chrono::steady_clock::time_point _end;
		std::uint64_t _cycles{ 0 };
		std::uint64_t _instructions{ 0 };

	public:
		Supervisor(const std::string& f, const uint64_t c, const uint64_t i)
			: _testname{ f.substr(f.find_last_of("/\\")+1) }, _expected_cycles{c}, _expected_instructions{i}
		{ }
		void start()
		{
			std::cout << std::endl << "*** TEST " << _testname << std::endl;
			_start = std::chrono::high_resolution_clock::now();
			_cycles = 0;
			_instructions = 0;
		}
		void inc_cycles() { _cycles++; }
		void inc_instructions() { _instructions++; }
		void stop()
		{
			_end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(_end - _start);
			std::cout << std::endl << "*** DURATION : " << duration.count() << "ms" << std::endl;
			bool success = true;

			std::cout << "*** " << _instructions << " INSTRUCTIONS IN " << _cycles << " CYCLES" << std::endl;
			if ((_expected_instructions > 0)&&(_instructions != _expected_instructions))
			{
				std::cout << "*** EXPECTED " << _expected_instructions << " INSTRUCTIONS" << std::endl;
				success = false;
			}
			if ((_expected_cycles > 0)&&(_cycles != _expected_cycles))
			{
				std::cout << "*** EXPECTED " << _expected_cycles << " CYCLES" << std::endl;
				success = false;
			}
			if (!success)
			{
				std::cout << "*** FAILED ***" << std::endl;
			}
			else
			{
				std::cout << "*** PASSED ***" << std::endl;
			}
		}
	};

	// CPU Tests
	bool i8080_run();
	bool sm83_run();
	bool z80_run();
}
