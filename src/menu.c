
void AppQuit();

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

void render_update_menu(bool active) {

    text_rect(renderer, floatarr(4, 10.0f, 10.0f, 50.0f, 50.0f), NULL, logo, false);

    int offsetx = 200;
    int offsety = 25;

    if (appstate == APP_MENU) {

        write_text(renderer, "STONE HAVEN", win.sw2, 25+offsety, COLOR_WHITE, 255, true);
        draw_line(renderer, win.sw2-125, 25+offsety+30, win.sw2+125, 25+offsety+30, COLOR_WHITE, 255);

        write_text(renderer, "NEW GAME", win.sw2, 100+offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, (float)win.sw2-100, (float)100+offsety-20, (float)200, (float)40), COLOR_WHITE, 255, false);
        if (mouse.x >= (float)(win.sw2 - 100) && mouse.x <= ((float)(win.sw2 - 100) + 200) && mouse.y >= (float)(100 + offsety - 20) && mouse.y <= ((float)(100 + offsety - 20) + 40)) {
            draw_rect(renderer, floatarr(4, (float)win.sw2-100, (float)100+offsety-20, (float)200, (float)40), COLOR_WHITE, 50, true);
            if (mouse.lp) {
                mouse.lp = 0;
                bool file_exists;
                FILE *file = fopen("gamesaves/world.save", "r");
                if (file) {
                    fclose(file);
                    file_exists = true;
                } else {
                    file_exists = false;
                }

                if (file_exists) {

                    SDL_MessageBoxButtonData buttons[] = {
                        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Cancel" },
                        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 1, "OK" }
                    };
                
                    SDL_MessageBoxData messageboxdata = {
                        SDL_MESSAGEBOX_WARNING,
                        window,
                        "Warning",
                        "This will delete your current save file.\nAre you sure you want to proceed?",
                        SDL_arraysize(buttons),
                        buttons,
                        NULL
                    };

                    int buttonID;
                    if (file_exists) SDL_ShowMessageBox(&messageboxdata, &buttonID);

                    if (buttonID == 0) return;

                }

                if (remove("gamesaves/world.save") == 0) {
                    printf("File deleted successfully\n");
                }
                if (!generate_world()) {
                    printf("Error generating world: %s", SDL_GetError());
                    running = false;
                }
                appstate = APP_PLAY;
            }
        }

        write_text(renderer, "LOAD GAME", win.sw2, 150+offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, (float)win.sw2-100, (float)150+offsety-20, (float)200, (float)40), COLOR_WHITE, 255, false);
        if (mouse.x >= (float)(win.sw2 - 100) && mouse.x <= ((float)(win.sw2 - 100) + 200) && mouse.y >= (float)(150 + offsety - 20) && mouse.y <= ((float)(150 + offsety - 20) + 40)) {
            draw_rect(renderer, floatarr(4, (float)win.sw2-100, (float)150+offsety-20, (float)200, (float)40), COLOR_WHITE, 50, true);
            if (mouse.lp) {
                mouse.lp = 0;
                bool file_exists;
                FILE *file = fopen("gamesaves/world.save", "r");
                if (file) {
                    fclose(file);
                    file_exists = true;
                } else {
                    file_exists = false;
                }

                if (!file_exists) {
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "", "No Savefile was found.", window);
                    return;
                }

                if (!load_world("gamesaves/world.save")) {
                    miner.x = WORLD_WIDTH*32;
                    miner.y = WORLD_HEIGHT*32;
                    miner.vx = 0;
                    miner.vy = 0;
                    if (!generate_world()) {
                        printf("Error generating world: %s", SDL_GetError());
                        running = false;
                    }
                }
                appstate = APP_PLAY;
            }
        }

        write_text(renderer, "SETTINGS", win.sw2, 200+offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, (float)win.sw2-100, (float)200+offsety-20, (float)200, (float)40), COLOR_WHITE, 255, false);
        if (mouse.x >= (float)(win.sw2 - 100) && mouse.x <= ((float)(win.sw2 - 100) + 200) && mouse.y >= (float)(200 + offsety - 20) && mouse.y <= ((float)(200 + offsety - 20) + 40)) {
            draw_rect(renderer, floatarr(4, (float)win.sw2-100, (float)200+offsety-20, (float)200, (float)40), COLOR_WHITE, 50, true);
            if (mouse.lp) {
                appstate = APP_SETTINGS;
                mouse.lp = 0;
            };
        }

        write_text(renderer, "STATISTICS", win.sw2, 250+offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, (float)win.sw2-100, (float)250+offsety-20, (float)200, (float)40), COLOR_WHITE, 255, false);
        if (mouse.x >= (float)(win.sw2 - 100) && mouse.x <= ((float)(win.sw2 - 100) + 200) && mouse.y >= (float)(250 + offsety - 20) && mouse.y <= ((float)(250 + offsety - 20) + 40)) {
            draw_rect(renderer, floatarr(4, (float)win.sw2-100, (float)250+offsety-20, (float)200, (float)40), COLOR_WHITE, 50, true);
            if (mouse.lp) {
                appstate = APP_STATISTICS;
                mouse.lp = 0;
            }
        }

        write_text(renderer, "QUIT", win.sw2, 325+offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, (float)win.sw2-100, (float)325+offsety-20, (float)200, (float)40), COLOR_WHITE, 255, false);
        if (mouse.x >= (float)(win.sw2 - 100) && mouse.x <= ((float)(win.sw2 - 100) + 200) && mouse.y >= (float)(325 + offsety - 20) && mouse.y <= ((float)(325 + offsety - 20) + 40)) {
            draw_rect(renderer, floatarr(4, (float)win.sw2-100, (float)325+offsety-20, (float)200, (float)40), COLOR_WHITE, 50, true);
            if (mouse.lp) {
                running = false;
                mouse.lp = 0;
            }
        }
    }

    if (appstate == APP_STATISTICS) {
        write_text(renderer, "STATISTICS", win.sw2, 25+offsety, COLOR_WHITE, 255, true);
        draw_line(renderer, win.sw2-175, 25+offsety+25, win.sw2+175, 25+offsety+25, COLOR_WHITE, 255);

        write_text(renderer, stringf("Total player blocks broken: %d", statistics.blocks_broken), win.sw2, 75+offsety, COLOR_WHITE, 255, true);
        write_text(renderer, stringf("Total player blocks placed: %d", statistics.blocks_placed), win.sw2, 105+offsety, COLOR_WHITE, 255, true);
        write_text(renderer, stringf("Total distance travelled: %.1f blocks", statistics.distance_travelled), win.sw2, 135+offsety, COLOR_WHITE, 255, true);
        write_text(renderer, stringf("Total time played: %.0f hrs %.0f min %.0f sec", floor(statistics.seconds_played/60/60), floor(statistics.seconds_played/60), floor(statistics.seconds_played)), win.sw2, 165+offsety, COLOR_WHITE, 255, true);

        write_text(renderer, "EXIT", win.sw2, 225+offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, (float)win.sw2-80, (float)225+offsety-20, (float)160, (float)40), COLOR_WHITE, 255, false);
        if (mouse.x >= (float)(win.sw2 - 80) && mouse.x <= ((float)(win.sw2 - 80) + 160) && mouse.y >= (float)(225 + offsety - 20) && mouse.y <= ((float)(225 + offsety - 20) + 40)) {
            draw_rect(renderer, floatarr(4, (float)win.sw2-80, (float)225+offsety-20, (float)160, (float)40), COLOR_WHITE, 50, true);
            if (mouse.lp) {
                appstate = APP_MENU;
                mouse.lp = 0;
            }
        }
    }

    if (appstate == APP_SETTINGS) {
        write_text(renderer, "SETTINGS", win.sw2, 25+offsety, COLOR_WHITE, 255, true);
        draw_line(renderer, win.sw2-175, 25+offsety+25, win.sw2+175, 25+offsety+25, COLOR_WHITE, 255);

        write_text(renderer, "Creative Mode       ", win.sw2, 75+offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, (float)win.sw2+100-35, (float)75+offsety-15, (float)30, (float)30), COLOR_WHITE, 255, false);
        if (set.gamemode) {
            draw_line(renderer, (float)win.sw2+100-35, (float)75+offsety-15, (float)win.sw2+100-35+30, (float)75+offsety-15+30, COLOR_WHITE, 255);
            draw_line(renderer, (float)win.sw2+100-35, (float)75+offsety-15+30, (float)win.sw2+100-35+30, (float)75+offsety-15, COLOR_WHITE, 255);
            draw_rect(renderer, floatarr(4, (float)win.sw2+100-35, (float)75+offsety-15, (float)30, (float)30), COLOR_WHITE, 50, true);
        }
        if (mouse.x >= (float)(win.sw2 + 100 - 35) && mouse.x <= ((float)(win.sw2 + 100 - 35) + 30) && mouse.y >= (float)(75 + offsety - 15) && mouse.y <= ((float)(75 + offsety - 15) + 30)) {
            draw_rect(renderer, floatarr(4, (float)win.sw2+100-35, (float)75+offsety-15, (float)30, (float)30), COLOR_WHITE, 50, true);
            if (mouse.lp) {
                set.gamemode = !set.gamemode;
                mouse.lp = 0;
            }
        }

        write_text(renderer, "Particles   ", win.sw2, 115+offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, (float)win.sw2+100-35-15, (float)115+offsety-15, (float)30, (float)30), COLOR_WHITE, 255, false);
        if (set.particles) {
            draw_line(renderer, (float)win.sw2+100-35-15, (float)115+offsety-15, (float)win.sw2+100-35+30-15, (float)115+offsety-15+30, COLOR_WHITE, 255);
            draw_line(renderer, (float)win.sw2+100-35-15, (float)115+offsety-15+30, (float)win.sw2+100-35+30-15, (float)115+offsety-15, COLOR_WHITE, 255);
            draw_rect(renderer, floatarr(4, (float)win.sw2+100-35-15, (float)115+offsety-15, (float)30, (float)30), COLOR_WHITE, 50, true);
        }
        if (mouse.x >= (float)(win.sw2 + 100 - 35 - 15) && mouse.x <= ((float)(win.sw2 + 100 - 35 - 15) + 30) && mouse.y >= (float)(115 + offsety - 15) && mouse.y <= ((float)(115 + offsety - 15) + 30)) {
            draw_rect(renderer, floatarr(4, (float)win.sw2+100-35-15, (float)115+offsety-15, (float)30, (float)30), COLOR_WHITE, 50, true);
            if (mouse.lp) {
                set.particles = !set.particles;
                mouse.lp = 0;
            }
        }

        write_text(renderer, "Hide HUD  ", win.sw2, 155+offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, (float)win.sw2+100-35-5, (float)155+offsety-15, (float)30, (float)30), COLOR_WHITE, 255, false);
        if (set.hide_hud) {
            draw_line(renderer, (float)win.sw2+100-35-5, (float)155+offsety-15, (float)win.sw2+100-35+30-5, (float)155+offsety-15+30, COLOR_WHITE, 255);
            draw_line(renderer, (float)win.sw2+100-35-5, (float)155+offsety-15+30, (float)win.sw2+100-35+30-5, (float)155+offsety-15, COLOR_WHITE, 255);
            draw_rect(renderer, floatarr(4, (float)win.sw2+100-35-5, (float)155+offsety-15, (float)30, (float)30), COLOR_WHITE, 50, true);
        }
        if (mouse.x >= (float)(win.sw2 + 100 - 35 - 5) && mouse.x <= ((float)(win.sw2 + 100 - 35 - 5) + 30) && mouse.y >= (float)(155 + offsety - 15) && mouse.y <= ((float)(155 + offsety - 15) + 30)) {
            draw_rect(renderer, floatarr(4, (float)win.sw2+100-35-5, (float)155+offsety-15, (float)30, (float)30), COLOR_WHITE, 50, true);
            if (mouse.lp) {
                set.hide_hud = !set.hide_hud;
                mouse.lp = 0;
            }
        }

        write_text(renderer, "Auto Save   ", win.sw2, 195+offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, (float)win.sw2+100-35-15, (float)195+offsety-15, (float)30, (float)30), COLOR_WHITE, 255, false);
        if (set.auto_save) {
            draw_line(renderer, (float)win.sw2+100-35-15, (float)195+offsety-15, (float)win.sw2+100-35+30-15, (float)195+offsety-15+30, COLOR_WHITE, 255);
            draw_line(renderer, (float)win.sw2+100-35-15, (float)195+offsety-15+30, (float)win.sw2+100-35+30-15, (float)195+offsety-15, COLOR_WHITE, 255);
            draw_rect(renderer, floatarr(4, (float)win.sw2+100-35-15, (float)195+offsety-15, (float)30, (float)30), COLOR_WHITE, 50, true);
        }
        if (mouse.x >= (float)(win.sw2 + 100 - 35 - 15) && mouse.x <= ((float)(win.sw2 + 100 - 35 - 15) + 30) && mouse.y >= (float)(195 + offsety - 15) && mouse.y <= ((float)(195 + offsety - 15) + 30)) {
            draw_rect(renderer, floatarr(4, (float)win.sw2+100-35-15, (float)195+offsety-15, (float)30, (float)30), COLOR_WHITE, 50, true);
            if (mouse.lp) {
                set.auto_save = !set.auto_save;
                mouse.lp = 0;
            }
        }

        write_text(renderer, "Set settings to best gameplay", win.sw2, 245+offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, (float)win.sw2-160, (float)245+offsety-20, (float)320, (float)40), COLOR_WHITE, 255, false);
        if (mouse.x >= (float)(win.sw2 - 160) && mouse.x <= ((float)(win.sw2 - 160) + 320) && mouse.y >= (float)(245 + offsety - 20) && mouse.y <= ((float)(245 + offsety - 20) + 40)) {
            draw_rect(renderer, floatarr(4, (float)win.sw2-160, (float)245+offsety-20, (float)320, (float)40), COLOR_WHITE, 50, true);
            if (mouse.l) draw_rect(renderer, floatarr(4, (float)win.sw2-160, (float)245+offsety-20, (float)320, (float)40), COLOR_WHITE, 100, true);
            if (mouse.lp) {
                set.max_fps = 60.0f;
                set.update_distance = 100;
                set.particles = true;
                mouse.lp = 0;
            }
        }

        write_text(renderer, "Set settings to fastest fps", win.sw2, 295+offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, (float)win.sw2-160, (float)295+offsety-20, (float)320, (float)40), COLOR_WHITE, 255, false);
        if (mouse.x >= (float)(win.sw2 - 160) && mouse.x <= ((float)(win.sw2 - 160) + 320) && mouse.y >= (float)(295 + offsety - 20) && mouse.y <= ((float)(295 + offsety - 20) + 40)) {
            draw_rect(renderer, floatarr(4, (float)win.sw2-160, (float)295+offsety-20, (float)320, (float)40), COLOR_WHITE, 50, true);
            if (mouse.l) draw_rect(renderer, floatarr(4, (float)win.sw2-160, (float)295+offsety-20, (float)320, (float)40), COLOR_WHITE, 100, true);
            if (mouse.lp) {
                set.max_fps = 30.0f;
                set.update_distance = 20;
                set.particles = false;
                mouse.lp = 0;
            }
        }

        write_text(renderer, stringf("Maximum FPS: %d", set.max_fps), win.sw2, 345+offsety, COLOR_WHITE, 255, true);
        write_text(renderer, stringf("Update Distance: %.0f blocks", set.update_distance), win.sw2, 385+offsety, COLOR_WHITE, 255, true);

        write_text(renderer, "EXIT", win.sw2, 450+offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, (float)win.sw2-80, (float)450+offsety-20, (float)160, (float)40), COLOR_WHITE, 255, false);
        if (mouse.x >= (float)(win.sw2 - 80) && mouse.x <= ((float)(win.sw2 - 80) + 160) && mouse.y >= (float)(450 + offsety - 20) && mouse.y <= ((float)(450 + offsety - 20) + 40)) {
            draw_rect(renderer, floatarr(4, (float)win.sw2-80, (float)450+offsety-20, (float)160, (float)40), COLOR_WHITE, 50, true);
            if (mouse.lp) {
                appstate = APP_MENU;
                mouse.lp = 0;
            }
        }

    }

}
