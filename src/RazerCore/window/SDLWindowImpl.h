#ifndef SDLWINDOWIMPL_H__
#define SDLWINDOWIMPL_H__

#include <RazerCore/window/Window.h>

class SDLWindowImpl : public Window
{
	public:

		SDLWindowImpl();

		void open();

		bool running();

		void display();

};

#endif
