#ifndef AEValueList_h__
#define AEValueList_h__

#include <AEON/Runtime/AEValue.h>

class AEValueList : public AEValue
{
public:

	AEValueList& operator << (const AEValue& v)
	{
		m_list.push_back(v);
		return *this;
	}

	std::vector<AEValue> m_list;
};

#endif // AEValueList_h__
