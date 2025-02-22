
bool save_world(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Failed to open file for writing.\n");
        return false;
    }

    // Save player data
    fprintf(file, "%s\n", miner.name);  // Save player's name
    fprintf(file, "%f %f %f %f %f %f %f %f %f %f %f\n",
            miner.x, miner.y, miner.vx, miner.vy, miner.speed,
            miner.health, miner.width, miner.height,
            miner.falling, miner.jumptimer, miner.breaktimer);

    // Save world data (blocks)
    for (int y = 0; y < WORLD_HEIGHT; y++) {
        for (int x = 0; x < WORLD_WIDTH; x++) {
            if (world[x][y] != NULL) {
                fprintf(file, "%d ", world[x][y]->type);  // Write block type
            } else {
                fprintf(file, "%d ", BLOCK_AIR);  // Write air (empty space) if no block
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
    fscanf(file, "%f %f %f %f %f %f %f %f %f %f %f\n",
            &miner.x, &miner.y, &miner.vx, &miner.vy, &miner.speed,
            &miner.health, &miner.width, &miner.height,
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
                world[x][y] = block[BLOCK_AIR];
            }
        }
    }

    fclose(file);
    printf("World and player loaded successfully from %s.\n", filename);
    return true;
}

bool save_statistics(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return false;
    }

    fwrite(&statistics, sizeof(Statistics), 1, file);
    
    fwrite(&set, sizeof(GameSettings), 1, file);

    fwrite(&achievements, sizeof(GameSettings), 1, file);

    fclose(file);
    printf("Data saved successfully!\n");
    return true;
}

bool load_statistics(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return false;
    }

    fread(&statistics, sizeof(Statistics), 1, file);
    
    fread(&set, sizeof(GameSettings), 1, file);
    
    fread(&achievements, sizeof(GameSettings), 1, file);

    fclose(file);
    printf("Data loaded successfully!\n");
    return true;
}

