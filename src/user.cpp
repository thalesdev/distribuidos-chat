#include "../include/user.hpp"
namespace UOL
{
	User::User(string username, string password, string nickname) : username(username),
																																	password(password),
																																	nickname(nickname),
																																	ban(false)
	{
	}

	User::~User()
	{
	}
} // namespace UOL