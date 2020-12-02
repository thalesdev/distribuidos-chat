#include <string>
#ifndef HPP_USER_CHAT_UOL
#define HPP_USER_CHAT_UOL
using std::string;

namespace UOL
{
	class User
	{
	private:
		string username;
		string password;
		string nickname;
		bool ban;

	public:
		User(string username, string password, string nickname);
		~User();
	};
} // namespace UOL

#endif