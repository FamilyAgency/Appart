#include "Tools.h"

using namespace synapse;

Tools& synapse::Tools::getInstance()
{
	static Tools tools;
	return tools;
}

synapse::Tools::Tools()
{

}

string synapse::Tools::getDate(const string & format)
{
	time_t rawtime = time(0);
	struct tm * timeinfo;
	char buffer[80];
	timeinfo = localtime(&rawtime);
	strftime(buffer, 80, format.c_str(), timeinfo);
	return buffer;
}

string synapse::Tools::getDate()
{
	const string dateFormat = "[%Y.%m.%d %H:%M:%S] ";
	return getDate(dateFormat);
}

std::vector<std::string> synapse::Tools::splitString(const std::string & s, char delim)
{
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}



