#include <iostream>
#include "scene.h"
#include "entity.h"
#include "level_component.h"
#include "player_component.h"
#include "billboard_component.h"
#include "collectible_component.h"
#include "exit_door_component.h"

int main()
{
	Scene* scene = new Scene(new RenderInfo(vec2i(200, 70)));

	// Level
	const bool level_tiles[] = {
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,0,1,
		1,0,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,0,1,
		1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,
		1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1,
		1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1,
		1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,
		1,0,1,1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1,1,0,1,
		1,0,1,1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1,1,0,1,
		1,0,1,1,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,1,1,0,1,
		1,0,1,1,1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,1,1,0,1,
		1,0,1,1,1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,1,1,0,1,
		1,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,1,
		1,0,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,0,1,
		1,0,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,1,1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,1,0,1,
		1,0,1,1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,1,0,1,
		1,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,1,
		1,0,1,1,1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,1,1,0,1,
		1,0,1,1,1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,1,1,0,1,
		1,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,1,
		1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,
		1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	};

	Entity* level_entity = new Entity();
	level_entity->add_component(new LevelComponent(vec2i(34, 27), level_tiles));
	scene->add_entity(level_entity);

	// Player
	Entity* player_entity = new Entity();
	player_entity->add_component(new PositionComponent(vec2(1.5f, 1.5f)));
	player_entity->add_component(new RotationComponent());
	player_entity->add_component(new PlayerComponent(level_entity->get_component_of_type<LevelComponent*>(), 1.5f));
	scene->add_entity(player_entity);

	// Counter
	const int collectible_amount = 4;

	Entity* counter_entity = new Entity();
	counter_entity->add_component(new CollectibleCounterComponent(collectible_amount));
	scene->add_entity(counter_entity);

	// Collectibles
	const char collectible_sprite[] = {
		'/','-','\\',' ',' ','.',' ','.',
		'|',' ','|','-','-','-','-','-',
		'\\','-','/',' ',' ',' ',' ',' ',
	};

	vec2i collectible_pos[collectible_amount] = {
		vec2i(5, 1),
		vec2i(10, 1),
		vec2i(1, 5),
		vec2i(1, 10),
	};

	for (int i = 0; i < collectible_amount; i++) {
		Entity* collectible_entity = new Entity();
		collectible_entity->add_component(new PositionComponent(collectible_pos[i]));
		collectible_entity->add_component(new BillboardComponent(vec2i(8, 5), collectible_sprite, 2.0));
		collectible_entity->add_component(new CollectibleComponent(1.0f, player_entity->get_component_of_type<PositionComponent*>(), counter_entity->get_component_of_type<CollectibleCounterComponent*>()));
		scene->add_entity(collectible_entity);
	}

	scene->ready();

	while (true)
	{
		scene->process();
		scene->draw();
	}

	return 0;
}