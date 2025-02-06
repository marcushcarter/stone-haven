
Uint8 colors[100][3] = {
  {0,0,0},        //0-Black
  {255,255,255},  //1-White
  {255,0,0},      //2-Red
  {100,100,100},  //3-grey
};

SDL_Texture* texture;
SDL_Texture* block_textures;
SDL_Texture* block_textures64;

// SDL_Surface* icon;

bool load_textures(SDL_Renderer* renderer) {
  texture = IMG_LoadTexture(renderer, "./assets/test.jpg");

  // icon = IMG_LoadTexture(renderer, "./assets/test.jpg");

  block_textures = IMG_LoadTexture(renderer, "./assets/blocktextures.png");
  // block_textures64 = IMG_LoadTexture(renderer, "./assets/blocktextures64.png");
  block_textures64 = IMG_LoadTexture(renderer, "./assets/blocktextures64proto.png");
  
	SDL_SetTextureScaleMode(block_textures, SDL_SCALEMODE_NEAREST);

  // item_texture[I_AIR] = IMG_LoadTexture(renderer, "assets/test.jpg");
  // item_texture[I_WORLDBORDER] = IMG_LoadTexture(renderer, "./assets/blocktextures/dirt.jpg");
  // item_texture[I_STONE] = IMG_LoadTexture(renderer, "./assets/blocktextures/stone.png");

  return true;
}
