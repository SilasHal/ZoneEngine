// Copyright (c) CedricZ1, 2025
// Distributed under the MIT license. See the LICENSE file in the project root for more information.

#include "ContentLoader.h"
#include "..\Components\Entity.h"
#include "..\Components\Transform.h"
#include "..\Components\Script.h"

#if !defined(SHIPPING)  

#include <fstream>

namespace zone::content {

bool load_game()
{
    std::ifstream game("game.bin", std::ios::in | std::ios::binary);
    utl::vector<uint8> buffer(std::istreambuf_iterator<char>(game), {});
    assert(buffer.size());
    const uint8* at{ buffer.data() };
    constexpr uint32 su32{ sizeof(uint32) };
    const uint32 num_entities{ *at };
    at += su32;
    if (!num_entities) return false;
    
    for (uint32 entity_index{ 0 }; entity_index < num_entities; ++entity_index)
    {
        game_entity::entity_info  info{};
        const uint32 entity_type{ *at }; at += su32;
        const uint32 num_components{ *at }; at += su32;
        if (!num_components) return false;

        for (uint32 component_index{ 0 }; component_index < num_components; ++component_index) 
        {
            const uint32 component_type{ *at }; at += su32;
        }
    }
}

bool unload_game()
{
    return false;
}

}

#endif //!defined(SHIPPING)
