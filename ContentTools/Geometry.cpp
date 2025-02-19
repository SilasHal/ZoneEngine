// Copyright (c) CedricZ1, 2025
// Distributed under the MIT license. See the LICENSE file in the project root for more information.

#include "Geometry.h"

namespace zone::tools {
namespace {

using namespace math;
using namespace DirectX;

void recalculateNormals(Mesh& mesh)
{
	const uint32 numIndices{ static_cast<uint32>(mesh.rawIndices.size()) };
	mesh.normals.reserve(numIndices);

	for (uint32 i{ 0 }; i < numIndices; ++i)
	{
		const uint32 i0{ mesh.rawIndices[i] };
		const uint32 i1{ mesh.rawIndices[++i] };
		const uint32 i2{ mesh.rawIndices[++i] };

		XMVECTOR v0{ XMLoadFloat3(&mesh.positions[i0]) };
		XMVECTOR v1{ XMLoadFloat3(&mesh.positions[i1]) };
		XMVECTOR v2{ XMLoadFloat3(&mesh.positions[i2]) };

		XMVECTOR e0{ v1 - v0 };
		XMVECTOR e1{ v2 - v0 };

		XMVECTOR normal{ XMVector3Normalize(XMVector3Cross(e0, e1)) };

		XMStoreFloat3(&mesh.normals[i], normal);
		mesh.normals[i - 1] = mesh.normals[i];
		mesh.normals[i - 2] = mesh.normals[i];
	}
}

void processNormals(Mesh& mesh, float smoothingAngle)
{
	const float cosAngle{ XMScalarCos(pi - smoothingAngle * pi / 180.0f) };
	const bool isHardEdge{ XMScalarNearEqual(smoothingAngle, 180.0f, epsilon) };
	const bool isSoftEdge{ XMScalarNearEqual(smoothingAngle, 0.0f, epsilon) };
	const uint32 numIndices{ static_cast<uint32>(mesh.rawIndices.size()) };
	const uint32 numVertices{ static_cast<uint32>(mesh.positions.size()) };
	assert(numIndices && numVertices);

	mesh.indices.resize(numIndices);

	utl::vector<utl::vector<uint32>> idxRef(numVertices);
	for (uint32 i{ 0 }; i < numIndices; ++i)
	{
		idxRef[mesh.rawIndices[i]].emplace_back(i);
	}

	for (uint32 i{ 0 }; i < numVertices; ++i)
	{
		auto& refs{ idxRef[i] };
		uint32 numRefs{ static_cast<uint32>(refs.size()) };
		for (uint32 j{ 0 }; j < numRefs; ++j)
		{
			mesh.indices[refs[j]] = static_cast<uint32>(mesh.vertices.size());
			Vertex& vertex{ mesh.vertices.emplace_back() };
			vertex.position = mesh.positions[mesh.rawIndices[refs[j]]];

			XMVECTOR n1{ XMLoadFloat3(&mesh.normals[refs[j]]) };
			if (!isHardEdge)
			{
				for (uint32 k{ j + 1 }; k < numRefs; ++k)
				{
					float n{ 0.0f };
					XMVECTOR n2{ XMLoadFloat3(&mesh.normals[refs[k]]) };
					if (!isSoftEdge)
					{
						XMStoreFloat(&n, XMVector3Dot(n1, n2) * XMVector3ReciprocalLength(n1));
					}

				if (isSoftEdge|| n >=cosAngle)
				{
					n1 += n2;
					mesh.indices[refs[k]] = mesh.indices[refs[j]];
					refs.erase(refs.begin() + k);
					--numRefs;
					--k;
				}

				}
			}
			XMStoreFloat3(&vertex.normal, XMVector3Normalize(n1));
		}
	}
}

void processUVs(Mesh& mesh)
{
	utl::vector<Vertex> oldVertices;
}

void processVertices(Mesh& mesh, const GeometryImportSettings& settings) 
{
	assert((mesh.rawIndices.size() % 3) == 0);
	if (settings.calculateNormals||mesh.normals.empty())
	{
		recalculateNormals(mesh);
	}

	processNormals(mesh, settings.smoothingAngle);

	if (!mesh.uvSets.empty())
	{
		processUVs(mesh);
	}
	packVerticesStatic(mesh);
}

} // anonymous namespace

void processScene(Scene& scene, const GeometryImportSettings& settings)
{

}

void packData(const Scene& scene, SceneData& data)
{

}

}

