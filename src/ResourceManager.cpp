#include "ResourceManager.h"

//constructor
ResourceManager::ResourceManager() {

}

//destructor
ResourceManager::~ResourceManager() {

}

void ResourceManager::loadMesh(string name, string path) {
	Mesh* mesh = new Mesh();
	mesh->loadASE(path.c_str());
	m_meshes[name] = mesh;
}

void ResourceManager::loadMesh(string name, Mesh* mesh) {
	m_meshes[name] = mesh;
}

void ResourceManager::loadTexture(string name, string path) {
	Texture* texture = new Texture();
	texture->load(path.c_str());
	m_textures[name] = texture;
}

void ResourceManager::loadShaders(string name, string vert, string frag) {
	Shader* shader = new Shader();

	if (!shader->load(vert, frag))
	{
		std::cout << "shader not found or error" << std::endl;
		//if your program quits straight after opening, the shader probably hasn't compiled,
		//if you put a breakpoint here you'll be able to read the compilation error in the console
		exit(0);
	}
	m_shaders[name] = shader;
}

Mesh* ResourceManager::getMesh(string name) {
	return m_meshes[name];
}

Texture* ResourceManager::getTexture(string name) {
	return m_textures[name];
}

Shader* ResourceManager::getShader(string name) {
	return m_shaders[name];
}