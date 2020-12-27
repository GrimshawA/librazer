#ifndef RZCONFIG_H_
#define RZCONFIG_H_

#ifdef _MSC_VER

#if !defined RZ_STATIC
#ifdef RZ_EXPORT
#define RZ_API __declspec(dllexport)
#else
#define RZ_API __declspec(dllimport)
#endif
#else
#define RZ_API
#endif

#else
#define RZ_API
#endif

#endif // RZCONFIG_H_
