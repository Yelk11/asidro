#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#include "map.h"
#include "underworld.h"

/* ---------------------- helpers: node allocation & free -------------------- */
static bsp_node* make_node(int x, int y, int w, int h)
{
    bsp_node* n = malloc(sizeof(bsp_node));
    if (!n) return NULL;
    n->x = x; n->y = y; n->w = w; n->h = h;
    n->room_x = n->room_y = n->room_w = n->room_h = 0;
    n->has_room = false;
    n->left = n->right = NULL;
    return n;
}

void free_bsp(bsp_node *root)
{
    if (!root) return;
    if (root->left) free_bsp(root->left);
    if (root->right) free_bsp(root->right);
    free(root);
}

/* ---------------------------- safe splitting ------------------------------ */
/* Try to split node. Return 1 if split happened, 0 otherwise. */
static int split_node(bsp_node* n)
{
    if (!n) return 0;
    if (n->left || n->right) return 0;  /* already split */

    /* if too small to split in either direction, bail */
    if (n->w < MIN_LEAF_SIZE * 2 && n->h < MIN_LEAF_SIZE * 2) return 0;

    bool split_h; /* split horizontally (cut along Y) if true, vertically if false */

    if (n->w > n->h) split_h = false;
    else if (n->h > n->w) split_h = true;
    else split_h = (rand() & 1);

    if (split_h) {
        /* vertical cut into top/bottom: choose split so both children >= MIN_LEAF_SIZE */
        int min = MIN_LEAF_SIZE;
        int max_allowed = n->h - MIN_LEAF_SIZE;
        int range_len = max_allowed - min + 1; /* inclusive */

        if (range_len <= 0) return 0; /* cannot split safely */

        int s = min + (rand() % range_len); /* s in [min, n->h - min] */

        n->left  = make_node(n->x,     n->y, n->w, s);
        n->right = make_node(n->x, n->y + s, n->w, n->h - s);

    } else {
        /* horizontal cut into left/right */
        int min = MIN_LEAF_SIZE;
        int max_allowed = n->w - MIN_LEAF_SIZE;
        int range_len = max_allowed - min + 1;

        if (range_len <= 0) return 0;

        int s = min + (rand() % range_len); /* s in [min, n->w - min] */

        n->left  = make_node(n->x,     n->y, s,     n->h);
        n->right = make_node(n->x + s, n->y, n->w - s, n->h);
    }

    return 1;
}

/* ---------------------------- safe room creation -------------------------- */
/* Create room inside leaf node. Returns 1 if room created, 0 otherwise. */
static int create_room(bsp_node* n)
{
    if (!n) return 0;

    /* room must be at least MIN_ROOM_SIZE and fit with 1-cell border inside leaf */
    int max_w = n->w - 2; /* leave 1-cell walls at left/right */
    int max_h = n->h - 2;

    if (max_w < MIN_ROOM_SIZE || max_h < MIN_ROOM_SIZE) {
        n->has_room = false;
        return 0;
    }

    int room_w_max = max_w;
    int room_h_max = max_h;

    /* clamp MAX_ROOM_SIZE */
    if (room_w_max > MAX_ROOM_SIZE) room_w_max = MAX_ROOM_SIZE;
    if (room_h_max > MAX_ROOM_SIZE) room_h_max = MAX_ROOM_SIZE;

    int rw_range = room_w_max - MIN_ROOM_SIZE + 1;
    int rh_range = room_h_max - MIN_ROOM_SIZE + 1;

    int rw = MIN_ROOM_SIZE + (rand() % rw_range);
    int rh = MIN_ROOM_SIZE + (rand() % rh_range);

    int pos_x_range = n->w - rw - 1; /* number of possible x offsets: >=1 */
    int pos_y_range = n->h - rh - 1;

    if (pos_x_range <= 0 || pos_y_range <= 0) {
        n->has_room = false;
        return 0;
    }

    int rx = n->x + 1 + (rand() % pos_x_range);
    int ry = n->y + 1 + (rand() % pos_y_range);

    n->room_x = rx;
    n->room_y = ry;
    n->room_w = rw;
    n->room_h = rh;
    n->has_room = true;
    return 1;
}

/* carve a room into the map (bounded write) */
static void carve_room(map_t* map, bsp_node* n)
{
    if (!n || !n->has_room) return;

    int sx = n->room_x;
    int sy = n->room_y;
    int ex = sx + n->room_w;
    int ey = sy + n->room_h;

    /* clamp to map just in case (safety) */
    if (sx < 0) sx = 0;
    if (sy < 0) sy = 0;
    if (ex > MAP_WIDTH)  ex = MAP_WIDTH;
    if (ey > MAP_HEIGHT) ey = MAP_HEIGHT;

    for (int y = sy; y < ey; y++) {
        for (int x = sx; x < ex; x++) {
            map->map[y][x] = '.'; /* floor */
        }
    }
}

/* ---------------------------- corridor helpers --------------------------- */
/* Find a room center in a subtree. Returns 1 and fills cx,cy on success. */
static int find_room_center(bsp_node *n, int *cx, int *cy)
{
    if (!n) return 0;
    if (n->has_room) {
        *cx = n->room_x + n->room_w / 2;
        *cy = n->room_y + n->room_h / 2;
        return 1;
    }
    if (n->left && find_room_center(n->left, cx, cy)) return 1;
    if (n->right && find_room_center(n->right, cx, cy)) return 1;
    return 0;
}

/* Dig an L-shaped corridor between centers (safe bounds checks) */
static void connect_points(map_t* map, int ax, int ay, int bx, int by)
{
    int x = ax, y = ay;
    while (x != bx) {
        if (y >= 0 && y < MAP_HEIGHT && x >= 0 && x < MAP_WIDTH) map->map[y][x] = '.';
        x += (bx > x ? 1 : -1);
    }
    while (y != by) {
        if (y >= 0 && y < MAP_HEIGHT && x >= 0 && x < MAP_WIDTH) map->map[y][x] = '.';
        y += (by > y ? 1 : -1);
    }
}

/* Connect rooms under two subtrees: find any room center from each subtree. */
static void connect_nodes(map_t* map, bsp_node* a, bsp_node* b)
{
    if (!a || !b) return;

    int ax = 0, ay = 0, bx = 0, by = 0;
    if (!find_room_center(a, &ax, &ay)) return;
    if (!find_room_center(b, &bx, &by)) return;

    connect_points(map, ax, ay, bx, by);
}

/* ---------------------------- tree generation ---------------------------- */
/* Carve rooms for all leaves and connect sibling subtrees. The function
   returns 1 if this subtree contains at least one room. */
static int generate_tree(map_t* map, bsp_node* n)
{
    if (!n) return 0;

    /* If leaf, attempt to create a room */
    if (!n->left && !n->right) {
        if (create_room(n)) {
            carve_room(map, n);
            return 1;
        } else {
            n->has_room = false;
            return 0;
        }
    }

    /* internal node */
    int left_has = generate_tree(map, n->left);
    int right_has = generate_tree(map, n->right);

    /* If both sides have a room, connect them */
    if (left_has && right_has) {
        connect_nodes(map, n->left, n->right);
    } else if (left_has && !right_has) {
        /* try to connect left subtree to any room in right subtree (if any) */
        connect_nodes(map, n->left, n->right);
    } else if (!left_has && right_has) {
        connect_nodes(map, n->left, n->right);
    }

    return left_has || right_has;
}

/* ---------------------------- iterative splitting ------------------------ */
static void bsp_iterative_split(bsp_node *root)
{
    if (!root) return;

    int cap = 4096;
    bsp_node **queue = malloc(sizeof(bsp_node*) * cap);
    if (!queue) return;

    int head = 0, tail = 0;
    queue[tail++] = root;

    while (head < tail) {
        bsp_node *n = queue[head++];
        if (!n) continue;

        if (split_node(n)) {
            /* Enqueue children */
            if (tail + 2 > cap) {
                cap *= 2;
                bsp_node **tmp = realloc(queue, sizeof(bsp_node*) * cap);
                if (!tmp) break; /* allocation failed: abort splitting gracefully */
                queue = tmp;
            }
            queue[tail++] = n->left;
            queue[tail++] = n->right;
        }
    }

    free(queue);
}

/* ---------------------------- public API -------------------------------- */
bsp_node* generate_underworld(map_t* map)
{


    /* Ideally seed RNG once in program start; seeding here for convenience */
    srand((unsigned)time(NULL));

    /* fill with walls */
    for (int y = 0; y < MAP_HEIGHT; y++)
        for (int x = 0; x < MAP_WIDTH; x++)
            map->map[y][x] = '#';

    bsp_node* root = make_node(0, 0, MAP_WIDTH, MAP_HEIGHT);

    /* split iteratively (safe) */
    bsp_iterative_split(root);

    /* carve rooms and connect corridors */
    generate_tree(map, root);

    return root;
}

void under_get_player_spawn(map_t* map, int* x, int* y)
{
    if (!map || !x || !y) return;

    /* 1. Try to find *any* walkable tile in a room.
       Rooms are marked '.' by the generator. */

    const int MAX_ATTEMPTS = 5000;

    for (int i = 0; i < MAX_ATTEMPTS; i++) {
        int rx = rand() % MAP_WIDTH;
        int ry = rand() % MAP_HEIGHT;

        if (map->map[ry][rx] == '.') {
            *x = rx;
            *y = ry;
            return;
        }
    }

    /* 2. Fallback: brute scan (guaranteed not to hang) */
    for (int y0 = 0; y0 < MAP_HEIGHT; y0++) {
        for (int x0 = 0; x0 < MAP_WIDTH; x0++) {
            if (map->map[y0][x0] == '.') {
                *x = x0;
                *y = y0;
                return;
            }
        }
    }

    /* 3. Absolute worst-case fallback */
    *x = 1;
    *y = 1;
}

void under_get_npc_spawn(map_t* map, int* x, int* y, int px, int py)
{
    if (!map || !x || !y) return;

    const int MAX_ATTEMPTS = 5000;
    const int MIN_DIST_FROM_PLAYER = 12; // tweak to taste

    for (int i = 0; i < MAX_ATTEMPTS; i++) {

        int rx = rand() % MAP_WIDTH;
        int ry = rand() % MAP_HEIGHT;

        /* must be walkable/floor */
        if (map->map[ry][rx] != '.') continue;

        /* keep NPCs from spawning right beside the player */
        int dx = rx - px;
        int dy = ry - py;
        int dist2 = dx*dx + dy*dy;
        if (dist2 < MIN_DIST_FROM_PLAYER * MIN_DIST_FROM_PLAYER)
            continue;

        /* ensure the tile is not cramped
           (avoid tiny 1-tile dead ends) */
        int open_adj = 0;
        if (ry > 0              && map->map[ry-1][rx] == '.') open_adj++;
        if (ry < MAP_HEIGHT-1   && map->map[ry+1][rx] == '.') open_adj++;
        if (rx > 0              && map->map[ry][rx-1] == '.') open_adj++;
        if (rx < MAP_WIDTH-1    && map->map[ry][rx+1] == '.') open_adj++;

        if (open_adj < 2) continue; // avoid cramped dead ends

        *x = rx;
        *y = ry;
        return;
    }

    /* last-resort fallback */
    for (int ry = 0; ry < MAP_HEIGHT; ry++) {
        for (int rx = 0; rx < MAP_WIDTH; rx++) {
            if (map->map[ry][rx] == '.') {
                *x = rx;
                *y = ry;
                return;
            }
        }
    }

    /* absolute fallback */
    *x = 1;
    *y = 1;
}
