#pragma once
#include "GameObjectMesh.h"

class GameObjectEnemy : public GameObjectMesh {
public:
	int current_waypoint = 0;
	float life;
	float init_life;
	GameObject* player;

	void update(float);
	bool onCollideBullet(float damage);
};