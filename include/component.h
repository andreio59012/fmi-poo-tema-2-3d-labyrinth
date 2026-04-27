#pragma once
#include <cstring>
#include <iostream>
#include "scene.h"

class Component {

private:
	bool enabled;
	Entity* entity;

public:
	Component() : enabled(true), entity(nullptr) {}

	virtual ~Component() {}

	bool get_enabled() const { return enabled; }
	void set_enabled(const bool enabled_) { enabled = enabled_; }

	Entity* get_entity() const { return entity; }
	void set_entity(Entity* entity_) { entity = entity_; }

	virtual bool process(const Scene&) { return false; }
	virtual void ready(const Scene&) {}
	virtual void draw(RenderInfo&) {}
	virtual std::string get_type_name() const = 0;
};