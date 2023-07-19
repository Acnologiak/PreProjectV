#include "VoxObj.h"

u8 VoxObj8Bit::GetVoxel(const i32vec3& _coordinates)
{
	i16vec3 externalCoordinates = GetChunkExternalCoordinates(_coordinates);

	auto chunk = chunks.find(I16vec3ToSizeT(externalCoordinates));
	if (chunk == chunks.end())
	{
		return 0;
	}
	else
	{
		u8vec3 internalCoordinates = GetChunkInternalCoordinates(_coordinates);
		return chunk->second->voxels[internalCoordinates.x][internalCoordinates.y][internalCoordinates.z];
	}
}

void VoxObj8Bit::SetVoxel(const i32vec3& _coordinates, u8 _color)
{
	i16vec3 externalCoordinates = GetChunkExternalCoordinates(_coordinates);
	size_t mapKey = I16vec3ToSizeT(externalCoordinates);

	auto chunk = chunks.find(mapKey);
	if (chunk == chunks.end())
	{
		CreateChunk(externalCoordinates);
		chunk = chunks.find(mapKey);
	}
	
	u8vec3 internalCoordinates = GetChunkInternalCoordinates(_coordinates);
	chunk->second->voxels[internalCoordinates.x][internalCoordinates.y][internalCoordinates.z] = _color;
}

bool VoxObj8Bit::CreateChunk(const i16vec3& _coordinates)
{
	size_t coordinates = I16vec3ToSizeT(_coordinates);

	if (chunks.find(coordinates) == chunks.end())
	{
		return false;
	}
	else
	{
		chunks.emplace(coordinates, make_shared<VoxObjChunk8Bit>());
		return true;
	}
}

void VoxObj8Bit::DeleteChunk(const i16vec3& _coordinates)
{
	size_t coordinates = I16vec3ToSizeT(_coordinates);
	chunks.erase(coordinates);
}

shared_ptr<VoxObjChunk8Bit> VoxObj8Bit::GetChunk(const i16vec3& _coordinates)
{
	size_t coordinates = I16vec3ToSizeT(_coordinates);

	auto chunk = chunks.find(coordinates);
	if (chunk == chunks.end())
	{
		return nullptr;
	}
	else
	{
		return chunk->second;
	}
}
