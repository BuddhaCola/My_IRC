#include <sstream>
#include "../includes/Command.hpp"
#include "../includes/my_irc.hpp"

CommandEnum verbToCommand(std::string &verb) {
	return CommandEnum(std::find(CommandNames, CommandNames + NumCommands,  verb) - CommandNames);
}

std::string command_to_name(CommandEnum command)
{
	return (command < NumCommands) ? CommandNames[command] : "";
}

Command::Command(const std::string &string, User &user) : body(string), user(user) {
	std::string	verb;
	size_t		delimeter = string.find(' ');
	verb = string.substr(0, delimeter);
	if (verb.empty())
		throw (FtException());
	type = verbToCommand(verb);
	body = string.substr(delimeter, string.length());
}

Command::~Command() {
}

Command &Command::operator=(const Command &other) {
	this->body = other.body;
	this->type = other.type;
	return (*this);
}

Command::Command(const Command &other) : type(other.getType()), body(other.getBody()), user(other.getUser()) {
}

CommandEnum Command::getType() const {
	return this->type;
}

std::string Command::getBody() const {
	return this->body;
}

std::ostream &Command::operator<<(std::ostream &os) {
	os << "command: " << command_to_name(this->type) << "\nbody: " << this->body;
	return os;
}

std::ostream &operator<<(std::ostream &os, const Command &command) {
	os << "command: " << command_to_name(command.type) << "\nbody: " << command.body;
	return os;
}

User &Command::getUser() const {
	return this->user;
}
