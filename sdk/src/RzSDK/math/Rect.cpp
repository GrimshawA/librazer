#include <RzSDK/math/Rect.h>
#include <razer/runtime/RzEngine.h>
#include <razer/vm/VirtualMachine.h>
#include <razer/utils/Logger.h>

RzRect::RzRect() {

}

void RzRect::move(float x, float y) {
    mx += x;
    my += y;
}

void RzRect::setPosition(float x, float y) {
    mx = x;
    my = y;
}

void RzRect::setSize(float width, float height) {
    mw = width;
    mh = height;
}

bool RzRect::contains(float x, float y) {
    bool r =  x >= mx && x <= mx + mw
            && y >= my && y <= my + mh;

    RZLOG("RECT CONTAINS %d %f %f\n", r, x, y);
    return r;
}

void RzRect::registerApi(RzModule& m) {
    m.registerType("Rect", sizeof(RzRect));
    m.registerTypeConstructor("Rect", [](void* memory, RzVirtualMachine* vm)
    {
        new (memory) RzRect;
    });

    m.registerMethod("Rect", "void setPosition(float, float)", [](RzGeneric g){
        RzRect* obj = (RzRect*)g.popObject();
        float x = g.popFloat();
        float y = g.popFloat();
        obj->setPosition(x,y);
    });

    m.registerMethod("Rect", "void move(float, float)", [](RzGeneric g){
        RzRect* obj = (RzRect*)g.popObject();
        float x = g.popFloat();
        float y = g.popFloat();
        obj->move(x,y);
    });

    m.registerMethod("Rect", "void setSize(float, float)", [](RzGeneric g){
        RzRect* obj = (RzRect*)g.popObject();
        float x = g.popFloat();
        float y = g.popFloat();
        obj->setSize(x,y);
    });

    m.registerMethod("Rect", "bool contains(float, float)", [](RzGeneric g){
        RzRect* obj = (RzRect*)g.popObject();
        float x = g.popFloat();
        float y = g.popFloat();
        bool r = obj->contains(x,y);
        g.pushBoolean(r);
    });
}
