
typedef enum {
	COLOR_BLACK,
	COLOR_WHITE,
	COLOR_RED,
	COLOR_GREY,
} Colors;

Uint8 colors[100][3] = {
  	{0,0,0},        //0-Black
  	{255,255,255},  //1-White
  	{255,0,0},      //2-Red
  	{100,100,100},  //3-grey
};

SDL_Texture* texture;
SDL_Texture* block_textures;
SDL_Texture* block_textures64;

bool load_textures(SDL_Renderer* renderer) {

	block_textures64 = IMG_LoadTexture(renderer, "./assets/blocktextures64.png");
	SDL_SetTextureScaleMode(block_textures64, SDL_SCALEMODE_NEAREST);

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

void draw_rect(SDL_Renderer* renderer, float position[4], int color, int transparency, bool fill) {
	SDL_SetRenderDrawColor(renderer, colors[color][0], colors[color][1], colors[color][2], transparency);
	SDL_FRect rect = {position[0], position[1], position[2], position[3]};
  if (fill) {
		SDL_RenderFillRect(renderer, &rect);
  } else {
	  SDL_RenderRect(renderer, &rect);
  }
}
