// Copyright (c) CedricZ1, 2024
// Distributed under the MIT license. See the LICENSE file in the project root for more information.

#pragma once
#include "ComponentsCommon.h"

namespace zone {

#define INIT_INFO(component) namespace component {struct init_info; }

INIT_INFO(transform);
INIT_INFO(script);

#undef INIT_INFO

namespace game_entity{
struct entity_info 
{
	transform::init_info* transform{ nullptr };
	script::init_info* script{ nullptr };
};
	
Entity create(entity_info info);
void remove(EntityID id);
bool is_alive(EntityID id);

}
}