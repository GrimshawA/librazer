#ifndef ExportStdModule_h__
#define ExportStdModule_h__

#include <razer/config.h>

class RzModule;
class RzEngine;

RZ_API void RegisterStd(RzEngine* ctx);

RZ_API void RegisterWindow(RzModule* m);
RZ_API void RegisterIO(RzModule* m);
RZ_API void RegisterNet(RzModule* m);

#endif // ExportStdModule_h__
