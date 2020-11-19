#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <math.h>

// If defined, logs will be shown on console and written to file.
// If commented out, logs will not be shown nor be saved.
#define LOG_ENABLED

/* Constants. */

// Frame rate (frame per second)
const int FPS = 60;
// Display (screen) width.
const int SCREEN_W = 800;
// Display (screen) height.
const int SCREEN_H = 600;
// At most 4 audios can be played at a time.
const int RESERVE_SAMPLES = 4;
// Same as:
// const int SCENE_MENU = 1;
// const int SCENE_START = 2;
//scene set
enum {
	SCENE_MENU = 1,
	SCENE_START = 2,
	// [HACKATHON 3-7]
	// TODO: Declare a new scene id.
	// , SCENE_SETTINGS = 3
	SCENE_SETTINGS = 3,
	SCENE_SETTINGS2 = 4,
	SCENE_WIN = 5,
	SCENE_LOSE = 6,
	SCENE_HELP = 7,
	SCENE_HELP2 = 8,
	SCENE_CHARACTER = 9,
	SCENE_BOSS = 10
};

/* Input states */

// The active scene id.
int active_scene;
// Keyboard state, whether the key is down or not.
bool key_state[ALLEGRO_KEY_MAX];
// Mouse state, whether the key is down or not.
// 1 is for left, 2 is for right, 3 is for middle.
bool *mouse_state;
// Mouse position.
// Mouse position.
int mouse_x, mouse_y;
// TODO: More variables to store input states such as joysticks, ...
bool *joysticks_state;


/* Variables for allegro basic routines. */

ALLEGRO_DISPLAY* game_display;
ALLEGRO_EVENT_QUEUE* game_event_queue;
ALLEGRO_TIMER* game_update_timer;

/* Shared resources*/

ALLEGRO_FONT* font_pirulen_32;
ALLEGRO_FONT* font_pirulen_24;
ALLEGRO_FONT* font_chinese_40;
// TODO: More shared resources or data that needed to be accessed
// across different scenes.

/* Menu Scene resources*/
ALLEGRO_BITMAP* main_img_background;
// [HACKATHON 3-1]
// TODO: Declare 2 variables for storing settings images.
// Uncomment and fill in the code below.
ALLEGRO_BITMAP* img_settings;
ALLEGRO_BITMAP* img_settings2;
ALLEGRO_SAMPLE* main_bgm;
ALLEGRO_SAMPLE_ID main_bgm_id;

/* Start Scene resources*/
ALLEGRO_BITMAP* start_img_background;
ALLEGRO_BITMAP* start_img_plane;
ALLEGRO_BITMAP* start_img_plane2;
ALLEGRO_BITMAP* start_img_plane3;
ALLEGRO_BITMAP* start_img_bigboss;
ALLEGRO_BITMAP* start_img_enemy;
ALLEGRO_SAMPLE* start_bgm;
ALLEGRO_SAMPLE_ID start_bgm_id;
ALLEGRO_SAMPLE* win_bgm;
ALLEGRO_SAMPLE_ID win_bgm_id;
ALLEGRO_SAMPLE* lose_bgm;
ALLEGRO_SAMPLE_ID lose_bgm_id;
ALLEGRO_SAMPLE* boss_bgm;
ALLEGRO_SAMPLE_ID boss_bgm_id;
ALLEGRO_BITMAP* boss_img_background;
ALLEGRO_SAMPLE* gun_bgm;
ALLEGRO_SAMPLE_ID gun_bgm_id;
ALLEGRO_BITMAP* rule_img_background;
ALLEGRO_BITMAP* setting_img_background;
ALLEGRO_SAMPLE* setting_bgm;
ALLEGRO_SAMPLE_ID setting_bgm_id;
// [HACKATHON 2-1]
// TODO: Declare a variable to store your bullet's image.
// Uncomment and fill in the code below.
ALLEGRO_BITMAP* img_bullet;
ALLEGRO_BITMAP* img_weapon;
ALLEGRO_BITMAP* img_boom;
ALLEGRO_BITMAP* img_BULLET;
ALLEGRO_BITMAP* character_img_background;

typedef struct {
	//The center coordinate of the image.
	float x, y;
	//The width and height of the object.
	float w, h;
	//The velocity in x, y axes.
	float vx, vy;
	//Should we draw this object on the screen.
	bool hidden;
	//The pointer to the object is image.
	ALLEGRO_BITMAP* img;
} MovableObject;
//call movable_object
void draw_movable_object(MovableObject obj);
#define MAX_ENEMY 5
// [HACKATHON 2-2]
// TODO: Declare the max bullet count that will show on screen.
// You can try max 4 bullets here and see if you needed more.
// Uncomment and fill in the code below.
#define MAX_BULLET 5
#define MAX_WEAPON 2
#define MAX_BOOM 5
#define MAX_BULLTEBOSS 1000
MovableObject plane;
MovableObject bigboss;
MovableObject enemies[MAX_ENEMY];
// [HACKATHON 2-3]
// TODO: Declare an array to store bullets with size of max bullet count.
// Uncomment and fill in the code below.
MovableObject bullets[MAX_BULLET];
MovableObject BULLETS[MAX_BULLTEBOSS];
MovableObject weapons[MAX_ENEMY][MAX_WEAPON];
MovableObject booms[MAX_BOOM];
// [HACKATHON 2-4]
// TODO: Set up bullet shooting cool-down variables.
// 1) Declare your shooting cool-down time as constant. (0.2f will be nice)
// 2) Declare your last shoot timestamp.
// Uncomment and fill in the code below.
const float MAX_COOLDOWN = 0.2f;
const float MAX_COOLDOWN_weapon = 1.5;
const float MAX_COOLDOWN_boom = 0.75;

double last_shoot_timestamp;
double last_shoot_timestamp_boss;
double last_shoot_timestamp_weapon1;
double last_shoot_timestamp_weapon2;
double last_shoot_timestamp_weapon3;
double last_shoot_timestamp_weapon4;
double last_shoot_timestamp_weapon5;
double last_shoot_timestamp_weapon6;
/* Declare function prototypes. */
int lives = 5;
int score = 0;
int flag_enemies[MAX_ENEMY] = { 0 };
int flag_bigboss = 0;
int flag_RE = 0;
int flag_CHARAC = 2;
// Initialize allegro5 library
void allegro5_init(void);
// Initialize variables and resources.
// Allows the game to perform any initialization it needs before
// starting to run.
void game_init(void);
// Process events inside the event queue using an infinity loop.
void game_start_event_loop(void);
// Run game logic such as updating the world, checking for collision,
// switching scenes and so on.
// This is called when the game should update its logic.
void game_update(void);
// Draw to display.
// This is called when the game should draw itself.
void game_draw(void);
// Release resources.
// Free the pointers we allocated.
void game_destroy(void);
// Function to change from one scene to another.
void game_change_scene(int next_scene);
// Load resized bitmap and check if failed.
ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h);
// [HACKATHON 3-2]
// TODO: Declare a function.
// Determines whether the point (px, py) is in rect (x, y, w, h).
// Uncomment the code below.
bool pnt_in_rect(int px, int py, int x, int y, int w, int h);

/* Event callbacks. */
void on_key_down(int keycode);
void on_mouse_down(int btn, int x, int y);

/* Declare function prototypes for debugging. */

// Display error message and exit the program, used like 'printf'.
// Write formatted output to stdout and file from the format string.
// If the program crashes unexpectedly, you can inspect "log.txt" for
// further information.
void game_abort(const char* format, ...);
// Log events for later debugging, used like 'printf'.
// Write formatted output to stdout and file from the format string.
// You can inspect "log.txt" for logs in the last run.
void game_log(const char* format, ...);
// Log using va_list.
void game_vlog(const char* format, va_list arg);

int main(int argc, char** argv) {
	// Set random seed for better random outcome.
	srand(time(NULL));
	allegro5_init();
	game_log("Allegro5 initialized");
	game_log("Game begin");
	// Initialize game variables.
	game_init();
	game_log("Game initialized");
	// Draw the first frame.
	game_draw();
	game_log("Game start event loop");
	// This call blocks until the game is finished.
	game_start_event_loop();
	game_log("Game end");
	game_destroy();
	return 0;
}

void allegro5_init(void) {
	if (!al_init())
		game_abort("failed to initialize allegro");

	// Initialize add-ons.
	if (!al_init_primitives_addon())
		game_abort("failed to initialize primitives add-on");
	if (!al_init_font_addon())
		game_abort("failed to initialize font add-on");
	if (!al_init_ttf_addon())
		game_abort("failed to initialize ttf add-on");
	if (!al_init_image_addon())
		game_abort("failed to initialize image add-on");
	if (!al_install_audio())
		game_abort("failed to initialize audio add-on");
	if (!al_init_acodec_addon())
		game_abort("failed to initialize audio codec add-on");
	if (!al_reserve_samples(RESERVE_SAMPLES))
		game_abort("failed to reserve samples");
	if (!al_install_keyboard())
		game_abort("failed to install keyboard");
	if (!al_install_mouse())
		game_abort("failed to install mouse");
	// TODO: Initialize other addons such as video, ...

	// Setup game display.
	game_display = al_create_display(SCREEN_W, SCREEN_H);
	if (!game_display)
		game_abort("failed to create display");
	al_set_window_title(game_display, "Final Project <105072145>");

	// Setup update timer.
	game_update_timer = al_create_timer(1.0f / FPS);
	if (!game_update_timer)
		game_abort("failed to create timer");

	// Setup event queue.
	game_event_queue = al_create_event_queue();
	if (!game_event_queue)
		game_abort("failed to create event queue");

	// Malloc mouse buttons state according to button counts.
	const unsigned m_buttons = al_get_mouse_num_buttons();
	game_log("There are total %u supported mouse buttons", m_buttons);
	// mouse_state[0] will not be used.
	mouse_state = malloc((m_buttons + 1) * sizeof(bool));
	memset(mouse_state, false, (m_buttons + 1) * sizeof(bool));

	// Register display, timer, keyboard, mouse events to the event queue.
	al_register_event_source(game_event_queue, al_get_display_event_source(game_display));
	al_register_event_source(game_event_queue, al_get_timer_event_source(game_update_timer));
	al_register_event_source(game_event_queue, al_get_keyboard_event_source());
	al_register_event_source(game_event_queue, al_get_mouse_event_source());
	// TODO: Register other event sources such as timer, video, ...

	// Start the timer to update and draw the game.
	al_start_timer(game_update_timer);
}

void game_init(void) {
	/* Shared resources*/
	font_pirulen_32 = al_load_font("pirulen.ttf", 32, 0);
	if (!font_pirulen_32)
		game_abort("failed to load font: pirulen.ttf with size 32");

	font_pirulen_24 = al_load_font("pirulen.ttf", 24, 0);
	if (!font_pirulen_24)
		game_abort("failed to load font: pirulen.ttf with size 24");

	font_chinese_40 = al_load_font("chinese.ttf", 40, 0);
	if (!font_chinese_40)
		game_abort("failed to load font: chinese.ttf with size 40");

	/* Menu Scene resources*/
	main_img_background = load_bitmap_resized("main-bg.jpg", SCREEN_W, SCREEN_H);

	main_bgm = al_load_sample("10.ogg");
	if (!main_bgm)
		game_abort("failed to load audio: 10.ogg");

	// [HACKATHON 3-4]
	// TODO: Load settings images.
	// Don't forget to check their return values.
	// Uncomment and fill in the code below.
	img_settings = al_load_bitmap("settings.png");
	if (!img_settings)
		game_abort("failed to load image: settings.png");
	img_settings2 = al_load_bitmap("settings2.png");
	if (!img_settings2)
		game_abort("failed to load image: settings2.png");

	/* Start Scene resources*/
	start_img_background = load_bitmap_resized("start-bg.jpg", SCREEN_W, SCREEN_H);
	start_img_plane = al_load_bitmap("plane.png");
	start_img_bigboss = al_load_bitmap("bigboss.png");
	if (!start_img_plane)
		game_abort("failed to load image: plane.png");
	if (!start_img_bigboss)
		game_abort("failed to load image: bigboss.png");
	start_img_plane2 = al_load_bitmap("plane1.png");
	if (!start_img_plane2)
		game_abort("failed to load image: plane1.png");
	start_img_plane3 = al_load_bitmap("plane3.png");
	if (!start_img_plane3)
		game_abort("failed to load image: plane3.png");

	start_img_enemy = al_load_bitmap("2.png");
	if (!start_img_enemy)
		game_abort("failed to load image: 2.png");

	start_bgm = al_load_sample("34.ogg");
	if (!start_bgm)
		game_abort("failed to load audio: 34.ogg");
	win_bgm = al_load_sample("6.ogg");
	if (!win_bgm)
		game_abort("failed to load audio: 6.ogg");
	lose_bgm = al_load_sample("22.ogg");
	if (!lose_bgm)
		game_abort("failed to load audio: 22.ogg");
	boss_bgm = al_load_sample("black.ogg");
	if (!boss_bgm)
		game_abort("failed to load audio: black.ogg");
	boss_img_background = load_bitmap_resized("images(2).jpg", SCREEN_W, SCREEN_H);
	if (!boss_img_background)
		game_abort("failed to load image: images(2).png");
	character_img_background = load_bitmap_resized("images(3).jpg", SCREEN_W, SCREEN_H);
	if (!character_img_background)
		game_abort("failed to load image: images(3).png");
	gun_bgm = al_load_sample("gun sound.ogg");
	if (!gun_bgm)
		game_abort("failed to load audio: gun sound.ogg");

	setting_bgm = al_load_sample("5.ogg");
	if (!setting_bgm)
		game_abort("failed to load audio: 5.ogg");
	rule_img_background = al_load_bitmap("images(4).png");
	if (!rule_img_background)
		game_abort("failed to load image: images(4).png");
	setting_img_background = load_bitmap_resized("images.jpg", SCREEN_W, SCREEN_H);
	if (!rule_img_background)
		game_abort("failed to load image: images.jpg");

	// [HACKATHON 2-5-1]
	// TODO: Initialize bullets.
	// 1) Search for a bullet image online and put it in your project.
	//    You can use the image we provided.
	// 2) Load it in by 'al_load_bitmap' or 'load_bitmap_resized'.
	// 3) If you use 'al_load_bitmap', don't forget to check its return value.
	// Uncomment and fill in the code below.
	img_bullet = al_load_bitmap("needle.png");
	if (!img_bullet)
		game_abort("failed to load image: needle.png");
	img_weapon = al_load_bitmap("bullet.png");
	if (!img_weapon)
		game_abort("failed to load image: bullet.png");
	img_boom = al_load_bitmap("pill.png");
	if (!img_boom)
		game_abort("failed to load image: pill.png");
	img_BULLET = al_load_bitmap("image13.png");
	if (!img_BULLET)
		game_abort("failed to load image: image13.png");
	// Change to first scene.
	game_change_scene(SCENE_MENU);
}

void game_start_event_loop(void) {
	bool done = false;
	ALLEGRO_EVENT event;
	int redraws = 0;
	while (!done) {
		al_wait_for_event(game_event_queue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			// Event for clicking the window close button.
			game_log("Window close button clicked");
			done = true;
		}
		else if (event.type == ALLEGRO_EVENT_TIMER) {
			// Event for redrawing the display.
			if (event.timer.source == game_update_timer)
				// The redraw timer has ticked.
				redraws++;
		}
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			// Event for keyboard key down.
			game_log("Key with keycode %d down", event.keyboard.keycode);
			key_state[event.keyboard.keycode] = true;
			on_key_down(event.keyboard.keycode);
		}
		else if (event.type == ALLEGRO_EVENT_KEY_UP) {
			// Event for keyboard key up.
			game_log("Key with keycode %d up", event.keyboard.keycode);
			key_state[event.keyboard.keycode] = false;
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			// Event for mouse key down.
			game_log("Mouse button %d down at (%d, %d)", event.mouse.button, event.mouse.x, event.mouse.y);
			mouse_state[event.mouse.button] = true;
			on_mouse_down(event.mouse.button, event.mouse.x, event.mouse.y);
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			// Event for mouse key up.
			game_log("Mouse button %d up at (%d, %d)", event.mouse.button, event.mouse.x, event.mouse.y);
			mouse_state[event.mouse.button] = false;
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
			if (event.mouse.dx != 0 || event.mouse.dy != 0) {
				// Event for mouse move.
				game_log("Mouse move to (%d, %d)", event.mouse.x, event.mouse.y);
				mouse_x = event.mouse.x;
				mouse_y = event.mouse.y;
			}
			else if (event.mouse.dz != 0) {
				// Event for mouse scroll.
				game_log("Mouse scroll at (%d, %d) with delta %d", event.mouse.x, event.mouse.y, event.mouse.dz);
			}
		}
		// TODO: Process more events and call callbacks by adding more
		// entries inside Scene.

		// Redraw
		if (redraws > 0 && al_is_event_queue_empty(game_event_queue)) {
			if (redraws > 1)
				game_log("%d frame(s) dropped", redraws - 1);
			// Update and draw the next frame.
			game_update();
			game_draw();
			redraws = 0;
		}
	}
}

void game_update(void) {
	if (active_scene == SCENE_START) {
		plane.vx = plane.vy = 0;
		if (key_state[ALLEGRO_KEY_UP] || key_state[ALLEGRO_KEY_W])
			plane.vy -= 1;
		if (key_state[ALLEGRO_KEY_DOWN] || key_state[ALLEGRO_KEY_S])
			plane.vy += 1;
		if (key_state[ALLEGRO_KEY_LEFT] || key_state[ALLEGRO_KEY_A])
			plane.vx -= 1;
		if (key_state[ALLEGRO_KEY_RIGHT] || key_state[ALLEGRO_KEY_D])
			plane.vx += 1;
		// 0.71 is (1/sqrt(2)).
		plane.y += plane.vy * 4 * (plane.vx ? 0.71f : 1);
		plane.x += plane.vx * 4 * (plane.vy ? 0.71f : 1);
		// [HACKATHON 1-1]
		// TODO: Limit the plane's collision box inside the frame.
		//       (x, y axes can be separated.)
		// Uncomment and fill in the code below.
		if (plane.x - plane.w / 2 < 0)
			plane.x = plane.w / 2;
		else if (plane.x + plane.w / 2 > SCREEN_W)
			plane.x = SCREEN_W - plane.w / 2;
		if (plane.y - plane.h / 2 < 0)
			plane.y = plane.h / 2;
		else if (plane.y + plane.h / 2 > SCREEN_H)
			plane.y = SCREEN_H - plane.h / 2;
		// [HACKATHON 2-6]
		// TODO: Update bullet coordinates.
		// 1) For each bullets, if it's not hidden, update x, y
		// according to vx, vy.
		// 2) If the bullet is out of the screen, hide it.
		// Uncomment and fill in the code below.
		int i, j;

		for (i = 0; i < MAX_ENEMY; i++) {
			if (i % 2 == 0) {
				enemies[i].x += enemies[i].vx;
				enemies[i].y -= enemies[i].vy;
			}
			else {
				enemies[i].x -= enemies[i].vx;
				enemies[i].y += enemies[i].vy;
			}
			if (enemies[i].x <= 0 + 50 || enemies[i].x >= SCREEN_W - 50)
				enemies[i].vx = -enemies[i].vx;
			if (enemies[i].y <= 0 + 60 || enemies[i].y >= SCREEN_W / 2 - 100)
				enemies[i].vy = -enemies[i].vy;

		}

		for (i = 0; i < MAX_BULLET; i++) {
			if (bullets[i].hidden)
				continue;
			bullets[i].x += bullets[i].vx;
			bullets[i].y += bullets[i].vy;

			for (j = 0; j < MAX_ENEMY; j++) {
				if (bullets[i].x >= enemies[j].x - enemies[j].w * 1 / 2 && bullets[i].x <= enemies[j].x + enemies[j].w * 1 / 2 && bullets[i].y <= enemies[j].y + enemies[j].h * 1 / 2 && bullets[i].y >= enemies[j].y - enemies[j].h * 1 / 2) {
					bullets[i].hidden = true;
					flag_enemies[j] += 1;
					if (flag_enemies[j] == MAX_ENEMY) {
						enemies[j].hidden = true;
						enemies[j].x = -100;
						enemies[j].y = -100;
					}
					bullets[i].x = -100;
					bullets[i].y = -100;
					score += 50;

				}
				/*if (plane.x + plane.w/2 == enemies[j].x -enemies[j].w*1/2 || plane.x-plane.w/2 == enemies[j].x + enemies[j].w*1/2 || plane.y-plane.h/2 == enemies[j].y + enemies[j].h*1/2 || plane.y+plane.h/2 == enemies[j].y - enemies[j].h*1/2 ){
					game_change_scene(SCENE_LOSE);
				}*/

			}
			if (bullets[i].y < 0) {
				bullets[i].hidden = true;
			}

		}
		for (i = 0; i < MAX_ENEMY; i++) {
			for (j = 0; j < MAX_WEAPON; j++) {
				weapons[i][j].x += weapons[i][j].vx;
				weapons[i][j].y += weapons[i][j].vy;
				if (weapons[i][j].y > SCREEN_H)
					weapons[i][j].hidden = true;
				if (enemies[i].hidden)
					weapons[i][j].hidden = true;
				if (weapons[i][j].x >= plane.x - plane.w * 1 / 2 && weapons[i][j].x <= plane.x + plane.w * 1 / 2 && weapons[i][j].y <= plane.y + plane.h * 1 / 2 && weapons[i][j].y >= plane.y - plane.h * 1 / 2) {
					weapons[i][j].hidden = true;
					weapons[i][j].x = -100;
					weapons[i][j].y = -100;
					lives -= 1;
					if (score > 20)
						score -= 20;
					continue;
				}
				/*for(k=0;k<MAX_BULLET;k++){
					if (weapons[i][j].x >= bullets[k].x - bullets[k].w*1/2 && weapons[i][j].x <= bullets[k].x + bullets[k].w*1/2 && weapons[i][j].y <= bullets[k].y + bullets[k].h*1/2 && weapons[i][j].y >= bullets[k].y - bullets[k].h*1/2 ){
						weapons[i][j].hidden = true;
						weapons[i][j].x=-100;
						weapons[i][j].y=-100;
					}
				}*/
			}
		}

		if (lives <= 0) {
			game_change_scene(SCENE_LOSE);
		}
		// [HACKATHON 2-7]
		// TODO: Shoot if key is down and cool-down is over.
		// 1) Get the time now using 'al_get_time'.
		// 2) If Space key is down in 'key_state' and the time
		//    between now and last shoot is not less that cool
		//    down time.
		// 3) Loop through the bullet array and find one that is hidden.
		//    (This part can be optimized.)
		// 4) The bullet will be found if your array is large enough.
		// 5) Set the last shoot time to now.
		// 6) Set hidden to false (recycle the bullet) and set its x, y to the
		//    front part of your plane.
		// Uncomment and fill in the code below.
		int flag = 0;
		for (i = 0; i < MAX_ENEMY; i++) {
			if (enemies[i].hidden != true) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			game_change_scene(SCENE_BOSS);
		}

		double now = al_get_time();
		if (key_state[ALLEGRO_KEY_SPACE] && now - last_shoot_timestamp >= MAX_COOLDOWN) {
			for (i = 0; i < MAX_BULLET; i++) {
				if (bullets[i].hidden)
					break;
			}
			if (i == MAX_BULLET)
				return;
			last_shoot_timestamp = now;
			bullets[i].hidden = false;
			bullets[i].x = plane.x - 6;
			bullets[i].y = plane.y;
			al_stop_sample(&gun_bgm_id);
			if (!al_play_sample(gun_bgm, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &gun_bgm_id))
				game_abort("failed to play audio (bgm)");
		}

		double now_weapon1 = al_get_time();
		while (enemies[0].hidden != true && now_weapon1 - last_shoot_timestamp_weapon1 >= MAX_COOLDOWN_weapon) {
			for (j = 0; j < MAX_WEAPON; j++) {
				if (weapons[0][j].hidden)
					break;
			}
			if (j == MAX_WEAPON)
				return;
			last_shoot_timestamp_weapon1 = now_weapon1;
			weapons[0][j].hidden = false;
			weapons[0][j].x = enemies[0].x - 6;
			weapons[0][j].y = enemies[0].y;
		}
		double now_weapon2 = al_get_time();
		while (enemies[1].hidden != true && now_weapon2 - last_shoot_timestamp_weapon2 >= MAX_COOLDOWN_weapon) {
			for (j = 0; j < MAX_WEAPON; j++) {
				if (weapons[1][j].hidden)
					break;
			}
			if (j == MAX_WEAPON)
				return;
			last_shoot_timestamp_weapon2 = now_weapon2;
			weapons[1][j].hidden = false;
			weapons[1][j].x = enemies[1].x - 6;
			weapons[1][j].y = enemies[1].y;
		}
		double now_weapon3 = al_get_time();
		while (enemies[2].hidden != true && now_weapon3 - last_shoot_timestamp_weapon3 >= MAX_COOLDOWN_weapon) {
			for (j = 0; j < MAX_WEAPON; j++) {
				if (weapons[2][j].hidden)
					break;
			}
			if (j == MAX_WEAPON)
				return;
			last_shoot_timestamp_weapon3 = now_weapon3;
			weapons[2][j].hidden = false;
			weapons[2][j].x = enemies[2].x - 6;
			weapons[2][j].y = enemies[2].y;
		}
		double now_weapon4 = al_get_time();
		while (enemies[3].hidden != true && now_weapon3 - last_shoot_timestamp_weapon4 >= MAX_COOLDOWN_weapon) {
			for (j = 0; j < MAX_WEAPON; j++) {
				if (weapons[3][j].hidden)
					break;
			}
			if (j == MAX_WEAPON)
				return;
			last_shoot_timestamp_weapon4 = now_weapon4;
			weapons[3][j].hidden = false;
			weapons[3][j].x = enemies[3].x - 6;
			weapons[3][j].y = enemies[3].y;
		}
		double now_weapon5 = al_get_time();
		while (enemies[4].hidden != true && now_weapon5 - last_shoot_timestamp_weapon5 >= MAX_COOLDOWN_weapon) {
			for (j = 0; j < MAX_WEAPON; j++) {
				if (weapons[4][j].hidden)
					break;
			}
			if (j == MAX_WEAPON)
				return;
			last_shoot_timestamp_weapon5 = now_weapon5;
			weapons[4][j].hidden = false;
			weapons[4][j].x = enemies[4].x - 6;
			weapons[4][j].y = enemies[4].y;
		}

	}
	else if (active_scene == SCENE_MENU) {

		plane.x += plane.vx;
		// [HACKATHON 1-1]
		// TODO: Limit the plane's collision box inside the frame.
		//       (x, y axes can be separated.)
		// Uncomment and fill in the code below.
		if (plane.x - plane.w / 2 < 0) {
			plane.x = plane.w / 2;
			plane.vx = -plane.vx;
		}

		else if (plane.x + plane.w / 2 > 400) {
			plane.x = 400 - plane.w / 2;
			plane.vx = -plane.vx;
		}

	}
	else if (active_scene == SCENE_BOSS) {
		int i;
		plane.vx = plane.vy = 0;
		if (key_state[ALLEGRO_KEY_UP] || key_state[ALLEGRO_KEY_W])
			plane.vy -= 1;
		if (key_state[ALLEGRO_KEY_DOWN] || key_state[ALLEGRO_KEY_S])
			plane.vy += 1;
		if (key_state[ALLEGRO_KEY_LEFT] || key_state[ALLEGRO_KEY_A])
			plane.vx -= 1;
		if (key_state[ALLEGRO_KEY_RIGHT] || key_state[ALLEGRO_KEY_D])
			plane.vx += 1;
		// 0.71 is (1/sqrt(2)).
		plane.y += plane.vy * 4 * (plane.vx ? 0.71f : 1);
		plane.x += plane.vx * 4 * (plane.vy ? 0.71f : 1);
		// [HACKATHON 1-1]
		// TODO: Limit the plane's collision box inside the frame.
		//       (x, y axes can be separated.)
		// Uncomment and fill in the code below.
		if (plane.x - plane.w / 2 < 0)
			plane.x = plane.w / 2;
		else if (plane.x + plane.w / 2 > SCREEN_W)
			plane.x = SCREEN_W - plane.w / 2;
		if (plane.y - plane.h / 2 < 0)
			plane.y = plane.h / 2;
		else if (plane.y + plane.h / 2 > SCREEN_H)
			plane.y = SCREEN_H - plane.h / 2;

		bigboss.hidden = false;
		bigboss.x += bigboss.vx;
		bigboss.y += bigboss.vy;
		if (bigboss.x - 100 < 0) {
			bigboss.vx = -bigboss.vx;
			bigboss.x = 100;
		}
		else if (bigboss.x > SCREEN_W) {
			bigboss.vx = -bigboss.vx;
			bigboss.x = SCREEN_W;
		}
		if (bigboss.y > SCREEN_H)
			game_change_scene(SCENE_LOSE);

		for (i = 0; i < MAX_BULLTEBOSS; i++) {
			if (BULLETS[i].hidden)
				continue;
			BULLETS[i].x += BULLETS[i].vx;
			BULLETS[i].y += BULLETS[i].vy;
		}
		if (BULLETS[i].y < 0) {
			BULLETS[i].hidden = true;
		}


		for (i = 0; i < MAX_BOOM; i++) {

			booms[i].x += booms[i].vx;
			booms[i].y += booms[i].vy;
			if (booms[i].y > SCREEN_H)
				booms[i].hidden = true;
			if (bigboss.hidden)
				booms[i].hidden = true;
			if (booms[i].x >= plane.x - plane.w * 1 / 2 && booms[i].x <= plane.x + plane.w * 1 / 2 && booms[i].y <= plane.y + plane.h * 1 / 2 && booms[i].y >= plane.y - plane.h * 1 / 2) {
				booms[i].hidden = true;
				booms[i].x = -100;
				booms[i].y = -100;
				if (score > 20)
					score -= 20;
				game_change_scene(SCENE_LOSE);
			}
		}

		for (i = 0; i < MAX_BULLTEBOSS; i++) {
			if (bigboss.hidden == false && BULLETS[i].x >= bigboss.x - bigboss.w * 1 / 2 && BULLETS[i].x <= bigboss.x + bigboss.w * 1 / 4 && BULLETS[i].y <= bigboss.y + bigboss.h * 1 / 2 && BULLETS[i].y >= bigboss.y - bigboss.h * 1 / 2) {
				BULLETS[i].hidden = true;
				flag_bigboss += 1;
				if (flag_bigboss == 15) {
					bigboss.hidden = true;
					bigboss.x = -100;
					bigboss.y = -100;
				}
				BULLETS[i].x = -100;
				BULLETS[i].y = -100;
				score += 100;
				/*al_stop_sample(&gun_bgm_id);
				if (!al_play_sample(gun_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &gun_bgm_id))
					game_abort("failed to play audio (bgm)");*/
			}
		}
		double now_boss = al_get_time();
		if (key_state[ALLEGRO_KEY_SPACE] && now_boss - last_shoot_timestamp_boss >= MAX_COOLDOWN) {
			for (i = 0; i < MAX_BULLTEBOSS; i++) {
				if (BULLETS[i].hidden)
					break;
			}
			if (i == MAX_BULLTEBOSS)
				return;
			last_shoot_timestamp_boss = now_boss;
			BULLETS[i].hidden = false;
			BULLETS[i].x = plane.x - 6;
			BULLETS[i].y = plane.y;
		}

		double now_weapon6 = al_get_time();
		while (bigboss.hidden != true && now_weapon6 - last_shoot_timestamp_weapon6 >= MAX_COOLDOWN_boom) {
			for (i = 0; i < MAX_BOOM; i++) {
				if (booms[i].hidden)
					break;
			}
			if (i == MAX_BOOM)
				return;
			last_shoot_timestamp_weapon6 = now_weapon6;
			booms[i].hidden = false;
			booms[i].x = bigboss.x - 60;
			booms[i].y = bigboss.y;
		}
		if (flag_bigboss >= 15) {
			game_change_scene(SCENE_WIN);
		}
	}
}

void game_draw(void) {
	if (active_scene == SCENE_MENU) {
		al_draw_bitmap(main_img_background, 0, 0, 0);
		al_draw_text(font_chinese_40, al_map_rgb(88, 147, 212), 380, SCREEN_H - 50, 0, "Press enter key to start");
		// [HACKATHON 3-5]
		// TODO: Draw settings images.
		// The settings icon should be located at (x, y, w, h) =
		// (SCREEN_W - 48, 10, 38, 38).
		// Change its image according to your mouse position.
		// Uncomment and fill in the code below.
		draw_movable_object(plane);
		if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W - 48, 10, 38, 38))
			al_draw_bitmap(img_settings2, SCREEN_W - 48, 10, 0);
		else
			al_draw_bitmap(img_settings, SCREEN_W - 48, 10, 0);

		
		al_draw_text(font_chinese_40, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 110, 70, ALLEGRO_ALIGN_CENTER, "Shooting Game");
		al_draw_text(font_chinese_40, al_map_rgb(255, 255, 255), SCREEN_W / 2 - 100, 30, ALLEGRO_ALIGN_CENTER, "One Truth Prevails!");
		al_draw_text(font_chinese_40, al_map_rgb(180, 180, 180), 380, SCREEN_H - 50, 0, "Press enter key to start");

	}
	else if (active_scene == SCENE_START) {
		int i, j;
		al_draw_bitmap(start_img_background, 0, 0, 0);
		// [HACKATHON 2-8]
		// TODO: Draw all bullets in your bullet array.
		// Uncomment and fill in the code below.
		for (i = 0; i < MAX_BULLET; i++)
			draw_movable_object(bullets[i]);
		draw_movable_object(plane);
		for (i = 0; i < MAX_ENEMY; i++) {
			draw_movable_object(enemies[i]);
			if (flag_enemies[i] < MAX_ENEMY) {
				al_draw_rectangle(enemies[i].x - 50, enemies[i].y - 60, enemies[i].x + 50, enemies[i].y - 50, al_map_rgb(0, 0, 0), 3);
				al_draw_filled_rectangle(enemies[i].x - 50, enemies[i].y - 60, enemies[i].x + 50 - flag_enemies[i] * 100 / MAX_ENEMY, enemies[i].y - 50, al_map_rgb(255, 0, 0));
			}
			else if (flag_enemies[i] >= MAX_ENEMY)
			{
				continue;
			}
			else {
				al_draw_rectangle(enemies[i].x - 50, enemies[i].y - 60, enemies[i].x + 50, enemies[i].y - 50, al_map_rgb(0, 0, 0), 3);
				al_draw_filled_rectangle(enemies[i].x - 50, enemies[i].y - 60, enemies[i].x + 50, enemies[i].y - 50, al_map_rgb(255, 0, 0));
			}
			for (j = 0; j < MAX_WEAPON; j++)
				draw_movable_object(weapons[i][j]);
		}

		al_draw_textf(font_pirulen_24, al_map_rgb(0, 0, 0), SCREEN_W / 2, SCREEN_H - 50, ALLEGRO_ALIGN_CENTER, "lives:%d", lives);
		al_draw_textf(font_pirulen_24, al_map_rgb(0, 0, 0), SCREEN_W / 2, SCREEN_H - 30, ALLEGRO_ALIGN_CENTER, "score:%d", score);
		if (pnt_in_rect(mouse_x, mouse_y, 0, 0, 38, 38))
			al_draw_bitmap(img_settings2, 0, 0, 0);
		else
			al_draw_bitmap(img_settings, 0, 0, 0);
	}
	// [HACKATHON 3-9]
	// TODO: If active_scene is SCENE_SETTINGS.
	// Draw anything you want, or simply clear the display.
	else if (active_scene == SCENE_SETTINGS) {
		al_draw_bitmap(setting_img_background, 0, 0, 0);

		if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W / 2 - 50, 100, 200, 30)) {
			al_draw_filled_rectangle(SCREEN_W / 2 - 100, 100, SCREEN_W / 2 + 100, 100 + 30, al_map_rgb(0, 0, 0));
			al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W / 2, 100, ALLEGRO_ALIGN_CENTER, "BACK");
		}
		else {
			al_draw_filled_rectangle(SCREEN_W / 2 - 100, 100, SCREEN_W / 2 + 100, 100 + 30, al_map_rgb(255, 255, 255));
			al_draw_text(font_pirulen_24, al_map_rgb(0, 0, 0), SCREEN_W / 2, 100, ALLEGRO_ALIGN_CENTER, "BACK");
		}
		if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W / 2 - 100, 200, 200, 30)) {
			al_draw_filled_rectangle(SCREEN_W / 2 - 100, 200, SCREEN_W / 2 + 100, 200 + 30, al_map_rgb(0, 0, 0));
			al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W / 2, 200, ALLEGRO_ALIGN_CENTER, "CONTINUE");
		}
		else {
			al_draw_filled_rectangle(SCREEN_W / 2 - 100, 200, SCREEN_W / 2 + 100, 200 + 30, al_map_rgb(255, 255, 255));
			al_draw_text(font_pirulen_24, al_map_rgb(0, 0, 0), SCREEN_W / 2, 200, ALLEGRO_ALIGN_CENTER, "CONTINUE");
		}
		if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W / 2 - 100, 300, 200, 30)) {
			al_draw_filled_rectangle(SCREEN_W / 2 - 100, 300, SCREEN_W / 2 + 100, 300 + 30, al_map_rgb(0, 0, 0));
			al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W / 2, 300, ALLEGRO_ALIGN_CENTER, "HELP");
		}
		else {
			al_draw_filled_rectangle(SCREEN_W / 2 - 100, 300, SCREEN_W / 2 + 100, 300 + 30, al_map_rgb(255, 255, 255));
			al_draw_text(font_pirulen_24, al_map_rgb(0, 0, 0), SCREEN_W / 2, 300, ALLEGRO_ALIGN_CENTER, "HELP");
		}
		if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W / 2 - 100, 400, 200, 30)) {
			al_draw_filled_rectangle(SCREEN_W / 2 - 100, 400, SCREEN_W / 2 + 100, 400 + 30, al_map_rgb(0, 0, 0));
			al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W / 2, 400, ALLEGRO_ALIGN_CENTER, "CHARACTER");
		}
		else {
			al_draw_filled_rectangle(SCREEN_W / 2 - 100, 400, SCREEN_W / 2 + 100, 400 + 30, al_map_rgb(255, 255, 255));
			al_draw_text(font_pirulen_24, al_map_rgb(0, 0, 0), SCREEN_W / 2, 400, ALLEGRO_ALIGN_CENTER, "CHARACTER");
		}
	}
	else if (active_scene == SCENE_SETTINGS2) {
		al_draw_bitmap(setting_img_background, 0, 0, 0);
		//???????????????
		if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W / 2 - 50, 100, 200, 30)) {
			al_draw_filled_rectangle(SCREEN_W / 2 - 100, 100, SCREEN_W / 2 + 100, 100 + 30, al_map_rgb(0, 0, 0));
			al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W / 2, 100, ALLEGRO_ALIGN_CENTER, "RESUME");
		}
		else {
			al_draw_filled_rectangle(SCREEN_W / 2 - 100, 100, SCREEN_W / 2 + 100, 100 + 30, al_map_rgb(255, 255, 255));
			al_draw_text(font_pirulen_24, al_map_rgb(0, 0, 0), SCREEN_W / 2, 100, ALLEGRO_ALIGN_CENTER, "RESUME");
		}
		if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W / 2 - 100, 200, 200, 30)) {
			al_draw_filled_rectangle(SCREEN_W / 2 - 100, 200, SCREEN_W / 2 + 100, 200 + 30, al_map_rgb(0, 0, 0));
			al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W / 2, 200, ALLEGRO_ALIGN_CENTER, "RESTART");
		}
		else {
			al_draw_filled_rectangle(SCREEN_W / 2 - 100, 200, SCREEN_W / 2 + 100, 200 + 30, al_map_rgb(255, 255, 255));
			al_draw_text(font_pirulen_24, al_map_rgb(0, 0, 0), SCREEN_W / 2, 200, ALLEGRO_ALIGN_CENTER, "RESTART");
		}
		if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W / 2 - 100, 300, 200, 30)) {
			al_draw_filled_rectangle(SCREEN_W / 2 - 100, 300, SCREEN_W / 2 + 100, 300 + 30, al_map_rgb(0, 0, 0));
			al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W / 2, 300, ALLEGRO_ALIGN_CENTER, "HELP");
		}
		else {
			al_draw_filled_rectangle(SCREEN_W / 2 - 100, 300, SCREEN_W / 2 + 100, 300 + 30, al_map_rgb(255, 255, 255));
			al_draw_text(font_pirulen_24, al_map_rgb(0, 0, 0), SCREEN_W / 2, 300, ALLEGRO_ALIGN_CENTER, "HELP");
		}
		if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W / 2 - 100, 400, 200, 30)) {
			al_draw_filled_rectangle(SCREEN_W / 2 - 100, 400, SCREEN_W / 2 + 100, 400 + 30, al_map_rgb(0, 0, 0));
			al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W / 2, 400, ALLEGRO_ALIGN_CENTER, "MENU");
		}
		else {
			al_draw_filled_rectangle(SCREEN_W / 2 - 100, 400, SCREEN_W / 2 + 100, 400 + 30, al_map_rgb(255, 255, 255));
			al_draw_text(font_pirulen_24, al_map_rgb(0, 0, 0), SCREEN_W / 2, 400, ALLEGRO_ALIGN_CENTER, "MENU");
		}
	}
	else if (active_scene == SCENE_BOSS) {
		int i;
		al_draw_bitmap(boss_img_background, 0, 0, 0);
		al_draw_textf(font_pirulen_24, al_map_rgb(0, 0, 0), SCREEN_W / 2, SCREEN_H - 30, ALLEGRO_ALIGN_CENTER, "score:%d", score);
		draw_movable_object(plane);
		draw_movable_object(bigboss);
		for (i = 0; i < MAX_BOOM; i++) {
			draw_movable_object(booms[i]);
		}
		for (i = 0; i < MAX_BULLTEBOSS; i++)
			draw_movable_object(BULLETS[i]);

		if (bigboss.hidden == false && flag_bigboss < 15) {
			al_draw_rectangle(bigboss.x - 100, bigboss.y - 90, bigboss.x, bigboss.y - 80, al_map_rgb(0, 0, 0), 3);
			al_draw_filled_rectangle(bigboss.x - 100, bigboss.y - 90, bigboss.x - flag_bigboss * 100 / 15, bigboss.y - 80, al_map_rgb(255, 0, 0));
		}
		else if (flag_bigboss == 0 && bigboss.hidden == false) {
			al_draw_rectangle(bigboss.x - 100, bigboss.y - 90, bigboss.x, bigboss.y - 80, al_map_rgb(0, 0, 0), 3);
			al_draw_filled_rectangle(bigboss.x - 100, bigboss.y - 90, bigboss.x, bigboss.y - 80, al_map_rgb(255, 0, 0));
		}
	}
	else if (active_scene == SCENE_WIN) {
		int i;
		al_clear_to_color(al_map_rgb(255, 255, 255));
		al_draw_text(font_pirulen_32, al_map_rgb(94, 134, 193), SCREEN_W / 2, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_CENTER, "YOU WIN!");
		al_draw_text(font_pirulen_32, al_map_rgb(94, 134, 193), SCREEN_W / 2, SCREEN_H / 2 - 50, ALLEGRO_ALIGN_CENTER, "YOUR SCORE:");
		al_draw_textf(font_pirulen_32, al_map_rgb(94, 134, 193), SCREEN_W / 2, SCREEN_H / 2 + 50, ALLEGRO_ALIGN_CENTER, "%d", score);
		if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W / 2 - 100, 500, 200, 30)) {
			al_draw_text(font_pirulen_24, al_map_rgb(0, 0, 0), SCREEN_W / 2, 500, ALLEGRO_ALIGN_CENTER, "RESTART");
			al_draw_rectangle(SCREEN_W / 2 - 100, 500, SCREEN_W / 2 + 100, 500 + 30, al_map_rgb(0, 0, 0), 3);
		}
		else {
			al_draw_text(font_pirulen_24, al_map_rgb(94, 134, 193), SCREEN_W / 2, 500, ALLEGRO_ALIGN_CENTER, "RESTART");
			al_draw_rectangle(SCREEN_W / 2 - 100, 500, SCREEN_W / 2 + 100, 500 + 30, al_map_rgb(94, 134, 193), 3);
		}
		for (i = 0; i < 10; i++) {
			al_clear_to_color(al_map_rgb(255, 255, 255));
			al_draw_text(font_pirulen_32, al_map_rgb(94 - i * 9.4, 134 - i * 13.4, 193 - i * 19.3), SCREEN_W / 2, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_CENTER, "YOU WIN!");
			al_draw_text(font_pirulen_32, al_map_rgb(94, 134, 193), SCREEN_W / 2, SCREEN_H / 2 - 50, ALLEGRO_ALIGN_CENTER, "YOUR SCORE:");
			al_draw_textf(font_pirulen_32, al_map_rgb(94, 134, 193), SCREEN_W / 2, SCREEN_H / 2 + 50, ALLEGRO_ALIGN_CENTER, "%d", score);
			if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W / 2 - 100, 500, 200, 30)) {
				al_draw_text(font_pirulen_24, al_map_rgb(0, 0, 0), SCREEN_W / 2, 500, ALLEGRO_ALIGN_CENTER, "RESTART");
				al_draw_rectangle(SCREEN_W / 2 - 100, 500, SCREEN_W / 2 + 100, 500 + 30, al_map_rgb(0, 0, 0), 3);
			}
			else {
				al_draw_text(font_pirulen_24, al_map_rgb(94, 134, 193), SCREEN_W / 2, 500, ALLEGRO_ALIGN_CENTER, "RESTART");
				al_draw_rectangle(SCREEN_W / 2 - 100, 500, SCREEN_W / 2 + 100, 500 + 30, al_map_rgb(94, 134, 193), 3);
			}
			al_flip_display();
		}
		for (i = 0; i < 10; i++) {
			al_clear_to_color(al_map_rgb(255, 255, 255));
			al_draw_text(font_pirulen_32, al_map_rgb(0 + i * 9.4, 0 + i * 13.4, 0 + i * 19.3), SCREEN_W / 2, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_CENTER, "YOU WIN!");
			al_draw_text(font_pirulen_32, al_map_rgb(94, 134, 193), SCREEN_W / 2, SCREEN_H / 2 - 50, ALLEGRO_ALIGN_CENTER, "YOUR SCORE:");
			al_draw_textf(font_pirulen_32, al_map_rgb(94, 134, 193), SCREEN_W / 2, SCREEN_H / 2 + 50, ALLEGRO_ALIGN_CENTER, "%d", score);
			if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W / 2 - 100, 500, 200, 30)) {
				al_draw_text(font_pirulen_24, al_map_rgb(0, 0, 0), SCREEN_W / 2, 500, ALLEGRO_ALIGN_CENTER, "RESTART");
				al_draw_rectangle(SCREEN_W / 2 - 100, 500, SCREEN_W / 2 + 100, 500 + 30, al_map_rgb(0, 0, 0), 3);
			}
			else {
				al_draw_text(font_pirulen_24, al_map_rgb(94, 134, 193), SCREEN_W / 2, 500, ALLEGRO_ALIGN_CENTER, "RESTART");
				al_draw_rectangle(SCREEN_W / 2 - 100, 500, SCREEN_W / 2 + 100, 500 + 30, al_map_rgb(94, 134, 193), 3);
			}
			al_flip_display();
		}
	}
	else if (active_scene == SCENE_LOSE) {
		int i;
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_CENTER, "YOU LOSE!");
		al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 - 50, ALLEGRO_ALIGN_CENTER, "YOUR SCORE:");
		al_draw_textf(font_pirulen_32, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 + 50, ALLEGRO_ALIGN_CENTER, "%d", score);
		if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W / 2 - 100, 500, 200, 30)) {
			al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W / 2, 500, ALLEGRO_ALIGN_CENTER, "RESTART");
			al_draw_rectangle(SCREEN_W / 2 - 100, 500, SCREEN_W / 2 + 100, 500 + 30, al_map_rgb(255, 255, 255), 3);
		}
		else {
			al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W / 2, 500, ALLEGRO_ALIGN_CENTER, "RESTART");
			al_draw_rectangle(SCREEN_W / 2 - 100, 500, SCREEN_W / 2 + 100, 500 + 30, al_map_rgb(255, 255, 255), 3);
		}
		for (i = 0; i < 10; i++) {
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_text(font_pirulen_32, al_map_rgb(155 - i * 24, 155 + i * 12, 155 + i * 12), SCREEN_W / 2, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_CENTER, "YOU LOSE!");
			al_draw_text(font_pirulen_32, al_map_rgb(155, 155, 155), SCREEN_W / 2, SCREEN_H / 2 - 50, ALLEGRO_ALIGN_CENTER, "YOUR SCORE:");
			al_draw_textf(font_pirulen_32, al_map_rgb(155, 155, 155), SCREEN_W / 2, SCREEN_H / 2 + 50, ALLEGRO_ALIGN_CENTER, "%d", score);

			if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W / 2 - 100, 500, 200, 30)) {
				al_draw_text(font_pirulen_24, al_map_rgb(155, 155, 155), SCREEN_W / 2, 500, ALLEGRO_ALIGN_CENTER, "RESTART");
				al_draw_rectangle(SCREEN_W / 2 - 100, 500, SCREEN_W / 2 + 100, 500 + 30, al_map_rgb(255, 255, 255), 3);
			}
			else {
				al_draw_text(font_pirulen_24, al_map_rgb(240, 120, 120), SCREEN_W / 2, 500, ALLEGRO_ALIGN_CENTER, "RESTART");
				al_draw_rectangle(SCREEN_W / 2 - 100, 500, SCREEN_W / 2 + 100, 500 + 30, al_map_rgb(240, 120, 120), 3);
			}
			al_flip_display();
			al_rest(0.005);
		}
		for (i = 0; i < 10; i++) {
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_text(font_pirulen_32, al_map_rgb(155 + i * 24, 155 - i * 12, 155 - i * 12), SCREEN_W / 2, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_CENTER, "YOU LOSE!");
			al_draw_text(font_pirulen_32, al_map_rgb(155, 155,155), SCREEN_W / 2, SCREEN_H / 2 - 50, ALLEGRO_ALIGN_CENTER, "YOUR SCORE:");
			al_draw_textf(font_pirulen_32, al_map_rgb(155, 155, 155), SCREEN_W / 2, SCREEN_H / 2 + 50, ALLEGRO_ALIGN_CENTER, "%d", score);
			if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W / 2 - 100, 500, 200, 30)) {
				al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W / 2, 500, ALLEGRO_ALIGN_CENTER, "RESTART");
				al_draw_rectangle(SCREEN_W / 2 - 100, 500, SCREEN_W / 2 + 100, 500 + 30, al_map_rgb(255, 255, 255), 3);
			}
			else {
				al_draw_text(font_pirulen_24, al_map_rgb(240, 120, 120), SCREEN_W / 2, 500, ALLEGRO_ALIGN_CENTER, "RESTART");
				al_draw_rectangle(SCREEN_W / 2 - 100, 500, SCREEN_W / 2 + 100, 500 + 30, al_map_rgb(240, 120, 120), 3);
			}
			al_flip_display();
			al_rest(0.005);
		}
	}
	else if (active_scene == SCENE_HELP) {
		al_draw_bitmap(setting_img_background, 0, 0, 0);
		al_draw_bitmap(rule_img_background, 0 + 20, 0 + 50, 0);
		if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W / 2 - 100, 500, 200, 30)) {
			al_draw_filled_rectangle(SCREEN_W / 2 - 100, 500, SCREEN_W / 2 + 100, 500 + 30, al_map_rgb(255, 255, 255));
			al_draw_text(font_pirulen_24, al_map_rgb(0, 0, 0), SCREEN_W / 2, 500, ALLEGRO_ALIGN_CENTER, "BACK");
		}
		else {
			al_draw_filled_rectangle(SCREEN_W / 2 - 100, 500, SCREEN_W / 2 + 100, 500 + 30, al_map_rgb(0, 0, 0));
			al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W / 2, 500, ALLEGRO_ALIGN_CENTER, "BACK");
		}
	}
	else if (active_scene == SCENE_HELP2) {
		al_draw_bitmap(setting_img_background, 0, 0, 0);
		al_draw_bitmap(rule_img_background, 0 + 20, 0 + 50, 0);
		if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W / 2 - 100, 500, 200, 30)) {
			al_draw_filled_rectangle(SCREEN_W / 2 - 100, 500, SCREEN_W / 2 + 100, 500 + 30, al_map_rgb(255, 255, 255));
			al_draw_text(font_pirulen_24, al_map_rgb(0, 0, 0), SCREEN_W / 2, 500, ALLEGRO_ALIGN_CENTER, "BACK");
		}
		else {
			al_draw_filled_rectangle(SCREEN_W / 2 - 100, 500, SCREEN_W / 2 + 100, 500 + 30, al_map_rgb(0, 0, 0));
			al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), SCREEN_W / 2, 500, ALLEGRO_ALIGN_CENTER, "BACK");
		}
	}
	else if (active_scene == SCENE_CHARACTER) {
		al_draw_bitmap(character_img_background, 0, 0, 0);

		if (pnt_in_rect(mouse_x, mouse_y, 700 - 85, 50, 170, 30)) {
			al_draw_filled_rectangle(700 - 85, 50, 700 + 85, 50 + 30, al_map_rgb(255, 255, 255));
			al_draw_text(font_pirulen_24, al_map_rgb(0, 0, 0), 700, 50, ALLEGRO_ALIGN_CENTER, "BACK");
		}
		else {
			al_draw_filled_rectangle(700 - 85, 50, 700 + 85, 50 + 30, al_map_rgb(0, 0, 0));
			al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 700, 50, ALLEGRO_ALIGN_CENTER, "BACK");
		}
		if (pnt_in_rect(mouse_x, mouse_y, 150 - 85, 500, 170, 30)) {
			al_draw_filled_rectangle(150 - 85, 500, 150 + 85, 530, al_map_rgb(255, 255, 255));
			al_draw_text(font_pirulen_24, al_map_rgb(0, 0, 0), 150, 500, ALLEGRO_ALIGN_CENTER, "1.");
		}
		else {
			al_draw_filled_rectangle(150 - 85, 500, 150 + 85, 530, al_map_rgb(0, 0, 0));
			al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 150, 500, ALLEGRO_ALIGN_CENTER, "1.");
		}
		if (pnt_in_rect(mouse_x, mouse_y, 400 - 85, 500, 170, 30)) {
			al_draw_filled_rectangle(400 - 85, 500, 400 + 85, 530, al_map_rgb(255, 255, 255));
			al_draw_text(font_pirulen_24, al_map_rgb(0, 0, 0), 400, 500, ALLEGRO_ALIGN_CENTER, "2.");
		}
		else {
			al_draw_filled_rectangle(400 - 85, 500, 400 + 85, 530, al_map_rgb(0, 0, 0));
			al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 400, 500, ALLEGRO_ALIGN_CENTER, "2.");
		}
		if (pnt_in_rect(mouse_x, mouse_y, 645 - 85, 500, 170, 30)) {
			al_draw_filled_rectangle(645 - 85, 500, 645 + 85, 530, al_map_rgb(255, 255, 255));
			al_draw_text(font_pirulen_24, al_map_rgb(0, 0, 0), 645, 500, ALLEGRO_ALIGN_CENTER, "3.");
		}
		else {
			al_draw_filled_rectangle(645 - 85, 500, 645 + 85, 530, al_map_rgb(0, 0, 0));
			al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 645, 500, ALLEGRO_ALIGN_CENTER, "3.");
		}

	}

	al_flip_display();
}

void game_destroy(void) {
	// Destroy everything you have created.
	// Free the memories allocated by malloc or allegro functions.
	// Destroy shared resources.
	al_destroy_font(font_pirulen_32);
	al_destroy_font(font_pirulen_24);
	al_destroy_font(font_chinese_40);

	/* Menu Scene resources*/
	al_destroy_bitmap(main_img_background);
	al_destroy_sample(main_bgm);
	// [HACKATHON 3-6]
	// TODO: Destroy the 2 settings images.
	// Uncomment and fill in the code below.
	al_destroy_bitmap(img_settings);
	al_destroy_bitmap(img_settings2);

	/* Start Scene resources*/
	al_destroy_bitmap(start_img_background);
	al_destroy_bitmap(start_img_plane);
	al_destroy_bitmap(start_img_plane2);
	al_destroy_bitmap(start_img_plane3);
	al_destroy_bitmap(start_img_bigboss);
	al_destroy_bitmap(start_img_enemy);
	al_destroy_sample(start_bgm);
	al_destroy_sample(win_bgm);
	al_destroy_sample(lose_bgm);
	al_destroy_sample(boss_bgm);
	al_destroy_sample(gun_bgm);
	al_destroy_sample(setting_bgm);
	al_destroy_bitmap(boss_img_background);
	al_destroy_bitmap(character_img_background);
	al_destroy_bitmap(rule_img_background);
	al_destroy_bitmap(setting_img_background);
	// [HACKATHON 2-9]
	// TODO: Destroy your bullet image.
	// Uncomment and fill in the code below.
	al_destroy_bitmap(img_bullet);
	al_destroy_bitmap(img_weapon);
	al_destroy_bitmap(img_boom);
	al_destroy_bitmap(img_BULLET);

	al_destroy_timer(game_update_timer);
	al_destroy_event_queue(game_event_queue);
	al_destroy_display(game_display);
	free(mouse_state);
}

void game_change_scene(int next_scene) {
	game_log("Change scene from %d to %d", active_scene, next_scene);
	// TODO: Destroy resources initialized when creating scene.
	if (active_scene == SCENE_MENU) {
		al_stop_sample(&main_bgm_id);
		al_stop_sample(&win_bgm_id);
		al_stop_sample(&lose_bgm_id);
		al_stop_sample(&boss_bgm_id);
		al_stop_sample(&setting_bgm_id);
		game_log("stop audio (bgm)");
	}
	else if (active_scene == SCENE_START) {
		al_stop_sample(&start_bgm_id);
		al_stop_sample(&win_bgm_id);
		al_stop_sample(&lose_bgm_id);
		al_stop_sample(&boss_bgm_id);
		al_stop_sample(&setting_bgm_id);
		game_log("stop audio (bgm)");
	}
	else if (active_scene == SCENE_WIN) {
		al_stop_sample(&start_bgm_id);
		al_stop_sample(&main_bgm_id);
		al_stop_sample(&lose_bgm_id);
		al_stop_sample(&win_bgm_id);
		al_stop_sample(&boss_bgm_id);
		al_stop_sample(&setting_bgm_id);
		game_log("stop audio (bgm)");
	}
	else if (active_scene == SCENE_LOSE) {
		al_stop_sample(&start_bgm_id);
		al_stop_sample(&win_bgm_id);
		al_stop_sample(&main_bgm_id);
		al_stop_sample(&lose_bgm_id);
		al_stop_sample(&boss_bgm_id);
		al_stop_sample(&setting_bgm_id);
		game_log("stop audio (bgm)");
	}
	else if (active_scene == SCENE_BOSS) {
		al_stop_sample(&start_bgm_id);
		al_stop_sample(&win_bgm_id);
		al_stop_sample(&main_bgm_id);
		al_stop_sample(&lose_bgm_id);
		al_stop_sample(&boss_bgm_id);
		al_stop_sample(&setting_bgm_id);
		game_log("stop audio (bgm)");
	}
	else if (active_scene == SCENE_SETTINGS) {
		al_stop_sample(&start_bgm_id);
		al_stop_sample(&win_bgm_id);
		al_stop_sample(&main_bgm_id);
		al_stop_sample(&lose_bgm_id);
		al_stop_sample(&boss_bgm_id);
		al_stop_sample(&setting_bgm_id);
		game_log("stop audio (bgm)");
	}
	else if (active_scene == SCENE_SETTINGS2) {
		al_stop_sample(&start_bgm_id);
		al_stop_sample(&win_bgm_id);
		al_stop_sample(&main_bgm_id);
		al_stop_sample(&lose_bgm_id);
		al_stop_sample(&boss_bgm_id);
		al_stop_sample(&setting_bgm_id);
		game_log("stop audio (bgm)");
	}
	else if (active_scene == SCENE_HELP) {
		al_stop_sample(&start_bgm_id);
		al_stop_sample(&win_bgm_id);
		al_stop_sample(&main_bgm_id);
		al_stop_sample(&lose_bgm_id);
		al_stop_sample(&boss_bgm_id);
		al_stop_sample(&setting_bgm_id);
		game_log("stop audio (bgm)");
	}
	else if (active_scene == SCENE_HELP2) {
		al_stop_sample(&start_bgm_id);
		al_stop_sample(&win_bgm_id);
		al_stop_sample(&main_bgm_id);
		al_stop_sample(&lose_bgm_id);
		al_stop_sample(&boss_bgm_id);
		al_stop_sample(&setting_bgm_id);
		game_log("stop audio (bgm)");
	}
	else if (active_scene == SCENE_CHARACTER) {
		al_stop_sample(&start_bgm_id);
		al_stop_sample(&win_bgm_id);
		al_stop_sample(&main_bgm_id);
		al_stop_sample(&lose_bgm_id);
		al_stop_sample(&boss_bgm_id);
		al_stop_sample(&setting_bgm_id);
		game_log("stop audio (bgm)");
	}
	active_scene = next_scene;
	// TODO: Allocate resources before entering scene.
	if (active_scene == SCENE_MENU) {
		if (!al_play_sample(main_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &main_bgm_id))
			game_abort("failed to play audio (bgm)");
		if (flag_CHARAC == 1) {
			plane.img = start_img_plane2;
			plane.w = 50;
			plane.h = 128;
		}
		else if (flag_CHARAC == 3) {
			plane.img = start_img_plane3;
			plane.w = 45;
			plane.h = 134;
		}

		else {
			plane.img = start_img_plane;
			plane.w = 87;
			plane.h = 66;
		}
		plane.x = 0;
		plane.y = 500;
		plane.w = 76;
		plane.h = 89;
		plane.vx = 3;
		plane.vy = 0;
		plane.hidden = false;
	}
	else if (active_scene == SCENE_START) {
		int i, j;
		if (flag_RE == 0) {
			lives = 5;
			score = 0;
			for (i = 0; i < MAX_ENEMY; i++) {
				flag_enemies[i] = 0;
				for (j = 0; j < MAX_WEAPON; j++) {
					weapons[i][j].x = -100;
					weapons[i][j].y = -100;
				}
			}
			flag_bigboss = 0;
		}
		if (flag_CHARAC == 1) {
			plane.img = start_img_plane2;
			plane.w = 50;
			plane.h = 128;
		}
		else if (flag_CHARAC == 3) {
			plane.img = start_img_plane3;
			plane.w = 45;
			plane.h = 134;
		}

		else {
			plane.img = start_img_plane;
			plane.w = 87;
			plane.h = 66;
		}
		plane.x = 400;
		plane.y = 500;
		plane.w = 76;
		plane.h = 89;

		enemies[-1].vy = 1.5;
		enemies[-1].vx = 1;
		for (i = 0; i < MAX_ENEMY; i++) {
			enemies[i].w = 87;
			enemies[i].h = 66;
			enemies[i].x = enemies[i].w / 2 + 10 + (float)rand() / RAND_MAX * (SCREEN_W - enemies[i].w - 20);
			enemies[i].y = 80;
			enemies[i].hidden = false;
			enemies[i].vy = enemies[i - 1].vy - 0.25;
			enemies[i].vx = enemies[i - 1].vx + 0.25;
			enemies[i].img = start_img_enemy;
		}
		// [HACKATHON 2-5-2]
		// TODO: Initialize bullets.
		// For each bullets in array, set their w and h to the size of
		// the image, and set their img to bullet image, hidden to true,
		// (vx, vy) to (0, -3).
		// Uncomment and fill in the code below.
		for (i = 0; i < MAX_BULLET; i++) {
			bullets[i].w = 26;
			bullets[i].h = 35;
			bullets[i].img = img_bullet;
			bullets[i].vx = 0;
			bullets[i].vy = -6;
			bullets[i].hidden = true;
		}
		for (i = 0; i < MAX_ENEMY; i++) {
			for (j = 0; j < MAX_WEAPON; j++) {
				weapons[i][j].w = 20;
				weapons[i][j].h = 48;
				weapons[i][j].img = img_weapon;
				weapons[i][j].vx = 0;
				weapons[i][j].vy = 3;
				weapons[i][j].hidden = true;
			}
		}

		if (!al_play_sample(start_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &start_bgm_id))
			game_abort("failed to play audio (bgm)");
	}
	else if (active_scene == SCENE_BOSS) {
		int i;
		if (flag_CHARAC == 1)
			plane.img = start_img_plane2;
		else if (flag_CHARAC == 3)
			plane.img = start_img_plane3;
		else
			plane.img = start_img_plane;
		plane.x = 400;
		plane.y = 500;
		plane.w = 76;
		plane.h = 89;

		for (i = 0; i < MAX_BULLTEBOSS; i++) {
			BULLETS[i].w = 42;
			BULLETS[i].h = 49;
			BULLETS[i].img = img_BULLET;
			BULLETS[i].vx = 0;
			BULLETS[i].vy = -6;
			BULLETS[i].hidden = true;
		}

		bigboss.w = 199;
		bigboss.h = 154;
		bigboss.img = start_img_bigboss;
		bigboss.y = 125;
		bigboss.x = bigboss.x + bigboss.w / 2 + 10 + (float)rand() / RAND_MAX * (SCREEN_W - bigboss.x - bigboss.w - 20);
		bigboss.vx = 5;
		bigboss.vy = 0.5;
		bigboss.hidden = true;

		for (i = 0; i < MAX_BOOM; i++) {
			booms[i].w = 20;
			booms[i].h = 48;
			booms[i].img = img_boom;
			booms[i].vx = 0;
			booms[i].vy = 3;
			booms[i].hidden = true;
		}

		if (!al_play_sample(boss_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &boss_bgm_id))
			game_abort("failed to play audio (bgm)");
	}
	else if (active_scene == SCENE_WIN) {
		if (!al_play_sample(win_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &win_bgm_id))
			game_abort("failed to play audio (bgm)");
	}
	else if (active_scene == SCENE_LOSE) {
		if (!al_play_sample(lose_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &lose_bgm_id))
			game_abort("failed to play audio (bgm)");
	}
	else if (active_scene == SCENE_SETTINGS) {
		if (!al_play_sample(setting_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &setting_bgm_id))
			game_abort("failed to play audio (bgm)");
	}
	else if (active_scene == SCENE_SETTINGS2) {
		if (!al_play_sample(setting_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &setting_bgm_id))
			game_abort("failed to play audio (bgm)");
	}
	else if (active_scene == SCENE_HELP) {
		if (!al_play_sample(setting_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &setting_bgm_id))
			game_abort("failed to play audio (bgm)");
	}
	else if (active_scene == SCENE_HELP2) {
		if (!al_play_sample(setting_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &setting_bgm_id))
			game_abort("failed to play audio (bgm)");
	}
	else if (active_scene == SCENE_CHARACTER) {
		if (!al_play_sample(setting_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &setting_bgm_id))
			game_abort("failed to play audio (bgm)");
	}
}

void on_key_down(int keycode) {
	if (active_scene == SCENE_MENU) {
		if (keycode == ALLEGRO_KEY_ENTER)
			game_change_scene(SCENE_START);
	}
}

void on_mouse_down(int btn, int x, int y) {
	// [HACKATHON 3-8]
	// TODO: When settings clicked, switch to settings scene.
	// Uncomment and fill in the code below.
	if (active_scene == SCENE_MENU) {
		if (btn == 1) {
			if (pnt_in_rect(x, y, SCREEN_W - 48, 10, 38, 38))
				game_change_scene(SCENE_SETTINGS);
		}
	}
	if (active_scene == SCENE_START) {
		if (btn == 1) {
			if (pnt_in_rect(x, y, 0, 0, 38, 38))
				game_change_scene(SCENE_SETTINGS2);
		}
	}
	if (active_scene == SCENE_SETTINGS) {
		if (btn == 1) {
			if (pnt_in_rect(x, y, SCREEN_W / 2 - 100, 100, 200, 30))
				game_change_scene(SCENE_MENU);
			if (pnt_in_rect(x, y, SCREEN_W / 2 - 100, 200, 200, 30)) {
				flag_RE = 1;
				game_change_scene(SCENE_START);
			}

			if (pnt_in_rect(x, y, SCREEN_W / 2 - 100, 300, 200, 30))
				game_change_scene(SCENE_HELP);
			if (pnt_in_rect(x, y, SCREEN_W / 2 - 100, 400, 200, 30))
				game_change_scene(SCENE_CHARACTER);
		}
	}
	if (active_scene == SCENE_SETTINGS2) {
		if (btn == 1) {
			if (pnt_in_rect(x, y, SCREEN_W / 2 - 100, 100, 200, 30)) {
				flag_RE = 1;
				game_change_scene(SCENE_START);
			}

			if (pnt_in_rect(x, y, SCREEN_W / 2 - 100, 200, 200, 30)) {
				flag_RE = 0;
				flag_CHARAC = 2;
				game_change_scene(SCENE_START);
			}

			if (pnt_in_rect(x, y, SCREEN_W / 2 - 100, 300, 200, 30))
				game_change_scene(SCENE_HELP2);
			if (pnt_in_rect(x, y, SCREEN_W / 2 - 100, 400, 200, 30))
				game_change_scene(SCENE_MENU);
		}
	}
	if (active_scene == SCENE_HELP) {
		if (btn == 1) {
			if (pnt_in_rect(x, y, SCREEN_W / 2 - 100, 500, 200, 30))
				game_change_scene(SCENE_MENU);
		}
	}
	if (active_scene == SCENE_HELP2) {
		if (btn == 1) {
			if (pnt_in_rect(x, y, SCREEN_W / 2 - 100, 500, 200, 30))
				game_change_scene(SCENE_START);
		}
	}
	if (active_scene == SCENE_CHARACTER) {
		if (btn == 1) {
			if (pnt_in_rect(x, y, 700 - 85, 50, 170, 30)) {
				game_change_scene(SCENE_MENU);
			}
			if (pnt_in_rect(mouse_x, mouse_y, 150 - 85, 500, 170, 30)) {
				flag_CHARAC = 1;
				game_change_scene(SCENE_MENU);
			}
			if (pnt_in_rect(mouse_x, mouse_y, 400 - 85, 500, 170, 30)) {
				flag_CHARAC = 2;
				game_change_scene(SCENE_MENU);
			}
			if (pnt_in_rect(mouse_x, mouse_y, 645 - 85, 500, 170, 30)) {
				flag_CHARAC = 3;
				game_change_scene(SCENE_MENU);
			}
		}
	}

	if (active_scene == SCENE_WIN) {
		if (btn == 1) {
			if (pnt_in_rect(x, y, SCREEN_W / 2 - 100, 500, 200, 30)) {
				flag_RE = 0;
				flag_CHARAC = 2;
				game_change_scene(SCENE_MENU);
			}
		}
	}
	if (active_scene == SCENE_LOSE) {
		if (btn == 1) {
			if (pnt_in_rect(x, y, SCREEN_W / 2 - 100, 500, 200, 30)) {
				flag_RE = 0;
				flag_CHARAC = 2;
				game_change_scene(SCENE_MENU);
			}
		}
	}
}

void draw_movable_object(MovableObject obj) {
	if (obj.hidden)
		return;
	al_draw_bitmap(obj.img, round(obj.x - obj.w / 2), round(obj.y - obj.h / 2), 0);
}

ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h) {
	ALLEGRO_BITMAP* loaded_bmp = al_load_bitmap(filename);
	if (!loaded_bmp)
		game_abort("failed to load image: %s", filename);
	ALLEGRO_BITMAP *resized_bmp = al_create_bitmap(w, h);
	ALLEGRO_BITMAP *prev_target = al_get_target_bitmap();

	if (!resized_bmp)
		game_abort("failed to create bitmap when creating resized image: %s", filename);
	al_set_target_bitmap(resized_bmp);
	al_draw_scaled_bitmap(loaded_bmp, 0, 0,
		al_get_bitmap_width(loaded_bmp),
		al_get_bitmap_height(loaded_bmp),
		0, 0, w, h, 0);
	al_set_target_bitmap(prev_target);
	al_destroy_bitmap(loaded_bmp);

	game_log("resized image: %s", filename);

	return resized_bmp;
}

// [HACKATHON 3-3]
// TODO: Define bool pnt_in_rect(int px, int py, int x, int y, int w, int h)
// Uncomment and fill in the code below.
bool pnt_in_rect(int px, int py, int x, int y, int w, int h) {
	if (px - x <= w && px - x >= 0 && py - y <= h && py - y >= 0)
		return true;
	else
		return false;
}


// +=================================================================+
// | Code below is for debugging purpose, it's fine to remove it.    |
// | Deleting the code below and removing all calls to the functions |
// | doesn't affect the game.                                        |
// +=================================================================+

void game_abort(const char* format, ...) {
	va_list arg;
	va_start(arg, format);
	game_vlog(format, arg);
	va_end(arg);
	fprintf(stderr, "error occured, exiting after 2 secs");
	// Wait 2 secs before exiting.
	al_rest(2);
	// Force exit program.
	exit(1);
}

void game_log(const char* format, ...) {
#ifdef LOG_ENABLED
	va_list arg;
	va_start(arg, format);
	game_vlog(format, arg);
	va_end(arg);
#endif
}

void game_vlog(const char* format, va_list arg) {
#ifdef LOG_ENABLED
	static bool clear_file = true;
	vprintf(format, arg);
	printf("\n");
	// Write log to file for later debugging.
	FILE* pFile = fopen("log.txt", clear_file ? "w" : "a");
	if (pFile) {
		vfprintf(pFile, format, arg);
		fprintf(pFile, "\n");
		fclose(pFile);
	}
	clear_file = false;
#endif
}