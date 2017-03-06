#include <RazerCore/window/Window.h>
#include <Rzr/RzModule.h>
#include <Logger.h>

void Window::registerApi(RzModule* m)
{
	m->registerType("Window", sizeof(Window));
	m->registerTypeConstructor("Window", [](void* memory, RzVirtualMachine* vm)
	{
		new (memory) Window();
	});

	m->registerMethod("Window", "void open()", [](AEGeneric g)
	{
		Window* obj = (Window*)g.unpack_ptr();
		obj->open();
	});

	m->registerMethod("Window", "bool running()", [](AEGeneric g)
	{
		Window* obj = (Window*)g.unpack_ptr();
		g.pack_int32(obj->running());
	});

	m->registerMethod("Window", "void display()", [](AEGeneric g)
	{
		Window* obj = (Window*)g.unpack_ptr();
		obj->display();
	});
}

Window::Window()
{

}

void Window::open()
{

}

bool Window::running()
{
	return false;
}

void Window::display()
{

}
