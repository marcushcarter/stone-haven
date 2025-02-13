
void render_ui (bool active) {
    if (active) {

        int color; if (world[mouse.worldx][mouse.worldy] != NULL && distance2d(miner.x/64, miner.y/64, mouse.worldx, mouse.worldy) <= 4 ) { color = 1; } else { color = 3; }
        if (miner.breaktimer > 0) draw_rect(renderer, floatarr(4, mouse.worldx*64 - camera.x - 32, mouse.worldy*64 - camera.y - 32, 64.0f, 64.0f), color, miner.breaktimer/set.break_speed*100, true);
        draw_rect(renderer, floatarr(4, mouse.worldx*64 - camera.x - 32, mouse.worldy*64 - camera.y - 32, 64.0f, 64.0f), color, 255, false);

        SDL_RenderLine(renderer, miner.x - camera.x - miner.width/2 + miner.width/2, miner.y - camera.y - miner.height/2 + miner.height/2, mouse.x, mouse.y); // player sight line
        // SDL_RenderLine(renderer, mouse.x+4, mouse.y+4, mouse.x-4, mouse.y-4); SDL_RenderLine(renderer, mouse.x+4, mouse.y-4, mouse.x-4, mouse.y+4); // plyer sights X
        SDL_RenderLine(renderer, mouse.x+5, mouse.y, mouse.x-5, mouse.y); SDL_RenderLine(renderer, mouse.x, mouse.y-5, mouse.x, mouse.y+5); // plyer sights +

        if (!set.hide_hud && inventory[inventory_slot].block != NULL) text_rect(renderer, floatarr(4, 0.0f, 0.0f, (float)(win.sh2+win.sw2)/2*0.25, (float)(win.sh2+win.sw2)/2*0.25), floatarr(4, (inventory[inventory_slot].block->type)*64.0f, 0.0f, 64.0f, 64.0f), block_textures64, true); // block picked 64px
    
    }
}