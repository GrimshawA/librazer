#include "aeNodeWhile.h"
#include "aeNodeBlock.h"

aeNodeWhile::aeNodeWhile()
{
	m_nodeType = AEN_WHILE;

	doWhile = false;

	block.reset(new aeNodeBlock());

	//add(block);
}

std::string aeNodeWhile::printtext()
{
	return std::string("While");
}