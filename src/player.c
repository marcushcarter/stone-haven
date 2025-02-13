
#include "inventory.c"

void resolve_player_collision(CollisionType type, IntersectionResult result, float x, float y) {

	/*	Explanation time:
	       -basically just gets the coordinates of the collision point
			and moves the player to that point if any of their collision 
			lines are intersecting with a specific blocks collision lines
	*/

	if (result.isIntersecting) {

		if (type == COLLISION_BOTTOM) {
			miner.y = result.cy - miner.height/2;
			miner.vy = 0;
			miner.falling=0;

		}

		if (type == COLLISION_TOP) {
			miner.y = result.cy + miner.height/2;
			miner.vy = 0;
		}

		if (type == COLLISION_RIGHT) {
			miner.x = result.cx - miner.width/2;
			miner.vx = 0;
			if (key.d && miner.vy > 0) miner.vy*= 0.993;
			if (key.w && miner.jumptimer <= 0 && key.a) {
				miner.vx = miner.speed*-1; 
				miner.vy = miner.speed*-1.15;
				miner.jumptimer = 0.15;
				miner.x-=1;
			}
		}

		if (type == COLLISION_LEFT) {
			miner.x = result.cx + miner.width/2;
			miner.vx = 0;
			if (key.a && miner.vy > 0) miner.vy*= 0.993;
			if (key.w && miner.jumptimer <= 0 && key.d) {
				miner.vx = miner.speed; 
				miner.vy = miner.speed*-1.15;
				miner.jumptimer = 0.15;
				miner.x+=1;
			}
		}
	}
}

void check_player_block(int x, int y) {

	/* The players hitbox consists of:
		- two lines pointing up (one on the left and one on the right) to detect if the player is touching any cieling blocks
		- two lines pointing down (left and right) to check if the player is touching any floor blocks
		- two lines pointing left (upper and lower) to check if the player is touching any left walls
		- two lines pointing right (upper and lower) to check if the player is touching any right walls
	*/

	if (world[x][y]->solid == false || world[x][y] == NULL) return;
	if (x < 0 || y < 0 || x >= WORLD_WIDTH || y >= WORLD_HEIGHT) return;

	IntersectionResult result;
	
	Line blkt = {x*64 - 32, y*64 - 32, x*64 +32, y*64 -32};
	Line blkb = {x*64 - 32, y*64 + 32, x*64 +32, y*64 +32};
	Line blkr = {x*64 + 32, y*64 + 32, x*64 + 32, y*64 - 32};
	Line blkl = {x*64 - 32, y*64 + 32, x*64 -32, y*64 -32};

	// bottom left player to top block / bottom right player to top blovk
	Line pbl = {miner.x+miner.width/2-1, miner.y+miner.height/2, miner.x+miner.width/2-1, miner.y};
	result = islinesintersecting(pbl, blkt);
	if (result.isIntersecting && miner.vy > 1000.0f ) { for (int i = 0; i < round(miner.vy/500); i++ ) { create_particle(P_GRAVITY, miner.x, miner.y, randfloat(-200, 200), randfloat(-100, -300), 1, COLOR_WHITE); } }
	if (result.isIntersecting) resolve_player_collision(COLLISION_BOTTOM, result, x, y);
	Line pbr = {miner.x-miner.width/2+1, miner.y+miner.height/2, miner.x-miner.width/2+1, miner.y};
	result = islinesintersecting(pbr, blkt);
	if (result.isIntersecting && miner.vy > 1000.0f ) { for (int i = 0; i < round(miner.vy/500); i++ ) { create_particle(P_GRAVITY, miner.x, miner.y, randfloat(-200, 200), randfloat(-100, -300), 1, COLOR_WHITE); } }
	if (result.isIntersecting) resolve_player_collision(COLLISION_BOTTOM, result, x, y);

	// top left player to bottom block / top right player to bottom block
	Line ptl = {miner.x+miner.width/2-1, miner.y-miner.height/2, miner.x+miner.width/2-1, miner.y};
	result = islinesintersecting(ptl, blkb);
	if (result.isIntersecting) resolve_player_collision(COLLISION_TOP, result, x, y);
	Line ptr = {miner.x-miner.width/2+1, miner.y-miner.height/2, miner.x-miner.width/2+1, miner.y};
	result = islinesintersecting(ptr, blkb);
	if (result.isIntersecting) resolve_player_collision(COLLISION_TOP, result, x, y);

	// left top player to right block / left bottom player to right block
	Line plt = {miner.x-miner.width/2, miner.y+miner.height/2-1, miner.x, miner.y+miner.height/2-1};
	result = islinesintersecting(plt, blkr);
	if (result.isIntersecting) resolve_player_collision(COLLISION_LEFT, result, x, y);
	Line plb = {miner.x-miner.width/2, miner.y-miner.height/2+1, miner.x, miner.y-miner.height/2+1};
	result = islinesintersecting(plb, blkr);
	if (result.isIntersecting) resolve_player_collision(COLLISION_LEFT, result, x, y);
	if (result.isIntersecting && randint(0, 10000) < miner.vy-315 && key.a && abs(miner.vy) > 300 ) create_particle(P_GRAVITY, result.cx+1, miner.y, randfloat(-100, 0), randfloat(-100, 100), 1, COLOR_WHITE);

	// right top player to left block / right bottom player to left block
	Line prt = {miner.x+miner.width/2, miner.y+miner.height/2-1, miner.x, miner.y+miner.height/2-1};
	result = islinesintersecting(prt, blkl);
	if (result.isIntersecting) resolve_player_collision(COLLISION_RIGHT, result, x, y);
	Line prb = {miner.x+miner.width/2, miner.y-miner.height/2+1, miner.x, miner.y-miner.height/2+1};
	result = islinesintersecting(prb, blkl);
	if (result.isIntersecting) resolve_player_collision(COLLISION_RIGHT, result, x, y);
	if (result.isIntersecting && randint(0, 10000) < miner.vy-315 && key.d && abs(miner.vy) > 300 ) create_particle(P_GRAVITY, result.cx-1, miner.y, randfloat(-100, 0), randfloat(-100, 100), 1, COLOR_WHITE);

}

void check_player_collision() {

	/* 	Only checks for collision with blocks that are: 
		- in the same tile as the player
		- above and below the player
		- to the left and right of rthe player
		- in all 4 diagonals to the player
	*/

	// block inside the player
	check_player_block((int)(miner.x/64), (int)(miner.y/64));

	// blocks above, below, and next to the player
	check_player_block((int)(miner.x/64), (int)(miner.y/64)-1);
	check_player_block((int)(miner.x/64), (int)(miner.y/64)+1);
	check_player_block((int)(miner.x/64)-1, (int)(miner.y/64));
	check_player_block((int)(miner.x/64)+1, (int)(miner.y/64));

 	// blocks diagonal to the player
	check_player_block((int)(miner.x/64)-1, (int)(miner.y/64)-1);
	check_player_block((int)(miner.x/64)+1, (int)(miner.y/64)-1);
	check_player_block((int)(miner.x/64)+1, (int)(miner.y/64)+1);
	check_player_block((int)(miner.x/64)-1, (int)(miner.y/64)+1);
}

void break_block(int worldx, int worldy) {
	if (world[worldx][worldy]->breakable ) {
		add_to_inventory(world[worldx][worldy]);
		world[worldx][worldy] = block[BLOCK_AIR];
		for (int i = 0; i < randint(2, 5); i++) create_particle(P_GRAVITY, worldx*64+randfloat(-32, 32), worldy*64+randfloat(-32, 32), randfloat(-100, 100), randfloat(-100, 100), 1.0f, COLOR_WHITE);
		miner.breaktimer = 0;
	}
}

void place_block(int worldx, int worldy) {
	if ( inventory[miner.inventory_slot].block != NULL && (!(round(miner.x/64) == mouse.worldx && round(miner.y/64) == mouse.worldy) || !inventory[miner.inventory_slot].block->solid)) {
		world[worldx][worldy] = inventory[miner.inventory_slot].block;
		remove_from_inventory(inventory[miner.inventory_slot].block);
	}
}

void update_player(bool active) {
    if (active) {

		if (set.gamemode == GM_FREECAM) {
			if (key.d) camera.targetx+=1000*dt;
			if (key.a) camera.targetx-=1000*dt;
			if (key.w) camera.targety-=1000*dt;
			if (key.s) camera.targety+=1000*dt;
			return;
		} else {
			camera.targetx = miner.x;
			camera.targety = miner.y;
		}

		miner.falling+=1*dt;
		miner.jumptimer-=1*dt;

		if (set.gamemode == GM_SURVIVAL) {
			if (key.a) miner.vx -= 2000 * dt;
			if ( miner.vx < -1*miner.speed ) miner.vx = -1*miner.speed;
			if (key.d) miner.vx += 2000 * dt;
			if ( miner.vx > miner.speed ) miner.vx = miner.speed;
			if (key.w && miner.falling <= 0.1 && miner.jumptimer <= 0) { miner.vy = miner.speed*-1.35; miner.jumptimer=0.1; }

			if (key.w && miner.vy < 0) { miner.vy += set.gravity*dt; } else { miner.vy += set.gravity*2*dt; }
			if ((key.a && key.d) || (!key.a && !key.d)) miner.vx *= 0.9;
			miner.vx*=0.995;
		}

		if (set.gamemode == GM_CREATIVE) {
			if (key.w) { miner.vy = miner.speed*-1.25;}
			if (key.s) { miner.vy = miner.speed*1.25;}
			if (key.a) { miner.vx = miner.speed*-1.25;}
			if (key.d) { miner.vx = miner.speed*1.25;}

			miner.vx*=0.9;
			miner.vy*=0.9;
		}

		if (miner.vy > 5000) miner.vy += (5000-miner.vy)/2;
		int iterations = 15;
		for (int i = 0; i < iterations; i++) {
			// if (set.gamemode != GM_FREECAM) {
				miner.x += miner.vx*dt/iterations; 
				miner.y += miner.vy*dt/iterations;
			// }
			check_player_collision();
		}

		// respawn if player falls out of the world perimiters
		if ( miner.y/64 > WORLD_HEIGHT + 15) {
			miner.x = WORLD_WIDTH*32;
			miner.y = WORLD_HEIGHT*32;
			miner.vx = 0;
			miner.vy = 0;
		}

		// inventory slot management
		if (key.upa) miner.inventory_slot+=1;
		if (key.downa) miner.inventory_slot-=1;
		if (miner.inventory_slot > MAX_INVENTORY_SIZE-1) miner.inventory_slot = MAX_INVENTORY_SIZE-1;
		if (miner.inventory_slot < 0) miner.inventory_slot = 0;


		if (mouse.r && world[mouse.worldx][mouse.worldy] != NULL && distance2d(miner.x/64, miner.y/64, mouse.worldx, mouse.worldy) <= 4 && world[mouse.worldx][mouse.worldy] == block[BLOCK_AIR]
		&& (world[mouse.worldx][mouse.worldy-1]->solid || world[mouse.worldx][mouse.worldy+1]->solid || world[mouse.worldx-1][mouse.worldy]->solid || world[mouse.worldx+1][mouse.worldy]->solid) 
		) {
			place_block(mouse.worldx, mouse.worldy);
		}

		if (mouse.l && world[mouse.worldx][mouse.worldy] != block[BLOCK_AIR]) { miner.breaktimer+=1*dt; } else { miner.breaktimer = 0; }
		if (miner.breaktimer > set.break_speed) miner.breaktimer = set.break_speed;

		if (!(mouse.worldx < 1 || mouse.worldx > WORLD_WIDTH-2 || mouse.worldy < 1 || mouse.worldy > WORLD_HEIGHT-2)) {
			if (set.gamemode == GM_CREATIVE && world[mouse.worldx][mouse.worldy] != block[BLOCK_AIR]) miner.breaktimer=999;
			if (miner.breaktimer >= set.break_speed && mouse.l && world[mouse.worldx][mouse.worldy] != NULL && distance2d(miner.x/64, miner.y/64, mouse.worldx, mouse.worldy) <= 4
			&& (!(world[mouse.worldx][mouse.worldy-1]->solid && world[mouse.worldx][mouse.worldy+1]->solid && world[mouse.worldx-1][mouse.worldy]->solid && world[mouse.worldx+1][mouse.worldy]->solid ) || (mouse.worldx == round(miner.x/64) && mouse.worldy == round(miner.y/64))) ) {
				break_block(mouse.worldx, mouse.worldy);
			}
		}

    }
}

void render_player(bool active) {
    if (active) {
		
		float rw = miner.width;
		float rh = miner.height;
		if (miner.vy > 250) {
			rw = miner.width-abs((miner.vy-250)/150);
			rh = miner.height+abs((miner.vy-250)/150);
		}
		if (rw < 25) rw = 25;
		if (rh > 75) rh = 75;

		draw_rect(renderer, floatarr(4, miner.x - camera.x - rw/2, miner.y - camera.y - rh/2, rw, rh), COLOR_WHITE, 255, true);
    }
}
