#pragma once

#include "VoxObjSurface.h"

namespace PreProjectV
{
	struct Polygon;

	struct Mtl;

	struct PolyObjSurface;

	struct PolyObj;

	//--------------------------------------------------------------------------------------------

	struct Polygon
	{
	public:
		i32vec4 vertices;
		i32vec4 textureCoordinates;
		i32vec4 vertexNormals;
	};

	struct Mtl
	{
	public:
		f32vec3 ambientColor;
		f32vec3 diffuseColor;
		f32vec3 specularColor;
	};

	struct PolyObjSurface
	{
	public:
		vector<Polygon> polygons;
	};

	struct PolyObj
	{
	public:
		vector<f32vec3> vertices;
		vector<f32vec2> textureCoordinates;
		vector<f32vec3> normalVectors;

		map<string, PolyObjSurface> surfaces;

		PolyObj();
		PolyObj(shared_ptr<VoxObjSurface8Bit> _voxObj);

		bool Save(const string& _path, const string& _name);
	private:
		bool SaveObjFile(const string& _path, const string& _name);
		bool SaveMtlFile(const string& _path, const string& _name);
	};
}