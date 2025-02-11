
typedef struct {
	const Uint8 *keystate;
    unsigned char a : 1, b : 1, c : 1, d : 1, e : 1, f : 1, g : 1, h : 1, i : 1;
    unsigned char j : 1, k : 1, l : 1, m : 1, n : 1, o : 1, p : 1, q : 1, r : 1;
    unsigned char s : 1, t : 1, u : 1, v : 1, w : 1, x : 1, y : 1, z : 1;
    unsigned char n1 : 1, n2 : 1, n3 : 1, n4 : 1, n5 : 1, n6 : 1, n7 : 1, n8 : 1, n9 : 1, n0 : 1;
    unsigned char enter : 1, tab : 1, shift : 1, space : 1, escape : 1, ctrl : 1, alt : 1, backspace : 1; 
    unsigned char upa : 1, downa : 1, lefta : 1, righta : 1;
    unsigned char f1 : 1, f2 : 1, f3 : 1, f4 : 1, f5 : 1, f6 : 1, f7 : 1, f8 : 1, f9 : 1, f10 : 1, f11 : 1, f12 : 1;
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

typedef enum {
	CD_KEYBOARD,
	CD_CONTROLLER,
} ControlDevice;

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
} Player;

Player miner = {    
                
    "Player",
    0.0f, 0.0f, 0.0f, 0.0f, 400.0f, 100.0f,
    // 0.0f, 0.0f, 0.0f, 0.0f, 500.0f, 100.0f,
    50.0f, 50.0f,
    0.0f, 0.0f, 0.0f
    
};

int inventory_slot = 0;

typedef struct {
  	int max_fps; //120.0f

  	bool fullscreen; //false
  	bool particles; //true
  	float brightness; //5.0f
  	ControlDevice controller; // keyboard or controller

  	bool creative;
  	bool hide_hud; //false
  	bool auto_save; // true
	float update_distance;

  	int master_vol; //100
  	int music_vol; //100
  	int sfx_vol; //100
  	int world_vol; //100

	int gravity; //1000.0f
} GameSettings;

GameSettings set = {
	120,
	false, true, 5, CD_KEYBOARD,
	true, false, true, 100,
	100, 100, 100, 100,
	1000,
};

typedef struct {
	float x, y;
	float targetx, targety;
	bool freecam;
} Camera; Camera camera;

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
    bool isIntersecting; 	// yes or no, are the lines intersecting?
    float uA, uB;  			// Intersection parameters along each line
    float cx, cy;  			// Intersection coordinates
} IntersectionResult;

#define WORLD_WIDTH 1000
#define WORLD_HEIGHT 1000

// up to 4,000,000 blocks with it being playably laggy (or x)
// up to 3,000,000 blocks with it being slightly laggy (or x)
// up to 2,500,000 blocks with it being not laggy at all (or 2000x1250)

// high fps 2,000,000 blocks
// medium fps 2,500,000 blocks
// slightly laggy 3,000,000 blocks
// playably laggy 4,000,000 blocks

#define MAX_PARTICLES 10000
#define MAX_INVENTORY_SIZE 2