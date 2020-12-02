#include <vector>
#include <string>
#include <queue>
#include "user.hpp"
#include "message.hpp"

#ifndef H_ROOM_CHAT_UOL
#define H_ROOM_CHAT_UOL
namespace UOL
{
	class Room
	{
	private:
		std::vector<User *> users;
		int number;
		int capacity;
		std::string slug;
		std::queue<Message *> queue;

	public:
		Room(int capacity, int number);
		~Room();
	};
} // namespace UOL

#endif
