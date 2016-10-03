#include <AEON/stdlib/ExportStd.h>

#include <AEON/stdlib/window/Window.h>

void RegisterStd(AEContext* ctx)
{
    AEModule* stdModule = ctx->createModule("std");
    RegisterIO(stdModule);
    RegisterNet(stdModule);
    RegisterWindow(stdModule);
}

void RegisterWindow(AEModule* m)
{
    m->registerType("Window", sizeof(Window));
}

void RegisterIO(AEModule* m)
{

}

void RegisterNet(AEModule* m)
{

}
