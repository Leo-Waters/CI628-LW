#include "Compression.h"

std::string Compresion::Compress(std::string str)
{
    //step 1 Shorten duplicate repetitive args such as 1,1,1,1
    //step 2 identify duplicate non-repetitive args
    //step 3 create key
    //step 4 create new string replacing args with key
    //keys value added to beginning of string

    std::string CompressedString = "";

    std::map<std::string, int> ArgumentAnalysis = std::map<std::string, int>();

    std::vector<NetworkCommand*> Commands = NetworkCommand::GetCommands(str);

   

    for (NetworkCommand* cmd : Commands) {//loop through all commands
        if (cmd->Command.empty()) {
            continue;
        }
       
        //shorten duplicate repetitive args such as 1,1,1,1,1,1,1,1----------------------------
        std::vector <std::string> ShrunkArgs = std::vector <std::string>();
        if (!cmd->Args.empty()) {//does the command have arguments

            std::string LastArg = cmd->Args.at(0);//first argument
            int Count = 1;//how many of thes arguments have appeared sequncialy 1,1,1,1,
            for (int i = 1; i < cmd->Args.size(); i++) {

                //is this the same as the last arg
                if (cmd->Args.at(i)==LastArg) {
                    Count++;//new repetitive duplicate found
                }
                //not the same
                else {
                    if (Count > 1) {//has duplicates add short form
                        ShrunkArgs.push_back('"' + LastArg + '"' + "x" + std::to_string(Count));
                        Count = 1;
                    }
                    else {//no duplicates
                        ShrunkArgs.push_back(LastArg);//add normal arg back to list as not duplicate
                        Count = 1;
                    }
                    LastArg = cmd->Args.at(i);
                }
            }

            //add remaining data

            if (Count > 1) {//has duplicates add short form
                ShrunkArgs.push_back('"' + LastArg + '"' + "x" + std::to_string(Count));
               
            }
            else {//no duplicates
                ShrunkArgs.push_back(LastArg);
            }
        }

        //add command name to analysis
        if (ArgumentAnalysis.count(cmd->Command)!=0) {
            ArgumentAnalysis.at(cmd->Command)+=1;//increase duplicate counter
        }
        else {
            ArgumentAnalysis.insert_or_assign(cmd->Command, 1);//initalize duplicate counter

        }

        //analise duplicate args
        for (std::string key : ShrunkArgs) {
           
            if (key.length() > 2) {//dont add keys shorter than 3 as can use more bytes
                if (ArgumentAnalysis.count(key) != 0) {
                    ArgumentAnalysis.at(key)+=1;//increase duplicate counter
                }
                else {
                    ArgumentAnalysis.insert_or_assign(key, 1);//initalize duplicate counter

                }
            }
        }
        cmd->Args = std::vector<std::string>(ShrunkArgs);//set args to short form args


        //-------------------------------------------------------------------------------------------------
    }





    int keyid = 0;
    //key and ID
    std::map< std::string, int> KeyIDS =  std::map< std::string, int>();
    //Create final Compressed string
    for (NetworkCommand* cmd : Commands) {

        for (std::string key : cmd->Args) {
            if (key.length() > 2) {//dont add keys shorter than 3 as can use more bytes
                if (ArgumentAnalysis.count(key) != 0) {
                    ArgumentAnalysis.at(key) += 1;//increase duplicate counter
                }
                else {
                    ArgumentAnalysis.insert_or_assign(key, 1);//initalize duplicate counter

                }
            }
        }

        //if analysis counted more than 2 of these cmd names create a key
        if (ArgumentAnalysis.count(cmd->Command)!=0 && ArgumentAnalysis.at(cmd->Command) > 2) {
            if (KeyIDS.count(cmd->Command) !=0) {
                cmd->Command = "*" +std::to_string(KeyIDS.at(cmd->Command));//replace cmd string with * and ID
            }
            else {
                KeyIDS.insert_or_assign(cmd->Command, keyid);
                cmd->Command = "*" + cmd->Command;// first command Add * to beginning of command name so that decompressor knows the key to replace others
                keyid++;
            }
        }

        for (int a = 0; a < cmd->Args.size(); a++) {
            if (cmd->Command.empty()) {
                continue;
            }
            //if analysis counted more than 2 of these cmd names create a key
            if (ArgumentAnalysis.count(cmd->Args.at(a))!=0 && ArgumentAnalysis.at(cmd->Args.at(a)) > 2) {

                if (KeyIDS.count(cmd->Args.at(a))!=0) {
                    cmd->Args.at(a)="*" + std::to_string(KeyIDS.at(cmd->Args.at(a)));//replace cmd string with * and ID
                }
                else {
                    KeyIDS.insert_or_assign(cmd->Args.at(a), keyid);
                    cmd->Args.at(a)="*" + cmd->Args.at(a);// first command Add * to beginning of command name so that decompressor knows the key to replace others
                    keyid++;
                }
            }
        }

        //convert compressed data into string form again
        CompressedString.append(cmd->Command);//add command name

        if (!cmd->Args.empty()) {//has args
            CompressedString.append(",");
            for (std::string shrunkArg : cmd->Args) {
                CompressedString.append(shrunkArg);//add arg to string
                CompressedString.append(",");
            }
        }
        //signifies end of command
        CompressedString.append("|");
    }

    return CompressedString;
}


std::string Compresion::Decompress_asString(std::string str)//this was written server side then converted from java side code using similar objects
{
    std::string Data = "";
    std::vector<NetworkCommand*> Commands = Decompress(str);
    
    for each (auto cmd in Commands)
    {
        //convert decompressed data into string form again
        Data.append(cmd->Command);//add command name

        if (!cmd->Args.empty()) {//has args
            Data.append(",");
            for (std::string shrunkArg : cmd->Args) {
                Data.append(shrunkArg);//add arg to string
                Data.append(",");
            }
        }
        //signifies end of command
        Data.append("|");
    }
   

    for (size_t i = 0; i < Commands.size(); i++)
    {
        delete Commands[i];
    }

    return Data;
}

std::vector<NetworkCommand*> Compresion::Decompress(std::string str)//this was written server side then converted from java side code using similar objects
{
    std::string Data = "";

    auto Commands = NetworkCommand::GetCommands(str);

    int Index = 0;
    std::map<std::string, std::string> Keys= std::map<std::string, std::string>();



    //-----------------------------------------------Identify keys and replace----------------
    for (NetworkCommand* cmd : Commands) {
        if (cmd->Command.empty()) {
            continue;
        }
        if (cmd->Command[0] == '*') {//found command Key
            if (Keys.count(cmd->Command) != 0) {//is key, replace with value
                cmd->Command = Keys.at(cmd->Command);
            }
            else {//is value register with key so that future keys can retrieve
                cmd->Command = cmd->Command.substr(1);
                Keys.insert_or_assign("*" + std::to_string(Index), cmd->Command);//add Command key
                Index++;//Increase Index for next find
            }
        }
        std::vector<std::string> ExpandedArgs = std::vector<std::string>();
        for (int i = 0; i < cmd->Args.size(); i++) {
            if (cmd->Args[i].empty()) {
                continue;
            }
            if (cmd->Args.at(i).at(0) == '*') {//found command Key

                if (Keys.count(cmd->Args.at(i)) != 0) {//is key, replace with value
                    cmd->Args[i]=Keys[cmd->Args[i]];
                }
                else {//is value register with key so that future keys can retrieve
                    cmd->Args[i]=cmd->Args[i].substr(1);
                    Keys.insert_or_assign("*" + std::to_string(Index), cmd->Args[i]);//add Command key
                    Index++;//Increase Index for next find
                }
            }
            //expand repetitive args
            if (cmd->Args[i].at(0) == '"') {

                std::string value = "";
                std::string Multiplier = "";

                bool GotValue = false;
                bool GettingMultiplier = false;
                for (int j = 1; j < cmd->Args[i].length(); j++) {
                    if (cmd->Args[i].at(j) == '"') {//reached end of value
                        GotValue = true;//has value
                    }
                    else {
                        if (!GotValue) {//if hasnt got value
                            value+=(cmd->Args[i].at(j));//add current char to value
                        }
                        else if (GotValue && cmd->Args[i].at(j) == 'x') {//gettingMultiplier
                            GettingMultiplier = true;
                        }
                        else if (GettingMultiplier) {
                            Multiplier+=(cmd->Args[i].at(j));//get multiplier
                        }
                    }
                }

                int Elem_Multiplier = std::atoi(Multiplier.c_str());//convert multiplier to int

                for (int e = 0; e < Elem_Multiplier; e++) {//add value to args by multiplier
                    ExpandedArgs.push_back(value);
                }

            }
            else {
                ExpandedArgs.push_back(cmd->Args.at(i));
            }

        }
        cmd->Args = ExpandedArgs;


        //convert decompressed data into string form again
        Data.append(cmd->Command);//add command name

        if (!cmd->Args.empty()) {//has args
            Data.append(",");
            for (std::string shrunkArg : cmd->Args) {
                Data.append(shrunkArg);//add arg to string
                Data.append(",");
            }
        }
        //signifies end of command
        Data.append("|");
    }
    //------------------------------------------------------------------------------------------------

    //Convert Data back to full



    return Commands;
}

void Compresion::Test()
{
    DEBUG("\n\n-----------------------------------Compresion and decompresion algorithm test-------------------------------------------------")
    std::string Testcase1 = "LEVELDATA,39,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,|";
    DEBUG("Comp Testcase1 Data:" << Testcase1 <<" Leng:"<<Testcase1.length());
    std::string Testcase1comp = Compress(Testcase1);
    DEBUG("Comp Testcase1 Compressed:" << Testcase1comp << " Leng:" << Testcase1comp.length());
    std::string Testcase1Decomp = Decompress_asString(Testcase1comp);
    DEBUG("Comp Testcase1 Decompressed:" << Testcase1Decomp << " Leng:" << Testcase1Decomp.length());

    std::string Testcase2 = "PLAYER_DATA,0,580.0,500.0,ACTIVE,100.0,0.0,0,|PLAYER_DATA,1,580.0,340.0,IDLE,100.0,0.0,0,|PLAYER_DATA,2,420.0,420.0,IDLE,100.0,0.0,0,|PLAYER_DATA,3,340.0,340.0,IDLE,100.0,0.0,0,|";
    DEBUG("Comp Testcase2 Data:" << Testcase2 << " Leng:" << Testcase2.length());
    std::string Testcase2comp = Compress(Testcase2);
    DEBUG("Comp Testcase2 Compressed:" << Testcase2comp << " Leng:" << Testcase2comp.length());
    std::string Testcase2Decomp = Decompress_asString(Testcase2comp);
    DEBUG("Comp Testcase2 Decompressed:" << Testcase2Decomp << " Leng:" << Testcase2Decomp.length());
    DEBUG("----------------------------------- END of Compresion and decompresion algorithm test-------------------------------------------------\n\n\n")
}
