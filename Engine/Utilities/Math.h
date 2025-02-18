#pragma once

#include "CommonHeaders.h"
#include "MathTypes.h"

namespace zone::math {

	template<typename T>
	constexpr T clamp(T value, T min, T max)
	{
		return value < min ? min : value > max ? max : value;
	}

	template<uint32 bits>
	constexpr uint32 PackUnitFloat(float f)
	{
		static_assert(bits <= sizeof(uint32) * 8);
		assert(f >= 0.f && f <= 1.f);
		constexpr float intervals{ (float)((1ui32 << bits) - 1) };
		return (uint32)(intervals * f + 0.5f);
	}

	template<uint32 bits>
	constexpr float UnpackToUnitFloat(uint32 i)
	{
		static_assert(bits <= sizeof(uint32) * 8);
		assert(i < (1ui32 << bits));
		constexpr float intervals{ (float)((1ui32 << bits) - 1) };
		return (float)i / intervals;
	}

	template<uint32 bits>
	constexpr uint32 PackFloat(float f, float min, float max)
	{
		assert(min < max);
		assert(f >= min && f <= max);
		const float distance{ (f - min) / max - min };
		return PackUnitFloat<bits>(distance);
	}

	template<uint32 bits>
	constexpr float UnpackFloat(uint32 i, float min, float max)
	{
		assert(min < max);
		return UnpackToUnitFloat<bits>(i) * (max - min) + min;
	}
}