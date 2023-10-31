#include <string>
#include <map>
#include<vector>
#include "NetworkCommand.h"
#include "Macros.h"
static class Compresion
{
public:
	static std::string Compress(std::string str);

	static std::vector<NetworkCommand*> Decompress(std::string str);
	static std::string Decompress_asString(std::string str);

	static void Test();
};

