#include "NetworkCommand.h"

//get commands from a string
std::vector<NetworkCommand*> NetworkCommand::GetCommands(std::string Data)
{
    std::vector<NetworkCommand*> commands = std::vector<NetworkCommand*>();

    bool GotCommand = false;

    std::string CurrentCommand = "";
    std::string CurrentArg = "";
    std::vector<std::string> CurrentArgs = std::vector<std::string>();

    for (size_t i = 0; i < Data.length(); i++)
    {
        if (Data[i] == '|') {//Reached end of command add to vector

            commands.push_back(new NetworkCommand(CurrentCommand, CurrentArgs));
            CurrentCommand = "";
            CurrentArgs.clear();
            GotCommand = false;
        }
        else {//reading letter by letter
            if (GotCommand == false) {
                if (Data[i] == ',') { //got first arg being the command name
                    GotCommand = true;
                }
                else {
                    CurrentCommand.push_back(Data[i]);//append letters until , is reached
                }

            }
            else if (Data[i] == ',') {//got arg
                CurrentArgs.push_back(CurrentArg);//add arg tp list
                CurrentArg.clear();//clear added
            }
            else {
                CurrentArg.push_back(Data[i]);//add chars
            }

        }

    }
	return commands;
}

NetworkCommand::NetworkCommand(std::string _Command, std::vector<std::string> _Args)
{
	Command = _Command;
	Args = _Args;
}
