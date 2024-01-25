#include "VoxObjSurface.h"
#include "PolyObj.h"
#include "VoxObjFuncs.h"

using namespace PreProjectV;

int main()
{
	auto voxObj1 = make_shared<VoxObj8Bit>();
	auto voxObj2 = make_shared<VoxObj8Bit>();

	i32vec3 position{ 0, 0, 0 };
	size_t radius = 15;
	u8 color = 1;

	AddSphereToVoxObj8Bit(voxObj1, position, radius, color);
	AddSphereToVoxObj8Bit(voxObj2, position - i32vec3{0, 0, 30}, radius, color);

	voxObj1->Extend(voxObj2, position);

	voxObj1->Save("Objs/", "test");

	auto voxObj3 = make_shared<VoxObj8Bit>();
	voxObj3->Load("Objs/", "test");

	auto voxObjSurface = make_shared<VoxObjSurface8Bit>(voxObj3);

	auto polyObj = make_shared<PolyObj>(voxObjSurface);
	polyObj->Save("Objs/", "test");

	return 0;
}