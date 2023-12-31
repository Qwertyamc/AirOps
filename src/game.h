/*  by Alun Evans 2016 LaSalle (alunthomasevasevans@gmail.com)
	based on framework by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "bass/c/bass.h"
#include "Scene.h"
#include "HUD.h"
#include "ResourceManager.h"
#include "BulletManager.h"

enum GameState {
	SPLASHSCREEN,
	MENUPLAY,
	MENUEXIT,
	GAMEINIT,
	GAMEPLAY,
	GAMELOST,
	PAUSEPLAY,
	PAUSEEXIT
};

class Game
{
public:
	static Game* instance;

	//window
	SDL_Window* window;
	int window_width;
	int window_height;
    
	float g_time;
	float angle;
	GameState gameState = SPLASHSCREEN;

	//keyboard state
	const Uint8* keystate;

	//mouse state
	int mouse_state; //tells which buttons are pressed
	Vector2 mouse_position; //last mouse position
	Vector2 mouse_delta; //mouse movement in the last frame
	bool mouse_locked; //tells if the mouse is locked (not seen)
	
	Scene* scene;
	HUD* hud;
	ResourceManager* resourceManager;
	BulletManager* bulletManager;

	int points = 0;

	HSAMPLE menumusic;
	HSAMPLE gamemusic;
	HSAMPLE gunsound;
	HSAMPLE explosionsound;
	HCHANNEL musicChannel;
	HCHANNEL gunChannel;
	HCHANNEL explosionChannel;

	Game(SDL_Window* window);
	void loadSplash();
	void init( void );
	void render( void );
	void renderMenu(Texture*);
	void menuPlay();
	void menuExit();
	void pausePlay();
	void pauseExit();
	void gameLost();
	void update( double dt );

	void onKeyPressed( SDL_KeyboardEvent event );
	void onMouseButton( SDL_MouseButtonEvent event );
    void onResize( SDL_Event e );
    
	void setWindowSize(int width, int height);
};


#endif 