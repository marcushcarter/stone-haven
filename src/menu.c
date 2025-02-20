
void render_ui (bool active) {
    if (active) {

        int color; if (world[mouse.worldx][mouse.worldy] != NULL && distance2d(miner.x/64, miner.y/64, mouse.worldx, mouse.worldy) <= 4 ) { color = COLOR_WHITE; } else { color = COLOR_GREY; }

        if (!(mouse.worldx < 1 || mouse.worldx > WORLD_WIDTH-2 || mouse.worldy < 1 || mouse.worldy > WORLD_HEIGHT-2)) {
            if (miner.breaktimer > 0) draw_rect(renderer, floatarr(4, mouse.worldx*64 - camera.x - 32, mouse.worldy*64 - camera.y - 32, 64.0f, 64.0f), color, miner.breaktimer/set.break_speed*100, true);
            draw_rect(renderer, floatarr(4, mouse.worldx*64 - camera.x - 32, mouse.worldy*64 - camera.y - 32, 64.0f, 64.0f), color, 255, false);
        }
        draw_line(renderer, mouse.x+5, mouse.y, mouse.x-5, mouse.y, color, 255); draw_line(renderer, mouse.x, mouse.y-5, mouse.x, mouse.y+5, color, 255);

        draw_line(renderer, miner.x - camera.x - 32, miner.y - camera.y - 64, miner.x - camera.x - 32+ miner.health/100*64, miner.y - camera.y - 64, COLOR_RED, 255); //health line

        // if (!set.hide_hud && inventory[miner.inventory_slot].block != NULL) text_rect(renderer, floatarr(4, 0.0f, 0.0f, (float)(win.sh2+win.sw2)/2*0.25, (float)(win.sh2+win.sw2)/2*0.25), floatarr(4, (inventory[miner.inventory_slot].block->type)*64.0f, 0.0f, 64.0f, 64.0f), block_textures64, true);
    
        for (int i = 0; i < MAX_INVENTORY_SIZE; i++) if (!set.hide_hud && inventory[i].block != NULL) text_rect(renderer, floatarr(4, 64.0f*i, 0.0f, 64.0f, 64.0f), floatarr(4, (inventory[i].block->type)*64.0f, 0.0f, 64.0f, 64.0f), block_textures64, true);
        draw_rect(renderer, floatarr(4, 64.0f*miner.inventory_slot, 0.0f, 64.0f, 64.0f), 1, 255, false);


        // float a = 10.0;
        // float b = 0.1;
        // float c = miner.x/10 + SDL_GetTicks()/100;
        // float d = 0;
        // for (int x = 0; x < win.sw; x++) {
        //     float y = a*sin(b*x+c)+d;
        //     draw_line(renderer, x, y+win.sh2, x, win.sh, COLOR_WHITE, 20);
        //     SDL_RenderPoint(renderer, x, y+win.sh2);
            
        // }

    }
}

void render_menu(bool active) {

    if (appstate == APP_MENU) {
        text_rect(renderer, floatarr(4, 0.0f, 0.0f, (float)win.sw, (float)win.sw), NULL, backsplash, false);
        draw_rect(renderer, floatarr(4, 25+(mouse.x-win.sw2)/25, 75+(mouse.y-win.sh2)/25, (float)win.sh2*1.5, (float)win.sh2*1.5), COLOR_WHITE, 255, false);
        text_rect(renderer, floatarr(4, 25+(mouse.x-win.sw2)/25, 75+(mouse.y-win.sh2)/25, (float)win.sh2*1.5, (float)win.sh2*1.5), NULL, splashscreen, false);
        text_rect(renderer, floatarr(4, 10.0f, 10.0f, 50.0f, 50.0f), NULL, logo, false);
    }
    // draw_rect(renderer, floatarr(4, 1.0f, 1.0f, 100.0f, 30.0f), COLOR_WHITE, 255, false);
    if (key.space) appstate = APP_PLAY;
}

void update_menu(bool active) {

}