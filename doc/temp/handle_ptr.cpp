// Problem is that all refs to components are done with ptr, but it could transparently be an handle with an accessor on top,
// allowing for memory moving behind the scenes

#include <iostream>
#include <vector>

class SpriteManager;

template<typename T>
class HandleManager {
public:
    virtual T* translate(uint32_t handle) = 0;
};

template<typename T>
class handle
{
public:
    T* operator->()
    {
        return handleManager->translate(handle);
    }
//private:
    uint32_t handle;
    HandleManager<T>* handleManager;
};

class Sprite
{
public:
    int color = 5;
};

class SpriteManager : HandleManager<Sprite>
{
public:
    Sprite* translate(uint32_t hnd)
    {
        return &sprites[hnd];
    }

    void breakit()
    {
        auto saved = sprites;
        sprites.resize(0);
        sprites.resize(200);
        sprites[0] = saved[0];
        sprites[1] = saved[1];
        sprites[2] = saved[2];
    }

    handle<Sprite> create()
    {
        sprites.resize(sprites.size() + 1);
        
        handle<Sprite> hnd;
        hnd.handle = sprites.size() - 1;
        hnd.handleManager = this;
        return hnd;
    }

    std::vector<Sprite> sprites;
};

class User
{
public:
    handle<Sprite> s;
};

SpriteManager sm;
User a, b, c;

void tick() {
    std::cout << "Value A: " << a.s->color << std::endl;
    std::cout << "Value B: " << b.s->color << std::endl;
    std::cout << "Value C: " << c.s->color << std::endl;

    a.s->color++;
    b.s->color++;
    c.s->color++;

    std::cout << "Value A: " << a.s->color << std::endl;
    std::cout << "Value B: " << b.s->color << std::endl;
    std::cout << "Value C: " << c.s->color << std::endl;
}

void move() {
    sm.breakit();
}

int main(int argc, char** argv)
{
    a.s = sm.create();
    b.s = a.s;
    c.s = sm.create();

    tick();
    move();
    tick();

    std::cout << "All went well!!" << std::endl;

    return 0;
}