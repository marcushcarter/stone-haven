
#include "noise.c"

void destroy_particles();

typedef enum {
    STRUCT1,
} StructureType;

bool generate_structure(StructureType type, int worldx, int worldy) {
    if (type == STRUCT1) {
        printf("Structure %d generated at x:%d y:%d\n", type, worldx, worldy);
    }
}

bool generate_world() {

    int **grid = (int **)malloc(WORLD_HEIGHT * sizeof(int *));
    for (int i = 0; i < WORLD_HEIGHT; i++) {
        grid[i] = (int *)malloc(WORLD_WIDTH * sizeof(int));
    }

    int **grid2 = (int **)malloc(WORLD_HEIGHT * sizeof(int *));
    for (int i = 0; i < WORLD_HEIGHT; i++) {
        grid2[i] = (int *)malloc(WORLD_WIDTH * sizeof(int));
    }

    /* World Generation Process: 
        
        1.) sets all blocks to AIR to initialize all blocks from NULL
        
        2.) sets all the AIR blocks to STONE
        3.) generates biome locations based on a large automaton grid
        4.) generates pockets of other stones based on the biomes they are in
        5.) generates the ores in the world (from common to rarest) (also based on biomes)
        6.) generates the cave systems from another autamaton grid
        7.) generates the finishing touches in the world (non solid objects)

        8.) generates all the structures in the world

        9.) spawn the players spawn box and makes the world borders
    
    */

    // STONE BLOCKS

    generate_cave(grid, WORLD_WIDTH, WORLD_HEIGHT, 0.5, 3);
    for (int x = 0; x < WORLD_WIDTH; x++){
        for (int y = 0; y < WORLD_HEIGHT; y++){
            world[x][y] = block[BLOCK_STONE];
            if (grid[y][x] == 1) {
                world[x][y] = block[BLOCK_SMOOTH_STONE];
            }
        }
    }

    generate_cave(grid, WORLD_WIDTH, WORLD_HEIGHT, 0.45, 3);
    generate_cave(grid2, WORLD_WIDTH, WORLD_HEIGHT, 0.5, 3);
    for (int x = 0; x < WORLD_WIDTH; x++){
        for (int y = 0; y < WORLD_HEIGHT; y++){
            if (grid2[y][x] == 1) {
                world[x][y] = block[BLOCK_DARK_STONE];
                if (grid[y][x] == 1) {
                    world[x][y] = block[BLOCK_SMOOTH_DARK_STONE];
                }
            }
        }
    }

    generate_cave(grid, WORLD_WIDTH, WORLD_HEIGHT, 0.4, 3);
    for (int x = 0; x < WORLD_WIDTH; x++){
        for (int y = 0; y < WORLD_HEIGHT; y++){
            if (grid[y][x] == 1) {
                world[x][y] = block[BLOCK_BLACK_STONE];
            }
        }
    }

    // DIRT PATCHES

    generate_cave(grid, WORLD_WIDTH, WORLD_HEIGHT, 0.35, 15);
    for (int x = 0; x < WORLD_WIDTH; x++){
        for (int y = 0; y < WORLD_HEIGHT; y++){
            if (grid[y][x] == 1) {
                world[x][y] = block[BLOCK_DIRT];
            }
        }
    }

    // ORES

    generate_cave(grid, WORLD_WIDTH, WORLD_HEIGHT, 0.016, 0);
    for (int x = 0; x < WORLD_WIDTH; x++){
        for (int y = 0; y < WORLD_HEIGHT; y++){
            if (grid[y][x] == 1 && world[x][y] == block[BLOCK_STONE]) {
                    world[x][y] = block[BLOCK_EMERALD_ORE];
            }
        }
    }

    generate_cave(grid, WORLD_WIDTH, WORLD_HEIGHT, 0.016, 0);
    for (int x = 0; x < WORLD_WIDTH; x++){
        for (int y = 0; y < WORLD_HEIGHT; y++){
            if (grid[y][x] == 1 && world[x][y] == block[BLOCK_STONE]) {
                world[x][y] = block[BLOCK_SILVER_ORE];
            }
        }
    }

    generate_cave(grid, WORLD_WIDTH, WORLD_HEIGHT, 0.04, 0);
    for (int x = 0; x < WORLD_WIDTH; x++){
        for (int y = 0; y < WORLD_HEIGHT; y++){
            if (grid[y][x] == 1 && world[x][y] == block[BLOCK_BLACK_STONE]) {
                    world[x][y] = block[BLOCK_GOLD_ORE];
            }
        }
    }

    generate_cave(grid, WORLD_WIDTH, WORLD_HEIGHT, 0.008, 0);
    for (int x = 0; x < WORLD_WIDTH; x++){
        for (int y = 0; y < WORLD_HEIGHT; y++){
            if (grid[y][x] == 1 && world[x][y] == block[BLOCK_DARK_STONE]) {
                    world[x][y] = block[BLOCK_DIAMOND_ORE];
            }
        }
    }

    generate_cave(grid, WORLD_WIDTH, WORLD_HEIGHT, 0.43, 6);
    for (int x = 0; x < WORLD_WIDTH; x++){
        for (int y = 0; y < WORLD_HEIGHT; y++){
            if (grid[y][x] == 1) {
                world[x][y] = block[BLOCK_AIR];
            }
        }
    }


    // for (int x = 0; x < WORLD_WIDTH; x++){
    //     for (int y = 0; y < WORLD_HEIGHT; y++){
    //         //
    //     }
    // }


    // spawn box and worldborder
    for (int x = 0; x < 3; x++){ for (int y = 0; y < 3; y++){ world[(int)(x+WORLD_WIDTH/2-5/2)][(int) (y+WORLD_HEIGHT/2-3/2)] = block[BLOCK_ROSE]; } }
    for (int x = 0; x < WORLD_WIDTH; x++) { world[x][1] = block[BLOCK_WORLD_BORDER]; world[x][WORLD_HEIGHT - 2] = block[BLOCK_WORLD_BORDER]; }
    for (int y = 0; y < WORLD_HEIGHT; y++) { world[1][y] = block[BLOCK_WORLD_BORDER]; world[WORLD_WIDTH - 2][y] = block[BLOCK_WORLD_BORDER]; }

    miner.x = (WORLD_WIDTH-2)*32;
    miner.y = (WORLD_HEIGHT+2)*32;
    miner.vx = 0;
    miner.vy = 0;

    update_blocks(true);
    destroy_particles();
    clear_inventory();

    statistics.blocks_broken = 0;
    statistics.blocks_placed = 0;
    statistics.distance_travelled = 0;
    statistics.seconds_played = 0;
    statistics.items_collected = 0;

    return true;
}

typedef enum {
    TAPESTRY_RANDOM_BLOCKS,
    TAPESTRY_WORLD_GENERATION,
    TAPESTRY_SOLID_BLOCKS,
    TAPESTRY_STRUCTURE,
} TapestryType;

bool make_menu_tapestry(TapestryType type) {

    if (type == TAPESTRY_RANDOM_BLOCKS) {
        for (int x = 0; x < TAPESTRY_WIDTH; x++){
            for (int y = 0; y < TAPESTRY_HEIGHT; y++){
                menu_tapestry[x][y] = block[randint(1, BLOCK_COUNT)];
            }
        }
    }

}
