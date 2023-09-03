#include "../include/sphere.h"
#define FLT_MAX 3.402823466e+38F


Sphere sphere_init(Vec3f center, float radius, Material material) {
  Sphere sphere;
  sphere.center = center;
  sphere.radius = radius;
  sphere.material = material;

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

bool scene_intersect(const Vec3f *ray_origin, const Vec3f *ray_direction, const Sphere *spheres, size_t num_spheres, Vec3f *hit_point, Vec3f *surface_normal, Material *intersected_material) {
    float nearest_intersection_dist = FLT_MAX; // FLT_MAX from float.h
    for (size_t i = 0; i < num_spheres; i++) {
        float intersection_dist;
        if (sphere_ray_intersect(&spheres[i], ray_origin, ray_direction, &intersection_dist) && intersection_dist < nearest_intersection_dist) {
            nearest_intersection_dist = intersection_dist;
            Vec3f intersection_point;
            intersection_point.data[0] = ray_origin->data[0] + ray_direction->data[0] * intersection_dist;
            intersection_point.data[1] = ray_origin->data[1] + ray_direction->data[1] * intersection_dist;
            intersection_point.data[2] = ray_origin->data[2] + ray_direction->data[2] * intersection_dist;
            *hit_point = intersection_point;
            Vec3f sphere_center_to_intersection;
            sphere_center_to_intersection.data[0] = intersection_point.data[0] - spheres[i].center.data[0];
            sphere_center_to_intersection.data[1] = intersection_point.data[1] - spheres[i].center.data[1];
            sphere_center_to_intersection.data[2] = intersection_point.data[2] - spheres[i].center.data[2];
            *surface_normal = vec3f_normalize(sphere_center_to_intersection);
            *intersected_material = spheres[i].material;
        }
    }
    return nearest_intersection_dist < 1000.0f;
}

Vec3f calculate_diffuse_reflection(Vec3f surface_point, Vec3f surface_normal, Material material, Light* lights, size_t num_lights) {
    Vec3f diffuse_reflection = {0.0f, 0.0f, 0.0f};

    for (size_t i = 0; i < num_lights; i++) {
        // Calculate light direction
        Vec3f light_direction;
        light_direction.data[0] = lights[i].position.data[0] - surface_point.data[0];
        light_direction.data[1] = lights[i].position.data[1] - surface_point.data[1];
        light_direction.data[2] = lights[i].position.data[2] - surface_point.data[2];

        // Normalize the light direction vector
        float light_distance = sqrtf(light_direction.data[1] * light_direction.data[1] + light_direction.data[2] * light_direction.data[1] + light_direction.data[2] * light_direction.data[2]);
        light_direction.data[0] /= light_distance;
        light_direction.data[1] /= light_distance;
        light_direction.data[2] /= light_distance;

        // Calculate the diffuse intensity
        float dot_product = light_direction.data[0] * surface_normal.data[0] + light_direction.data[1] * surface_normal.data[1] + light_direction.data[2] * surface_normal.data[2];
        float diffuse_intensity = lights[i].intensity * fmaxf(0.0f, dot_product);

        // Accumulate the diffuse reflection
        diffuse_reflection.data[0] += diffuse_intensity * material.material_color.data[0];
        diffuse_reflection.data[1] += diffuse_intensity * material.material_color.data[1];
        diffuse_reflection.data[2] += diffuse_intensity * material.material_color.data[2];
    }

    return diffuse_reflection;
}

Vec3f cast_ray(const Vec3f *orig, const Vec3f *dir, const Sphere *spheres,
               size_t num_spheres, Light* lights, size_t num_lights) {
    Vec3f hit_point;
    Vec3f surface_normal;
    Material intersected_material;

    if (scene_intersect(orig, dir, spheres, num_spheres, &hit_point, &surface_normal, &intersected_material)) {
        Vec3f diffuse_reflection = calculate_diffuse_reflection(hit_point, surface_normal, intersected_material, lights, num_lights);
        return diffuse_reflection;
    }

    // No intersection, return background color
    Vec3f bg_color = vec3f_init_values(0.2f, 0.7f, 0.8f);
    return bg_color;
}
