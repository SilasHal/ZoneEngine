// Copyright (c) CedricZ1, 2025
// Distributed under the MIT license. See the LICENSE file in the project root for more information.
#pragma once
#include "ToolsCommon.h"

namespace zone::tools {
enum PrimitiveMeshType :uint32
{
	Plane,
	Cube,
	UVSphere,
	ICOSphere,
	CyLinder,
	Capsule,

	Count
};

struct PrimitiveInitInfo
{
	PrimitiveMeshType		type;
	uint32					segments[3]{ 1, 1, 1 };
	math::Vec3F				size{ 1.0f, 1.0f, 1.0f };
	uint32					lod{ 0 };
};

}