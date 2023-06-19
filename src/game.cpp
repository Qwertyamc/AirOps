#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "rendertotexture.h"
#include "shader.h"
#include <time.h>

#include <cmath>
#include "GameObjectMesh.h"
#include "GameObjectEnemy.h"
#include "GameObjectPlayer.h"
#include "GameObjectEnvironment.h"

#define PI 3.14159265

RenderToTexture* rt = NULL;

Game* Game::instance = NULL;

Game::Game(SDL_Window* window)
{
	this->window = window;
	instance = this;

	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl
	SDL_GetWindowSize( window, &window_width, &window_height );
	std::cout << " * Window size: " << window_width << " x " << window_height << std::endl;

	keystate = NULL;
	mouse_locked = false;

	BASS_Init(1, 44100, 0, 0, NULL);
}

//Here we have already GL working, so we can create meshes and textures
void Game::init(void)
{
	renderMenu(resourceManager->getTexture("initGame"));
    std::cout << " * Path: " << getPath() << std::endl;
    
	srand(time(NULL));

    //SDL_SetWindowSize(window, 50,50);

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	scene = new Scene();

	//create our camera
	scene->camera = new Camera();
	scene->camera->lookAt(Vector3(0,25,25),Vector3(0,0,0), Vector3(0,1,0)); //position the camera and point to 0,0,0
	scene->camera->setPerspective(70.0f,(float)window_width/(float)window_height,0.01f,10000.0f); //set the projection, we want to be perspective

	//load meshes
	resourceManager->loadMesh("x3_fighter", "data/assets/x3_fighter.ASE");
	resourceManager->loadMesh("box", "data/assets/box.ASE");
	resourceManager->loadMesh("sphere", "data/assets/sphere.ASE");
	resourceManager->loadMesh("cielo", "data/assets/cielo.ASE");
	resourceManager->loadMesh("terrain", "data/assets/terrain.ASE");
	resourceManager->loadMesh("p38", "data/assets/p38.ASE");
	resourceManager->loadMesh("spitfire", "data/assets/spitfire.ASE");
	resourceManager->loadMesh("wildcat", "data/assets/wildcat.ASE");
	resourceManager->loadMesh("bullet", "data/assets/bullet.ASE");

	resourceManager->loadTexture("terrain", "data/assets/terrain.tga");
	resourceManager->loadTexture("x3_fighter", "data/assets/x3_fighter.tga");
	resourceManager->loadTexture("p38", "data/assets/p38.tga");
	resourceManager->loadTexture("spitfire", "data/assets/spitfire.tga");
	resourceManager->loadTexture("wildcat", "data/assets/wildcat.tga");
	resourceManager->loadTexture("environment", "data/assets/milkyway.tga");
	resourceManager->loadTexture("environment2", "data/assets/cielo.tga");

	resourceManager->loadShaders("phong", "data/shaders/simple.vert", "data/shaders/simple.frag");
	resourceManager->loadShaders("environment", "data/shaders/simple.vert", "data/shaders/environment.frag");
	resourceManager->loadShaders("terrain", "data/shaders/simple.vert", "data/shaders/terrain.frag");
	resourceManager->loadShaders("bullet", "data/shaders/simple.vert", "data/shaders/bullet.frag");
	resourceManager->loadShaders("hud", "data/shaders/hud.vert", "data/shaders/menu.frag");
	resourceManager->loadShaders("minimap", "data/shaders/hud.vert", "data/shaders/minimap.frag");

	hud = new HUD(window_width, window_height);

	bulletManager = new BulletManager(resourceManager->getMesh("bullet"), resourceManager->getShader("bullet"), 200);

	GameObjectEnvironment* environment = new GameObjectEnvironment();
	environment->name = "environment";
	environment->type = "skybox";
	environment->mesh = resourceManager->getMesh("cielo");
	environment->texture = resourceManager->getTexture("environment2");
	environment->shader = resourceManager->getShader("environment");
	environment->is_collider = false;
	environment->update(0);
	scene->root->addChild(environment);

	for (int x = -10; x <= 10; x++){
		for (int y = -10; y <= 10; y++){
			float distance = 5000;
			GameObjectMesh* new_object = new GameObjectMesh();
			new_object->name = "terrain";
			new_object->type = "obstacle";
			new_object->model.traslate(distance *x, -100, distance *y);
			new_object->mesh = resourceManager->getMesh("terrain");
			new_object->texture = resourceManager->getTexture("terrain");
			new_object->shader = resourceManager->getShader("environment");
			new_object->is_collider = true;
			new_object->setCollisionModel();

			scene->root->addChild(new_object);
		}
	}

	GameObjectMesh* player_object = new GameObjectPlayer();
	player_object->name = "player";
	player_object->type = "player";
	player_object->angle = 0.1;
	player_object->model.setTranslation(0, 0, 0);
	player_object->mesh = resourceManager->getMesh("wildcat");
	player_object->texture = resourceManager->getTexture("wildcat");
	player_object->shader = resourceManager->getShader("phong");
	player_object->is_collider = true;
	player_object->setCollisionModel();

	scene->root->addChild(player_object);

	for (int i = 0; i < 50; i++) {
		GameObjectEnemy* new_object = new GameObjectEnemy();
		new_object->name = string("p38 - ") + to_string(i);
		new_object->type = "enemy";
		new_object->life = 50;
		new_object->init_life = 50;
		new_object->angle = 0.1;
		new_object->player = player_object;
		new_object->model.traslate(rand() % 5000 -2500, rand() % 500 + 1, rand() % 5000 - 2500);
		new_object->mesh = resourceManager->getMesh("p38");
		new_object->texture = resourceManager->getTexture("p38");
		new_object->shader = resourceManager->getShader("phong");
		new_object->is_collider = true;
		new_object->setCollisionModel();

		cout << new_object->name << endl;

		scene->root->addChild(new_object);
	}

	for (int i = 0; i < 10; i++) {
		GameObjectEnemy* new_object = new GameObjectEnemy();
		new_object->name = string("x-fighter - ") + to_string(i);
		new_object->type = "enemy";
		new_object->life = 250;
		new_object->init_life = 250;
		new_object->angle = 0.1;
		new_object->player = player_object;
		new_object->model.scale(3, 3, 3);
		new_object->model.traslate(rand() % 5000 - 2500, rand() % 500 + 1, rand() % 5000 - 2500);
		new_object->mesh = resourceManager->getMesh("x3_fighter");
		new_object->texture = resourceManager->getTexture("x3_fighter");
		new_object->shader = resourceManager->getShader("phong");
		new_object->is_collider = true;
		new_object->setCollisionModel();

		cout << new_object->name << endl;

		scene->root->addChild(new_object);
	}

	scene->getColliders(&scene->cols, scene->root);

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse

	BASS_ChannelStop(musicChannel);
	gamemusic = BASS_SampleLoad(false, "data/music/game.mp3", 0, 0, 3, BASS_SAMPLE_LOOP);
	musicChannel = BASS_SampleGetChannel(gamemusic, false);
	BASS_ChannelPlay(musicChannel, true);

	gunsound = BASS_SampleLoad(false, "data/music/gun.mp3", 0, 0, 3, 0);
	gunChannel = BASS_SampleGetChannel(gunsound, false);
	explosionsound = BASS_SampleLoad(false, "data/music/explosion.mp3", 0, 0, 3, 0);
	explosionChannel = BASS_SampleGetChannel(explosionsound, false);

	gameState = GAMEPLAY;
}

//what to do when the image has to be draw
void Game::render(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//set the clear color (the background color)
	glClearColor(0.345, 0.3785, 0.251, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//update view and projection matrices, and push gl matrices for fixed pipeline rendering
	scene->camera->set();

	//*** Drawing code starts here ***
   
	scene->render();

	hud->render();

	hud->renderPoint();

	bulletManager->render();
	
	//*** Drawing code ends here ***

	//disable blending to draw text on top
    glDisable( GL_BLEND );

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	double speed = seconds_elapsed * 100; //the speed is defined by the seconds_elapsed so it goes constant

	scene->update((float) seconds_elapsed);

	vector<Vector3>* vertices = new vector<Vector3>();
	for each (GameObjectMesh * goa in scene->cols) {
		if (goa->name == "player") {
			Vector3 P = Vector3(goa->model.m[12], goa->model.m[13], goa->model.m[14]);
			Vector3 F = goa->model.frontVector()*-1;
			F.y = 0;
			for each (GameObjectMesh * gob in scene->cols) {
				if (gob->type == "enemy") {
					Vector3 E = Vector3(gob->model.m[12], gob->model.m[13], gob->model.m[14]);
					Vector3 PE = E - P;
					PE.y = 0;

					float ϑ = acos((F.dot(PE)) / (F.length() * PE.length()));
					float enemy_dist = PE.length();

					Vector3 xp = F.cross(PE);
					if (xp.y > 0) ϑ = -ϑ;

					float max_distance = 1000;
					float radius = (90 / max_distance) * enemy_dist;

					if (radius > 90) {
						radius = 90;
					}

					Vector2 C = Vector2(window_width - 128, 128);

					vertices->push_back(Vector3(C.x + radius * sin(ϑ), C.y + radius * cos(ϑ), 1));
				}
			}
		}
	}

	hud->update((float)seconds_elapsed, vertices);

	bulletManager->update((float)seconds_elapsed);

	for each (GameObjectMesh* goa in scene->cols){
		if (goa->name == "player") {
			for each (GameObjectMesh * gob in scene->cols) {
				//update collision model matrix for each mesh
				goa->collision_model->setTransform(goa->model.m);
				gob->collision_model->setTransform(gob->model.m);
				//test
				bool collision = goa->collision_model->collision(gob->collision_model);
				if (collision && goa->name != gob->name) {
					cout << goa->name << " collisioned with " << gob->name << endl;
					gameState = GAMELOST;
				}
			}
		}
	}
	bulletManager->checkCollisions(&scene->cols);
}

//Keyboard event handler (sync input)
void Game::onKeyPressed( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: 
			switch (gameState) {
				case GAMEPLAY:
					gameState = PAUSEPLAY;
					break;
				case GAMELOST:
					exit(0);
					break;
			}
			break;
		case SDLK_RETURN:
			switch (gameState) {
				case MENUPLAY:
					gameState = GAMEINIT;
					break;
				case MENUEXIT:
					exit(0);
					break;
				case PAUSEPLAY:
					gameState = GAMEPLAY;
					break;
				case PAUSEEXIT:
					exit(0);
					break;
			}
			break;
		case SDLK_UP:
			switch (gameState) {
				case MENUPLAY:
					gameState = MENUEXIT;
					break;
				case MENUEXIT:
					gameState = MENUPLAY;
					break;
				case PAUSEPLAY:
					gameState = PAUSEEXIT;
					break;
				case PAUSEEXIT:
					gameState = PAUSEPLAY;
					break;
			}
			break;
		case SDLK_DOWN:
			switch (gameState) {
				case MENUPLAY:
					gameState = MENUEXIT;
					break;
				case MENUEXIT:
					gameState = MENUPLAY;
					break;
				case PAUSEPLAY:
					gameState = PAUSEEXIT;
					break;
				case PAUSEEXIT:
					gameState = PAUSEPLAY;
					break;
			}
			break;
	}
}


void Game::onMouseButton( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::setWindowSize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
    
	/*
    Uint32 flags = SDL_GetWindowFlags(window);
    if(flags & SDL_WINDOW_ALLOW_HIGHDPI)
    {
        width *= 2;
        height *= 2;
    }
	*/

	glViewport( 0,0, width, height );
	scene->camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

void Game::loadSplash() {
	menumusic = BASS_SampleLoad(false, "data/music/menu.mp3", 0, 0, 3, BASS_SAMPLE_LOOP);
	musicChannel = BASS_SampleGetChannel(menumusic, false);
	BASS_ChannelPlay(musicChannel, true);

	resourceManager = new ResourceManager();

	Mesh* menuScreen = new Mesh();
	menuScreen->vertices.resize(4);
	menuScreen->uvs.resize(4);
	menuScreen->vertices[0] = Vector3(-1, -1, 0); menuScreen->uvs[0] = Vector2(0, 1);
	menuScreen->vertices[1] = Vector3(1, -1, 0); menuScreen->uvs[1] = Vector2(1, 1);
	menuScreen->vertices[2] = Vector3(1, 1, 0); menuScreen->uvs[2] = Vector2(1, 0);
	menuScreen->vertices[3] = Vector3(-1, 1, 0); menuScreen->uvs[3] = Vector2(0, 0);
	menuScreen->indices.resize(6);
	menuScreen->indices[0] = 0; menuScreen->indices[1] = 1; menuScreen->indices[2] = 2;
	menuScreen->indices[3] = 0; menuScreen->indices[4] = 2; menuScreen->indices[5] = 3;
	menuScreen->genBuffers();

	resourceManager->loadMesh("menuScreen", menuScreen);

	resourceManager->loadTexture("splashScreen", "data/assets/splashScreen.tga");
	resourceManager->loadTexture("menuPlay", "data/assets/menuPlay.tga");
	resourceManager->loadTexture("menuExit", "data/assets/menuExit.tga");
	resourceManager->loadTexture("initGame", "data/assets/initGame.tga");
	resourceManager->loadTexture("pausePlay", "data/assets/pausePlay.tga");
	resourceManager->loadTexture("pauseExit", "data/assets/pauseExit.tga");
	resourceManager->loadTexture("gameLost", "data/assets/gameLost.tga");

	resourceManager->loadShaders("menu", "data/shaders/menu.vert", "data/shaders/menu.frag");

	//render splash
	renderMenu(resourceManager->getTexture("splashScreen"));

	Sleep(2000);
	gameState = MENUPLAY;
}

void Game::menuPlay() {
	renderMenu(resourceManager->getTexture("menuPlay"));
}

void Game::menuExit() {
	renderMenu(resourceManager->getTexture("menuExit"));
}

void Game::pausePlay() {
	renderMenu(resourceManager->getTexture("pausePlay"));
}

void Game::pauseExit() {
	renderMenu(resourceManager->getTexture("pauseExit"));
}

void Game::gameLost() {
	renderMenu(resourceManager->getTexture("gameLost"));
}

void Game::renderMenu(Texture* texture) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Mesh* mesh = resourceManager->getMesh("menuScreen");
	Shader* shader = resourceManager->getShader("menu");

	if (!shader) return;

	//enable shader and set uniforms
	shader->enable();
	shader->setTexture("u_texture", texture);

	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	mesh->render(GL_TRIANGLES, shader);

	//disable the shader
	shader->disable();

	glDisable(GL_BLEND);
	SDL_GL_SwapWindow(this->window);
}