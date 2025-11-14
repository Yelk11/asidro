#ifndef OVERWORLD_H
#define OVERWORLD_H

#include "map.h"

/* Generate an overworld using Perlin-style value noise.
 * The generator fills `map->map` with the following characters:
 *  - '.' dirt (dominant)
 *  - 'T' tree
 *  - 'o' rock
 */
void generate_overworld(map_t *map, int seed);

#endif
