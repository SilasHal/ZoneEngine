// Copyright (c) CedricZ1, 2024
// Distributed under the MIT license. See the LICENSE file in the project root for more information.

#pragma once
#include "CommonHeaders.h"

namespace zone::id {

using id_type = uint32;

namespace detail {
	constexpr uint32 generation_bits{ 10 };
	constexpr uint32 index_bits{ sizeof(id_type) * 8 - generation_bits };
	constexpr id_type index_mask{ (id_type{1} << index_bits) - 1 };
	constexpr id_type generation_mask{ (id_type{1} << generation_bits) - 1 };
}

constexpr id_type invalid_id{ static_cast<id_type>(-1) };
constexpr uint32 min_deleted_elements{ 1024 }; 

using generation_type = std::conditional_t<detail::generation_bits <= 16, std::conditional_t<detail::generation_bits <= 8, uint8, uint16>, uint32>;
static_assert(sizeof(generation_type) * 8 >= detail::generation_bits);
static_assert((sizeof(id_type) - sizeof(generation_type)) > 0);

constexpr bool is_valid(id_type id) 
{
	return id != invalid_id;
}

constexpr id_type index(id_type id)
{
	id_type	index{ id & detail::index_mask };
	assert(index != detail::index_mask);
	return index;
}

constexpr id_type generation(id_type id)
{
	return (id >> detail::index_bits) & detail::generation_mask;
}

constexpr id_type new_generation(id_type id)
{
	const id_type generation{ id::generation(id) + 1 };
	assert(generation < (((uint64)1 << detail::generation_bits) - 1));
	return index(id) | (generation << detail::index_bits);
}


#if _DEBUG
namespace detail {
	struct IDBase
	{
		constexpr explicit IDBase(id_type id) : _id{ id } {}
		constexpr operator id_type() const { return _id; }
	private:
		id_type	_id;
	};
}

#define DEFINE_TYPED_ID(name)								\
	struct name final : id::detail::IDBase				    \
	{														\
		constexpr explicit name(id::id_type id)				\
			: IDBase{ id } {}								\
		constexpr name() : IDBase{ 0 } {}					\
	};

#else
#define DEFINE_TYPED_ID(name) using name = id::id_type;
#endif

}

