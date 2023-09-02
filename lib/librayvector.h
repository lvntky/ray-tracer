/* librayvector.h | Levent Kaya - 2023
 *
 * The librayvector is a lightweight single-header library for performing vector operations in C programming.
 * It provides basic functionalities for working with 2D and 3D vectors,
 * including addition, subtraction, dot product, cross product, normalization, and more.
 * This library is suitable for C developers who need simple vector manipulation
 * capabilities in their projects.
 *
 */

#ifndef __LIBRAYVECTOR_H__
#define __LIBRAYVECTOR_H__

#include <math.h>
#include <stdio.h>

#define DIMENSION 3
#define PLANE 2

typedef struct {
  float data[DIMENSION];
} Vec3f;

typedef struct {
  float x, y;
} Vec2f;

// 3D Vector function implementations

static inline Vec3f vec3f_init() {
  Vec3f vec;
  for(int i = 0; i < DIMENSION; i++) {
    vec.data[i] = 0.0f;
  }
  return vec;
}

static inline Vec3f vec3f_init_values(float x, float y, float z) {
    Vec3f vec;
    vec.data[0] = x;
    vec.data[1] = y;
    vec.data[2] = z;
    return vec;
}


static inline Vec3f vec3f_add(Vec3f vec1, Vec3f vec2) {
  Vec3f result = vec3f_init();
  for(int i = 0; i < DIMENSION; i++) {
    result.data[i] = vec1.data[i] + vec2.data[i];
  }
  return result;
}

static inline Vec3f vec3f_sub(Vec3f vec1, Vec3f vec2) {
  Vec3f result = vec3f_init();
  for(int i = 0; i < DIMENSION; i++) {
    result.data[i] = vec1.data[i] - vec2.data[i];
  }
  return result;
}

static inline float vec3f_dot(Vec3f vec1, Vec3f vec2) {
  float result = 0.0f;
  for(int i = 0; i < DIMENSION; i++) {
    result += vec1.data[i] * vec2.data[i];
  }
  return result;
}

static inline Vec3f vec3f_cross(Vec3f v1, Vec3f v2) {
    Vec3f result;
    result.data[0] = v1.data[1] * v2.data[2] - v1.data[2] * v2.data[1];
    result.data[1] = v1.data[2] * v2.data[0] - v1.data[0] * v2.data[2];
    result.data[2] = v1.data[0] * v2.data[1] - v1.data[1] * v2.data[0];
    return result;
}

static inline float vec3f_norm(Vec3f vec) {
    float result = 0.0f;
    for (int i = 0; i < DIMENSION; i++) {
        result += vec.data[i] * vec.data[i];
    }
    return sqrtf(result);
}

static inline Vec3f vec3f_normalize(Vec3f vec) {
    float norm = vec3f_norm(vec);
    if (norm != 0.0f) {
        for (int i = 0; i < DIMENSION; i++) {
            vec.data[i] /= norm;
        }
    }
    return vec;
}

// 2D Vector functions implementation
static inline Vec2f vec2f_init() {
  Vec2f vec;
  vec.x = 0.0f;
  vec.y = 0.f;
  return vec;
}

static inline Vec2f vec2f_add(Vec2f vec1, Vec2f vec2) {
  Vec2f result = vec2f_init();
  result.x = vec1.x + vec2.x;
  result.y = vec1.y + vec2.y;
  return result;
}

static inline Vec2f vec2f_sub(Vec2f vec1, Vec2f vec2) {
  Vec2f result = vec2f_init();
  result.x = vec1.x - vec2.x;
  result.y = vec1.y - vec2.y;
  return result;
}

static inline float vec2f_dot(Vec2f vec1, Vec2f vec2) {
  return vec1.x + vec2.x + vec1.y * vec2.y;
}

// Helper print functions
static inline void vec3f_print(Vec3f vec) {
  for(int i = 0; i < DIMENSION; i++) {
    printf("%f ", vec.data[i]);
  }
}

static inline void vec3f_print_pretty(Vec3f* vec) {
  printf("< ");
  for(int i = 0; i < DIMENSION; i++) {
    printf("%f, ", vec->data[i]);
  }
  printf(" >");
}

static inline void vec2f_print(Vec2f vec) { printf("%f %f", vec.x, vec.y); }

static inline void vec2f_print_pretty(Vec2f vec) { printf("< %f, %f >", vec.x, vec.y); }

#endif
