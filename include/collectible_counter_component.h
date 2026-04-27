#pragma once
#include "component.h"

class CollectibleCounterComponent : public Component
{
private:
	int current;
	int target;

public:
	CollectibleCounterComponent(const int target_) :
		current(0),
		target(target_)
	{}

	std::string get_type_name() const override { return "CollectibleCounter"; }

	void increment_counter() 
	{
		current += 1;
	}

	void draw(RenderInfo& render_info) override {
		render_info.set_label("Keys left", std::to_string(target - current));
	}
};