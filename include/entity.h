#pragma once
#include <vector>
#include <string>
#include <type_traits>
#include <typeinfo>
#include "component.h"

class Scene;
class RenderInfo;

class Entity {
private:
    std::vector<Component*> components;

public:
    Entity() {}
    ~Entity();

    void add_component(Component* component);

    template<typename T> T get_component_of_type(bool only_enabled = false) const {
        for (Component* c : components) {
            if (only_enabled && !c->get_enabled()) continue;
            if (T casted = dynamic_cast<T>(c)) return casted;
        }
        return nullptr;
    }

    Component* get_component_of_type_name(const std::string& type_name, bool only_enabled = false) const;

    void ready(const Scene&);
    bool process(const Scene&);
    void draw(RenderInfo&);
};