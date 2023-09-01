#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../lib/librayvector.h"

#define WIDTH 1024
#define HEIGHT 768

void render() {
    // Set the background color to RGB(0, 100, 255)
    Vec3f background_color = vec3f_init();
    background_color.data[0] = 0.0f;
    background_color.data[1] = 50.0f / 178.0f;
    background_color.data[2] = 0.5f;

    Vec3f *framebuffer = (Vec3f *)malloc(WIDTH * HEIGHT * sizeof(Vec3f));
    if (framebuffer == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }

    FILE *ofs = fopen("out.ppm", "wb");
    if (ofs == NULL) {
        fprintf(stderr, "Failed to open output file.\n");
        free(framebuffer);
        return;
    }

    fprintf(ofs, "P6\n%d %d\n255\n", WIDTH, HEIGHT);

    // Write the background color to the entire framebuffer
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        for (int k = 0; k < 3; k++) {
            unsigned char color = (unsigned char)(255 * fmaxf(0.0f, fminf(1.0f, background_color.data[k])));
            fwrite(&color, 1, 1, ofs);
        }
    }

    fclose(ofs);
    free(framebuffer);
}

int main() {
    render();
    return 0;
}
