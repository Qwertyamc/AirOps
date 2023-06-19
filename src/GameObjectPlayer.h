#pragma once
#include "GameObjectMesh.h"

class GameObjectPlayer : public GameObjectMesh {
public:
	float pitch_speed = 1;
	float roll_speed = 1;

	void update(float);
};