#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <string>

class AEModule;
class aeBindMethod;

void registerFunction(AEModule* module, const std::string& signature, aeBindMethod* fn);

#endif // REGISTRATION_H
