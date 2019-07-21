#ifndef RZTYPEDESC_H__
#define RZTYPEDESC_H__

#include <string>
#include <vector>
#include <bitset>

class TypeDesc
{
public:
	std::bitset<16> modifiers;
	std::string name;
	std::vector<TypeDesc> params;
};

#endif // RZTYPEDESC_H__
