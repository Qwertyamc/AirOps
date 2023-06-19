#pragma once
#include "includes.h"
#include "GameObject.h";
#include "camera.h"

class Scene {
public:
	Scene();
	~Scene();
	//scene graph starts from here
	GameObject* root;
	Camera* camera;

	std::vector<GameObject*> cols;

	void getColliders(std::vector<GameObject*>* colliders, GameObject* current);

	void render();
	void update(float);
};