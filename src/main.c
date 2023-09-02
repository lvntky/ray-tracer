#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/sphere.h"

#define WIDTH 1024
#define HEIGHT 768

void render(const Sphere *sphere) {
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
                frame_buffer[index] = cast_ray(&ray_origin, &dir, sphere);
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
    Sphere sphere;
    sphere.center = vec3f_init_values(0.0f, 0.0f, -16.0f);
    sphere.radius = 2.0f;

    render(&sphere);

    return 0;
}
