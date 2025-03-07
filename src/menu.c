
void AppQuit();

void render_ui (bool active) {
    if (active) {

        Colors color; 
        if (world[mouse.worldx][mouse.worldy] != NULL && distance2d(miner.x/64, miner.y/64, mouse.worldx, mouse.worldy) <= 4 ) { 
            color = COLOR_WHITE; 
        } else { 
            color = COLOR_GREY; 
        }

        draw_line(renderer, miner.x - camera.x - 32, miner.y - camera.y - 64, miner.x - camera.x - 32+ miner.health/100*64, miner.y - camera.y - 64, COLOR_RED, 255); //health line

        if (inventory[miner.inventory_slot].block != NULL) text_rect(renderer, floatarr(4, (float)win.sw2-32, (float)win.sh-64-32, 64.0f, 64.0f), floatarr(4, (inventory[miner.inventory_slot].block->type)*64.0f, 0.0f, 64.0f, 64.0f), block_textures64, true);
        if (inventory[miner.inventory_slot].block == NULL) write_text(renderer, stringf("%d", miner.inventory_slot+1), (float)win.sw2, (float)win.sh-64-32-16, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, (float)win.sw2-32, (float)win.sh-64-32, 64.0f, 64.0f), COLOR_WHITE, 255, false);

        if (pause) {

            draw_rect(renderer, floatarr(4, 0.0f, 0.0f, (float)win.sw, (float)win.sh), COLOR_BLACK, 100, true);

            int offsety = win.sh;
            float width = 220;
            float height = 40;
            float x = (float)(win.sw - width - 30);
            float y = (float)(offsety - height - height/2);
    
            write_text(renderer, "QUIT", x + width/2, offsety - height, COLOR_WHITE, 255, true);
            draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 255, false);
            if (mouse.x >= x && mouse.x <= x + width && mouse.y >= y && mouse.y <= y + height) {
                draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 50, true);
                if (mouse.lp) {
                    appstate = APP_MENU;
                    mouse.lp = 0;
                }
            }

            y = (float)(offsety - height - 70);
    
            write_text(renderer, "SAVE AND QUIT", x + width/2, offsety - 50 - height, COLOR_WHITE, 255, true);
            draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 255, false);
            if (mouse.x >= x && mouse.x <= x + width && mouse.y >= y && mouse.y <= y + height) {
                draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 50, true);
                if (mouse.lp) {
                    save_world("gamesaves/world.save");
                    appstate = APP_MENU;
                    mouse.lp = 0;
                }
            }

            offsety = 10;

            write_text(renderer, "STATISTICS", 30, 25+offsety, COLOR_WHITE, 255, false);
            draw_line(renderer, 30, offsety+60, 380, offsety+60, COLOR_WHITE, 255);

            write_text(renderer, stringf("Total player blocks broken: %d", statistics.blocks_broken), 30, 75+offsety, COLOR_WHITE, 255, false);
            write_text(renderer, stringf("Total player blocks placed: %d", statistics.blocks_placed), 30, 105+offsety, COLOR_WHITE, 255, false);
            write_text(renderer, stringf("Total distance travelled: %.1f m", statistics.distance_travelled), 30, 135+offsety, COLOR_WHITE, 255, false);
            write_text(renderer, stringf("Total time played: %.0f hrs %.0f min %.0f sec", floor(statistics.seconds_played/60/60), (float)((int)(statistics.seconds_played/60) % 60), (float)((int)(statistics.seconds_played) % 60)), 30, 165+offsety, COLOR_WHITE, 255, false);
            write_text(renderer, stringf("Player deaths: %d", statistics.number_deaths), 30, 195+offsety, COLOR_WHITE, 255, false);
            write_text(renderer, stringf("Total damage taken: %.1f hp", statistics.damage_taken), 30, 225+offsety, COLOR_WHITE, 255, false);
            write_text(renderer, stringf("Total damage healed: %.1f hp", statistics.damage_healed), 30, 255+offsety, COLOR_WHITE, 255, false);

            write_text(renderer, "[W][A][S][D] - Move Player", 30, win.sh-145, COLOR_WHITE, 160, false);
            write_text(renderer, "[Mouse L/R] - Break/Place Blocks", 30, win.sh-115, COLOR_WHITE, 160, false);
            write_text(renderer, "[Q] - Scroll Inventory", 30, win.sh-85, COLOR_WHITE, 160, false);
            write_text(renderer, "[E] - Unpause Game", 30, win.sh-55, COLOR_WHITE, 160, false);

        } else {

            write_text(renderer, "[E] - Pause Game", 30, win.sh-55, COLOR_WHITE, 160, false);

            if (!(mouse.worldx < 1 || mouse.worldx > WORLD_WIDTH-2 || mouse.worldy < 1 || mouse.worldy > WORLD_HEIGHT-2)) {
                if (miner.breaktimer > 0) draw_rect(renderer, floatarr(4, mouse.worldx*64 - camera.x - 32, mouse.worldy*64 - camera.y - 32, 64.0f, 64.0f), color, miner.breaktimer/set.break_speed*100, true);
                draw_rect(renderer, floatarr(4, mouse.worldx*64 - camera.x - 32, mouse.worldy*64 - camera.y - 32, 64.0f, 64.0f), color, 255, false);
            }

            // for (int i = 0; i < MAX_INVENTORY_SIZE; i++) if (!set.hide_hud && inventory[i].block != NULL) text_rect(renderer, floatarr(4, 64.0f*i, 0.0f, 64.0f, 64.0f), floatarr(4, (inventory[i].block->type)*64.0f, 0.0f, 64.0f, 64.0f), block_textures64, true);
            // draw_rect(renderer, floatarr(4, 64.0f*miner.inventory_slot, 0.0f, 64.0f, 64.0f), 1, 255, false);
    
        }

    }
}

void render_update_menu(bool active) {

    for (int x = 0; x < TAPESTRY_WIDTH; x++) {
        for (int y = 0; y < TAPESTRY_HEIGHT; y++) {
            text_rect(renderer, floatarr(4, (float)(x*TAPESTRY_TILE_SIZE), (float)(y*TAPESTRY_TILE_SIZE), (float)TAPESTRY_TILE_SIZE, (float)TAPESTRY_TILE_SIZE), floatarr(4, (float)(menu_tapestry[x][y]->type)*64.0f, 0.0f, 64.0f, 64.0f), block_textures64, true); // render block 64px
        }
    }

    int offsety = 25;
    float x, y, size;
    float width, height;

    draw_rect(renderer, floatarr(4, (float)win.sw2-200, 0.0f, 400.0f, (float)win.sh), COLOR_BLACK, 190, true);

    if (appstate == APP_MENU) {

        write_text(renderer, "STONE HAVEN", win.sw2, 25+offsety, COLOR_WHITE, 255, true);
        draw_line(renderer, win.sw2-125, 25+offsety+30, win.sw2+125, 25+offsety+30, COLOR_WHITE, 255);

        x = (float)(win.sw2 - 100);
        y = (float)(100 + offsety - 20);
        width = 200;
        height = 40;

        write_text(renderer, "NEW GAME", win.sw2, 100 + offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 255, false);
        if (mouse.x >= x && mouse.x <= x + width && mouse.y >= y && mouse.y <= y + height) {
            draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 50, true);
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

        y = (float)(150 + offsety - 20);
        
        write_text(renderer, "LOAD GAME", win.sw2, 150 + offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 255, false);
        if (mouse.x >= x && mouse.x <= x + width && mouse.y >= y && mouse.y <= y + height) {
            draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 50, true);
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

        y = (float)(200 + offsety - 20);

        write_text(renderer, "SETTINGS", win.sw2, 200 + offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 255, false);
        if (mouse.x >= x && mouse.x <= x + width && mouse.y >= y && mouse.y <= y + height) {
            draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 50, true);        
            if (mouse.lp) {
                appstate = APP_SETTINGS;
                mouse.lp = 0;
            };
        }

        y = (float)(250 + offsety - 20);

        write_text(renderer, "STATISTICS", win.sw2, 250 + offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 255, false);
        if (mouse.x >= x && mouse.x <= x + width && mouse.y >= y && mouse.y <= y + height) {
            draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 50, true);
            if (mouse.lp) {
                appstate = APP_STATISTICS;
                mouse.lp = 0;
            }
        }

        y = (float)(300 + offsety - 20);

        write_text(renderer, "ACHIEVEMENTS", win.sw2, 300 + offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 255, false);
        if (mouse.x >= x && mouse.x <= x + width && mouse.y >= y && mouse.y <= y + height) {
            draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 50, true);
            if (mouse.lp) {
                appstate = APP_ACHIEVEMENTS;
                mouse.lp = 0;
            }
        }

        y = (float)(375 + offsety - 20);

        write_text(renderer, "QUIT", win.sw2, 375 + offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 255, false);
        if (mouse.x >= x && mouse.x <= x + width && mouse.y >= y && mouse.y <= y + height) {
            draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 50, true);
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
        write_text(renderer, stringf("Total distance travelled: %.1f m", statistics.distance_travelled), win.sw2, 135+offsety, COLOR_WHITE, 255, true);
        write_text(renderer, stringf("Total time played: %.0f hrs %.0f min %.0f sec", floor(statistics.seconds_played/60/60), (float)((int)(statistics.seconds_played/60) % 60), (float)((int)(statistics.seconds_played) % 60)), win.sw2, 165+offsety, COLOR_WHITE, 255, true);
        write_text(renderer, stringf("Player deaths: %d", statistics.number_deaths), win.sw2, 195+offsety, COLOR_WHITE, 255, true);
        write_text(renderer, stringf("Total damage taken: %.1f hp", statistics.damage_taken), win.sw2, 225+offsety, COLOR_WHITE, 255, true);
        write_text(renderer, stringf("Total damage healed: %.1f hp", statistics.damage_healed), win.sw2, 255+offsety, COLOR_WHITE, 255, true);

        x = (float)(win.sw2 - 80);
        y = (float)(315 + offsety - 20);
        width = 160;
        height = 40;

        write_text(renderer, "EXIT", win.sw2, 315 + offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 255, false);
        if (mouse.x >= x && mouse.x <= x + width && mouse.y >= y && mouse.y <= y + height) {
            draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 50, true);
            if (mouse.lp) {
                appstate = APP_MENU;
                mouse.lp = 0;
            }
        }
        
    }

    if (appstate == APP_SETTINGS) {


        write_text(renderer, "SETTINGS", win.sw2, offsety+25, COLOR_WHITE, 255, true);
        draw_line(renderer, win.sw2-175, offsety+50, win.sw2+175, offsety+50, COLOR_WHITE, 255);

        x = (float)(win.sw2 + 65);
        y = (float)(offsety + 60);
        size = 30;
        
        write_text(renderer, "Creative Mode       ", win.sw2, offsety + 75, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, x, y, size, size), COLOR_WHITE, 255, false);
        if (set.gamemode) {
            draw_line(renderer, x, y, x + size, y + size, COLOR_WHITE, 255);
            draw_line(renderer, x, y + size, x + size, y, COLOR_WHITE, 255);
            draw_rect(renderer, floatarr(4, x, y, size, size), COLOR_WHITE, 50, true);
        }
        if (mouse.x >= x && mouse.x <= (x + size) && mouse.y >= y && mouse.y <= (y + size)) {
            draw_rect(renderer, floatarr(4, x, y, size, size), COLOR_WHITE, 50, true);
            if (mouse.lp) {
                set.gamemode = !set.gamemode;
                mouse.lp = 0;
            }
        }

        x = (float)(win.sw2 + 55);
        y = (float)(115 + offsety - 15);

        write_text(renderer, "Particles   ", win.sw2, 115 + offsety, COLOR_WHITE, 255, true);    
        draw_rect(renderer, floatarr(4, x, y, size, size), COLOR_WHITE, 255, false);
        if (set.particles) {
            draw_line(renderer, x, y, x + size, y + size, COLOR_WHITE, 255);
            draw_line(renderer, x, y + size, x + size, y, COLOR_WHITE, 255);
            draw_rect(renderer, floatarr(4, x, y, size, size), COLOR_WHITE, 50, true);
        }
        if (mouse.x >= x && mouse.x <= (x + size) && mouse.y >= y && mouse.y <= (y + size)) {
            draw_rect(renderer, floatarr(4, x, y, size, size), COLOR_WHITE, 50, true);
            if (mouse.lp) {
                set.particles = !set.particles;
                mouse.lp = 0;
            }
        }
        
        x = (float)(win.sw2 + 60);
        y = (float)(155 + offsety - 15);

        write_text(renderer, "Hide HUD  ", win.sw2, 155 + offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, x, y, size, size), COLOR_WHITE, 255, false);
        if (set.hide_hud) {
            draw_line(renderer, x, y, x + size, y + size, COLOR_WHITE, 255);
            draw_line(renderer, x, y + size, x + size, y, COLOR_WHITE, 255);
            draw_rect(renderer, floatarr(4, x, y, size, size), COLOR_WHITE, 50, true);
        }
        if (mouse.x >= x && mouse.x <= x + size && mouse.y >= y && mouse.y <= y + size) {
            draw_rect(renderer, floatarr(4, x, y, size, size), COLOR_WHITE, 50, true);
            if (mouse.lp) {
                set.hide_hud = !set.hide_hud;
                mouse.lp = 0;
            }
        }
        
        x = (float)(win.sw2 + 55);
        y = (float)(195 + offsety - 15);

        write_text(renderer, "Auto Save   ", win.sw2, 195 + offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, x, y, size, size), COLOR_WHITE, 255, false);
        if (set.auto_save) {
            draw_line(renderer, x, y, x + size, y + size, COLOR_WHITE, 255);
            draw_line(renderer, x, y + size, x + size, y, COLOR_WHITE, 255);
            draw_rect(renderer, floatarr(4, x, y, size, size), COLOR_WHITE, 50, true);
        }
        if (mouse.x >= x && mouse.x <= x + size && mouse.y >= y && mouse.y <= y + size) {
            draw_rect(renderer, floatarr(4, x, y, size, size), COLOR_WHITE, 50, true);
            if (mouse.lp) {
                set.auto_save = !set.auto_save;
                mouse.lp = 0;
            }
        }

        x = (float)(win.sw2 + 70);
        y = (float)(235 + offsety - 15);

        write_text(renderer, "Block Updates     ", win.sw2, 235 + offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, x, y, size, size), COLOR_WHITE, 255, false);
        if (set.block_updates) {
            draw_line(renderer, x, y, x + size, y + size, COLOR_WHITE, 255);
            draw_line(renderer, x, y + size, x + size, y, COLOR_WHITE, 255);
            draw_rect(renderer, floatarr(4, x, y, size, size), COLOR_WHITE, 50, true);
        }
        if (mouse.x >= x && mouse.x <= x + size && mouse.y >= y && mouse.y <= y + size) {
            draw_rect(renderer, floatarr(4, x, y, size, size), COLOR_WHITE, 50, true);
            if (mouse.lp) {
                mouse.lp = 0;
            }
        }

        x = (float)(win.sw2 - 160);
        y = (float)(285 + offsety - 20);
        width = 320;
        height = 40;

        write_text(renderer, "Set settings to low consumption", win.sw2, 285 + offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 255, false);
        if (mouse.x >= x && mouse.x <= x + width && mouse.y >= y && mouse.y <= y + height) {
            draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 50, true);
            if (mouse.l) draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 100, true);
            if (mouse.lp) {
                set.max_fps = 30.0f;
                set.block_updates = false;
                set.particles = false;
                mouse.lp = 0;
            }
        }

        write_text(renderer, stringf("Maximum FPS %d", set.max_fps), win.sw2, 345+offsety, COLOR_WHITE, 255, true);
        // write_text(renderer, stringf("Update Distance: %.0f blocks", set.update_distance), win.sw2, 385+offsety, COLOR_WHITE, 255, true);

        x = (float)(win.sw2 + 110);
        y = (float)(offsety + 330);
        size = 30;
        
        draw_rect(renderer, floatarr(4, x, y, size, size), COLOR_WHITE, 255, false);
        draw_line(renderer, x + size/6, y + size/1.5, x + size/2, y + size/3, COLOR_WHITE, 255);
        draw_line(renderer, x + size - size/6, y + size/1.5, x + size/2, y + size/3, COLOR_WHITE, 255);
        if (mouse.x >= x && mouse.x <= (x + size) && mouse.y >= y && mouse.y <= (y + size)) {
            draw_rect(renderer, floatarr(4, x, y, size, size), COLOR_WHITE, 50, true);
            if (mouse.l) draw_rect(renderer, floatarr(4, x, y, size, size), COLOR_WHITE, 100, true);
            if (mouse.lp) {
                set.max_fps += 30;
                mouse.lp = 0;
                if (set.max_fps > 120) set.max_fps = 120;
            }
        }

        x = (float)(win.sw2 - 140);
        
        draw_rect(renderer, floatarr(4, x, y, size, size), COLOR_WHITE, 255, false);
        draw_line(renderer, x + size/6, y + size/3, x + size/2, y + size/1.5, COLOR_WHITE, 255);
        draw_line(renderer, x + size - size/6,  y + size/3, x + size/2, y + size/1.5, COLOR_WHITE, 255);
        if (mouse.x >= x && mouse.x <= (x + size) && mouse.y >= y && mouse.y <= (y + size)) {
            draw_rect(renderer, floatarr(4, x, y, size, size), COLOR_WHITE, 50, true);
            if (mouse.l) draw_rect(renderer, floatarr(4, x, y, size, size), COLOR_WHITE, 100, true);
            if (mouse.lp) {
                set.max_fps -= 30;
                mouse.lp = 0;
                if (set.max_fps < 30) set.max_fps = 30;
            }
        }

        x = (float)(win.sw2 - 80);
        y = (float)(450 + offsety - 20);
        width = 160;
        height = 40;

        write_text(renderer, "EXIT", win.sw2, 450 + offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 255, false);
        if (mouse.x >= x && mouse.x <= x + width && mouse.y >= y && mouse.y <= y + height) {
            draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 50, true);
            if (mouse.lp) {
                appstate = APP_MENU;
                mouse.lp = 0;
            }
        }


    }

    if (appstate == APP_ACHIEVEMENTS) {

        write_text(renderer, "ACHIEVEMENTS", win.sw2, 25+offsety, COLOR_WHITE, 255, true);
        draw_line(renderer, win.sw2-175, 25+offsety+25, win.sw2+175, 25+offsety+25, COLOR_WHITE, 255);

        write_text(renderer, stringf("The Begginner (play for 1 hr)"), win.sw2, 75+offsety, (achievements.play_1h)? COLOR_WHITE : COLOR_GREY, 255, true); //add 30 to y
        write_text(renderer, stringf("The Amateur (play for 5 hrs)"), win.sw2, 105+offsety, (achievements.play_5h)? COLOR_WHITE : COLOR_GREY, 255, true);
        write_text(renderer, stringf("The Master (play for 10 hrs)"), win.sw2, 135+offsety, (achievements.play_10h)? COLOR_WHITE : COLOR_GREY, 255, true);

        write_text(renderer, stringf("Break your first block"), win.sw2, 175+offsety, (achievements.break_1b)? COLOR_WHITE : COLOR_GREY, 255, true); //add 30 to y
        write_text(renderer, stringf("Dig Dug (break 1,000 blocks)"), win.sw2, 205+offsety, (achievements.break_1000b)? COLOR_WHITE : COLOR_GREY, 255, true);
        write_text(renderer, stringf("Demolition Squad (break 5,000 blocks)"), win.sw2, 235+offsety, (achievements.break_5000b)? COLOR_WHITE : COLOR_GREY, 255, true);
        write_text(renderer, stringf("World Eater (break 50,000 blocks)"), win.sw2, 265+offsety, (achievements.break_50000b)? COLOR_WHITE : COLOR_GREY, 255, true);

        write_text(renderer, stringf("Place your first block"), win.sw2, 305+offsety, (achievements.place_1b)? COLOR_WHITE : COLOR_GREY, 255, true); //add 30 to y
        write_text(renderer, stringf("(place 1,000 blocks)"), win.sw2, 335+offsety, (achievements.place_1000b)? COLOR_WHITE : COLOR_GREY, 255, true);
        write_text(renderer, stringf("Bob the Builder (place 5,000 blocks)"), win.sw2, 365+offsety, (achievements.place_5000b)? COLOR_WHITE : COLOR_GREY, 255, true);
        write_text(renderer, stringf("Architect Degree (place 50,000 blocks)"), win.sw2, 395+offsety, (achievements.place_50000b)? COLOR_WHITE : COLOR_GREY, 255, true);
        
        write_text(renderer, stringf("Take your first steps"), win.sw2, 435+offsety, (achievements.walk_1m)? COLOR_WHITE : COLOR_GREY, 255, true); //add 30 to y
        write_text(renderer, stringf("Faster than Usain (travel 100m)"), win.sw2, 465+offsety, (achievements.walk_100m)? COLOR_WHITE : COLOR_GREY, 255, true);
        write_text(renderer, stringf("What even is a mile? (travel 1,609m)"), win.sw2, 495+offsety, (achievements.walk_1609m)? COLOR_WHITE : COLOR_GREY, 255, true);
        write_text(renderer, stringf("My first marathon (travel 42,195m)"), win.sw2, 525+offsety, (achievements.walk_42195m)? COLOR_WHITE : COLOR_GREY, 255, true);
        write_text(renderer, stringf("Ultramarathon (travel 100,000m)"), win.sw2, 555+offsety, (achievements.walk_100000m)? COLOR_WHITE : COLOR_GREY, 255, true);

        write_text(renderer, stringf("Aweeeee...Your first death ;P"), win.sw2, 595+offsety, (achievements.die_1t)? COLOR_WHITE : COLOR_GREY, 255, true); //add 30 to y
        write_text(renderer, stringf("Cats have 9 lives (die 9 times)"), win.sw2, 625+offsety, (achievements.die_9t)? COLOR_WHITE : COLOR_GREY, 255, true);
        write_text(renderer, stringf("Century Death (die 100 times)"), win.sw2, 655+offsety, (achievements.die_100t)? COLOR_WHITE : COLOR_GREY, 255, true);

        x = (float)(win.sw2 - 80);
        y = (float)(705 + offsety - 20);
        width = 160;
        height = 40;

        write_text(renderer, "EXIT", win.sw2, 705 + offsety, COLOR_WHITE, 255, true);
        draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 255, false);
        if (mouse.x >= x && mouse.x <= x + width && mouse.y >= y && mouse.y <= y + height) {
            draw_rect(renderer, floatarr(4, x, y, width, height), COLOR_WHITE, 50, true);
            if (mouse.lp) {
                appstate = APP_MENU;
                mouse.lp = 0;
            }
        }
    }

}
