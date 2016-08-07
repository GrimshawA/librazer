#ifndef aeon_livecore_h__
#define aeon_livecore_h__

#include <vector>
#include <string>
#include <functional>

struct LiveCoreMenuItem
{
	std::string name;
	std::string trigger;
	std::function<void()> cb;
};

struct LiveCoreMenu
{
	std::vector<LiveCoreMenuItem> items;

	void add(const std::string& trigger, const std::string& name, std::function<void()> callback)
	{
		LiveCoreMenuItem item;
		item.trigger = trigger;
		item.name = name;
		item.cb = callback;
		items.push_back(item);
	}
};

class livecore
{
public:
	enum States
	{
		STATE_LIVECODE,
		STATE_TESTS,
		STATE_EXITING,
	};

	int                      m_state;
	std::vector<std::string> m_history;
	LiveCoreMenu             m_rootmenu;
	LiveCoreMenu*            m_currentMenu;

public:
	livecore();

	void run();

	void prompt();
};

#endif // aeon_livecore_h__
