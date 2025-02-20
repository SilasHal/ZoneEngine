// Copyright (c) CedricZ1, 2025
// Distributed under the MIT license. See the LICENSE file in the project root for more information.
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
constexpr uint32 packUnitFloat(float value)
{
	static_assert(bits <= sizeof(uint32) * 8, "bits must be less than or equal to 32");
	assert(value >= 0.0f && value <= 1.0f);
	constexpr float intervals{ static_cast<float>((1ui32 << bits) - 1) };
	return static_cast<uint32>(value * intervals + 0.5f);
}

template<uint32 bits>
constexpr uint32 unpackToUnitFloat(uint32 i)
{
	static_assert(bits <= sizeof(uint32) * 8, "bits must be less than or equal to 32");
	assert(i < (1ui32 << bits));
	constexpr float intervals{ static_cast<float>((1ui32 << bits) - 1) };
	return static_cast<float>(i) / intervals;
}

template<uint32 bits>
constexpr uint32 packFloat(float value, float min, float max)
{
	assert(min < max);
	assert(value >= min && value <= max);
	const float distance{ (value - min) / (max - min) };
	return	packUnitFloat<bits>(distance);
}

template<uint32 bits>
constexpr float unpackToFloat(uint32 i, float min, float max)
{
	assert(min < max);
	return unpackToUnitFloat<bits>(i) * (max - min) + min;
}
} // namespace zone::math