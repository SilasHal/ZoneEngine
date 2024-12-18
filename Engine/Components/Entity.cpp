// Copyright (c) CedricZ1, 2024
// Distributed under the MIT license. See the LICENSE file in the project root for more information.

#include "Entity.h"

namespace zone::game_entity {

namespace {

utl::vector<id::generation_type> generations;

}

entity_id create_game_entity(const entity_info& info)
{
	assert(info.transform);
	if (!info.transform) {
		return entity_id{};
	}
}

void remove_game_entity(entity_id id)
{
}

bool is_alive(entity_id id)
{
	return false;
}

}
