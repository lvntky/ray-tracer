#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "../lib/librayvector.h"

typedef struct Light {
    Vec3f position;
    float intensity;
} Light;
#endif // __LIGHT_H__
