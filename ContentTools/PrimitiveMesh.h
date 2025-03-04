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
		math::Vec3F				size{ 1, 1, 1 };
		uint32					lod{ 0 };
	};

}