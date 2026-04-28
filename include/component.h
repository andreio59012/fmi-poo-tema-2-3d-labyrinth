#pragma once
#include <string>

class Scene;
class RenderInfo;
class Entity;

class Component {

private:
	static int instance_count;
	bool enabled;
	Entity* entity;

public:
	Component();
	virtual ~Component();

	Component(const Component&) = delete;
	Component& operator=(const Component&) = delete;

	[[nodiscard]] virtual Component* clone() const = 0;
	[[nodiscard]] virtual std::string get_type_name() const = 0;
	[[nodiscard]] virtual char get_map_symbol() const;
	[[nodiscard]] bool get_enabled() const { return enabled; }
	[[nodiscard]] void set_enabled(const bool enabled_) { enabled = enabled_; }

	[[nodiscard]] Entity* get_entity() const { return entity; }
	[[nodiscard]] void set_entity(Entity* entity_) { entity = entity_; }

	[[nodiscard]] static int get_instance_count() { return instance_count; }

	virtual bool process(const Scene&) { return false; }
	virtual void ready(const Scene&) {}
	virtual void draw(RenderInfo&) {}
};