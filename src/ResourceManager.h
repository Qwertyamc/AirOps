#pragma once
#include <unordered_map>
#include "mesh.h"
#include "texture.h"
#include "shader.h"

using namespace std;

class ResourceManager {
private:
	unordered_map<string, Mesh*> m_meshes;
	unordered_map<string, Texture*> m_textures;
	unordered_map<string, Shader*> m_shaders;

public:
	ResourceManager();
	~ResourceManager();

	//load functions
	void loadMesh(string, string);
	void loadMesh(string, Mesh*);
	void loadTexture(string, string);
	void loadShaders(string, string, string);

	//unload functions

	//getters
	Mesh* getMesh(string);
	Texture* getTexture(string);
	Shader* getShader(string);
};