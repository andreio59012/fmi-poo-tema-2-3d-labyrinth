#include "scene.h"
#include "entity.h"
#include <chrono>
#include <thread>
#include <cstdlib>

RenderInfo::RenderInfo(const vec2i& screen_size_) : 
	camera_position(vec2()),
	camera_angle(0.0f),
	camera_fov(0.0f),
	screen_size(screen_size_)
{
	const int sx = screen_size.x;
	const int sy = screen_size.y;

	depth_buffer = new float[(screen_size.x)];
	ascii_buffer = new char[(screen_size.x * screen_size.y)];

	for (int i = 0; i < sx; ++i)
		depth_buffer[i] = 0.0f;

	labels = {};

	for (int x = 0; x < sx; ++x)
		for (int y = 0; y < sy; ++y)
			ascii_buffer[y * sx + x] = ' ';

	for (int x = 0; x < sx; ++x) {
		ascii_buffer[0 * sx + x] = '-';
		ascii_buffer[(sy - 1) * sx + x] = '-';
	}

	for (int y = 0; y < sy; ++y) {
		ascii_buffer[y * sx + 0] = '|';
		ascii_buffer[y * sx + (sx - 2)] = '|';
		ascii_buffer[y * sx + (sx - 1)] = '\n';
	}

	ascii_buffer[sx * sy - 1] = 0;
}

RenderInfo::~RenderInfo() {
	delete[] depth_buffer;
	delete[] ascii_buffer;
}

Scene::Scene(RenderInfo* render_info_)
	: entities(), render_info(render_info_), game_state(GameState::WAITING) {}

Scene::~Scene() {
	for (Entity* e : entities)
		delete e;

	delete render_info;
}

void Scene::ready() {
	set_game_state(GameState::RUNNING);

	for (Entity* e : entities)
		e->ready(*this);
}

void Scene::process() {
	bool block_thread = false;

	for (Entity* e : entities)
		if (e->process(*this))
			block_thread = true;

	if (!block_thread)
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
}

void Scene::draw() {
	for (Entity* e : entities)
		e->draw(*render_info);

	render_info->output_labels_and_ascii();
}