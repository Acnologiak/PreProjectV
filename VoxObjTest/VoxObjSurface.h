#pragma once

#include <vector>
#include <iostream>

#include "VoxObj.h"
#include "BaseValues.h"
#include "ObjReader.h"

using std::vector;
using std::pair;
using std::make_pair;
using glm::u16;

using namespace ObjReader;

class VoxObjChunkSurface
{
public:
	vector<pair<i16vec3, u16>> surface;
private:
};

class VoxObjSurface
{
public:
	VoxObjSurface();
	VoxObjSurface(const VoxObj& _voxObj);

	map<size_t, shared_ptr<VoxObjChunkSurface>> surfaces;
private:

};

class MiniVoxChunkSpace
{
public:
	MiniVoxChunkSpace(
		shared_ptr<VoxObjChunk> _centralChunk,
		shared_ptr<VoxObjChunk> _xpChunk,
		shared_ptr<VoxObjChunk> _xnChunk,
		shared_ptr<VoxObjChunk> _ypChunk,
		shared_ptr<VoxObjChunk> _ynChunk,
		shared_ptr<VoxObjChunk> _zpChunk,
		shared_ptr<VoxObjChunk> _znChunk
	);

	u16 GetVoxel(const i16vec3& _coordinates);
private:
	VoxObjChunk* centralChunk;
	VoxObjChunk* xpChunk{ nullptr };
	VoxObjChunk* xnChunk{ nullptr };
	VoxObjChunk* ypChunk{ nullptr };
	VoxObjChunk* ynChunk{ nullptr };
	VoxObjChunk* zpChunk{ nullptr };
	VoxObjChunk* znChunk{ nullptr };
};

shared_ptr<VoxObjChunkSurface> VoxObjChunkToVoxObjChunkSurface(MiniVoxChunkSpace& _space);

shared_ptr<ObjFile> GetObj(shared_ptr<VoxObjSurface> _voxObjSurface);