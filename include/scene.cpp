#pragma once
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
	depth_buffer = new float[(screen_size.x)];
	ascii_buffer = new char[(screen_size.x * screen_size.y)];
	labels = {};

	for (int x = 0; x < screen_size.x; ++x)
		for (int y = 0; y < screen_size.y; ++y)
			ascii_buffer[y * screen_size.x + x] = ' ';

	for (int x = 0; x < screen_size.x; ++x) {
		ascii_buffer[0 * screen_size.x + x] = '-';
		ascii_buffer[(screen_size.y - 1) * screen_size.x + x] = '-';
	}

	for (int y = 0; y < screen_size.y; ++y) {
		ascii_buffer[y * screen_size.x + 0] = '|';
		ascii_buffer[y * screen_size.x + (screen_size.x - 2)] = '|';
		ascii_buffer[y * screen_size.x + (screen_size.x - 1)] = '\n';
	}

	ascii_buffer[screen_size.x * screen_size.y - 1] = 0;
}

Scene::Scene(RenderInfo* render_info_)
	: entities(), render_info(render_info_) {}

Scene::~Scene() {
	for (Entity* e : entities)
		delete e;

	delete render_info;
}

void Scene::ready() {
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