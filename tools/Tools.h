#pragma once
#include "ofMain.h"

namespace synapse
{
	class Tools
	{
	public:
		static Tools& getInstance();
		Tools();
		static string getDate();
		static string getDate(const string& format);
		std::vector<std::string> splitString(const std::string &s, char delim);
	private:
		template<class Out>
		inline void split(const std::string & s, char delim, Out result)
		{
			std::stringstream ss(s);
			std::string item;
			while (std::getline(ss, item, delim))
			{
				*(result++) = item;
			}
		}

	protected:
	};

	inline Tools& tools()
	{
		return Tools::getInstance();
	}
}
