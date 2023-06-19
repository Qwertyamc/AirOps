#pragma once
#include <string>
#include "framework.h"

using namespace std;

class GameObject {
public:
	string name;
	string type;
	Matrix44 model;
	bool is_collider;

	GameObject* parent = nullptr;
	std::vector<GameObject*> children;

	virtual void render();
	virtual void update(float);
	void addChild(GameObject* obj);
	void removeChild(GameObject* obj);
	void clear();

	Matrix44 getGlobalMatrix();
};