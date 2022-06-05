#include "../includes/User.hpp"
#include <sstream>
#include <unistd.h>
#include <iostream>

User::User() : _fd(-1), _nick(std::string()), _timestamp(std::time(NULL)) {
}

User::User(int fd_user) : _fd(fd_user), _nick(std::string()), _timestamp(std::time(NULL)) {
}

User::~User() {
	close(_fd);
	std::cout << "fd[" << _fd << "] closed" << std::endl;
}

int User::getFd() const {
	return this->_fd;
}

std::string User::getNick() const {
	return this->_nick;
}

void User::setNick(std::string nick) {
	this->_nick = nick;
}

User &User::operator=(const User &other) {
	this->_fd = other.getFd();
	this->_nick = other.getNick();
	return (*this);
}

bool User::isAuthorized() const {
	return _authorized;
}
void User::setAuthorized(bool authorized) {
	_authorized = authorized;
}
std::ostream &User::operator<<(std::ostream &os) {
	os << "fd: " << _fd;
	if (!_nick.empty())
		os << " nick " << _nick;
	return os;
}

std::ostream &operator<<(std::ostream &os, const User &user) {
	os << "fd " << user.getFd();
	if (!user.getNick().empty())
		os << " nick " << user.getNick();
	return os;
}

const std::string &User::getUsername() const {
	return _username;
}

void User::setUsername(const std::string &username) {
	_username = username;
}

const std::string &User::getRealname() const {
	return _realname;
}

void User::setRealname(const std::string &realname) {
	_realname = realname;
}

const std::string &User::getIp() const {
	return _IP;
}

void User::setIp(const std::string &ip) {
	_IP = ip;
}

int User::getPort() const {
	return _port;
}

void User::setPort(int port) {
	User::_port = port;
}

const std::string &User::getNickLowercase() const {
	return _nickLowercase;
}

void User::setNickLowercase(const std::string &nickLowercase) {
	_nickLowercase = nickLowercase;
}

std::string User::getUserInfoString() {
	std::string info;

	info = this->getNick() + "!" + this->getUsername() + "@" + this->getIp();
//	info += ":" + htons(this->getPort()); //Not sure if it right way
	return info;
}

time_t User::getTimestamp() const
{
	return _timestamp;
}

void User::setTimestamp(time_t timestamp)
{
	_timestamp = timestamp;
}
