#pragma once
#include "ofMain.h"

namespace synapse
{
	class ConfigLoader
	{
	public:
		enum class LoadType
		{
			None,
			LocalFile,
			ResourceFile,
			Url
		};

		ConfigLoader();
		~ConfigLoader();

		void load(const string& param, LoadType type = LoadType::LocalFile);
	
		ofEvent<string> loadSuccessEvent;
		ofEvent<void> loadErrorEvent;

	private:
		string configPath;
		ofBuffer configBuffer;

	protected:
	};
}
