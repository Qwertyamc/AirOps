#include "GameObjectEnvironment.h"
#include "game.h"

void GameObjectEnvironment::render() {
	Game* g = Game::instance;

	//create our mvp
	Matrix44 mvp = this->getGlobalMatrix() * g->scene->camera->viewprojection_matrix;

	Matrix44 inverse_m = this->getGlobalMatrix();
	inverse_m.inverse();
	Matrix44 normal_m = inverse_m.transpose();

	Vector3 light_dir(100.0f, 100.0f, 100.0f);

	if (!shader) return;

	//enable shader and set uniforms
	shader->enable();
	shader->setMatrix44("u_model", this->getGlobalMatrix());
	shader->setMatrix44("u_mvp", mvp);
	shader->setMatrix44("u_normal_matrix", normal_m);
	shader->setTexture("u_texture", texture);

	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	glDisable(GL_DEPTH_TEST);
	//glCullFace(GL_FRONT);

	mesh->render(GL_TRIANGLES, shader);

	glEnable(GL_DEPTH_TEST);
	//glCullFace(GL_BACK);

	//disable the shader
	shader->disable();

	GameObject::render();
}

void GameObjectEnvironment::update(float dt) {
	Game* g = Game::instance;
	model.setTranslation(g->scene->camera->eye.x, g->scene->camera->eye.y, g->scene->camera->eye.z);

	GameObject::update(dt);
}