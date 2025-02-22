
typedef struct {
	const Uint8 *keystate;
    unsigned char a : 1, b : 1, c : 1, d : 1, e : 1, f : 1, g : 1, h : 1, i : 1;
    unsigned char j : 1, k : 1, l : 1, m : 1, n : 1, o : 1, p : 1, q : 1, r : 1;
    unsigned char s : 1, t : 1, u : 1, v : 1, w : 1, x : 1, y : 1, z : 1;
    unsigned char n1 : 1, n2 : 1, n3 : 1, n4 : 1, n5 : 1, n6 : 1, n7 : 1, n8 : 1, n9 : 1, n0 : 1;
    unsigned char enter : 1, tab : 1, shift : 1, space : 1, escape : 1, ctrl : 1, alt : 1, backspace : 1; 
    unsigned char upa : 1, downa : 1, lefta : 1, righta : 1;
    unsigned char f1 : 1, f2 : 1, f3 : 1, f4 : 1, f5 : 1, f6 : 1, f7 : 1, f8 : 1, f9 : 1, f10 : 1, f11 : 1, f12 : 1;

    unsigned char a_ : 1, b_ : 1, c_ : 1, d_ : 1, e_ : 1, f_ : 1, g_ : 1, h_ : 1, i_ : 1;
    unsigned char j_ : 1, k_ : 1, l_ : 1, m_ : 1, n_ : 1, o_ : 1, p_ : 1, q_ : 1, r_ : 1;
    unsigned char s_ : 1, t_ : 1, u_ : 1, v_ : 1, w_ : 1, x_ : 1, y_ : 1, z_ : 1;
    unsigned char n1_ : 1, n2_ : 1, n3_ : 1, n4_ : 1, n5_ : 1, n6_ : 1, n7_ : 1, n8_ : 1, n9_ : 1, n0_ : 1;
    unsigned char enter_ : 1, tab_ : 1, shift_ : 1, space_ : 1, escape_ : 1, ctrl_ : 1, alt_ : 1, backspace_ : 1;
    unsigned char upa_ : 1, downa_ : 1, lefta_ : 1, righta_ : 1;
    unsigned char f1_ : 1, f2_ : 1, f3_ : 1, f4_ : 1, f5_ : 1, f6_ : 1, f7_ : 1, f8_ : 1, f9_ : 1, f10_ : 1, f11_ : 1, f12_ : 1;

} keyio; keyio key;

typedef struct {
	float x, y;
	float xrelative, yrelative;
	int worldx, worldy;
	Uint32 state, prev_state;
    Uint8 l, r, m;      // current mouse button states
    Uint8 prev_l, prev_r, prev_m; // previous mouse button states
    Uint8 lp, rp, mp;   // left, right, middle press (1 if pressed this frame)
} mouseio; mouseio mouse;

SDL_Window* window;
SDL_Renderer* renderer;
bool running;
SDL_Event event;

typedef struct {
  int sw, sh;
  int sw2, sh2;
} WindowDetails;

WindowDetails win = {1024, 768, 512, 384};

clock_t previous_time = 0;
float dt;

typedef enum {
    COLLISION_BOTTOM,
    COLLISION_TOP,
    COLLISION_LEFT,
    COLLISION_RIGHT,
} CollisionType;

typedef struct {
    float x1, y1, x2, y2;
} Line; Line l1, l2;

typedef struct {
    bool isIntersecting;
    float uA, uB;
    float cx, cy;
} IntersectionResult;

// ------------------------------------------------------------------------------------------------------------

typedef enum {
    APP_MENU,
    APP_SETTINGS,
    APP_STATISTICS,
    APP_PLAY,
} AppState;

AppState appstate = APP_MENU;

bool pause = false;

typedef enum {
    GM_SURVIVAL,
    GM_CREATIVE,
    GM_FREECAM,
} Gamemode;

typedef struct {
  	int max_fps; //120.0f

  	bool fullscreen; //false
  	bool particles; //true
  	float brightness; //5.0f

    Gamemode gamemode;
  	bool hide_hud; //false
  	bool auto_save; // true
	float update_distance;
    float break_speed;

  	int master_vol; //100
  	int music_vol; //100
  	int sfx_vol; //100
  	int world_vol; //100

	int gravity; //1000.0f
} GameSettings;

GameSettings set = {
	120,
	false, true, 5,
    GM_SURVIVAL, false, true, 100, 0.5,
	100, 100, 100, 100,
	1000,
};

typedef struct {
	float x, y;
	float targetx, targety;
} Camera; Camera camera;

typedef struct {
    int blocks_broken;
    int blocks_placed;
    float distance_travelled;
    float seconds_played;
    int items_collected;
} Statistics;

Statistics statistics = {
    0, 0, 0.0f, 0.0f, 0,
};

// ------------------------------------------------------------------------------------------------------------

typedef struct {
    char name[100];

    float x, y;
    float vx, vy;
    float speed;
    float health;

    float width, height;

    float falling;
    float jumptimer;
    float breaktimer;
    float healtimer;
    float scaffoldtimer;

    int inventory_slot;
} Player;

Player miner = {    
                
    "Player",
    0.0f, 0.0f, 0.0f, 0.0f, 400.0f, 100.0f,
    // 0.0f, 0.0f, 0.0f, 0.0f, 500.0f, 100.0f,
    50.0f, 50.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0
    
};

typedef enum {
    P_GRAVITY,
    P_EASE,
    P_FLOAT,
    P_FALL,
} ParticleType;

typedef struct {
    ParticleType type;
    float life;
    float x, y, vx, vy;
    int color;
} Particle;

#define WORLD_WIDTH 50 //2000
#define WORLD_HEIGHT 50 //2000

#define MAX_PARTICLES 10000
#define MAX_INVENTORY_SIZE 15

#define TAPESTRY_WIDTH 120 //240
#define TAPESTRY_HEIGHT 75 //150
#define TAPESTRY_TILE_SIZE 32 //16