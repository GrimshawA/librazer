#ifndef ExportStdModule_h__
#define ExportStdModule_h__

class RzModule;
class RzEngine;

void RegisterStd(RzEngine* ctx);

void RegisterWindow(RzModule* m);
void RegisterIO(RzModule* m);
void RegisterNet(RzModule* m);

#endif // ExportStdModule_h__
