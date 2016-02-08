#ifndef aeNodeStatement_h__
#define aeNodeStatement_h__

#include "aeNodeBase.h"

#include <map>

class aeNodeStatement : public aeNodeBase
{
public:
	virtual bool isNullStatement();
};

#endif // aeNodeStatement_h__
