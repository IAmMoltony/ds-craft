#include "gamever.hpp"
#include <stdio.h>
#include <string.h>

namespace gamever
{
	static std::string _versionPrefix = "";
	static std::string _versionString = "";
	static uint8_t _versionMajor = 0;
	static uint8_t _versionMinor = 0;
	static uint8_t _versionPatch = 0;

	InitStatus init(const char *gameVerName)
	{
		FILE *f = fopen(gameVerName, "r");
		if (!f)
			return InitStatus::FileOpenError;

		uint8_t count = 0;
		char line[20];
		while (fgets(line, sizeof(line), f) != NULL)
		{
			if (line[strlen(line) - 1] == '\n')
				line[strlen(line) - 1] = 0; // remove \n

			if (count == 0)
				_versionMajor = atoi(line);
			else if (count == 1)
				_versionMinor = atoi(line);
			else if (count == 2)
				_versionPatch = atoi(line);
			else if (count == 3)
				_versionPrefix = std::string(line);

			++count;
		}
		fclose(f);

		// if patch is 0, then don't put patch in version string
		if (_versionPatch == 0)
			_versionString = _versionPrefix + std::to_string(_versionMajor) + '.' +
							 std::to_string(_versionMinor);
		else
			_versionString = _versionPrefix + std::to_string(_versionMajor) + '.' +
							 std::to_string(_versionMinor) + '.' + std::to_string(_versionPatch);

		return InitStatus::OK;
	}

	const char *getVersionString(void)
	{
		return _versionString.c_str();
	}

	enum class VersionType
	{
		Alpha,
		Beta,
		Release,
	};

	uint64_t getVersionHash(const std::string &_versionString)
	{
		// extract the major, minor, and patch numbers from the version string
		int major = 0;
		int minor = 0;
		int patch = 0;

		sscanf(_versionString.c_str(), "%*[^0-9]%d.%d.%d", &major, &minor, &patch);
		if (patch == 0)
			sscanf(_versionString.c_str(), "%*[^0-9]%d.%d", &major, &minor);

		// determine version type
		VersionType versionType = VersionType::Release;
		if (_versionString.find("alpha") != std::string::npos)
			versionType = VersionType::Alpha;
		else if (_versionString.find("beta") != std::string::npos)
			versionType = VersionType::Beta;

		// calculate the hash based on type and numbers (courtesy of chat gpt)
		uint64_t versionHash = static_cast<uint32_t>(versionType);
		versionHash <<= 24;
		versionHash |= (major << 16) + (minor << 8) + patch;

		return versionHash;
	}
}