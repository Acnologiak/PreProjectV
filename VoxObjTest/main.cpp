#include <iostream>
#include "VoxObj.h"
#include "VoxObjSurface.h"

using namespace std;
using namespace glm;


int main()
{
	ivec3 coordinates_x{ 1, 2, 3 }, coordinates_y{ 3, 2, 1 }, coordinates_z{ 0, 0, 0 };

	VoxObj testVoxObj;

	for (int i = -32; i <= 32; i++)
	{
		for (int j = -32; j <= 32; j++)
		{
			for (int k = -32; k <= 32; k++)
			{
				if ((i * i + j * j + k * k) < 768)
				{
					testVoxObj.SetVoxel({ i, j, k }, 1);
				}
			}
		}
	}

	/*for (auto& i : testVoxObj.voxObjChunks)
	{
		i16vec3 value = SizeTToI16vec3(i.first);
		cout << value.x << " " << value.y << " " << value.z << endl;

		for (int x = 0; x < CHUNK_SIZE; x++)
		{
			for (int y = 0; y < CHUNK_SIZE; y++)
			{
				for (int z = 0; z < CHUNK_SIZE; z++)
				{
					cout << (int)i.second->GetVoxel({ x, y, z }) << endl;
				}
			}
		}
	}*/


	//auto testVoxObjSurface = make_shared<VoxObjSurface>(testVoxObj);
	//auto testObjFile = GetObj(testVoxObjSurface);

	//cout << testVoxObjSurface.surfaces.size() << endl;


	/*testVoxObj.CreateObjChunk(coordinates_x);
	testVoxObj.CreateObjChunk(coordinates_y);
	testVoxObj.CreateObjChunk(coordinates_z);

	auto value = testVoxObj.GetObjChunk(coordinates_x);*/



	return 0;
}