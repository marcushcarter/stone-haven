
// void create_particle(ParticleType type, float x, float y, float vx, float vy, float life, int color);

typedef enum {
    BLOCK_AIR,
    BLOCK_WORLD_BORDER,

    BLOCK_STONE,
    BLOCK_STONE_BRICKS,
    BLOCK_DECORATIVE_STONE,
    BLOCK_SMOOTH_STONE,
    BLOCK_DARK_STONE,
    BLOCK_DARK_STONE_BRICKS,
    BLOCK_SMOOTH_DARK_STONE,
    BLOCK_BLACK_STONE,
    BLOCK_BLACK_STONE_BRICKS,
    BLOCK_GILDED_BLACK_STONE_BRICKS,
    BLOCK_DIRT,
    BLOCK_SILVER_ORE,
    BLOCK_EMERALD_ORE,
    BLOCK_DIAMOND_ORE,
    BLOCK_GOLD_ORE,
    BLOCK_BOUNCE_PAD,
    BLOCK_SNOW,
    BLOCK_ICE,
    BLOCK_BONE,
    BLOCK_EMERALD_BLOCK,
    BLOCK_SAPPHIRE_BLOCK,
    BLOCK_DIAMOND_BLOCK,
    BLOCK_GOLD_BLOCK,
    BLOCK_RUBY_BLOCK,
    BLOCK_DISPLAY_CASE,
    BLOCK_BOOKSHELF,
    BLOCK_LADDER,
    BLOCK_SHELF,
    BLOCK_WOODEN_CRATE,
    BLOCK_WOOD,
    BLOCK_CHEST,
    BLOCK_SCAFOLDING,
    BLOCK_LANTERN,
    BLOCK_CHAIN,
    BLOCK_PYLON,
    BLOCK_MUSHROOM,
    BLOCK_IRIS_FLOWER,
    BLOCK_ROSE,
    BLOCK_GRASS,
    BLOCK_SKULL,
    BLOCK_BRICKS,
    BLOCK_BLUE_BRICKS,
    BLOCK_VASE,

    BLOCK_COUNT,
    NO_BLOCK = -1,
} BlockType;

const char* BlockNames[] = {
    "AIR",
    "BORDER BLOCK",
    "STONE",
    "STONE BRICKS",
    "DECORATIVE STONE",
    "SMOOTH STONE",
    "DARK STONE",
    "DARK STONE BRICKS",
    "SMOOTH DARK STONE",
    "BLACK STONE",
    "BLACK STONE BRICKS",
    "GILDED BLACK STONE BRICKS",
    "DIRT",
    "SILVER ORE",
    "EMERALD ORE",
    "DIAMOND ORE",
    "GOLD ORE",
    "BOUNCE PAD",
    "SNOW",
    "ICE",
    "BONE",
    "EMERALD BLOCK",
    "SAPPHIRE BLOCK",
    "DIAMOND BLOCK",
    "GOLD BLOCK",
    "RUBY BLOCK",
    "DISPLAY CASE",
    "BOOKSHELF",
    "LADDER",
    "SHELF",
    "WOODEN CRATE",
    "WOOD",
    "CHEST",
    "SCAFFOLDING",
    "LANTERN",
    "CHAIN",
    "PYLON",
    "MUSHROOM",
    "IRIS FLOWER",
    "ROSE",
    "GRASS",
    "SKULL",
    "BRICKS",
    "BLUE BRICKS",
    "VASE",
};

typedef struct Block {
    BlockType type;       // Type of block (e.g., STONE, GRASS)
    bool solid;           // Is the block solid (whether you can walk through it)
    bool breakable;       // Can the block be broken by the player?
    float brightness;     // brightness of the block based on distance from the camera
} Block;

// void break_block(int worldx, int worldy);
void add_to_inventory(Block* block);
// void remove_from_inventory(Block* block);
void clear_inventory();

Block* make_block(BlockType type, bool solid, bool breakable) {
    Block* new_block = (Block*)malloc(sizeof(Block));
    if (!new_block) {
        printf("Failed to allocate memory for block\n");
        return NULL;
    }

    new_block->type = type;
    new_block->solid = solid;
    new_block->breakable = breakable;

    return new_block;
}

Block* block[1000];
SDL_Texture* block_texture[1000];
Block* world[WORLD_WIDTH][WORLD_HEIGHT];

bool make_blocks() {

    block[BLOCK_AIR] = make_block(BLOCK_AIR, false, false);
    block[BLOCK_WORLD_BORDER] = make_block(BLOCK_WORLD_BORDER, true, false);
    
    block[BLOCK_STONE] = make_block(BLOCK_STONE, true, true);
    block[BLOCK_STONE_BRICKS] = make_block(BLOCK_STONE_BRICKS, true, true);
    block[BLOCK_DECORATIVE_STONE] = make_block(BLOCK_DECORATIVE_STONE, true, true);
    block[BLOCK_SMOOTH_STONE] = make_block(BLOCK_SMOOTH_STONE, true, true);

    block[BLOCK_DARK_STONE] = make_block(BLOCK_DARK_STONE, true, true);
    block[BLOCK_DARK_STONE_BRICKS] = make_block(BLOCK_DARK_STONE_BRICKS, true, true);
    block[BLOCK_SMOOTH_DARK_STONE] = make_block(BLOCK_SMOOTH_DARK_STONE, true, true);

    block[BLOCK_BLACK_STONE] = make_block(BLOCK_BLACK_STONE, true, true);
    block[BLOCK_BLACK_STONE_BRICKS] = make_block(BLOCK_BLACK_STONE_BRICKS, true, true);
    block[BLOCK_GILDED_BLACK_STONE_BRICKS] = make_block(BLOCK_GILDED_BLACK_STONE_BRICKS, true, true);

    block[BLOCK_DIRT] = make_block(BLOCK_DIRT, true, true);
    block[BLOCK_SILVER_ORE] = make_block(BLOCK_SILVER_ORE, true, true);
    block[BLOCK_EMERALD_ORE] = make_block(BLOCK_EMERALD_ORE, true, true);
    block[BLOCK_DIAMOND_ORE] = make_block(BLOCK_DIAMOND_ORE, true, true);
    block[BLOCK_GOLD_ORE] = make_block(BLOCK_GOLD_ORE, true, true);

    block[BLOCK_BOUNCE_PAD] = make_block(BLOCK_BOUNCE_PAD, true, true);
    block[BLOCK_SNOW] = make_block(BLOCK_SNOW, true, true);
    block[BLOCK_ICE] = make_block(BLOCK_ICE, true, true);
    block[BLOCK_BONE] = make_block(BLOCK_BONE, true, true);
    block[BLOCK_BRICKS] = make_block(BLOCK_BRICKS, true, true);
    block[BLOCK_BLUE_BRICKS] = make_block(BLOCK_BLUE_BRICKS, true, true);

    block[BLOCK_EMERALD_BLOCK] = make_block(BLOCK_EMERALD_BLOCK, true, true);
    block[BLOCK_SAPPHIRE_BLOCK] = make_block(BLOCK_SAPPHIRE_BLOCK, true, true);
    block[BLOCK_DIAMOND_BLOCK] = make_block(BLOCK_DIAMOND_BLOCK, true, true);
    block[BLOCK_GOLD_BLOCK] = make_block(BLOCK_GOLD_BLOCK, true, true);
    block[BLOCK_RUBY_BLOCK] = make_block(BLOCK_RUBY_BLOCK, true, true);

    block[BLOCK_DISPLAY_CASE] = make_block(BLOCK_DISPLAY_CASE, false, true);
    block[BLOCK_BOOKSHELF] = make_block(BLOCK_BOOKSHELF, false, true);
    block[BLOCK_LADDER] = make_block(BLOCK_LADDER, false, true);
    block[BLOCK_SHELF] = make_block(BLOCK_SHELF, false, true);
    block[BLOCK_WOODEN_CRATE] = make_block(BLOCK_WOODEN_CRATE, true, true);
    block[BLOCK_WOOD] = make_block(BLOCK_WOOD, true, true);
    block[BLOCK_CHEST] = make_block(BLOCK_CHEST, false, true);
    block[BLOCK_SCAFOLDING] = make_block(BLOCK_SCAFOLDING, false, true);
    block[BLOCK_LANTERN] = make_block(BLOCK_LANTERN, false, true);
    block[BLOCK_CHAIN] = make_block(BLOCK_CHAIN, false, true);
    block[BLOCK_PYLON] = make_block(BLOCK_PYLON, false, true);
    block[BLOCK_VASE] = make_block(BLOCK_VASE, false, true);
    
    block[BLOCK_COUNT] = make_block(BLOCK_COUNT, false, true);

    block[BLOCK_MUSHROOM] = make_block(BLOCK_MUSHROOM, false, true);
    block[BLOCK_IRIS_FLOWER] = make_block(BLOCK_IRIS_FLOWER, false, true);
    block[BLOCK_ROSE] = make_block(BLOCK_ROSE, false, true);
    block[BLOCK_GRASS] = make_block(BLOCK_GRASS, false, true);
    block[BLOCK_SKULL] = make_block(BLOCK_SKULL, false, true);
    
    return true;
}

void render_world(bool active) {
    if (active) {
        for (int x = 0; x < WORLD_WIDTH; x++) {
            for (int y = 0; y < WORLD_HEIGHT; y++) {
                if ((x < 1) || (x > WORLD_WIDTH-2) || (y < 1) || (y > WORLD_HEIGHT-2)) continue;
                if (world[x][y] == NULL) continue;
                if ((y-1)*64 - camera.y > win.sh || (y+1)*64 - camera.y < 0 || (x+1)*64 - camera.x < 0 || (x-1)*64 - camera.x > win.sw ) continue;
                // if ((y+2)*64 - camera.y > win.sh || (y-2)*64 - camera.y < 0 || (x-2)*64 - camera.x < 0 || (x+2)*64 - camera.x > win.sw ) continue;

                float brightness;
                // shadow / brightness
                float dx = (x * 64) - camera.x - win.sw2; float dy = (y * 64) - camera.y - win.sh2;
                // float dx = (x * 64) - miner.x; float dy = (y * 64) - miner.y;
                brightness = 220.0f - ((sqrt(dx * dx + dy * dy) / 64) / set.brightness) * 220.0f;
                if ((sqrt(dx * dx + dy * dy) / 64) > set.brightness) brightness = 0;
                if (brightness < 0) { brightness = 0; } else if (brightness > 220.0f) { brightness = 220.0f; }
                // blocks behind blocks
                if (world[x][y+1]->solid == false || world[x-1][y]->solid == false || world[x][y-1]->solid == false || world[x+1][y]->solid == false ) { world[x][y]->brightness = 220.0f-abs(brightness); } else { world[x][y]->brightness = 255; }

                if (world[x][y] == block[BLOCK_AIR] || world[x][y]->brightness == 255 || (!world[x][y]->solid && world[x][y]->brightness == 220.0f)) continue;
                if (world[x][y]->brightness == 255) { continue; }
                
                text_rect(renderer, floatarr(4, x*64 - camera.x - 32, y*64 - camera.y - 32, 64.0f, 64.0f), floatarr(4, (float)(world[x][y]->type)*64.0f, 0.0f, 64.0f, 64.0f), block_textures64, true); // render block 64px
                
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
	if (active && set.gamemode != GM_FREECAM) {
	for (int x = 0; x < WORLD_WIDTH; x++) {
	for (int y = 0; y < WORLD_HEIGHT; y++) {
		if (world[x][y] == NULL) continue;
        if (distance2d(camera.x+(win.sw2), camera.y+(win.sh2), x, y) > set.update_distance*64) continue;


    
        if ( // a plant block must have dirt or vase under it
            world[x][y+1] != block[BLOCK_DIRT] && world[x][y+1] != block[BLOCK_VASE] && (
                world[x][y] == block[BLOCK_MUSHROOM] || 
                world[x][y] == block[BLOCK_IRIS_FLOWER] || 
                world[x][y] == block[BLOCK_ROSE] || 
                world[x][y] == block[BLOCK_GRASS]
            )
        ) { add_to_inventory(world[x][y]); world[x][y] = block[BLOCK_AIR]; }



        if ( // must have solid block below and above
            world[x][y-1] == block[BLOCK_AIR] && world[x][y+1] == block[BLOCK_AIR] && (
                world[x][y] == block[BLOCK_CHAIN] ||
                world[x][y] == block[BLOCK_VASE] 
            )
        ) { add_to_inventory(world[x][y]); world[x][y] = block[BLOCK_AIR]; }



        if ( // must have a block below it
            world[x][y+1] == block[BLOCK_AIR] && (
                0==1
            )
        ) { add_to_inventory(world[x][y]); world[x][y] = block[BLOCK_AIR]; }


        
        if ( // must have a block above it
            world[x][y-1] == block[BLOCK_AIR] && (
                world[x][y] == block[BLOCK_LANTERN]
            )
        ) { add_to_inventory(world[x][y]); world[x][y] = block[BLOCK_AIR]; }



        if (set.particles) { // particle to blocks
            if (world[x][y] == block[BLOCK_LANTERN]) { if (randint(0, 250) == 0) create_particle(P_FLOAT, x*64, y*64, randfloat(-20, 20), randfloat(100, 100), 1.0f, COLOR_ORANGE); }
            // if (world[x][y] == block[BLOCK_LANTERN]) { if (randint(0, 100) == 0) create_particle(P_FALL, x*64, y*64, randfloat(-20, 20), randfloat(100, 100), 1.0f, COLOR_WHITE); }
        }



	}
	}
	}
}

#include "worldgen.c"