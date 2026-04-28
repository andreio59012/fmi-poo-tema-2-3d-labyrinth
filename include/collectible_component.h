#pragma once
#include "collectible_counter_component.h"
#include "player_component.h"

class CollectibleComponent : public Component
{
private:
	const float minimum_distance;
	const PositionComponent* player_position_component;
	PositionComponent* my_position_component;
	CollectibleCounterComponent* counter_component;

public:
	CollectibleComponent(
		const float minimum_distance_,
		const PositionComponent* player_position_component_,
		CollectibleCounterComponent* counter_component_
	) :
		Component(),
		minimum_distance(minimum_distance_),
		player_position_component(player_position_component_),
		my_position_component(nullptr),
		counter_component(counter_component_)
	{}

	[[nodiscard]] Component* clone() const override {
		return new CollectibleComponent(minimum_distance, player_position_component, counter_component);
	}

	[[nodiscard]] std::string get_type_name() const override { return "Collectible"; }
	[[nodiscard]] char get_map_symbol() const override { return 'K'; }

	void ready(const Scene&) override {
		my_position_component = get_entity()->get_component_of_type<PositionComponent*>();
	}

	bool process(const Scene&) override {
		float distance = my_position_component->get_position().get_dist_to(player_position_component->get_position());

		if (distance <= minimum_distance)
		{
			counter_component->increment_counter();
			my_position_component->set_position(vec2(999.0, 999.0));
		}

		return false;
	}
};