#include "entity.h"

Entity::~Entity() {
	for (Component* c : components)
		delete c;
}

void Entity::add_component(Component* component) {
	component->set_entity(this);
	components.push_back(component);
}

Component* Entity::get_component_of_type_name(const std::string& type_name, bool only_enabled) const {
	for (Component* c : components)
		if (c->get_type_name() == type_name && (!only_enabled || c->get_enabled()))
			return c;
	return nullptr;
}

void Entity::ready(const Scene& render_info) {
	for (Component* c : components)
		c->ready(render_info);
}

bool Entity::process(const Scene& scene) {
	bool block_thread = false;
	for (Component* c : components)
		if (c->get_enabled() && c->process(scene))
			block_thread = true;
	return block_thread;
}

void Entity::draw(RenderInfo& render_info) {
	for (Component* c : components)
		c->draw(render_info);
}
