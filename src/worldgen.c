
#include "noise.c"

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

    // set all blocks to air
    for (int x = 0; x < WORLD_WIDTH; x++){
        for (int y = 0; y < WORLD_HEIGHT; y++){
            world[x][y] = block[BLOCK_STONE];
        }
    }

    generate_cave(grid, WORLD_WIDTH, WORLD_HEIGHT, 0.43, 4);
    for (int x = 0; x < WORLD_WIDTH; x++){
        for (int y = 0; y < WORLD_HEIGHT; y++){
            if (grid[y][x] == 0) {
                world[x][y] = block[BLOCK_BLACKSTONE];
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

    // spawn box and worldborder
    for (int x = 0; x < 3; x++){ for (int y = 0; y < 3; y++){ world[(int)(x+WORLD_WIDTH/2-5/2)][(int) (y+WORLD_HEIGHT/2-3/2)] = block[BLOCK_AIR]; } }
    for (int x = 0; x < WORLD_WIDTH; x++) { world[x][1] = block[BLOCK_WORLD_BORDER]; world[x][WORLD_HEIGHT - 2] = block[BLOCK_WORLD_BORDER]; }
    for (int y = 0; y < WORLD_HEIGHT; y++) { world[1][y] = block[BLOCK_WORLD_BORDER]; world[WORLD_WIDTH - 2][y] = block[BLOCK_WORLD_BORDER]; }

    miner.x = (WORLD_WIDTH-2)*32;
    miner.y = (WORLD_HEIGHT+2)*32;
    miner.vx = 0;
    miner.vy = 0;

    return true;
}