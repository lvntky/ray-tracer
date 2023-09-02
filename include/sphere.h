#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "../lib/librayvector.h"
#include "material.h"
#include <stdbool.h>


typedef struct {
  Vec3f center;
  float radius;
  Material material;
} Sphere;

Sphere sphere_init(Vec3f, float, Material);
bool sphere_ray_intersect(Sphere *, Vec3f *, Vec3f *, float *);
Vec3f cast_ray(const Vec3f *, const Vec3f *, const Sphere *, size_t);
bool scene_intersect(const Vec3f *, const Vec3f *, const Sphere *, size_t , Vec3f *, Vec3f *, Material *);

#endif
