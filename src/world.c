
// up to 4,000,000 blocks with it being playably laggy (or x)
// up to 3,000,000 blocks with it being slightly laggy (or x)
// up to 2,500,000 blocks with it being not laggy at all (or 2000x1250)

// high fps 2,000,000 blocks
// medium fps 2,500,000 blocks
// slightly laggy 3,000,000 blocks
// playably laggy 4,000,000 blocks
//

#define NUM_BLOCKS 33

typedef enum {
    B_AIR,
    B_WORLDBORDER,

    B_STONE,
    B_SILVERORE,
    B_EMERALDORE,
    B_SAPHIREORE,
    B_DECORATIVESTONE,

    B_GLOWSTONE,
    B_DAIMONDORE,
    B_DECORATIVEGLOWSTONE,
    
    B_BLACKSTONE,
    B_GOLDORE,
    B_RUBYORE,
    B_DECORATIVEBLACKSTONE,
    B_DECORATIVEGOLDBLACKSTONE,
    
    B_LIMESTONE,
    B_MARBLE,

    B_DECORATIVEEMERALD,
    B_DECORATIVESAPHIRE,
    B_DECORATIVEDAIMOND,
    B_DECORATIVEGOLD,
    B_DECORATIVERUBY,

    B_DIRT,
    B_GRASS,

    B_WORKBENCH,
    B_BOOKSHELF,
    B_CABINET,
    B_BOX,
    B_DISPLAYCASE,
    B_ICE,
    B_GLOWMUSHROOM,
    B_CHEST,
    B_FLOWER,

} BlockType;

typedef struct {
    BlockType type;       // Type of block (e.g., STONE, GRASS)
    bool solid;           // Is the block solid (whether you can walk through it)
    bool breakable;       // Can the block be broken by the player?
    // int lightlevel;    // Light level emitted by the block (0-15)
    float brightness;     // brightness of the block based on distance from the camera
    int hardness;         // Hardness of the block (how difficult it is to mine)
    float health;         // Health of the block (used for things like crops or breakable blocks)
    // int metadata;      // Metadata or variant (e.g., color of wool, stage of growth)

    // Item* item;

} Block;

Block* make_block(BlockType type, bool solid, bool breakable, int hardness) {
    Block* new_block = (Block*)malloc(sizeof(Block));
    if (new_block == NULL) {
        printf("Memory allocation failed for make_block");
        return NULL;
    }

    new_block->type = type;
    new_block->solid = solid;
    new_block->breakable = breakable;
    new_block->hardness = hardness;
    new_block->health = hardness;

    // new_block->item = item;

    return new_block;
}

Block* block[1000];
SDL_Texture* block_texture[1000];
Block* world[WORLD_WIDTH][WORLD_HEIGHT];

bool make_blocks() {

    block[B_AIR] = make_block(B_AIR, false, false, 0);
    block[B_WORLDBORDER] = make_block(B_WORLDBORDER, true, false, 9999);
    
    block[B_STONE] = make_block(B_STONE, true, true, 3);
    block[B_SILVERORE] = make_block(B_SILVERORE, true, true, 4);
    block[B_EMERALDORE] = make_block(B_EMERALDORE, true, true, 4);
    block[B_SAPHIREORE] = make_block(B_SAPHIREORE, true, true, 4);
    block[B_DECORATIVESTONE] = make_block(B_DECORATIVESTONE, true, true, 4);

    block[B_GLOWSTONE] = make_block(B_GLOWSTONE, true, true, 3);
    block[B_DAIMONDORE] = make_block(B_DAIMONDORE, true, true, 4);
    block[B_DECORATIVEGLOWSTONE] = make_block(B_DECORATIVEGLOWSTONE, true, true, 3);

    block[B_BLACKSTONE] = make_block(B_BLACKSTONE, true, true, 3);
    block[B_GOLDORE] = make_block(B_GOLDORE, true, true, 4);
    block[B_RUBYORE] = make_block(B_RUBYORE, true, true, 4);
    block[B_DECORATIVEBLACKSTONE] = make_block(B_DECORATIVEBLACKSTONE, true, true, 3);
    block[B_DECORATIVEGOLDBLACKSTONE] = make_block(B_DECORATIVEGOLDBLACKSTONE, true, true, 4);
    
    block[B_LIMESTONE] = make_block(B_LIMESTONE, true, true, 3);
    block[B_MARBLE] = make_block(B_MARBLE, true, true, 3);

    block[B_DECORATIVEEMERALD] = make_block(B_DECORATIVEEMERALD, true, true, 5);
    block[B_DECORATIVESAPHIRE] = make_block(B_DECORATIVESAPHIRE, true, true, 5);
    block[B_DECORATIVEDAIMOND] = make_block(B_DECORATIVEDAIMOND, true, true, 5);
    block[B_DECORATIVEGOLD] = make_block(B_DECORATIVEGOLD, true, true, 4);
    block[B_DECORATIVERUBY] = make_block(B_DECORATIVERUBY, true, true, 5);

    block[B_DIRT] = make_block(B_DIRT, true, true, 2);
    block[B_GRASS] = make_block(B_GRASS, false, true, 1);

    block[B_WORKBENCH] = make_block(B_WORKBENCH, false, true, 2);
    block[B_BOOKSHELF] = make_block(B_BOOKSHELF, false, true, 2);
    block[B_CABINET] = make_block(B_CABINET, false, true, 2);
    block[B_BOX] = make_block(B_BOX, true, true, 2);
    block[B_DISPLAYCASE] = make_block(B_DISPLAYCASE, false, true, 2);
    block[B_ICE] = make_block(B_ICE, true, true, 1);
    block[B_GLOWMUSHROOM] = make_block(B_GLOWMUSHROOM, false, true, 1);
    block[B_CHEST] = make_block(B_CHEST, false, true, 2);
    block[B_FLOWER] = make_block(B_FLOWER, false, true, 1);
    
    return true;
}

void render_world(bool active) {
    if (active) {
        for (int x = 0; x < WORLD_WIDTH; x++) {
            for (int y = 0; y < WORLD_HEIGHT; y++) {
                if (world[x][y] == NULL) continue;
                if ((y-1)*64 - camera.y > win.sh || (y+1)*64 - camera.y < 0 || (x+1)*64 - camera.x < 0 || (x-1)*64 - camera.x > win.sw ) continue;

                float brightness;
                // shadow / brightness
                float dx = (x * 64) - miner.x; float dy = (y * 64) - miner.y;
                brightness = 220.0f - ((sqrt(dx * dx + dy * dy) / 64) / set.brightness) * 220.0f;
                if ((sqrt(dx * dx + dy * dy) / 64) > set.brightness) brightness = 0;
                if (brightness < 0) { brightness = 0; } else if (brightness > 220.0f) { brightness = 220.0f; }
                // blocks behind blocks
                if (world[x][y+1]->solid == false || world[x-1][y]->solid == false || world[x][y-1]->solid == false || world[x+1][y]->solid == false ) { world[x][y]->brightness = 220.0f-abs(brightness); } else { world[x][y]->brightness = 255; }

                if (world[x][y] == block[B_AIR] || world[x][y]->brightness == 255 || (!world[x][y]->solid && world[x][y]->brightness == 220.0f)) continue;
                // if (world[x][y]->brightness == 255) { continue; }
                
                text_rect(renderer, floatarr(4, x*64 - camera.x - 32, y*64 - camera.y - 32, 64.0f, 64.0f), floatarr(4, (float)(world[x][y]->type)*64.0f, 0.0f, 64.0f, 64.0f), block_textures64, true); // render block 64px
                // text_rect(renderer, floatarr(4, x*64 - camera.x - 32, y*64 - camera.y - 32, 64.0f, 64.0f), floatarr(4, (float)(world[x][y]->type)*16.0f, 0.0f, 16.0f, 16.0f), block_textures, true); // render block 16px
                
                if (world[x][y]->solid) { // block outlines
                    if (!world[x][y-1]->solid) draw_rect(renderer, floatarr(4, x*64 - camera.x - 32, y*64 - camera.y - 32, 64.0f, 3.0f), COLOR_WHITE, 255, true);
                    if (!world[x][y+1]->solid) draw_rect(renderer, floatarr(4, x*64 - camera.x - 32, y*64 - camera.y + 32, 64.0f, -3.0f), COLOR_WHITE, 255, true);
                    if (!world[x-1][y]->solid) draw_rect(renderer, floatarr(4, x*64 - camera.x - 32, y*64 - camera.y - 32, 3.0f, 64.0f), COLOR_WHITE, 255, true);
                    if (!world[x+1][y]->solid) draw_rect(renderer, floatarr(4, x*64 - camera.x + 32, y*64 - camera.y - 32, -3.0f, 64.0f), COLOR_WHITE, 255, true);
                }
                
                draw_rect(renderer, floatarr(4, x*64 - camera.x - 32, y*64 - camera.y - 32, 64.0f, 64.0f), COLOR_BLACK, world[x][y]->brightness, true); // block brightness overlay
            }
	    }
    }
}

void update_blocks(bool active) {
	if (active) {
		for (int x = 0; x < WORLD_WIDTH; x++) {
			for (int y = 0; y < WORLD_HEIGHT; y++) {
				if (world[x][y] == NULL) continue;

				// if (world[x][y] == block[B_GRASS] && world[x][y+1] != block[B_DIRT]) {
				// 	world[x][y] = block[B_AIR];
				// }

				// if ((world[x][y] == block[B_GLOWCRYSTAL] || world[x][y] == block[B_GLOWMUSHROOMCLUSTER1] || world[x][y] == block[B_GLOWMUSHROOMCLUSTER2] || world[x][y] == block[B_GLOWMUSHROOMSTEM] || world[x][y] == block[B_STALAG]) && world[x][y+1]->solid == false) {
				// 	world[x][y] = block[B_AIR];
				// }

				// if (world[x][y] == block[B_GLOWMUSHROOMBUD] && world[x][y+1] != block[B_GLOWMUSHROOMSTEM]) {
				// 	world[x][y] = block[B_AIR];
				// }

				// if (world[x][y] == block[B_MOSSBLOCK] && world[x][y-1]->solid) {
				// 	world[x][y] = block[B_AIR];
				// }
			}
		}
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