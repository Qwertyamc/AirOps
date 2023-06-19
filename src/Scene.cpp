#include "Scene.h"

Scene::Scene() {
	this->root = new GameObject();
}

Scene::~Scene() {
	this->root->clear();
	delete this->root;
}

void Scene::getColliders(std::vector<GameObject*>* colliders, GameObject* current) {
	if (current->is_collider) {
		colliders->push_back(current);
	}

	for each (GameObject* child in current->children){
		getColliders(colliders, child);
	}
}

void Scene::render() {
	this->root->render();
}
void Scene::update(float dt) {
	this->root->update(dt);
}
