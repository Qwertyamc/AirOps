#pragma once
#include "GameObject.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "coldet/coldet.h"

class GameObjectMesh : public GameObject {
public:
	float angle;
	Mesh* mesh;
	Texture* texture;
	Shader* shader;

	CollisionModel3D* collision_model;

	void render();
	void update(float);

	void setCollisionModel();
};