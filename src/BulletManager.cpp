#include "BulletManager.h"
#include "game.h"
#include "GameObjectEnemy.h"

BulletManager::BulletManager(Mesh* theMesh, Shader* theShader, int max_bullets) {
	this->bullet_mesh = theMesh;
	this->shader = theShader;
	this->bullets_vector.resize(max_bullets);
}

void BulletManager::render() {
	Game* g = Game::instance;

	if (!shader) return;

	//enable shader and set uniforms
	shader->enable();
	for each (Bullet* bullet in bullets_vector){
		if (bullet != NULL && bullet->valid) {
			Matrix44 mvp = bullet->model * g->scene->camera->viewprojection_matrix;

			Matrix44 inverse_m = bullet->model;
			inverse_m.inverse();
			Matrix44 normal_m = inverse_m.transpose();

			shader->setMatrix44("u_model", bullet->model);
			shader->setMatrix44("u_mvp", mvp);
			shader->setMatrix44("u_normal_matrix", normal_m);
			bullet_mesh->render(GL_TRIANGLES, shader);
		}
	}

	shader->disable();
}

void BulletManager::update(float elapsed_time) {
	for each (Bullet* bullet in bullets_vector){
		if (bullet != NULL && bullet->valid) {
			bullet->old_model = bullet->model;
			bullet->model.traslate(bullet->velocity.x, bullet->velocity.y, bullet->velocity.z);
			bullet->ttl -= elapsed_time;
			if (bullet->ttl < 0) {
				bullet->valid = false;
			}
		}
	}
}

void BulletManager::createBullet(Matrix44 model, float vel, float power, float ttl, GameObject* owner) {
	Bullet* b = new Bullet;

	b->model = model;
	b->velocity = owner->model.frontVector() * -vel;
	b->power = power;
	b->ttl = ttl;
	b->owner = owner;

	for (int i = 0; i < bullets_vector.size(); i++){
		if (bullets_vector[i] == NULL || !bullets_vector[i]->valid) {
			bullets_vector[i] = b;
 			break;
		}
	}
}

void BulletManager::checkCollisions(std::vector<GameObject*>* colliders) {
	Game* g = Game::instance;

	for each (Bullet * bullet in bullets_vector) {
		if (bullet != NULL && bullet->valid) {
			Vector3 old_pos = Vector3(bullet->old_model.m[12], bullet->old_model.m[13], bullet->old_model.m[14]);
			Vector3 new_pos = Vector3(bullet->model.m[12], bullet->model.m[13], bullet->model.m[14]);

			for each (GameObject* go in *colliders){
				if (go->name != "player" && go->name != "terrain") {
					GameObjectMesh* gom = dynamic_cast<GameObjectMesh*>(go);

					gom->collision_model->setTransform(gom->model.m);

					if (gom->collision_model->rayCollision(old_pos.v, new_pos.v, false, 0, 1)) {
						bullet->valid = false;

						Vector3 collisionPoint;
						gom->collision_model->getCollisionPoint(collisionPoint.v, true);

						GameObjectEnemy* goe = dynamic_cast<GameObjectEnemy*>(gom);
						if (goe->onCollideBullet(bullet->power)) {
							g->points++;
							if (goe->name.rfind("x3", 0) == 0) g->points++;
							cout << g->points << endl;
						 }

						std::cout << "Bullet collided with " << gom->name << endl;
						continue;
					}
				}
			}
		}
	}
}