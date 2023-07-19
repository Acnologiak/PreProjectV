/*
	Це проста тестова бібліотека для читання і запису моделей в OBJ форматі.
	Яка була створена для тренування і саморозвитку.
	Тут може бути велика кількість багів і також може бути необхідність в добавленні нового функціоналу.

	Дата останьої зміни 05.03.2023
	Версія 0.6
*/

#pragma once

#include <deque>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include "glm/glm.hpp"

using std::string;
using std::deque;
using std::vector;
using std::map;
using std::ifstream;
using std::pair;
using std::move;
using std::make_shared;
using std::shared_ptr;
using glm::vec2;
using glm::vec3;
using glm::ivec3;

//----------------------------------------------------------------------------------------------------------------------------------------

namespace ObjReader
{
	struct Mtl;

	struct ObjSurface;

	struct Obj;

	struct ObjFile;

	//----------------------------------------------------------------------------------------------------------------------------------------

	struct Mtl
	{
		//Ka
		vec3 ambientColor;
		//Kd 
		vec3 diffuseColor;
		//Ks
		vec3 specularColor;
		//Ke
		vec3 emissiveCoeficient;
		//Ns
		float specularExponent;
		//Ni 
		float opticalDensity{ 1.45f };
		//d
		float transparent{ 1.0f };
		//illum
		size_t illuminationModels{ 2 };
	};

	//----------------------------------------------------------------------------------------------------------------------------------------

	struct ObjSurface
	{
		deque<ivec3> triangleVertices;
		deque<ivec3> triangleVertexNormals;
		deque<ivec3> triangleTextureCoordinates;
	};

	//----------------------------------------------------------------------------------------------------------------------------------------

	struct Obj
	{
	public:
		friend ObjFile;

		deque<vec3> vertices;
		deque<vec2> textureCoordinates;
		deque<vec3> vertexNormals;
		bool smoothShading{ false };

		bool AddObjSurface(const string& _name, shared_ptr<ObjSurface> _objSurface);
		bool RenameObjSurface(const string& _name1, const string& _name2);
		bool DeleteObjSurface(const string& _name);
		size_t GetNumberObjSurfaces();
		vector<string> GetNamesObjSurface();
		shared_ptr<ObjSurface> GetObjSurface(const string& _name);
		map<string, shared_ptr<ObjSurface>>::iterator GetBeginObjSurfacesMap();
		map<string, shared_ptr<ObjSurface>>::iterator GetEndObjSurfacesMap();
	private:
		//key - MTL name
		map<string, shared_ptr<ObjSurface>> surfaces;
	};

	//----------------------------------------------------------------------------------------------------------------------------------------

	struct ObjFile
	{
	public:
		ObjFile();
		ObjFile(const char* _fullPath, bool _mtlRead = false);
		//ObjFile(const char* _path, const char* _fileName, bool _mtlRead = false);

		bool Save(const char* _fullPath, bool _mtlWrite = false);
		//bool Save(const char* _path, const char* _fileName, bool _mtlWrite = false);

		bool AddObj(const string& _name, shared_ptr<Obj> _obj);
		bool RenameObj(const string& _name1, const string& _name2);
		bool DeleteObj(const string& _name);
		size_t GetNumberObjs();
		vector<string> GetNamesObj();
		shared_ptr<Obj> GetObj(const string& _name);
		map<string, shared_ptr<Obj>>::iterator GetBeginObjsMap();
		map<string, shared_ptr<Obj>>::iterator GetEndObjsMap();

		bool AddMtl(const string& _name, shared_ptr<Mtl> _mtl);
		bool RenameMtl(const string& _name1, const string& _name2);
		bool DeleteMtl(const string& _name);
		size_t GetNumberMtls();
		vector<string> GetNamesMtl();
		shared_ptr<Mtl> GetMtl(const string& _name);
		map<string, shared_ptr<Mtl>>::iterator GetBeginMtlsMap();
		map<string, shared_ptr<Mtl>>::iterator GetEndMtlsMap();
	private:
		//key - OBJ name
		map<string, shared_ptr<Obj>> objs;
		//key - MTL name
		map<string, shared_ptr<Mtl>> mtls;

		pair<bool, string> ReadObjFile(const string& fullPath);
		bool ReadMtlFile(const string& _fullPath);
		bool SaveObjFile(const string& _fullPath, const string& _name);
		bool SaveMtlFile(const string& _fullPath);
	};

	//----------------------------------------------------------------------------------------------------------------------------------------

	vector<string> SplitString(const string& _str, const string& _delim, size_t _numberDivisions = -1);
}






