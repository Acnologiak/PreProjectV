#include <iostream>

#include "VoxObj.h"

using namespace std;


void main()
{
	shared_ptr<VoxObj8Bit> voxObj = make_shared<VoxObj8Bit>();

	for (int i = -64; i <= 64; i++)
	{
		for (int j = -64; j <= 64; j++)
		{
			for (int k = 0; k <= 64; k++)
			{
				if ((i * i + j * j + k * k) < 756)
				{
					voxObj->SetVoxel({ i, j, k }, 1);
				}
			}
		}
	}

	cout << voxObj->chunks.size() << endl;
}