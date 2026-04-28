#pragma once
#include "component.h"
#include "exceptions.h"

class CollectibleCounterComponent : public Component
{
private:
	int current;
	int target;

public:
	explicit CollectibleCounterComponent(const int target_) :
		Component(),
		current(0),
		target(target_)
	{
		if (target_ <= 0)
			throw InvalidLevelException("collectible target must be positive");
	}

	[[nodiscard]] Component* clone() const override {
		CollectibleCounterComponent* c = new CollectibleCounterComponent(target);
		c->current = current;
		return c;
	}

	[[nodiscard]] std::string get_type_name() const override { return "CollectibleCounter"; }

	void increment_counter() { ++current; }

	[[nodiscard]] bool get_finished() const {
		return current >= target;
	}

	void draw(RenderInfo& render_info) override {
		render_info.set_label("Keys", std::to_string(current) + '/' + std::to_string(target));
	}
};