
typedef enum {
    BLOCK_AIR,
    BLOCK_WORLD_BORDER,

    BLOCK_STONE,
    BLOCK_BLACKSTONE,
    BLOCK_GRASS,
    BLOCK_BOOKSHELF,

    BLOCK_COUNT,
} BlockType;

const char* BlockNames[] = {
    "AIR",
    "WORLD BORDER",
    "STONE",
    "BLACKSTONE",
    "GRASS",
    "BOOKSHELF",
};

typedef struct Block {
    BlockType type;       // Type of block (e.g., STONE, GRASS)
    bool solid;           // Is the block solid (whether you can walk through it)
    bool breakable;       // Can the block be broken by the player?
    float brightness;     // brightness of the block based on distance from the camera
} Block;

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
    block[BLOCK_BLACKSTONE] = make_block(BLOCK_BLACKSTONE, true, true);
    block[BLOCK_GRASS] = make_block(BLOCK_GRASS, false, true);
    block[BLOCK_BOOKSHELF] = make_block(BLOCK_BOOKSHELF, false, true);
    
    return true;
}

#include "worldgen.c"

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

                if (world[x][y] == block[BLOCK_AIR] || world[x][y]->brightness == 255 || (!world[x][y]->solid && world[x][y]->brightness == 220.0f)) continue;
                if (world[x][y]->brightness == 255) { continue; }
                
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
	if (active && set.gamemode != GM_FREECAM) {
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