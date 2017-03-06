#ifndef WINDOW_H__
#define WINDOW_H__

#include <vector>

class RzModule;

class Window
{
public:

	static void registerApi(RzModule* mod);

public:
	Window();

	virtual void open();

	virtual bool running();

	virtual void display();

	void* data;
	bool m_running;
};

#endif
