
void render_ui (bool active) {
    if (active) {

        text_rect(renderer, floatarr(4, 0.0f, 0.0f, (float)win.sh2*0.75, (float)win.sh2*0.75), floatarr(4, (2+ blockheld % (NUM_BLOCKS-2))*64.0f, 0.0f, 64.0f, 64.0f), block_textures64, true); // block picked 64px
	    // text_rect(renderer, floatarr(4, 0.0f, 0.0f, 50.0f*(float)win.pixsizex/2, 50.0f*(float)win.pixsizey/2), floatarr(4, blockheld*16.0f, 0.0f, 16.0f, 16.0f), block_textures, true); // block picked 16px

    }
}