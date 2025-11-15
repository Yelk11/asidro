/* Overworld generator using value-noise (Perlin-like) fBm.
 * Produces a farmland-like map: mostly dirt, with patches of trees and rocks.
 */
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "overworld.h"

/* Simple integer hash / noise function. Deterministic for given x,y,seed.
 * Returns a double in range [-1, 1].
 */
static double value_noise_int(int x, int y, int seed) {
    int n = x + y * 57 + seed * 131;
    n = (n << 13) ^ n;
    int nn = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
    return 1.0 - ((double)nn / 1073741824.0);
}

static double lerp(double a, double b, double t) {
    return a + t * (b - a);
}

/* smootherstep fade function */
static double fade(double t) {
    /* 6t^5 - 15t^4 + 10t^3 */
    return t * t * t * (t * (t * 6 - 15) + 10);
}

/* Interpolated noise at fractional coordinates using bilinear interpolation
 * with smoothing via fade().
 */
static double interpolated_noise(double x, double y, int seed) {
    int ix = (int)floor(x);
    int iy = (int)floor(y);
    double fx = x - ix;
    double fy = y - iy;

    double v00 = value_noise_int(ix, iy, seed);
    double v10 = value_noise_int(ix + 1, iy, seed);
    double v01 = value_noise_int(ix, iy + 1, seed);
    double v11 = value_noise_int(ix + 1, iy + 1, seed);

    double sx = fade(fx);
    double sy = fade(fy);

    double ix0 = lerp(v00, v10, sx);
    double ix1 = lerp(v01, v11, sx);
    return lerp(ix0, ix1, sy);
}

/* Fractional Brownian motion (fBm) combining multiple octaves.
 * Returns value in approx [-1,1].
 */
static double perlin_fbm(double x, double y, int seed, int octaves, double persistence, double lacunarity) {
    double amplitude = 1.0;
    double frequency = 1.0;
    double sum = 0.0;
    double max = 0.0;

    for (int o = 0; o < octaves; o++) {
        double v = interpolated_noise(x * frequency, y * frequency, seed + o * 997);
        sum += v * amplitude;
        max += amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }
    /* normalize to [-1,1] roughly */
    return sum / max;
}

void generate_overworld(map_t *map) {
    if (!map) return;

    /* Parameters chosen to give large smooth patches (farmland-looking).
     * Scale maps coordinate space into noise space; larger scale = bigger features.
     */
    const double scale = 0.06;    /* base frequency */
    const int octaves = 5;
    const double persistence = 0.5;
    const double lacunarity = 2.0;

    /* thresholds (normalized to [0,1]) -- tuned for majority dirt */
    const double dirt_thresh = 0.72;   /* below -> dirt */
    const double tree_thresh = 0.88;   /* between dirt_thresh and tree_thresh -> tree */
    /* above tree_thresh -> rock */

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            double nx = x * scale;
            double ny = y * scale;
            double v = perlin_fbm(nx, ny, map->seed, octaves, persistence, lacunarity);
            /* normalize to [0,1] */
            double n = (v + 1.0) * 0.5;

            if (n < dirt_thresh) {
                set_tile(map, x, y, '.'); /* dirt */
            } else if (n < tree_thresh) {
                set_tile(map, x, y, 'T'); /* tree */
            } else {
                set_tile(map, x, y, 'o'); /* rock */
            }
        }
    }
}



