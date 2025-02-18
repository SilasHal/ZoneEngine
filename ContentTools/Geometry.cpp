#include "Geometry.h"

namespace zone::tools {
	namespace {

		using namespace math;
		using namespace DirectX;

		void RecalculateNormals(Mesh& m)
		{
			const uint32 numIndiecs{ (uint32)m.rawIndices.size() };
			m.normals.reserve(numIndiecs);

			for (uint32 i{ 0 }; i < numIndiecs; ++i)
			{
				const uint32 i0{ m.rawIndices[i] };
				const uint32 i1{ m.rawIndices[++i] };
				const uint32 i2{ m.rawIndices[++i] };

				XMVECTOR v0{ XMLoadFloat3(&m.positions[i0]) };
				XMVECTOR v1{ XMLoadFloat3(&m.positions[i1]) };
				XMVECTOR v2{ XMLoadFloat3(&m.positions[i2]) };

				XMVECTOR e0{ v1 - v0 };
				XMVECTOR e1{ v2 - v0 };
				XMVECTOR n{ XMVector3Normalize(XMVector3Cross(e0, e1)) };

				XMStoreFloat3(&m.normals[i], n);
				m.normals[i - 1] = m.normals[i];
				m.normals[i - 2] = m.normals[i];
			}
		}

		void ProcessNormals(Mesh& m, float smoothingAngle)
		{
			const float cosAlpha{ XMScalarCos(pi - smoothingAngle * pi / 180.f) };
			const bool isHardEdge{ XMScalarNearEqual(smoothingAngle, 180.f, epsilon) };
			const bool isSoftEdge{ XMScalarNearEqual(smoothingAngle, 0.f, epsilon) };
			const uint32 numIndices{ (uint32)m.rawIndices.size() };
			const uint32 numVertices{ (uint32)m.positions.size() };
			assert(numIndices && numVertices);

			m.indices.resize(numIndices);

			utl::vector<utl::vector<uint32>> idxRef(numVertices);
			for (uint32 i{ 0 }; i < numIndices; ++i)
				idxRef[m.rawIndices[i]].emplace_back(i);

			for (uint32 i{ 0 }; i < numVertices; ++i)
			{
				auto& refs{ idxRef[i] };
				uint32 numRefs{ (uint32)refs.size() };
				for (uint32 j{ 0 }; j < numRefs; ++j)
				{
					m.indices[refs[j]] = (uint32)m.vertices.size();
					Vertex& v{ m.vertices.emplace_back() };
					v.position = m.positions[m.rawIndices[refs[j]]];

					XMVECTOR n1{ XMLoadFloat3(&m.normals[refs[j]]) };
					if (!isHardEdge)
					{
						for (uint32 k{ j + 1 }; k < numRefs; ++k)
						{
							// this value represents the cosine of the angle between normals.
							float cosTheta{ 0.f };
							XMVECTOR n2{ XMLoadFloat3(&m.normals[refs[k]]) };
							if (!isSoftEdge)
							{
								// NOTE: we're accounting for the length of n1 in this calculation because
								//       it can possibly change in this loop iteration. We assume unit length
								//       for n2.
								//       cos(angle) = dot(n1, n2) / (||n1| * |n2||)
								XMStoreFloat(&cosTheta, XMVector3Dot(n1, n2) * XMVector3ReciprocalLength(n1));
							}

							if (isSoftEdge || cosTheta >= cosAlpha)
							{
								n1 += n2;

								m.indices[refs[k]] = m.indices[refs[j]];
								refs.erase(refs.begin() + k);
								--numRefs;
								--k;
							}
						}
					}
					XMStoreFloat3(&v.normal, XMVector3Normalize(n1));
				}
			}
		}

		void ProcessUvs(Mesh& m)
		{
			utl::vector<Vertex> oldVertices;
			oldVertices.swap(m.vertices);
			utl::vector<uint32> oldIndices(m.indices.size());
			oldIndices.swap(m.indices);

			const uint32 numVertices{ (uint32)oldVertices.size() };
			const uint32 numIndices{ (uint32)oldIndices.size() };
			assert(numVertices && numIndices);

			utl::vector<utl::vector<uint32>> idxRef(numVertices);
			for (uint32 i{ 0 }; i < numIndices; ++i)
				idxRef[oldIndices[i]].emplace_back(i);

			for (uint32 i{ 0 }; i < numIndices; ++i)
			{
				auto& refs{ idxRef[i] };
				uint32 numRefs{ (uint32)refs.size() };
				for (uint32 j{ 0 }; j < numRefs; ++j)
				{
					m.indices[refs[j]] = (uint32)m.vertices.size();
					Vertex& v{ oldVertices[oldIndices[refs[j]]] };
					v.uv = m.uvSets[0][refs[j]];
					m.vertices.emplace_back(v);

					for (uint32 k{ j + 1 }; k < numRefs; ++k)
					{
						Vec2F& uv1{ m.uvSets[0][refs[k]] };
						if (XMScalarNearEqual(v.uv.x, uv1.x, epsilon) && XMScalarNearEqual(v.uv.y, uv1.y, epsilon))
						{
							m.indices[refs[k]] = m.indices[refs[j]];
							refs.erase(refs.begin() + k);
							--numRefs;
							--k;
						}
					}
				}
			}
		}

		void PackVerticesStatic(Mesh& m)
		{
			const uint32 numVertices{ (uint32)m.vertices.size() };
			assert(numVertices);
			m.packedVerticesStatic.reserve(numVertices);

			for (uint32 i{ 0 }; i < numVertices; ++i)
			{
				Vertex& v{ m.vertices[i] };
				const uint8 signs{ (uint8)((v.normal.z > 0.f) << 1) };
				const uint16 normalX{ (uint16)PackFloat<16>(v.normal.x, -1.f, 1.f) };
				const uint16 normalY{ (uint16)PackFloat<16>(v.normal.y, -1.f, 1.f) };
				// TODO: pack tangents in sign and in x/y components

				m.packedVerticesStatic.emplace_back(packedVertex::VertexStatic
					{
						v.position,
						{0, 0, 0},
						signs,
						{ normalX, normalY },
						{},
						v.uv
					});
			}
		}

		void ProcessVertices(Mesh& m, const GeometryImportSettings& settings)
		{
			assert(m.rawIndices.size() % 3 == 0);
			if (settings.calculateNormals || m.normals.empty())
			{
				RecalculateNormals(m);
			}

			ProcessNormals(m, settings.smoothingAngle);

			if (!m.uvSets.empty())
			{
				ProcessUvs(m);
			}

			PackVerticesStatic(m);
		}

		uint64 GetMeshSize(const Mesh& m)
		{
			const uint64 numVertices{ m.vertices.size() };
			const uint64 vertexBufferSize{ sizeof(packedVertex::VertexStatic) * numVertices };
			const uint64 indexSize{ (numVertices < (1 << 16)) ? sizeof(uint16) : sizeof(uint32) };
			const uint64 indexBufferSize{ indexSize * m.indices.size() };
			constexpr uint64 saveUint32{ sizeof(uint32) };
			const uint64 size{
				saveUint32 + m.name.size() +		// mesh name length and room for mesh name string
				saveUint32 +						// lod id
				saveUint32 +						// vertex size
				saveUint32 +						// number of vertices
				saveUint32 +						// index size (16 bit or 32 bit)
				saveUint32 +						// number of indices
				sizeof(float) +						// LOD threshold
				vertexBufferSize +					// room for vertices
				indexBufferSize						// room for indices
			};

			return size;
		}

		uint64 GetSceneSize(const Scene& scene)
		{
			constexpr uint64 saveUint32{ sizeof(uint32) };
			uint64 size
			{
				saveUint32 +				// name length
				scene.name.size() +			// room for scene name string
				saveUint32					// number of LOD groups	
			};

			for (auto& LodGroup : scene.lodGroups)
			{
				uint64 lodSize
				{
					saveUint32 + LodGroup.name.size() +	// LOD group name length and room for LPD name string
					saveUint32							// number of meshes in this LOD group
				};

				for (auto& m : LodGroup.meshes)
				{
					lodSize += GetMeshSize(m);
				}

				size += lodSize;
			}

			return size;
		}

		void PackMeshData(const Mesh& m, uint8* const buffer, uint64& at)
		{
			constexpr uint64 saveUint32{ sizeof(uint32) };
			uint32 save{ 0 };
			// mesh name
			save = (uint32)m.name.size();
			memcpy(&buffer[at], &save, saveUint32); at += saveUint32;
			memcpy(&buffer[at], m.name.c_str(), save); at += save;
			// lod id
			save = m.lodId;
			memcpy(&buffer[at], &save, saveUint32); at += saveUint32;
			// vertex size
			constexpr uint32 vertexSize{ sizeof(packedVertex::VertexStatic) };
			save = vertexSize;
			memcpy(&buffer[at], &save, saveUint32); at += saveUint32;
			// number of vertices
			const uint32 numVertices{ (uint32)m.vertices.size() };
			save = numVertices;
			memcpy(&buffer[at], &save, saveUint32); at += saveUint32;
			// index size (16 bit or 32 bit)
			const uint32 indexSize{ (numVertices < (1 << 16)) ? sizeof(uint16) : sizeof(uint32) };
			save = indexSize;
			memcpy(&buffer[at], &save, saveUint32); at += saveUint32;
			// number of indices
			const uint32 numIndices{ (uint32)m.indices.size() };
			save = numIndices;
			memcpy(&buffer[at], &save, saveUint32); at += saveUint32;
			// LOD threshold
			memcpy(&buffer[at], &m.lodThreshold, sizeof(float)); at += sizeof(float);
			//vertex data
			save = vertexSize * numVertices;
			memcpy(&buffer[at], m.packedVerticesStatic.data(), save); at += save;
			// index data
			save = indexSize * numIndices;
			void* data{ (void*)m.indices.data() };
			utl::vector<uint16> indices;

			if (indexSize == sizeof(uint16))
			{
				indices.resize(numIndices);
				for (uint32 i{ 0 }; i < numIndices; ++i) indices[i] = (uint16)m.indices[i];
				data = (void*)indices.data();
			}
			memcpy(&buffer[at], data, save); at += save;

		}

	} // anonymous namespace

	void ProcessScene(Scene& scene, const GeometryImportSettings& settings)
	{
		for(auto& LodGroup: scene.lodGroups )
			for (auto& m : LodGroup.meshes)
			{
				ProcessVertices(m, settings);
			}
	}

	void PackData(const Scene& scene, SceneData& data)
	{
		constexpr uint64 saveUint32{ sizeof(uint32) };
		const uint64 sceneSize{ GetSceneSize(scene) };
		data.bufferSize = (uint32)sceneSize;
		data.buffer = (uint8*)CoTaskMemAlloc(sceneSize);
		assert(data.buffer);

		uint8* buffer{ data.buffer };
		uint64 at{ 0 };
		uint32 save{ 0 };

		// scene name
		save = (uint32)scene.name.size();
		memcpy(&buffer[at], &save, saveUint32); at += saveUint32;
		memcpy(&buffer[at], scene.name.c_str(), save); at += save;
		// number of LOD groups
		save = (uint32)scene.lodGroups.size();
		memcpy(&buffer[at], &save, saveUint32); at += saveUint32;

		for (auto& LodGroup : scene.lodGroups)
		{
			// LOD group name
			save = (uint32)LodGroup.name.size();
			memcpy(&buffer[at], &save, saveUint32); at += saveUint32;
			memcpy(&buffer[at], LodGroup.name.c_str(), save); at += save;
			// number of meshes in this LOD group
			save = (uint32)LodGroup.meshes.size();
			memcpy(&buffer[at], &save, saveUint32); at += saveUint32;

			for (auto& m : LodGroup.meshes)
			{
				PackMeshData(m, buffer, at);
			}
		}

		assert(sceneSize == at);
	}
}