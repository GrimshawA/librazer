#ifndef AEValueList_h__
#define AEValueList_h__

#include <Rzr/RzValue.h>

class AEValueList : public RzValue
{
public:

	AEValueList& operator << (const RzValue& v)
	{
		m_list.push_back(v);
		return *this;
	}

	std::vector<RzValue> m_list;
};

#endif // AEValueList_h__
