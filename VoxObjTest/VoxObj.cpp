#include "VoxObj.h"

u8 VoxObjChunk::GetVoxel(const u8vec3& _coordinates)
{
	return voxObjChunkVoxels[_coordinates.x][_coordinates.y][_coordinates.z];
}

void VoxObjChunk::SetVoxel(const u8vec3& _coordinates, u8 _color)
{
	u8& color = voxObjChunkVoxels[_coordinates.x][_coordinates.y][_coordinates.z];

	if (_color != 0 && color == 0)
	{
		numberVoxels++;
		color = _color;
	}
	else if (_color != 0 && color != 0)
	{
		color = _color;
	}
	else if (_color == 0 && color != 0)
	{
		numberVoxels--;
		color = _color;
	}
}

u8 VoxObj::GetVoxel(const i32vec3& _coordinates)
{
	i16vec3 internalCoordinates, externalCoordinates;

	internalCoordinates = GetInternalCoordinates(_coordinates);
	externalCoordinates = GetExternalCoordinates(_coordinates);

	auto chunk = GetObjChunk(externalCoordinates);
	if (chunk == nullptr)
	{
		return 0;
	}
	else
	{
		return chunk->GetVoxel(externalCoordinates);
	}
}

void VoxObj::SetVoxel(const i32vec3& _coordinates, u8 _color)
{
	i16vec3 internalCoordinates, externalCoordinates;

	internalCoordinates = GetInternalCoordinates(_coordinates);
	externalCoordinates = GetExternalCoordinates(_coordinates);

	auto chunk = GetObjChunk(externalCoordinates);
	if (chunk == nullptr)
	{
		chunk = CreateObjChunk(externalCoordinates);
	}

	chunk->SetVoxel(internalCoordinates, _color);
}

shared_ptr<VoxObjChunk> VoxObj::GetObjChunk(const i16vec3& _coordinates)
{
	size_t coordinates = I16vec3ToSizeT(_coordinates);
	auto result = voxObjChunks.find(coordinates);

	if (result == voxObjChunks.end())
	{
		return nullptr;
	}
	else
	{
		return result->second;
	}
}

bool VoxObj::DeleteObjChunk(const i16vec3& _coordinates)
{
	size_t coordinates = I16vec3ToSizeT(_coordinates);
	voxObjChunks.erase(coordinates);

	return true;
}

bool VoxObj::AddObjChunk(const i16vec3& _coordinates, shared_ptr<VoxObjChunk> _voxObjChunk)
{
	size_t coordinates = I16vec3ToSizeT(_coordinates);
	if (voxObjChunks.find(coordinates) == voxObjChunks.end())
	{
		voxObjChunks.emplace(coordinates, _voxObjChunk);
		return true;
	}
	else
	{
		return false;
	}
}

bool VoxObj::UpdateObjChunk(const i16vec3& _coordinates, shared_ptr<VoxObjChunk> _voxObjChunk)
{
	size_t coordinates = I16vec3ToSizeT(_coordinates);
	voxObjChunks[coordinates] = _voxObjChunk;

	return true;
}

shared_ptr<VoxObjChunk> VoxObj::CreateObjChunk(const i16vec3& _coordinates)
{
	shared_ptr<VoxObjChunk> voxObjChunk = make_shared<VoxObjChunk>();

	size_t coordinates = I16vec3ToSizeT(_coordinates);
	voxObjChunks.emplace(coordinates, voxObjChunk);

	return voxObjChunk;
}

i16vec3 VoxObj::GetInternalCoordinates(const i32vec3& _coordinates)
{
	return i16vec3 { _coordinates.x & 0xF, _coordinates.y & 0xF, _coordinates.z & 0xF };
}

i16vec3 VoxObj::GetExternalCoordinates(const i32vec3& _coordinates)
{
	return i16vec3{ _coordinates.x >> 4, _coordinates.y >> 4, _coordinates.z >> 4 };
}