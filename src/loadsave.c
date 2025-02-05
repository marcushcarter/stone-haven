
bool save_world(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Failed to open file for writing.\n");
        return false;
    }

    // Save player data
    fprintf(file, "%s\n", miner.name);  // Save player's name
    fprintf(file, "%f %f %f %f %f %f %f %f %f %f %f %f\n",
            miner.x, miner.y, miner.vx, miner.vy, miner.speed,
            miner.health, miner.width, miner.height, miner.illumination,
            miner.falling, miner.jumptimer, miner.breaktimer);

    // Save world data (blocks)
    for (int y = 0; y < WORLD_HEIGHT; y++) {
        for (int x = 0; x < WORLD_WIDTH; x++) {
            if (world[x][y] != NULL) {
                fprintf(file, "%d ", world[x][y]->type);  // Write block type
            } else {
                fprintf(file, "%d ", B_AIR);  // Write air (empty space) if no block
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("World and player saved successfully to %s.\n", filename);
    return true;
}


bool load_world(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file for reading.\n");
        return false;
    }

    // Load player data
    fscanf(file, "%s", miner.name);  // Read player's name
    fscanf(file, "%f %f %f %f %f %f %f %f %f %f %f %f\n",
            &miner.x, &miner.y, &miner.vx, &miner.vy, &miner.speed,
            &miner.health, &miner.width, &miner.height, &miner.illumination,
            &miner.falling, &miner.jumptimer, &miner.breaktimer);

    // Load world data (blocks)
    for (int y = 0; y < WORLD_HEIGHT; y++) {
        for (int x = 0; x < WORLD_WIDTH; x++) {
            int blockTypeInt;
            fscanf(file, "%d", &blockTypeInt);

            // Check if the blockTypeInt is valid
            if (blockTypeInt >= 0 && blockTypeInt < sizeof(block) / sizeof(block[0])) {
                world[x][y] = block[blockTypeInt];
            } else {
                // If blockTypeInt is invalid, use a default block (e.g., B_AIR)
                world[x][y] = block[B_AIR];
            }
        }
    }

    fclose(file);
    printf("World and player loaded successfully from %s.\n", filename);
    return true;
}
