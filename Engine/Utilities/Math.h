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

}