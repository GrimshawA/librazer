#ifndef SDLWINDOWIMPL_H__
#define SDLWINDOWIMPL_H__

#include <RazerCore/window/Window.h>

class SDLWindowImpl : public Window
{
public:

    SDLWindowImpl();

    void poll() override;

    void drawRect(int x, int y, int w, int h) override;

    void drawString(const std::string& str, int x, int y) override;

    void open() override;

    bool running() override;

    void display() override;
};

#endif
