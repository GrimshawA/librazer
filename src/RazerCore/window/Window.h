#ifndef WINDOW_H__
#define WINDOW_H__

#include <vector>
#include <map>

class RzModule;

class Window
{
public:

	static void registerApi(RzModule* mod);

public:
	Window();

    virtual void open();

    virtual void setSize(int width, int height);

    virtual void poll();

    bool keyPressed(int key);

    void setFillColor(int r, int g, int b);

    virtual void drawRect(int x, int y, int w, int h);
    virtual void drawString(const std::string& str, int x, int y);

	virtual bool running();

	virtual void display();

	void* data;
	bool m_running;

    int r,g,b;

    std::map<int, bool> m_keys;
};

#endif
