#ifndef aeNodeEnum_h__
#define aeNodeEnum_h__

#include "aeNodeBase.h"
#include <vector>

/**
	\class aeNodeEnum
*/
class aeNodeEnum : public aeNodeBase
{
public:

	std::string name;
	std::vector<std::string> members;

public:
	aeNodeEnum();

	void addField(std::string name);

	std::string str();
};

#endif // aeNodeEnum_h__
