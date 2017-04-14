#include <RazerCore/window/Window.h>
#include <RazerCore/container/String.h>
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

    m->registerMethod("Window", "void drawRect(int x, int y, int w, int h)", [](AEGeneric g)
    {
        Window* obj = (Window*)g.unpack_ptr();
        int x = g.unpack_int32();
        int y = g.unpack_int32();
        int w = g.unpack_int32();
        int h = g.unpack_int32();

        obj->drawRect(x,y,w,h);
    });

    m->registerMethod("Window", "void drawString(String s, int x, int y)", [](AEGeneric g)
    {
        Window* obj = (Window*)g.unpack_ptr();
        RzString* s = (RzString*)g.unpack_ptr();
        int x = g.unpack_int32();
        int y = g.unpack_int32();
        obj->drawString(s->toStdString(), x, y);
    });

	m->registerMethod("Window", "void display()", [](AEGeneric g)
	{
		Window* obj = (Window*)g.unpack_ptr();
		obj->display();
	});
}

Window::Window() {

}

void Window::open() {

}

void Window::drawRect(int x, int y, int w, int h) {

}

void Window::drawString(const std::string& str, int x, int y) {

}

bool Window::running() {
    return true;
}

void Window::display() {

}
