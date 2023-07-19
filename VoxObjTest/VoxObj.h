#pragma once

#include <map>
#include <memory>
#include <glm/glm.hpp>

#include "VoxObjChunk.h"
#include "TransformationTools.h"

using std::map;
using std::shared_ptr;
using std::make_shared;



struct VoxObj8Bit
{
public:
	map<size_t, shared_ptr<VoxObjChunk8Bit>> chunks;

	u8 GetVoxel(const i32vec3& _coordinates);
	void SetVoxel(const i32vec3& _coordinates, u8 _color);

	bool CreateChunk(const i16vec3& _coordinates);
	void DeleteChunk(const i16vec3& _coordinates);
	shared_ptr<VoxObjChunk8Bit> GetChunk(const i16vec3& _coordinates);
private:

};