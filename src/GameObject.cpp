#include "GameObject.h"
#include <algorithm>

void GameObject::render() {
	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->render();
	}
}

void GameObject::update(float dt) {
	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->update(dt);
	}
}

void GameObject::addChild(GameObject* obj) {
	//make this object the parent of the child
	obj->parent = this;
	//add to list
	this->children.push_back(obj);
}

void GameObject::removeChild(GameObject* obj) {
	//moves obj to end of vector
	std::vector<GameObject*>::iterator i = std::remove(this->children.begin(),
		this->children.end(), obj);
	//actually removes it
	this->children.erase(i, this->children.end());
	//deletes the memory associated with obj
	delete (obj);
}

Matrix44 GameObject::getGlobalMatrix() {
	if (this->parent)
		return this->parent->getGlobalMatrix() * this->model;
	else
		return this->model;
}

void GameObject::clear() {

}