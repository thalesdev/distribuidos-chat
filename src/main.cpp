#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <queue>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>	 // pthread
#include <chrono>			 // time library
#include <iomanip>		 //put_time
#include <ctime>			 // localtime
#include <sstream>		 // stringstream
#include <thread>			 // pthread
#include <arpa/inet.h> // socket
#include <mutex>			 // mutex for socket
#include <map>				 // stl data struct
#include "../include/server.hpp"
#include "../include/utility.hpp"
#define PORT 5000
#define BUFFER_SIZE 1024
#define SERVER_ADDRESS "127.0.0.1"

using namespace std;

// clientID, message
typedef pair<int, string> message;

map<string, int> users;
map<int, pthread_t> threads;
queue<message> message_queue;
pthread_t thread_broadcasting;
pthread_t thread_client_read_message;
std::mutex queue_mutex;
std::mutex users_mutex;

using UOL::bold_off;
using UOL::bold_on;
using UOL::c_time;
using UOL::clear;
using UOL::current_time;

void *readMessages(void *clientId)
{
	char buffer[BUFFER_SIZE] = {0};
	long clientID = (long)clientId;
	while (true)
	{
		if (recv(clientID, buffer, BUFFER_SIZE, 0) > 0)
		{
			cout << "[" << current_time() << "] ";
			cout << buffer << endl;
		};

		this_thread::sleep_for(std::chrono::milliseconds(50));
	}
};

void *handleClient(void *clientId)
{
	int clientID = (long)clientId;
	char buffer[BUFFER_SIZE] = {0};
	int valread;
	while (true)
	{

		buffer[BUFFER_SIZE] = {0};
		valread = recv(clientID, buffer, BUFFER_SIZE, 0);
		if (valread > 0)
		{

			if (strcmp(buffer, CHECK_NICKNAME) == 0)
			{
				buffer[BUFFER_SIZE] = {0};
				valread = recv(clientID, buffer, BUFFER_SIZE, 0);
				bool isAvailable = true;
				users_mutex.lock();
				for (auto it = users.begin(); it != users.end(); it++)
				{
					if ((string)buffer == it->first)
					{
						isAvailable = false;
					}
				}
				if (isAvailable)
				{
					users[buffer] = clientID;
					send(clientID, NICKANME_HAS_SET, strlen(NICKANME_HAS_SET), 0);
				}
				else
				{
					send(clientID, ALREADY_TAKEN_NICKNAME, strlen(ALREADY_TAKEN_NICKNAME), 0);
				}
				users_mutex.unlock();
			}
			else
			{
				queue_mutex.lock();
				message_queue.push(std::make_pair(clientID, buffer));
				queue_mutex.unlock();
			}
		}
	};
};

void *broadcasting(void *arg)
{
	// dequeue service
	while (true)
	{

		if (message_queue.size() > 0)
		{
			queue_mutex.lock();

			// client id,  message
			message dispatch = message_queue.front();
			message_queue.pop();
			queue_mutex.unlock();

			for (auto it = users.begin(); it != users.end(); it++)
			{
				// it: 1 nickanme, 2 clientid
				if (it->second != dispatch.first)
				{
					send(it->second, dispatch.second.c_str(), BUFFER_SIZE, 0);
				}
			}
		}
	};
	this_thread::sleep_for(std::chrono::milliseconds(100));
};

int server()
{
	clear();
	int socketId, clientId;
	struct sockaddr_in server_address = {.sin_family = AF_INET, .sin_port = htons(PORT), .sin_addr = {.s_addr = INADDR_ANY}};
	int server_address_len = sizeof(server_address);
	int opt = 1;
	int clientID;
	socketId = socket(AF_INET, SOCK_STREAM, 0);
	cout << c_time() << "Servidor do uol iniciado..." << endl;
	if (socketId == 0)
	{
		cerr << c_time() << "Erro ao crir o socket" << endl;
		exit(EXIT_FAILURE);
	}
	// Forcefully attaching socket to the port 8080
	if (setsockopt(socketId, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
								 &opt, sizeof(opt)) < 0)
	{

		cerr << c_time() << "Erro ao executar o setsockopt" << endl;
		exit(EXIT_FAILURE);
	}

	if (bind(socketId, (struct sockaddr *)&server_address,
					 sizeof(server_address)) < 0)
	{
		cerr << c_time() << "Erro ao executar o bind do socket" << endl;
		exit(EXIT_FAILURE);
	}
	if (listen(socketId, 3) < 0)
	{
		cerr << c_time() << "Erro ao executar o listen do socket" << endl;
		exit(EXIT_FAILURE);
	}
	cout << c_time() << "Servidor aberto na porta:" << PORT << endl;
	cout << c_time() << "Iniciando serviço de broadcasting." << endl;
	pthread_create(&thread_broadcasting, NULL, broadcasting, NULL);
	while (true)
	{
		cout.flush();
		clientID = accept(socketId, (struct sockaddr *)&server_address,
											(socklen_t *)&server_address_len);
		if (clientID < 0)
		{
			cerr << c_time() << "Erro ao aceitar um novo cliente" << endl;
			exit(EXIT_FAILURE);
		}
		auto time = chrono::system_clock::now();
		auto timestamp = chrono::duration_cast<chrono::milliseconds>(
												 time.time_since_epoch())
												 .count();
		time_t rawtime;
		cout << endl
				 << c_time() << "Novo cliente conectado." << endl;
		threads[clientID] = 0;
		pthread_create(&threads[clientID], NULL, handleClient, (void *)clientID);
	}
	return 0;
}

int client()
{
	int socketID = 0, valread;
	struct sockaddr_in server_address;
	char buffer[BUFFER_SIZE] = {0};
	socketID = socket(AF_INET, SOCK_STREAM, 0);
	if (socketID < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if (inet_pton(AF_INET, SERVER_ADDRESS, &server_address.sin_addr) <= 0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(socketID, (struct sockaddr *)&server_address,
							sizeof(server_address)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}

	string nickname;
	do
	{
		clear();
		cin.clear();
		cin.sync();
		cout << "Por favor escolha seu nome de usuario: ";
		getline(cin >> ws, nickname);
		cout << endl;
		send(socketID, CHECK_NICKNAME, BUFFER_SIZE, 0);
		send(socketID, nickname.c_str(), BUFFER_SIZE, 0);
		valread = recv(socketID, buffer, BUFFER_SIZE, 0);
		if (strcmp(buffer, ALREADY_TAKEN_NICKNAME) == 0)
		{
			cout << "Nome de usuario já utilizado por favor tente novamente" << endl;
		}
		if (strcmp(buffer, NICKANME_HAS_SET) == 0)
		{
			cout << "Apelido de definido com sucesso..." << endl;
			break;
		}
	} while (true);
	pthread_create(&thread_client_read_message, NULL, readMessages, (void *)socketID);
	clear();
	while (true)
	{
		string msg;
		cout << "Digite uma mensagem:" << endl;
		// cin >> msg;
		getline(std::cin, msg);
		string slug = (bold_on + nickname + bold_off + ": " + msg);
		int res = send(socketID, slug.c_str(), BUFFER_SIZE, 0);
		if (res <= 0)
		{
			cout << "erro ao enviar mensagem tente novamente" << endl;
		}
	}
}

int main(int argc, char const *argv[])
{
	int opt = -1;
	do
	{
		clear();
		cout << "Welcome to uol chat" << endl;
		cout << "\t1. Server mode"
				 << endl;
		cout << "\t2. Client mode"
				 << endl;
		cout << "\t3. To exit"
				 << endl;
		cin >> opt;
		if (opt == 1)
		{
			return server();
		}
		else if (opt == 2)
		{
			return client();
		}

	} while (opt != 3);

	return 0;
}
