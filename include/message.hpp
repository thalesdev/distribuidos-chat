#include <string>
#include "user.hpp"

#ifndef H_MESSAGE_UOL
#define H_MESSAGE_UOL
namespace UOL
{
	class Message
	{
	private:
		User *from;
		std::string content;

	public:
		Message(User *from, std::string content);
		~Message();
	};

} // namespace UOL
#endif