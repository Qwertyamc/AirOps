#include "HUD.h"
#include "game.h"
#include <math.h>

HUD::HUD(int sw, int sh) {
	Game* g = Game::instance;

	camera = new Camera();
	camera->setOrthographic(0, sw, 0, sh, 0.1, 2);
	camera->updateViewMatrix();

	minimap_mesh = new Mesh();
	minimap_mesh->vertices.resize(4);
	minimap_mesh->uvs.resize(4);
	minimap_mesh->vertices[0] = Vector3(sw-256, 0, 1); minimap_mesh->uvs[0] = Vector2(0, 1);
	minimap_mesh->vertices[1] = Vector3(sw, 0, 1); minimap_mesh->uvs[1] = Vector2(1, 1);
	minimap_mesh->vertices[2] = Vector3(sw, 256, 1); minimap_mesh->uvs[2] = Vector2(1, 0);
	minimap_mesh->vertices[3] = Vector3(sw-256, 256, 1); minimap_mesh->uvs[3] = Vector2(0, 0);
	minimap_mesh->indices.resize(6);
	minimap_mesh->indices[0] = 0; minimap_mesh->indices[1] = 1; minimap_mesh->indices[2] = 2;
	minimap_mesh->indices[3] = 0; minimap_mesh->indices[4] = 2; minimap_mesh->indices[5] = 3;
	minimap_mesh->genBuffers();

	g->resourceManager->loadMesh("minimap", minimap_mesh);
	g->resourceManager->loadTexture("minimap", "data/assets/minimap.tga");

	minimap_tex = g->resourceManager->getTexture("minimap");

	shader = g->resourceManager->getShader("hud");
	point_shader = g->resourceManager->getShader("minimap");

	points_mesh = new Mesh();

	num1_mesh = new Mesh();
	num1_mesh->vertices.resize(4);
	num1_mesh->uvs.resize(4);
	num1_mesh->vertices[0] = Vector3(0, sh-64, 1);
	num1_mesh->vertices[1] = Vector3(64, sh - 64, 1);
	num1_mesh->vertices[2] = Vector3(64, sh, 1);
	num1_mesh->vertices[3] = Vector3(0, sh, 1);
	num1_mesh->indices.resize(6);
	num1_mesh->indices[0] = 0; num1_mesh->indices[1] = 1; num1_mesh->indices[2] = 2;
	num1_mesh->indices[3] = 0; num1_mesh->indices[4] = 2; num1_mesh->indices[5] = 3;
	num1_mesh->genBuffers();

	num2_mesh = new Mesh();
	num2_mesh->vertices.resize(4);
	num2_mesh->uvs.resize(4);
	num2_mesh->vertices[0] = Vector3(64, sh - 64, 1);
	num2_mesh->vertices[1] = Vector3(128, sh - 64, 1);
	num2_mesh->vertices[2] = Vector3(128, sh, 1);
	num2_mesh->vertices[3] = Vector3(64, sh, 1);
	num2_mesh->indices.resize(6);
	num2_mesh->indices[0] = 0; num2_mesh->indices[1] = 1; num2_mesh->indices[2] = 2;
	num2_mesh->indices[3] = 0; num2_mesh->indices[4] = 2; num2_mesh->indices[5] = 3;
	num2_mesh->genBuffers();

	g->resourceManager->loadMesh("num1", num1_mesh);
	g->resourceManager->loadMesh("num2", num2_mesh);
	g->resourceManager->loadTexture("numbers", "data/assets/numbers.tga");

	num_texture = g->resourceManager->getTexture("numbers");
}

void HUD::render() {
	Game* g = Game::instance;

	int dec = floor(g->points / 10);
	int unit = g->points % 10;

	int row1 = floor(dec / 4);
	int column1 = dec % 4;
	num1_mesh->uvs[3] = Vector2(0.25 * column1, 1 - 0.25*row1);
	num1_mesh->uvs[2] = Vector2(0.25 + 0.25 * column1, 1 - 0.25 * row1);
	num1_mesh->uvs[1] = Vector2(0.25 + 0.25 * column1, 0.75 - 0.25 * row1);
	num1_mesh->uvs[0] = Vector2(0.25 * column1, 0.75 - 0.25 * row1);
	num1_mesh->genBuffers();

	int row2 = floor(unit / 4);
	int column2 = unit % 4;
	num2_mesh->uvs[3] = Vector2(0.25 * column2, 1 - 0.25 * row2);
	num2_mesh->uvs[2] = Vector2(0.25 + 0.25 * column2, 1 - 0.25 * row2);
	num2_mesh->uvs[1] = Vector2(0.25 + 0.25 * column2, 0.75 - 0.25 * row2);
	num2_mesh->uvs[0] = Vector2(0.25 * column2, 0.75 - 0.25 * row2);
	num2_mesh->genBuffers();


	if (!shader) return;

	glDisable(GL_DEPTH_TEST);

	//enable shader and set uniforms
	shader->enable();
	shader->setMatrix44("u_mvp", camera->viewprojection_matrix);

	shader->setTexture("u_texture", minimap_tex);	
	minimap_mesh->render(GL_TRIANGLES, shader);

	shader->setTexture("u_texture", num_texture);
	num1_mesh->render(GL_TRIANGLES, shader);
	num2_mesh->render(GL_TRIANGLES, shader);

	//disable the shader
	shader->disable();

	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_BLEND);
	//SDL_GL_SwapWindow(g->window);
}

void HUD::renderPoint() {
	Game* g = Game::instance;

	if (!point_shader || points_mesh->vertices.size() == 0) return;

	glDisable(GL_DEPTH_TEST);
	glPointSize(7.0);

	//enable shader and set uniforms
	point_shader->enable();
	point_shader->setMatrix44("u_mvp", camera->viewprojection_matrix);

	points_mesh->render(GL_POINTS, point_shader);

	//disable the shader
	point_shader->disable();

	glEnable(GL_DEPTH_TEST);
}

void HUD::update(float dt, vector<Vector3>* vertices) {
	points_mesh = new Mesh();
	points_mesh->vertices.resize(vertices->size());
	for (int i = 0; i < vertices->size(); i++){
		points_mesh->vertices[i] = vertices->at(i);
	}
	points_mesh->genBuffers();
}
