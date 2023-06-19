#include "GameObjectPlayer.h"
#include "game.h"

void GameObjectPlayer::update(float dt) {
	Game* g = Game::instance;

	float speed = dt * 1000;

	if(g->keystate[SDL_SCANCODE_LSHIFT]) speed *= 10; //move faster with left shift
	if (g->keystate[SDL_SCANCODE_W] || g->keystate[SDL_SCANCODE_UP]) this->model.rotateLocal(dt * pitch_speed, Vector3(1, 0, 0));
	if (g->keystate[SDL_SCANCODE_S] || g->keystate[SDL_SCANCODE_DOWN]) this->model.rotateLocal(-dt * pitch_speed, Vector3(1, 0, 0));
	if (g->keystate[SDL_SCANCODE_A] || g->keystate[SDL_SCANCODE_LEFT]) this->model.rotateLocal(-dt * roll_speed, Vector3(0, 0, 1));
	if (g->keystate[SDL_SCANCODE_D] || g->keystate[SDL_SCANCODE_RIGHT]) this->model.rotateLocal(dt * roll_speed, Vector3(0, 0, 1));
	if (g->keystate[SDL_SCANCODE_SPACE]) { 
		g->bulletManager->createBullet(this->getGlobalMatrix(), 50, 10, 2, this); 
		BASS_ChannelPlay(g->gunChannel, true);
	}
	if (g->keystate[SDL_SCANCODE_B]) { 
		g->bulletManager->createBullet(this->getGlobalMatrix(), 0, 50, 10, this); 
		BASS_ChannelPlay(g->gunChannel, true);
	}

	Vector3 player_pos = Vector3(this->model.m[12], this->model.m[13], this->model.m[14]);
	Vector3 player_up = Vector3(this->model.m[4], this->model.m[5], this->model.m[6]);
	g->scene->camera->lookAt(g->scene->camera->eye.lerp((player_pos + this->model.frontVector() * 10 + player_up * 5), 0.1f), 
		player_pos - this->model.frontVector() * 10, 
		player_up);

	Vector3 f = this->model.frontVector().normalize();
	this->model.traslate(-f.x * dt * speed, -f.y * dt * speed, -f.z * dt * speed);

	GameObjectMesh::update(dt);
}