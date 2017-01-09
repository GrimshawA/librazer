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

	void open();

	bool running();

	void display();

	void* data;
};

#endif