#pragma once
#include "ComponentsCommon.h"

namespace zone::transform {

struct init_info 
{
	float position[3]{};
	float rotation[4]{};
	float scale[3]{ 1.f,1.f,1.f };
};

component create_transform(const init_info& info, game_entity::entity entity);
void remove_transform(component _component);
}