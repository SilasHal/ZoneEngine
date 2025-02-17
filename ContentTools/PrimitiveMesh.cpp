// Copyright (c) CedricZ1, 2025
// Distributed under the MIT license. See the LICENSE file in the project root for more information.
#include "PrimitiveMesh.h"
#include "Geometry.h"
namespace zone::tools {
namespace {

//using PrimitiveMeshCreator = void(*)(Scene&, const PrimitivesInitInfo& info);
//
//void createPlane(Scene& scene, const PrimitivesInitInfo& info);
//void createCube(Scene& scene, const PrimitivesInitInfo& info);
//void createUVSphere(Scene& scene, const PrimitivesInitInfo& info);
//void createICOSphere(Scene& scene, const PrimitivesInitInfo& info);
//void createCylinder(Scene& scene, const PrimitivesInitInfo& info);
//void createCapsule(Scene& scene, const PrimitivesInitInfo& info);
//
//PrimitiveMeshCreator creators[]
//{
//	createPlane,
//	createCube,
//	createUVSphere,
//	createICOSphere,
//	createCylinder,
//	createCapsule
//};
//
//static_assert(_countof(creators) == PrimitivesMeshType::Count);

} // anonymous namespace

EDITOR_INTERFACE void CreateMesh(SceneData* data, PrimitivesInitInfo* meshInfo)
{
	assert(data && meshInfo);
	assert(meshInfo->type < PrimitivesMeshType::Count);
	Scene scene{};
}

}