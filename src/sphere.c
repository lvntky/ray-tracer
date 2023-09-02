#include "../include/sphere.h"
#define FLT_MAX 3.402823466e+38F


Sphere sphere_init(Vec3f center, float radius) {
  Sphere sphere;
  sphere.center = center;
  sphere.radius = radius;

  return sphere;
}

bool sphere_ray_intersect(Sphere *sphere, Vec3f *ray_origin, Vec3f *ray_direction, float *intersection_distance) {
  // distance between rays origin and sphere's center
  Vec3f ray_to_center = vec3f_sub(sphere->center, *ray_origin);

  // Distance from the ray's origin to the closest point on the ray to the sphere's center
  float tca = vec3f_dot(ray_to_center, *ray_direction);

  // if tca is negative, sphere is behind the ray origin
  if(tca < 0) {
    return false;
  }

  // Squared distance from the closest point to the sphere's center
  float d2 = vec3f_dot(ray_to_center, ray_to_center) - tca * tca;

  // If d2 is greater than the sphere's radius squared, there's no intersection
  if (d2 > sphere->radius * sphere->radius) {
    return false;
  }

  // Distance from the closest point to the intersection point
  float thc = sqrtf(sphere->radius * sphere->radius - d2);

  // Compute the two possible intersection distances
  float t0 = tca - thc;
  float t1 = tca + thc;

  // check either of the intersection points behind of the ray
  if(t0 < 0 && t1 < 0) {
    return false;
  }

  // Store the intersection distance in intersection_distance (the closest intersection point)
  *intersection_distance = (t0 < 0) ? t1 : t0;

  return true;
}

Vec3f cast_ray(const Vec3f *orig, const Vec3f *dir, const Sphere *sphere) {
    float sphere_dist = FLT_MAX; // FLT_MAX from float.h

    if (!sphere_ray_intersect(sphere, orig, dir, &sphere_dist)) {
        // background color
        Vec3f bg_color = vec3f_init_values(0.2f, 0.7f, 0.8f);
        return bg_color;
    }

    // sphere intersection color
    Vec3f sp_color = vec3f_init_values(0.4f, 0.4f, 0.3f);
    return sp_color;
}
