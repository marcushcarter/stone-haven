
void render_ui (bool active) {
    if (active) {

        int color; if (world[mouse.worldx][mouse.worldy] != NULL && distance2d(miner.x/64, miner.y/64, mouse.worldx, mouse.worldy) <= 4 ) { color = COLOR_WHITE; } else { color = COLOR_GREY; }

        if (miner.breaktimer > 0) draw_rect(renderer, floatarr(4, mouse.worldx*64 - camera.x - 32, mouse.worldy*64 - camera.y - 32, 64.0f, 64.0f), color, miner.breaktimer/set.break_speed*100, true);
        draw_rect(renderer, floatarr(4, mouse.worldx*64 - camera.x - 32, mouse.worldy*64 - camera.y - 32, 64.0f, 64.0f), color, 255, false);
        SDL_RenderLine(renderer, mouse.x+5, mouse.y, mouse.x-5, mouse.y); SDL_RenderLine(renderer, mouse.x, mouse.y-5, mouse.x, mouse.y+5);

        if (!set.hide_hud && inventory[miner.inventory_slot].block != NULL) text_rect(renderer, floatarr(4, 0.0f, 0.0f, (float)(win.sh2+win.sw2)/2*0.25, (float)(win.sh2+win.sw2)/2*0.25), floatarr(4, (inventory[miner.inventory_slot].block->type)*64.0f, 0.0f, 64.0f, 64.0f), block_textures64, true);
    
    }
}