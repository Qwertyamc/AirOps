#pragma once
#include "includes.h"
#include "GameObject.h"
#include "camera.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"

class HUD {
public:
	HUD(int sw, int sh);
	~HUD();
	
	Camera* camera;
	Shader* shader;
	Shader* point_shader;

	//num1
	Mesh* num1_mesh;

	//Num2
	Mesh* num2_mesh;

	Texture* num_texture;

	//minimap
	Mesh* minimap_mesh;
	Mesh* points_mesh;
	Texture* minimap_tex;

	void render();
	void renderPoint();
	void update(float, vector<Vector3>* vertices);
};