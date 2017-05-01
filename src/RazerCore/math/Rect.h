#ifndef RZRECT_H__
#define RZRECT_H__

#include <vector>
#include <stdint.h>

class RzModule;

class RzRect {
public:
    RzRect();

    void move(float x, float y);

    void setPosition(float x, float y);

    void setSize(float width, float height);

    bool contains(float x, float y);

public:
    static void registerApi(RzModule& m);

private:
    float mx = 0.f;
    float my = 0.f;
    float mw = 0.f;
    float mh = 0.f;
};

#endif

