#pragma once
#include <vector>
#include <string>
class NetworkCommand
{
public:
	std::string Command;
	std::vector<std::string> Args;

	NetworkCommand(std::string _Command, std::vector<std::string> _Args);
};

