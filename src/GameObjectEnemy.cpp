#include "GameObjectEnemy.h"
#include <time.h>
#include "game.h"

void GameObjectEnemy::update(float dt) {
	float speed = 10;

	Vector3 wp = Vector3(this->player->model.m[12], this->player->model.m[13], this->player->model.m[14]);

	Vector3 f = this->model.frontVector().normalize();
	Vector3 invf = f * -1;
	Vector3 pos = Vector3(this->model.m[12], this->model.m[13], this->model.m[14]);
	Vector3 to_target = (wp - pos).normalize();
	Vector3 distance = (wp - pos);

	float cos_angle = to_target.dot(invf);

	if (cos_angle < 0.9999f) {
		Vector3 axis = to_target.cross(invf);
		Matrix44 inv = this->model;
		inv.inverse();
		Vector3 rot_axis = inv.rotateVector(axis);
		this->model.rotateLocal(0.5 * dt, rot_axis);

	}
	else {
		this->model.setFrontAndOrthonormalize(to_target*-1);
	}
	
	this->model.traslate(-f.x * dt * speed, -f.y * dt * speed, -f.z * dt * speed);
	
	if (sqrt(pow(distance.x, 2) + pow(distance.y, 2) + pow(distance.z, 2)) < 1.0f) {
		cout << "Arrived. Next checkpoint: " << wp.x << ", " << wp.y << ", " << wp.z << ", " << endl;
	}

	GameObjectMesh::update(dt);
}

bool GameObjectEnemy::onCollideBullet(float damage) {
	life -= damage;
	if (life <= 0) {
		Game* g = Game::instance;
		BASS_ChannelPlay(g->explosionChannel, true);
		life = init_life;
		srand(time(NULL));
		model.traslate(rand() % 2000 + 1, rand() % 500 + 1, rand() % 2000 + 1);
		return true;
	} else {
		return false;
	}
}