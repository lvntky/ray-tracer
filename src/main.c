#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/sphere.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"

#define WIDTH 1024
#define HEIGHT 768
#define SUPER_SAMPLING 4  // Adjust this for anti-aliasing level

int bg_width, bg_height, bg_channels;
unsigned char *background_image;

Vec3f background_color; // vec3f_init_values(0.2f, 0.7f, 0.8f);

void render(const Sphere *spheres, size_t num_spheres, Light *lights, size_t num_lights) {
    background_color = vec3f_init_values(0.2f, 0.7f, 0.8f);
    float fov = M_PI / 2.0f; // Set the field of view to 90 degrees
    Vec3f *frame_buffer = (Vec3f *)malloc(WIDTH * HEIGHT * sizeof(Vec3f));
    if (frame_buffer == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }

    // Load background image and get its dimensions
    int bg_width, bg_height, bg_channels;
    unsigned char *background_image = stbi_load("../doc/background.jpg", &bg_width, &bg_height, &bg_channels, 0);

#pragma omp parallel for
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            Vec3f pixel_color = vec3f_init(); // Initialize pixel color to black

            // Supersampling loop (sub-pixels for anti-aliasing)
            for (int s = 0; s < SUPER_SAMPLING; s++) {
                for (int t = 0; t < SUPER_SAMPLING; t++) {
                    float x = (2 * (i + (s + 0.5) / SUPER_SAMPLING) / (float)WIDTH - 1) * tanf(fov / 2) * WIDTH / (float)HEIGHT;
                    float y = -(2 * (j + (t + 0.5) / SUPER_SAMPLING) / (float)HEIGHT - 1) * tanf(fov / 2);

                    // Ray direction calculation
                    Vec3f dir = vec3f_normalize(vec3f_init_values(x, y, -1.0f));

                    // Ray origin (camera position)
                    Vec3f ray_origin = vec3f_init_values(0.0f, 0.0f, 0.0f);

                    // Check for intersections with spheres
                    Vec3f hit_point;
                    Vec3f surface_normal;
                    Material intersected_material;

                    if (scene_intersect(&ray_origin, &dir, spheres, num_spheres, &hit_point, &surface_normal, &intersected_material)) {
                        // Intersection occurred, calculate lighting
                        Vec3f diffuse_reflection = calculate_diffuse_reflection(hit_point, surface_normal, intersected_material, lights, num_lights);
                        pixel_color = diffuse_reflection;
                    } else {
                        // No intersection, set pixel color from the stretched background image
                        // Calculate the background image coordinates (stretching)
                        float bg_x = ((WIDTH - i) / (float)WIDTH) * bg_width;
                        float bg_y = (j / (float)HEIGHT) * bg_height;
                        int bg_i = (int)bg_x;
                        int bg_j = (int)bg_y;
                        unsigned char r, g, b;

                        // Check if within background image bounds
                        if (bg_i >= 0 && bg_i < bg_width && bg_j >= 0 && bg_j < bg_height) {
                            int bg_index = bg_i + bg_j * bg_width;
                            r = background_image[bg_index * bg_channels];
                            g = background_image[bg_index * bg_channels + 1];
                            b = background_image[bg_index * bg_channels + 2];
                        } else {
                            // Use the background color if outside background image bounds
                            r = (unsigned char)(background_color.data[0] * 255);
                            g = (unsigned char)(background_color.data[1] * 255);
                            b = (unsigned char)(background_color.data[2] * 255);
                        }
                        pixel_color = vec3f_init_values(r / 255.0, g / 255.0, b / 255.0);
                    }

                    // Accumulate pixel color from multiple rays
                    frame_buffer[i + j * WIDTH] = vec3f_add(frame_buffer[i + j * WIDTH], pixel_color);
                }
            }

            // Average the accumulated pixel color
            frame_buffer[i + j * WIDTH].data[0] /= (SUPER_SAMPLING * SUPER_SAMPLING);
            frame_buffer[i + j * WIDTH].data[1] /= (SUPER_SAMPLING * SUPER_SAMPLING);
            frame_buffer[i + j * WIDTH].data[2] /= (SUPER_SAMPLING * SUPER_SAMPLING);
        }
    }

    FILE *ofs = fopen("out.ppm", "wb");
    if (ofs == NULL) {
        fprintf(stderr, "Failed to open output file.\n");
        free(frame_buffer);
        return;
    }

    fprintf(ofs, "P6\n%d %d\n255\n", WIDTH, HEIGHT);

    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            int index = i + j * WIDTH;

            // Convert the pixel color to the 0-255 range
            unsigned char r = (unsigned char)(frame_buffer[index].data[0] * 255);
            unsigned char g = (unsigned char)(frame_buffer[index].data[1] * 255);
            unsigned char b = (unsigned char)(frame_buffer[index].data[2] * 255);

            // Write to the file (note the reversed order for j)
            fwrite(&r, 1, 1, ofs);
            fwrite(&g, 1, 1, ofs);
            fwrite(&b, 1, 1, ofs);
        }
    }

    fclose(ofs);
    free(frame_buffer);
    stbi_image_free(background_image);
}

int main() {
    const int num_spheres = 5;
    const int num_lights = 1;

    Sphere spheres[num_spheres];
    Material red_velvet;
    Material ivory;

    red_velvet.material_color = vec3f_init_values(0.3, 0.1, 0.1);
    ivory.material_color = vec3f_init_values(0.4, 0.4, 0.3);

    spheres[0] = sphere_init(vec3f_init_values(4.0f, 3.0f, -10.0f), 2.0f, red_velvet);
    spheres[1] = sphere_init(vec3f_init_values(6.0f, 1.5f, -8.0f), 1.5f, ivory);
    spheres[2] = sphere_init(vec3f_init_values(2.5f, 2.0f, -15.0f), 2.0f, ivory);
    spheres[3] = sphere_init(vec3f_init_values(0.0f, -2.0f, -12.0f), 1.0f, red_velvet);
    spheres[4] = sphere_init(vec3f_init_values(-7.0f, 8.0f, -10.0f), 2.0f, ivory);

    Light lights[num_lights];
    lights[0].position = vec3f_init_values(-50, 20, 20);
    lights[0].intensity = 1.5f;

    render(spheres, num_spheres, lights, num_lights);

    return 0;
}
