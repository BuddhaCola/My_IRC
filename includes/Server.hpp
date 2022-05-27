#ifndef MY_IRC_SERVER_HPP
#define MY_IRC_SERVER_HPP

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <iostream>

#include "User.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "my_irc.hpp"
#include "Logger.hpp"
#include <sys/select.h>
#include <poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>

typedef struct Response {
	std::string	code;
	std::string	body;
}	Response;

class Server {
public:
	Server(int, std::string);
	int 						creat_listen_socket(int);
	void						startLoop(int);
	std::string					getPassword() const;
	int 						getPort() const;
	Server & operator= (const Server &other);

	~Server();
private:
	int						_port;
	std::string				_password;
	std::vector<User *>		_users;
	std::vector<Channel *>	_channels;
	int						_flagReg;
	Logger					logger;

	void						StartLogMessage();
	void						handleRequest(char *request, User &user);
	std::vector<Command>		parseRequest(std::string const &request, User &);

	//request handling implementations
	int 						getFlagReg() const;
	void						executeCommand(Command const &);
	void						handleUser(Command const &);
	void						handlePassword(Command const &);
	void						handleSetNick(Command const &);
	void						handlePrivateMessage(Command const &);
	void						handlePing(Command const &);
	Server();
};

#endif //MY_IRC_SERVER_HPP
