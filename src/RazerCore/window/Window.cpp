#include <RazerCore/window/Window.h>
#include <RazerCore/container/String.h>
#include <RazerRuntime/Module.h>
#include <Logger.h>

#ifdef RAZER_SDL
#include <RazerCore/window/SDLWindowImpl.h>
#endif

void Window::registerApi(RzModule* m)
{
#ifdef RAZER_SDL
    int typeSize = sizeof(SDLWindowImpl);
#else
    int typeSize = sizeof(Window);
#endif

    m->registerType("Window", typeSize);
	m->registerTypeConstructor("Window", [](void* memory, RzVirtualMachine* vm)
	{
#ifdef RAZER_SDL
        new (memory) SDLWindowImpl();
#else
        new (memory) Window();
#endif
	});

	m->registerMethod("Window", "void open()", [](RzGeneric g)
	{
		Window* obj = (Window*)g.popObject();
		obj->open();
	});

    m->registerMethod("Window", "void poll()", [](RzGeneric g)
    {
        Window* obj = (Window*)g.popObject();
        obj->poll();
    });

    m->registerMethod("Window", "void setSize(int32, int32)", [](RzGeneric g)
    {
        Window* obj = (Window*)g.popObject();
        int32_t w = g.popInt32();
        int32_t h = g.popInt32();
        obj->setSize(w, h);
    });

    m->registerMethod("Window", "bool keyPressed(int32)", [](RzGeneric g)
    {
        Window* obj = (Window*)g.popObject();
        int32_t k = g.popInt32();
        bool r = obj->keyPressed(k);
        g.pushInt32(r);
    });

    m->registerMethod("Window", "void setFillColor(int32,int32,int32)", [](RzGeneric g)
    {
        Window* obj = (Window*)g.popObject();
        int r = g.popInt32();
        int gg = g.popInt32();
        int b = g.popInt32();
        obj->setFillColor(r,gg,b);
    });

	m->registerMethod("Window", "bool running()", [](RzGeneric g)
	{
		Window* obj = (Window*)g.popObject();
        g.pushBoolean(obj->running());
	});

    m->registerMethod("Window", "void drawRect(int32, int32, int32, int32)", [](RzGeneric g)
    {
        Window* obj = (Window*)g.popObject();
        int x = g.popInt32();
        int y = g.popInt32();
        int w = g.popInt32();
        int h = g.popInt32();

        obj->drawRect(x,y,w,h);
    });

    m->registerMethod("Window", "void drawString(String s, int x, int y)", [](RzGeneric g)
    {
        Window* obj = (Window*)g.popObject();
        RzString* s = (RzString*)g.popObject();
        int x = g.popInt32();
        int y = g.popInt32();
        obj->drawString(s->toStdString(), x, y);
    });

	m->registerMethod("Window", "void display()", [](RzGeneric g)
	{
		Window* obj = (Window*)g.popObject();
        RZLOG("WINDOW OBJ %x\n", obj);
		obj->display();
	});

    m->registerMethod("Window", "int32 height()", [](RzGeneric g) {
       Window* obj = (Window*)g.popObject();
       int32_t h = obj->height();
       g.pushInt32(h);
    });

    m->registerMethod("Window", "int32 width()", [](RzGeneric g) {
       Window* obj = (Window*)g.popObject();
       int32_t w = obj->width();
       g.pushInt32(w);
    });
}

Window::Window() {
    r = g = b = 100;
    RZLOG("Window instanced %x\n", this);
}

void Window::open() {

}

void Window::setSize(int width, int height) {

}

void Window::poll() {
    RZLOG("POLL\n");
}

bool Window::keyPressed(int key) {
    if (m_keys.find(key) != m_keys.end()) {
        return m_keys[key];
    }
    return false;
}

void Window::setFillColor(int r, int g, int b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

void Window::drawRect(int x, int y, int w, int h) {
    RZLOG("DRAWING RECT\n");
}

void Window::drawString(const std::string& str, int x, int y) {

}

bool Window::running() {
    return true;
}

void Window::display() {

}

int32_t Window::width() {
    return 0;
}

int32_t Window::height() {
    return 0;
}
