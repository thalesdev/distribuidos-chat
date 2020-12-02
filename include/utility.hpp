#include <iostream> //  cout
#include <chrono>		// chrono::system_clock
#include <ctime>		// localtime
#include <sstream>	// stringstream
#include <iomanip>	// put_time
#include <string>		// string
#ifndef H_UTILITY_UOL
#define H_UTILITY_UOL
namespace UOL
{
	std::string bold_on = "\e[1m";

	std::string bold_off = "\e[0m";

	std::string current_time()
	{
		auto now = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t(now);

		std::stringstream ss;
		ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
		return ss.str();
	}

	std::string c_time()
	{
		return "[" + current_time() + "] ";
	}

	void clear()
	{
		// CSI[2J clears screen, CSI[H moves the cursor to top-left corner
		std::cout << "\x1B[2J\x1B[H";
	}

} // namespace UOL
#endif