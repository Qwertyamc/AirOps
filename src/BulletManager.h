#pragma once
#include "GameObjectMesh.h"
#include "mesh.h"
#include "shader.h"

struct Bullet {
	bool valid;
	Matrix44 model;
	Matrix44 old_model;
	Vector3 velocity;
	float ttl;
	float power;
	GameObject* owner;
};

class BulletManager {
public:
	Mesh* bullet_mesh;
	std::vector<Bullet*>bullets_vector;
	Shader* shader;
	bool can_fire = true;

	BulletManager(Mesh* theMesh, Shader* theShader, int max_bullets);

	void render();
	void update(float elapsed_time);
	void createBullet(Matrix44 model, float vel, float power, float ttl, GameObject* owner);
	void checkCollisions(std::vector<GameObject*>* colliders);
};