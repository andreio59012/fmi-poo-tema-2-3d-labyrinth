#include "entity.h"

Entity::~Entity() {
	for (Component* c : components)
		delete c;
}

Entity::Entity(const Entity& other) {
	components.reserve(other.components.size());

	for (const Component* c : other.components) {
		Component* cloned = c->clone();
		cloned->set_entity(this);
		components.push_back(cloned);
	}
}

void swap(Entity& a, Entity& b) noexcept {
	std::swap(a.components, b.components);

	for (Component* c : a.components)
		c->set_entity(&a);
	for (Component* c : b.components)
		c->set_entity(&b);
}

Entity& Entity::operator=(Entity other) {
	swap(*this, other);
	return *this;
}

void Entity::add_component(Component* component) {
	component->set_entity(this);
	components.push_back(component);
}

char Entity::get_map_symbol() const {
	for (const Component* c : components) {
		const char sym = c->get_map_symbol();
		if (sym != ' ') return sym;
	}
	return ' ';
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
