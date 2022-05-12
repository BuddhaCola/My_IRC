#include "../includes/Server.hpp"
#include "../includes/Command.hpp"
#include "../includes/User.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password){
}

Server::Server() : _port(-1), _password(std::string()) {
}

int Server::creat_listen_socket(int port)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);//The second parameter here represents TCP
	if( sock < 0 )
		throw("socket fail...\n");

	int opt = 1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = htonl(INADDR_ANY);// Address of any type
	local.sin_port = htons(port);

	if( bind(sock,(struct sockaddr *)&local,sizeof(local)) < 0 )
		throw("bind fail...\n");

	if( listen(sock,5) < 0 )
		throw("listen fail...\n");
	return sock;
}

void Server::startLoop(int listen_sock)
{
	std::cout << "Starting server on port " << this->_port << std::endl;
	std::cout << "Password is " << (getPassword().empty() ? "not set" :
									("set to |" + getPassword()) + "|")
			  << std::endl;
	struct pollfd fd_list[1024];
	int num = sizeof(fd_list) / sizeof(fd_list[0]);
	int i = 0;

	for (; i < num; i++)
	{
		fd_list[i].fd = -1;// File descriptor
		fd_list[i].events = 0;// Set of events to monitor
		fd_list[i].revents = 0;// Ready Event Set of Concerned Descriptors
	}
	// 3. Add read-only events for file descriptors of interest

	i = 0;
	for (; i < num; i++)
	{
		if (fd_list[i].fd == -1)
		{
			fd_list[i].fd = listen_sock;
			fd_list[i].events = POLLIN;// Concern about Read-Only Events
			break;
		}
	}



	while (1)
	{
		//4. Start calling poll and wait for the file descriptor set of interest to be ready
		switch (poll(fd_list, num, 3000))
		{
			case 0:// The state of the denominator has exceeded before it has changed. timeout Time
				continue;
			case -1:// failed
				std::cerr << "poll fail..." << std::endl;
				continue;
			default: // Succeed
			{
				//   If it is a listener file descriptor, call accept to accept a new connection
				//   If it is a normal file descriptor, read is called to read the data
				int i = 0;
				for (; i < num; i++)
				{
					if (fd_list[i].fd == -1)
					{
						continue;
					}
					if (fd_list[i].fd == listen_sock &&
						(fd_list[i].revents & POLLIN))
					{
						// 1. Provide a connection acceptance service if the listening socket is ready to read

						struct sockaddr_in client;
						socklen_t len = sizeof(client);
						int new_sock = accept(listen_sock,
											  (struct sockaddr *) &client,
											  &len);
						if (new_sock < 0)
						{
							std::cerr << "accept fail..." << std::endl;
							continue;
						}
						//After obtaining the new file descriptor, add the file descriptor to the array for the next time you care about the file descriptor
						int i = 0;
						for (; i < num; i++)
						{
							if (fd_list[i].fd == -1)//Place the first value
								// in the array at - 1
								break;
						}
						if (i < num)
						{
							fd_list[i].fd = new_sock;
							fd_list[i].events = POLLIN;
						} else
						{
							close(new_sock);
						}

						std::cout << "get a new link " <<
							   inet_ntoa(client.sin_addr) << ":" <<
							   ntohs(client.sin_port) << std::endl;
						_users.push_back(new User(fd_list[i].fd));
						std::cout << "Created a new user" << std::endl;
						for (size_t i = 0; i < _users.size(); ++i) {
							std::cout << "Pointer to a user " <<  _users.at(i)
							<< std::endl;
						}

						continue;
					}

					//2. At this point, we are concerned with ordinary file descriptors.
					//   Provide services to read data at this time
					if (fd_list[i].revents & POLLIN)
					{
						char buf[1024];
						ssize_t s = read(fd_list[i].fd, buf, sizeof(buf) - 1);
						if (s < 0)
						{
							std::cerr << "read fail..." << std::endl;
							continue;
						} else if (s == 0)
						{
							std::cout << "client quit..." << std::endl;
							close(fd_list[i].fd);
							fd_list[i].fd = -1;
						} else
						{
							buf[s] = 0;
							handleRequest(buf);
							std::cout << "client fd = " << fd_list[i].fd << " "
													  "" <<
							buf << std::endl;
						}
					}
				}
			}
				break;
		}
	}
}

Server::~Server() {
}

std::string Server::getPassword() const {
	return this->_password;
}

int Server::getPort() const
{
	return this->_port;
}

Server &Server::operator=(const Server &other) {
	this->_port = other._port;
	this->_password = other._password;
	return(*this);
}

void	executeCommand(Command const &command){
	//TODO
	std::cout << "test" << std::endl;
#ifdef MORE_INFO
	std::cout << CYAN << "executing " << command << RESET << std::endl;
#endif
}

void Server::handleRequest(char *request) {
	std::vector<Command> 	commands;
	std::vector<User> 		users;
	commands = parseRequest(std::string(request));
	if (commands.empty()) {
		#ifdef MORE_INFO
		std::cout << CYAN "|unrecognized request: " << request << RESET << std::endl;
		#endif
	}
	for (std::vector<Command>::iterator it = commands.begin(); it != commands.end(); ++it) {
		executeCommand(*it);
	}
}

std::vector<Command> Server::parseRequest(std::string const &request) {
	std::vector<Command>	commands;

	std::string				strRequest;

	strRequest = std::string(request);
	size_t pos = 0;
	std::string commandBody;
	while ((pos = strRequest.find("\r\n")) != std::string::npos) {
		commandBody = strRequest.substr(0, pos);
		std::cout << commandBody << std::endl;
		commands.push_back(Command(commandBody));
		strRequest = strRequest.erase(0, pos + 2);
	}
	return commands;
}
