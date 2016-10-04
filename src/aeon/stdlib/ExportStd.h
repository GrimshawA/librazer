#ifndef ExportStdModule_h__
#define ExportStdModule_h__

class AEModule;
class AEContext;

void RegisterStd(AEContext* ctx);

void RegisterWindow(AEModule* m);
void RegisterIO(AEModule* m);
void RegisterNet(AEModule* m);

#endif // ExportStdModule_h__
