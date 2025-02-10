
void render_ui (bool active) {
    if (active) {

        int color; if (world[mouse.worldx][mouse.worldy] != NULL && distance2d(miner.x/64, miner.y/64, mouse.worldx, mouse.worldy) <= 3 ) { color = 1; } else { color = 3; }
        draw_rect(renderer, floatarr(4, mouse.worldx*64 - camera.x - 32, mouse.worldy*64 - camera.y - 32, 64.0f, 64.0f), color, 255, false);
        SDL_RenderLine(renderer, miner.x - camera.x - miner.width/2 + miner.width/2, miner.y - camera.y - miner.height/2 + miner.height/2, mouse.x, mouse.y); // player sight line
        SDL_RenderLine(renderer, mouse.x+5, mouse.y+5, mouse.x-5, mouse.y-5); SDL_RenderLine(renderer, mouse.x+5, mouse.y-5, mouse.x-5, mouse.y+5); // plyer sights X

        if (!set.hide_hud) {
            text_rect(renderer, floatarr(4, 0.0f, 0.0f, (float)(win.sh2+win.sw2)/2*0.25, (float)(win.sh2+win.sw2)/2*0.25), floatarr(4, (2+ blockheld % (NUM_BLOCKS-2))*64.0f, 0.0f, 64.0f, 64.0f), block_textures64, true); // block picked 64px
	        // text_rect(renderer, floatarr(4, 0.0f, 0.0f, 50.0f/2, 50.0f/2, floatarr(4, blockheld*16.0f, 0.0f, 16.0f, 16.0f), block_textures, true); // block picked 16px
        }
    
    }
}