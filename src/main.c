#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/sphere.h"

#define WIDTH 1024
#define HEIGHT 768

void render(const Sphere *sphere, size_t sphere_num, Light* lights, size_t light_num) {
    float fov = M_PI_2 / 2.0f; // Corrected fov to float
    Vec3f *frame_buffer = (Vec3f *)malloc(WIDTH * HEIGHT * sizeof(Vec3f));
    if (frame_buffer == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }

    #pragma omp parallel for
    for(int j = 0; j < HEIGHT; j++) {
        for(int i = 0; i < WIDTH; i++) {
            int index = i + j * WIDTH;

            float x = (2 * (i + 0.5) / (float)WIDTH - 1) * tanf(fov / 2) * WIDTH / (float)HEIGHT;
            float y = -(2 * (j + 0.5) / (float)HEIGHT - 1) * tanf(fov / 2);
            Vec3f dir = vec3f_normalize(vec3f_init_values(x, y, -1.0f)); // Corrected dir to -1.0f

            // ray origin : x = 0, y = 0, z = 0
            Vec3f ray_origin = vec3f_init_values(0.0f, 0.0f, 0.0f);

            // Ensure that index is within bounds
            if (index >= 0 && index < WIDTH * HEIGHT) {
	      frame_buffer[index] = cast_ray(&ray_origin, &dir, sphere, sphere_num, lights, light_num);
            }
        }
    }

    FILE *ofs = fopen("out.ppm", "wb");
    if (ofs == NULL) {
        fprintf(stderr, "Failed to open output file.\n");
        free(frame_buffer);
        return;
    }

    fprintf(ofs, "P6\n%d %d\n255\n", WIDTH, HEIGHT);

    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        unsigned char r = (unsigned char)(frame_buffer[i].data[0] * 255);
        unsigned char g = (unsigned char)(frame_buffer[i].data[1] * 255);
        unsigned char b = (unsigned char)(frame_buffer[i].data[2] * 255);
        fwrite(&r, 1, 1, ofs);
        fwrite(&g, 1, 1, ofs);
        fwrite(&b, 1, 1, ofs);
    }

    fclose(ofs);

    free(frame_buffer);
}

int main() {
    const int num_spheres = 4; // Set the number of spheres
    const int num_light = 1;

    Sphere spheres[num_spheres];
    Material red_velvet;
    Material ivory;

    red_velvet.material_color = vec3f_init_values(0.3, 0.1, 0.1);
    ivory.material_color = vec3f_init_values(0.4, 0.4, 0.3);

    
    spheres[0].center = vec3f_init_values(4.0f, 3.0f, -15.0f);
    spheres[0].radius = 2.0f;
    spheres[0].material = red_velvet;

    spheres[1].center = vec3f_init_values(4.0f, 1.0f, -12.0f);
    spheres[1].radius = 1.5f;
    spheres[1].material = ivory;

    
    spheres[2].center = vec3f_init_values(1.0f, 1.1f, -11.0f);
    spheres[2].radius = 1.0;
    spheres[2].material = ivory;

    spheres[3].center = vec3f_init_values(-3.0f, -0.5f, -13.0f);
    spheres[3].radius = 1.0f;
    spheres[3].material = red_velvet;

    Light lights[num_light];
    lights[0].position = vec3f_init_values(-50, 20, 20);
    lights[0].intensity = 1.5f;

    render(spheres, num_spheres, lights, num_light);

    return 0;
}
