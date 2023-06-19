#pragma once
#include "GameObject.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"

class GameObjectEnvironment : public GameObject {
public:
	Mesh* mesh;
	Texture* texture;
	Shader* shader;

	void render();
	void update(float);
};