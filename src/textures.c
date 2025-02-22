
typedef enum {
	COLOR_BLACK,
	COLOR_WHITE,
	COLOR_GREY,
	COLOR_RED,
	COLOR_ORANGE,
} Colors;

Uint8 colors[100][3] = {
  	{0,0,0},        //0-Black
  	{255,255,255},  //1-White
  	{100,100,100},  //2-grey
  	{255,0,0},      //3-Red
	{255, 85,  0},	//4-fireorange
};

SDL_Texture* splashscreen;
SDL_Texture* logo;
SDL_Texture* backsplash;

SDL_Texture* block_textures64;

TTF_Font* font;
SDL_Color textColor;
SDL_Surface *textSurface;
SDL_Texture *textTexture;

bool load_textures(SDL_Renderer* renderer) {

	block_textures64 = IMG_LoadTexture(renderer, "./assets/blocktextures64.png");
	logo = IMG_LoadTexture(renderer, "./assets/icon.png");
	splashscreen = IMG_LoadTexture(renderer, "./assets/splashscreen.png");
	backsplash = IMG_LoadTexture(renderer, "./assets/splashscreen.png");
	SDL_SetTextureScaleMode(block_textures64, SDL_SCALEMODE_NEAREST);

	font = TTF_OpenFont("assets/Times New Roman.ttf", 24);
    if (font == NULL) {
        // printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        return false;
    }

	return true;
}

void text_rect(SDL_Renderer* renderer, float position[4], float frame[4], SDL_Texture* texture, bool isframe) {
	if (isframe) {
	  	SDL_FRect rposition = {position[0], position[1], position[2], position[3]};
	  	SDL_FRect rframe = {frame[0], frame[1], frame[2], frame[3]};
	  	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR);
	  	SDL_RenderTexture(renderer, texture, &rframe, &rposition);
	} else {
	  	SDL_FRect rposition = {position[0], position[1], position[2], position[3]};
	  	// SDL_FRect rframe = {frame[0], frame[1], frame[2], frame[3]};
	  	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR);
	  	SDL_RenderTexture(renderer, texture, NULL, &rposition);
	}
}

void draw_rect(SDL_Renderer* renderer, float position[4], Colors color, int transparency, bool fill) {
		SDL_SetRenderDrawColor(renderer, colors[color][0], colors[color][1], colors[color][2], transparency);
		SDL_FRect rect = {position[0], position[1], position[2], position[3]};
  if (fill) {
		SDL_RenderFillRect(renderer, &rect);
  } else {
	  	SDL_RenderRect(renderer, &rect);
  }
}

void draw_line(SDL_Renderer* renderer, float x1, float y1, float x2, float y2, Colors color, int transparency) {
	SDL_SetRenderDrawColor(renderer, colors[color][0], colors[color][1], colors[color][2], transparency);
	SDL_RenderLine(renderer, x1, y1, x2, y2);
}

void write_text(SDL_Renderer* renderer, char text[100], float x, float y, Colors color, int transparency, bool center) {
	textColor.r = colors[color][0];
	textColor.g = colors[color][1];
	textColor.b = colors[color][2];
	textColor.a = transparency;
	textSurface = TTF_RenderText_Solid(font, text, strlen(text), textColor);
	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
	if (center) {
		x = x-textWidth/2;
		y = y-textHeight/2;
	}
	SDL_FRect renderQuad = { x, y, textWidth, textHeight };
    SDL_RenderTexture(renderer, textTexture, NULL, &renderQuad);
    SDL_DestroySurface(textSurface);
}
