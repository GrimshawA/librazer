#ifndef RZINSTRUCTIONITERATOR_H__
#define RZINSTRUCTIONITERATOR_H__

class InstructionIterator
{
public:
	InstructionIterator();

	void next();

	InstructionIterator& operator++();

private:
	int pc;
};

#endif
