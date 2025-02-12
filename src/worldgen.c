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
        7.) generates the fiishing touches in the world (non solid objects)

        8.) generates all the structures in the world

        9.) spawn the players spawn box and makes the world borders
    
    */

    // set all blocks to air
    for (int x = 0; x < WORLD_WIDTH; x++){
        for (int y = 0; y < WORLD_HEIGHT; y++){
            world[x][y] = block[B_STONE];
        }
    }

    generate_cave(grid, WORLD_WIDTH, WORLD_HEIGHT, 0.43, 4);
    for (int x = 0; x < WORLD_WIDTH; x++){
        for (int y = 0; y < WORLD_HEIGHT; y++){
            if (grid[y][x] == 0) {
                world[x][y] = block[B_GLOWSTONE];
            }
        }
    }

    generate_cave(grid, WORLD_WIDTH, WORLD_HEIGHT, 0.43, 6);
    for (int x = 0; x < WORLD_WIDTH; x++){
        for (int y = 0; y < WORLD_HEIGHT; y++){
            if (grid[y][x] == 1) {
                world[x][y] = block[B_AIR];
            }
        }
    }

    //

    // spawn box and worldborder
    for (int x = 0; x < 3; x++){ for (int y = 0; y < 3; y++){ world[(int)(x+WORLD_WIDTH/2-5/2)][(int) (y+WORLD_HEIGHT/2-3/2)] = block[B_AIR]; } }
    for (int x = 0; x < WORLD_WIDTH; x++) { world[x][0] = block[B_WORLDBORDER]; world[x][WORLD_HEIGHT - 1] = block[B_WORLDBORDER]; }
    for (int y = 0; y < WORLD_HEIGHT; y++) { world[0][y] = block[B_WORLDBORDER]; world[WORLD_WIDTH - 1][y] = block[B_WORLDBORDER]; }

    miner.x = (WORLD_WIDTH-2)*32;
    miner.y = (WORLD_HEIGHT+2)*32;
    miner.vx = 0;
    miner.vy = 0;

    return true;
}