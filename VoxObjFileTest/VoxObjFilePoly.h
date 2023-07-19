#pragma once

#include <vector>
#include <map>
#include <string>
#include <glm/glm.hpp>

using std::vector;
using std::map;
using std::string;
using glm::vec2;
using glm::vec3;
using glm::ivec4;

namespace VoxObjFilePoly
{
	struct Polygon
	{
		ivec4 vertices;
		ivec4 textureCoordinates;
		ivec4 vertexNormals;
	};

	struct ObjSurface
	{
		vector<Polygon> polygons;
	};

	struct Obj
	{
		vector<vec3> vertices;
		vector<vec2> textureCoordinates;
		vector<vec3> vertexNormals;
		bool smoothShading{ false };

		map<string, ObjSurface> surfaces;
	};
}