#include "bwStrings.hpp"
#include "Namespaces.hpp"

namespace ZerGreenAI
{

	bool removePrefix(const char * prefix, const char * string)
	{
		if (strstr(string, prefix) != nullptr)
		{
			string += strlen(prefix);
			return true;
		}
		else
		{
			return false;
		}
	}

	const char * ZerGreenAI::noPrefixC_str(UnitType type)
	{
		const char * removePrefixString = type.c_str();
		if (removePrefix("Terran_", removePrefixString)) { return removePrefixString; }
		if (removePrefix("Protoss_", removePrefixString)) { return removePrefixString; }
		if (removePrefix("Zerg_", removePrefixString)) { return removePrefixString; }
		if (removePrefix("Critter_", removePrefixString)) { return removePrefixString; }
		if (removePrefix("Hero_", removePrefixString)) { return removePrefixString; }
		if (removePrefix("Powerup_", removePrefixString)) { return removePrefixString; }
		if (removePrefix("Special_", removePrefixString)) { return removePrefixString; }
		return removePrefixString;
	}
}