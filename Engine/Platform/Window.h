#pragma once
#include "CommonHeaders.h"

namespace zone::platform {

	DEFINE_TYPED_ID(window_id)

		class Window
	{
	public:
		constexpr explicit Window(window_id id) : _id{ id } {};
		constexpr Window() : _id{ id::invalid_id } {};
		constexpr window_id getID() const { return _id; }
		constexpr bool isValid() const { return id::is_valid(_id); }

		void setFullscreen(bool _isFullscreen) const;
		bool isFullscreen() const;
		void* handle() const;
		void setCaption(const wchar_t* caption) const;
		math::Vec4U size() const;
		void resize(uint32 width, uint32 height) const;
		uint32 width() const;
		uint32 height() const;
		bool isClosed() const;
	private:
		window_id _id{ id::invalid_id };
	};

}