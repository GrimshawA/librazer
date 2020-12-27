#ifndef RzBuilder_h__
#define RzBuilder_h__

#include <razer/config.h>
#include <vector>
#include <string>

class RzEngine;

class RZ_API RzBuilder
{
public:
	class Batch
	{
	public:
		std::vector<std::string> files;

		Batch& operator<<(const std::string& file)
		{
			files.push_back(file);
			return *this;
		}
	};

public:
	RzBuilder(RzEngine& engine);

    bool build(const Batch& b);

private:
	RzEngine& m_engine;
};


#endif // RzBuilder_h__
