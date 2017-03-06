#include <RazerVM/InstructionIterator.h>

InstructionIterator::InstructionIterator()
	: pc(0)
{

}

void InstructionIterator::next()
{
	pc ++;
}

InstructionIterator& InstructionIterator::operator++()
{
	next();
	return *this;
}
