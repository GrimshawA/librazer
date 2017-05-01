#ifndef RZBYTEARRAY_H__
#define RZBYTEARRAY_H__

#include <vector>
#include <stdint.h>

class RzModule;

class RzByteArray {
public:

    int32_t size();

public:
    static void registerApi(RzModule& m);

private:
    std::vector<uint8_t> m_data;
};

#endif

