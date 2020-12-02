#include <vector>
#include <string>
#include "../include/room.hpp"
namespace UOL
{
	Room::Room(int capacity, int number) : capacity(capacity),
																				 number(number)
	{
		this->users = std::vector<User *>(capacity, nullptr);
		this->slug = "Sala (" + std::to_string(number) + ")";
	}

	Room::~Room()
	{
	}
} // namespace UOL