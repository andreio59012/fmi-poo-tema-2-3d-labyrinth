#include "component.h"
#include "scene.h"

int Component::instance_count = 0;

Component::Component() : enabled(true), entity(nullptr) { ++instance_count; }
Component::~Component() { --instance_count; }

char Component::get_map_symbol() const { return ' '; }