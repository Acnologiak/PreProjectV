#pragma once

#define CHUNK_SIZE 16
#define VOXEL_SIZE 0,03125

#include <map>
#include <memory>
#include <iostream>
#include <glm/glm.hpp>

#include "TransformationTools.h"

using std::map;
using std::shared_ptr;
using std::make_shared;
using std::memcpy;

using glm::u8;
using glm::u16;
using glm::u8vec3;
using glm::i16vec3;
using glm::i32vec3;


class VoxObjChunk
{
public:
	u8 voxObjChunkVoxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

	u8 GetVoxel(const u8vec3& _coordinates);
	void SetVoxel(const u8vec3& _coordinates, u8 _color);
private:
	u16 numberVoxels{ 0 };
};

class VoxObj
{
public:
	map<size_t, shared_ptr<VoxObjChunk>> voxObjChunks;

	u8 GetVoxel(const i32vec3& _coordinates);
	void SetVoxel(const i32vec3& _coordinates, u8 _color);

	shared_ptr<VoxObjChunk> GetObjChunk(const i16vec3& _coordinates);
	bool DeleteObjChunk(const i16vec3& _coordinates);
	bool AddObjChunk(const i16vec3& _coordinates, shared_ptr<VoxObjChunk> _voxObjChunk);
	bool UpdateObjChunk(const i16vec3& _coordinates, shared_ptr<VoxObjChunk> _voxObjChunk);
	shared_ptr<VoxObjChunk> CreateObjChunk(const i16vec3& _coordinates);

	i16vec3 GetInternalCoordinates(const i32vec3& _coordinates);
	i16vec3 GetExternalCoordinates(const i32vec3& _coordinates);
private:
};