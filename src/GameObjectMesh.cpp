#include "GameObjectMesh.h"
#include "game.h"

void GameObjectMesh::render() {
	Game* g = Game::instance;
	float radius = 0;

	radius = 2500;

	if (g->scene->camera->testSphereInFrustum(Vector3(this->model.m[12], this->model.m[13], this->model.m[14]), radius)) {

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
		shader->setUniform3("u_cam_pos", g->scene->camera->eye);
		shader->setUniform3("u_light_dir", light_dir);
		shader->setTexture("u_texture", texture);

		//call render function for mesh, passing shader as param
		//mesh->render() binds the relevant attributes, and drawing code
		mesh->render(GL_TRIANGLES, shader);

		//disable the shader
		shader->disable();
	}

	GameObject::render();
}

void GameObjectMesh::update(float dt) {
	GameObject::update(dt);
}

void GameObjectMesh::setCollisionModel() {
	collision_model = newCollisionModel3D();
	collision_model->setTriangleNumber(mesh->indices.size() / 3);

	for (int i = 0; i < mesh->indices.size()/3; i++){
		Vector3 v1 = mesh->vertices[mesh->indices[i * 3]];
		Vector3 v2 = mesh->vertices[mesh->indices[i * 3 + 1]];
		Vector3 v3 = mesh->vertices[mesh->indices[i * 3 + 2]];	

		collision_model->addTriangle(v1.x, v1.y, v1.z, 
			v2.x, v2.y, v2.z, 
			v3.x, v3.y, v3.z);
	}

	collision_model->finalize();
}