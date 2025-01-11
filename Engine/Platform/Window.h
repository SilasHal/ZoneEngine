// Copyright (c) CedricZ1, 2025
// Distributed under the MIT license. See the LICENSE file in the project root for more information.
#pragma once
#include "CommonHeaders.h"

namespace zone::platform {

DEFINE_TYPED_ID(window_id)

class Window
{
public:
	constexpr explicit Window(window_id id) : _id{ id } {};
	constexpr explicit Window() : _id{ id::invalid_id } {};
	constexpr window_id get_id() const { return _id; }
	constexpr bool is_valid() const { return id::is_valid(_id); }

	void set_fullscreen(bool is_fullscreen) const;
	void is_fullscreen() const;
	void* handle() const;
	void set_caption(const char* caption) const;
	const math::Vec4U size() const;
	void resize(uint32 width, uint32 height) const;
	const uint32 width() const;
	const uint32 height() const;
	bool is_closed() const;
private:
	window_id _id{ id::invalid_id };
};

}