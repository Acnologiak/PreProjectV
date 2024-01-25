#pragma once

#include "TransformationTools.h"

namespace PreProjectV
{
	struct VoxObj8Bit;

	struct VoxObjChunk8Bit;

	struct VoxObj8BitOctree;

	struct VoxObj16Bit;

	struct VoxObjChunk16Bit;

	//-----------------------------------------------------------------------------------------------

	struct VoxObj8Bit
	{
		map<size_t, shared_ptr<VoxObjChunk8Bit>> chunks;

		bool CreateChunk(i16vec3 _coordinates);
		bool CreateChunk(size_t _key);
		shared_ptr<VoxObjChunk8Bit> GetChunk(i16vec3 _coordinates);
		shared_ptr<VoxObjChunk8Bit> GetChunk(size_t _key);
		bool DeleteChunk(i16vec3 _coordinates);
		bool DeleteChunk(size_t _key);
		bool AddChunk(i16vec3 _coordinates, shared_ptr<VoxObjChunk8Bit> _chunk);
		bool AddChunk(size_t _key, shared_ptr<VoxObjChunk8Bit> _chunk);

		u8 GetVoxel(const i32vec3& _coordinates);
		void SetVoxel(const i32vec3& _coordinates, u8 _color);
		void Extend(shared_ptr<VoxObj8Bit>  _voxObj, i32vec3& _coordinates);

		size_t Save(const string& _path, const string& _name);
		size_t Load(const string& _path, const string& _name);
	};

	struct VoxObjChunk8Bit
	{
		u8 voxels[16][16][16];

		// Keys:
		// 0 - error
		// 1 - ok
		// 2 - empty chunk
		size_t Save(size_t _key, ofstream& _file);
		// Result - <status, key>
		pair<size_t, size_t> Load(ifstream& _file);
	private:
		void GenOctree(VoxObj8BitOctree& _octree);
		void FillOctree(VoxObj8BitOctree& _octree);
		size_t WriteToFile(ofstream& _file, VoxObj8BitOctree& _octree);
		size_t LoadFromFile(ifstream& _file, VoxObj8BitOctree& _octree);
	};

	struct VoxObj8BitOctree
	{
		u16 dataBlockSize{ 0 };

		u8 cStatusL0;
		u8 cStatusL1;
		u8 cStatusL2[2][2][2];
		u8 cStatusL3[4][4][4];

		u8 voxelsL0;
		u8 voxelsL1[2][2][2];
		u8 voxelsL2[4][4][4];
		u8 voxelsL3[8][8][8];
		u8 voxelsL4[16][16][16];
	};
}