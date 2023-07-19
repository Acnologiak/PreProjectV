#include "ObjReader.h"

namespace ObjReader
{
	ObjFile::ObjFile()
	{
	}

	ObjFile::ObjFile(const char* _fullPath, bool _mtlRead)
	{
		string mtllib;
		string fullPath{ _fullPath };

		auto pointer1 = fullPath.rfind("/");
		pointer1++;

		string path{ fullPath.substr(0, pointer1) };

		auto info = ReadObjFile(fullPath);

		if (_mtlRead == true)
		{
			mtllib = info.second;
			fullPath = path + mtllib;
			ReadMtlFile(fullPath);
		}
	}

	bool ObjFile::Save(const char* _fullPath, bool _mtlWrite)
	{
		string fullPath{ _fullPath };

		auto pointer1 = fullPath.rfind(".");
		auto pointer2 = fullPath.rfind("/", pointer1);
		pointer1;
		pointer2++;

		string mtlName{ fullPath.substr(pointer2, pointer1 - pointer2) + string { ".mtl" } };
		string path{ fullPath.substr(0, pointer2) };

		SaveObjFile(fullPath, mtlName);

		if (_mtlWrite == true)
		{
			fullPath = string{ path } + string{ mtlName };
			SaveMtlFile(fullPath);
		}

		return true;
	}

	bool ObjFile::AddObj(const string& _name, shared_ptr<Obj> _obj)
	{
		if (_obj == nullptr || _name.size() == 0)
		{
			return false;
		}
		else
		{
			if (objs.find(_name) != objs.end())
			{
				return false;
			}
			else
			{
				//Не використовую try_emplace оскільки дана функція завжди повертає ітератор на елемен і не відомо чи виконалась операція зі вставлення об'єкту.
				objs.emplace(_name, _obj);
				return true;
			}
		}
	}

	bool ObjFile::RenameObj(const string& _name1, const string& _name2)
	{
		if (_name1.size() == 0 || _name2.size() == 0)
		{
			return false;
		}
		else
		{
			auto it = objs.find(_name1);
			if (it == objs.end())
			{
				return false;
			}
			else
			{
				objs.emplace(_name2, move(it->second));
				objs.erase(it);
			}
		}
	}

	bool ObjFile::DeleteObj(const string& _name)
	{
		if (_name.size() == 0)
		{
			return false;
		}
		else
		{
			return objs.erase(_name);
		}
	}

	size_t ObjFile::GetNumberObjs()
	{
		return objs.size();
	}

	vector<string> ObjFile::GetNamesObj()
	{
		vector<string> names;
		names.reserve(objs.size());

		for (auto const& i : objs) {
			names.push_back(i.first);
		}

		return move(names);
	}

	shared_ptr<Obj> ObjFile::GetObj(const string& _name)
	{
		if (_name.size() == 0)
		{
			return nullptr;
		}
		else
		{
			auto it = objs.find(_name);
			if (it == objs.end())
			{
				return nullptr;
			}
			else
			{
				return it->second;
			}
		}
	}

	map<string, shared_ptr<Obj>>::iterator ObjFile::GetBeginObjsMap()
	{
		return objs.begin();
	}

	map<string, shared_ptr<Obj>>::iterator ObjFile::GetEndObjsMap()
	{
		return objs.end();
	}

	bool ObjFile::AddMtl(const string& _name, shared_ptr<Mtl> _mtl)
	{
		if (_mtl == nullptr || _name.size() == 0)
		{
			return false;
		}
		else
		{
			if (mtls.find(_name) != mtls.end())
			{
				return false;
			}
			else
			{
				//Не використовую try_emplace оскільки дана функція завжди повертає ітератор на елемен і не відомо чи виконалась операція зі вставлення об'єкту.
				mtls.emplace(_name, _mtl);
				return true;
			}
		}
	}

	bool ObjFile::RenameMtl(const string& _name1, const string& _name2)
	{
		if (_name1.size() == 0 || _name2.size() == 0)
		{
			return false;
		}
		else
		{
			auto it = mtls.find(_name1);
			if (it == mtls.end())
			{
				return false;
			}
			else
			{
				mtls.emplace(_name2, move(it->second));
				mtls.erase(it);
			}
		}
	}

	bool ObjFile::DeleteMtl(const string& _name)
	{
		if (_name.size() == 0)
		{
			return false;
		}
		else
		{
			return mtls.erase(_name);
		}
	}

	size_t ObjFile::GetNumberMtls()
	{
		return mtls.size();
	}

	vector<string> ObjFile::GetNamesMtl()
	{
		vector<string> names;
		names.reserve(mtls.size());

		for (auto const& i : mtls) {
			names.push_back(i.first);
		}

		return move(names);
	}

	shared_ptr<Mtl> ObjFile::GetMtl(const string& _name)
	{
		if (_name.size() == 0)
		{
			return nullptr;
		}
		else
		{
			auto it = mtls.find(_name);
			if (it == mtls.end())
			{
				return nullptr;
			}
			else
			{
				return it->second;
			}
		}
	}

	map<string, shared_ptr<Mtl>>::iterator ObjFile::GetBeginMtlsMap()
	{
		return mtls.begin();
	}

	map<string, shared_ptr<Mtl>>::iterator ObjFile::GetEndMtlsMap()
	{
		return mtls.end();
	}

	pair<bool, string> ObjFile::ReadObjFile(const string& _fullPath)
	{
		char line[256];
		pair<string, shared_ptr<Obj>> obj{ "", make_shared<Obj>() };
		pair<string, shared_ptr<ObjSurface>> surface{ "", make_shared<ObjSurface>() };
		pair<bool, string> mtllibInfo{ false, "" };
		string mtllib;

		size_t verticesShift{ 1 };
		size_t vertexNormalsShift{ 1 };
		size_t textureCoordinatesShift{ 1 };

		ifstream objFile(_fullPath);
		if (!objFile.is_open())
		{
			objFile.close();
			return mtllibInfo;
		}
		else
		{
			while (objFile.getline(line, 256))
			{
				auto tokens = SplitString(line, " ");

				if (tokens.size() > 1)
				{
					if (tokens[0] == "mtllib")
					{
						mtllibInfo.first = true;
						mtllibInfo.second = SplitString(line, " ", 1)[1];
					}
					else if (tokens[0] == "o")
					{
						if (obj.first.size() == 0)
						{
							obj.first = SplitString(line, " ", 1)[1];
						}
						else
						{
							if (surface.second->triangleVertices.size() != 0 || surface.second->triangleVertexNormals.size() || surface.second->triangleTextureCoordinates.size())
							{
								obj.second->AddObjSurface(move(surface.first), surface.second);
								surface.second = make_shared<ObjSurface>();
							}
							if (obj.second->vertices.size() != 0 || obj.second->textureCoordinates.size() != 0 || obj.second->vertexNormals.size() != 0)
							{
								verticesShift += obj.second->vertices.size();
								vertexNormalsShift += obj.second->vertexNormals.size();
								textureCoordinatesShift += obj.second->textureCoordinates.size();

								AddObj(move(obj.first), obj.second);
								obj.second = make_shared<Obj>();
							}

							obj.first = SplitString(line, " ", 1)[1];
						}
					}
					else if (tokens[0] == "v")
					{
						obj.second->vertices.emplace_back(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()));
					}
					else if (tokens[0] == "vn")
					{
						obj.second->vertexNormals.emplace_back(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()));
					}
					else if (tokens[0] == "vt")
					{
						obj.second->textureCoordinates.emplace_back(atof(tokens[1].c_str()), atof(tokens[2].c_str()));
					}
					else if (tokens[0] == "s")
					{
						if (tokens[1] == "1")
						{
							obj.second->smoothShading = true;
						}
					}
					else if (tokens[0] == "usemtl")
					{
						if (surface.first.size() == 0)
						{
							surface.first = SplitString(line, " ", 1)[1];
						}
						else
						{
							if (surface.second->triangleVertices.size() != 0 || surface.second->triangleVertexNormals.size() || surface.second->triangleTextureCoordinates.size())
							{
								obj.second->AddObjSurface(move(surface.first), surface.second);
								surface.second = make_shared<ObjSurface>();
							}
							surface.first = SplitString(line, " ", 1)[1];
						}
					}
					else if (tokens[0] == "f")
					{
						size_t numberPoints = tokens.size() - 1;
						vector<ivec3> points;
						for (size_t i = 0; i < numberPoints; i++)
						{
							size_t pointer_1{ 0 };
							size_t pointer_2{ 0 };
							ivec3 value{ 0, 0, 0 };

							pointer_2 = tokens[i + 1].find("/");
							if (pointer_2 == string::npos)
							{
								pointer_2 = tokens[i + 1].size();
								value.x = atoi(tokens[i + 1].substr(pointer_1, pointer_2 - pointer_1).c_str());

								points.emplace_back(value);

								continue;
							}
							else
							{
								value.x = atoi(tokens[i + 1].substr(pointer_1, pointer_2 - pointer_1).c_str());
							}

							pointer_1 = pointer_2 + 1;
							pointer_2 = tokens[i + 1].find("/", pointer_1);
							if (pointer_2 == string::npos)
							{
								pointer_2 = tokens[i + 1].size();
								value.y = atoi(tokens[i + 1].substr(pointer_1, pointer_2 - pointer_1).c_str());

								points.emplace_back(value);

								continue;
							}
							else
							{
								value.y = atoi(tokens[i + 1].substr(pointer_1, pointer_2 - pointer_1).c_str());
							}

							pointer_1 = pointer_2 + 1;
							pointer_2 = tokens[i + 1].size();
							value.z = atoi(tokens[i + 1].substr(pointer_1, pointer_2 - pointer_1).c_str());

							points.emplace_back(value);
						}

						ivec3 numbers{ 0, 1, 2 };
						for (size_t i = 2; i < numberPoints; i++)
						{
							if (i % 2 == 0)
							{
								surface.second->triangleVertices.emplace_back(
									points[numbers.x].x - verticesShift,
									points[numbers.y].x - verticesShift,
									points[numbers.z].x - verticesShift
								);

								surface.second->triangleTextureCoordinates.emplace_back(
									points[numbers.x].y - textureCoordinatesShift,
									points[numbers.y].y - textureCoordinatesShift,
									points[numbers.z].y - textureCoordinatesShift
								);

								surface.second->triangleVertexNormals.emplace_back(
									points[numbers.x].z - vertexNormalsShift,
									points[numbers.y].z - vertexNormalsShift,
									points[numbers.z].z - vertexNormalsShift
								);

								numbers.y = i + 1;
							}
							else
							{
								surface.second->triangleVertices.emplace_back(
									points[numbers.y].x - verticesShift,
									points[numbers.x].x - verticesShift,
									points[numbers.z].x - verticesShift
								);

								surface.second->triangleTextureCoordinates.emplace_back(
									points[numbers.y].y - textureCoordinatesShift,
									points[numbers.x].y - textureCoordinatesShift,
									points[numbers.z].y - textureCoordinatesShift
								);

								surface.second->triangleVertexNormals.emplace_back(
									points[numbers.y].z - vertexNormalsShift,
									points[numbers.x].z - vertexNormalsShift,
									points[numbers.z].z - vertexNormalsShift
								);

								numbers.z = i + 1;
							}
						}
					}
				}
			}
			if (surface.second->triangleVertices.size() != 0 || surface.second->triangleVertexNormals.size() || surface.second->triangleTextureCoordinates.size())
			{
				obj.second->AddObjSurface(move(surface.first), surface.second);
				surface.second = make_shared<ObjSurface>();
			}
			if (obj.second->vertices.size() != 0 || obj.second->textureCoordinates.size() != 0 || obj.second->vertexNormals.size() != 0)
			{
				AddObj(move(obj.first), obj.second);
				obj.second = make_shared<Obj>();
			}
		}
		objFile.close();

		return mtllibInfo;
	}

	bool ObjFile::ReadMtlFile(const string& _fullPath)
	{
		char line[256];
		pair<string, shared_ptr<Mtl>> mtl{ "", make_shared<Mtl>() };

		ifstream mtlFile(_fullPath);
		if (!mtlFile.is_open())
		{
			mtlFile.close();
			return false;
		}
		else
		{
			while (mtlFile.getline(line, 256))
			{
				auto tokens = SplitString(line, " ");

				if (tokens.size() > 1)
				{
					if (tokens[0] == "newmtl")
					{
						if (mtl.first.size() == 0)
						{
							mtl.first = SplitString(line, " ", 1)[1];
						}
						else
						{
							AddMtl(move(mtl.first), mtl.second);
							mtl.first = SplitString(line, " ", 1)[1];
							mtl.second = make_shared<Mtl>();
						}
					}
					else if (tokens[0] == "Ns")
					{
						mtl.second->specularExponent = atof(tokens[1].c_str());
					}
					else if (tokens[0] == "Ni")
					{
						mtl.second->opticalDensity = atof(tokens[1].c_str());
					}
					else if (tokens[0] == "d")
					{
						mtl.second->transparent = atof(tokens[1].c_str());
					}
					else if (tokens[0] == "illum")
					{
						mtl.second->illuminationModels = atoi(tokens[1].c_str());
					}
					else if (tokens[0] == "Ka")
					{
						mtl.second->ambientColor.x = atof(tokens[1].c_str());
						mtl.second->ambientColor.y = atof(tokens[2].c_str());
						mtl.second->ambientColor.z = atof(tokens[3].c_str());
					}
					else if (tokens[0] == "Kd")
					{
						mtl.second->diffuseColor.x = atof(tokens[1].c_str());
						mtl.second->diffuseColor.y = atof(tokens[2].c_str());
						mtl.second->diffuseColor.z = atof(tokens[3].c_str());
					}
					else if (tokens[0] == "Ks")
					{
						mtl.second->specularColor.x = atof(tokens[1].c_str());
						mtl.second->specularColor.y = atof(tokens[2].c_str());
						mtl.second->specularColor.z = atof(tokens[3].c_str());
					}
					else if (tokens[0] == "Ke")
					{
						mtl.second->emissiveCoeficient.x = atof(tokens[1].c_str());
						mtl.second->emissiveCoeficient.y = atof(tokens[2].c_str());
						mtl.second->emissiveCoeficient.z = atof(tokens[3].c_str());
					}
				}
			}
			if (mtl.first.size() != 0)
			{
				AddMtl(move(mtl.first), mtl.second);
				mtl.second = make_shared<Mtl>();
			}
		}

		return true;
	}

	bool ObjFile::SaveObjFile(const string& _fullPath, const string& _name)
	{
		size_t verticesShift{ 1 };
		size_t vertexNormalsShift{ 1 };
		size_t textureCoordinatesShift{ 1 };

		std::ofstream objFile(_fullPath.c_str());
		if (!objFile.is_open())
		{
			objFile.close();
			return false;
		}
		else
		{
			if (mtls.size() != 0)
			{
				objFile << "mtllib " << _name << "\n";
			}

			for (const auto& i : objs)
			{
				if (i.second->vertices.size() == 0)
				{
					continue;
				}


				if (i.first.size() != 0)
				{
					objFile << "o " << i.first << "\n";
				}
				for (const auto& j : i.second->vertices)
				{
					objFile << "v " << j.x << " " << j.y << " " << j.z << "\n";
				}
				for (const auto& j : i.second->vertexNormals)
				{
					objFile << "vn " << j.x << " " << j.y << " " << j.z << "\n";
				}
				for (const auto& j : i.second->textureCoordinates)
				{
					objFile << "vt " << j.x << " " << j.y << "\n";
				}

				if (i.second->smoothShading == true)
				{
					objFile << "s 1\n";
				}
				else
				{
					objFile << "s off\n";
				}

				for (const auto& j : i.second->surfaces)
				{
					if (j.first.size() != 0)
					{
						objFile << "usemtl " << j.first << "\n";
					}

					if (
						j.second->triangleVertices.size() != 0 &&
						j.second->triangleTextureCoordinates.size() != 0 &&
						j.second->triangleVertexNormals.size() != 0 &&
						j.second->triangleVertices.size() == j.second->triangleTextureCoordinates.size() &&
						j.second->triangleTextureCoordinates.size() == j.second->triangleVertexNormals.size()
						)
					{
						for (size_t k = 0; k < j.second->triangleVertices.size(); k++)
						{
							objFile << "f  " <<
								j.second->triangleVertices[k].x + verticesShift << "/" <<
								j.second->triangleTextureCoordinates[k].x + textureCoordinatesShift << "/" <<
								j.second->triangleVertexNormals[k].x + vertexNormalsShift << " " <<
								j.second->triangleVertices[k].y + verticesShift << "/" <<
								j.second->triangleTextureCoordinates[k].y + textureCoordinatesShift << "/" <<
								j.second->triangleVertexNormals[k].y + vertexNormalsShift << " " <<
								j.second->triangleVertices[k].z + verticesShift << "/" <<
								j.second->triangleTextureCoordinates[k].z + textureCoordinatesShift << "/" <<
								j.second->triangleVertexNormals[k].z + vertexNormalsShift << "\n";
						}
					}
					else if (
						j.second->triangleVertices.size() != 0 &&
						j.second->triangleTextureCoordinates.size() != 0 &&
						j.second->triangleVertices.size() == j.second->triangleTextureCoordinates.size()
						)
					{
						for (size_t k = 0; k < j.second->triangleVertices.size(); k++)
						{
							objFile << "f  " <<
								j.second->triangleVertices[k].x + verticesShift << "/" <<
								j.second->triangleTextureCoordinates[k].x + textureCoordinatesShift << " " <<
								j.second->triangleVertices[k].y + verticesShift << "/" <<
								j.second->triangleTextureCoordinates[k].y + textureCoordinatesShift << " " <<
								j.second->triangleVertices[k].z + verticesShift << "/" <<
								j.second->triangleTextureCoordinates[k].z + textureCoordinatesShift << "\n";
						}
					}
					else if (
						j.second->triangleVertices.size() != 0 &&
						j.second->triangleVertexNormals.size() != 0 &&
						j.second->triangleVertices.size() == j.second->triangleVertexNormals.size()
						)
					{
						for (size_t k = 0; k < j.second->triangleVertices.size(); k++)
						{
							objFile << "f  " <<
								j.second->triangleVertices[k].x + verticesShift << "//" <<
								j.second->triangleVertexNormals[k].x + vertexNormalsShift << " " <<
								j.second->triangleVertices[k].y + verticesShift << "//" <<
								j.second->triangleVertexNormals[k].y + vertexNormalsShift << " " <<
								j.second->triangleVertices[k].z + verticesShift << "//" <<
								j.second->triangleVertexNormals[k].z + vertexNormalsShift << "\n";
						}
					}
					else if (
						j.second->triangleVertices.size() != 0
						)
					{
						for (size_t k = 0; k < j.second->triangleVertices.size(); k++)
						{
							objFile << "f  " <<
								j.second->triangleVertices[k].x + verticesShift << " " <<
								j.second->triangleVertices[k].y + verticesShift << " " <<
								j.second->triangleVertices[k].z + verticesShift << "\n";
						}
					}
				}

				verticesShift += i.second->vertices.size();
				vertexNormalsShift += i.second->vertexNormals.size();
				textureCoordinatesShift += i.second->textureCoordinates.size();
			}
		}
		objFile.close();

		return true;
	}

	bool ObjFile::SaveMtlFile(const string& _fullPath)
	{
		if (mtls.size() != 0)
		{
			std::ofstream mtlFile(_fullPath.c_str());
			if (!mtlFile.is_open())
			{
				mtlFile.close();
				return false;
			}
			else
			{
				for (const auto& i : mtls)
				{
					mtlFile << "newmtl " << i.first << "\n";
					mtlFile << "Ns " << i.second->specularExponent << "\n";
					mtlFile << "Ka " << i.second->ambientColor.x << " " << i.second->ambientColor.y << " " << i.second->ambientColor.z << "\n";
					mtlFile << "Kd " << i.second->diffuseColor.x << " " << i.second->diffuseColor.y << " " << i.second->diffuseColor.z << "\n";
					mtlFile << "Ks " << i.second->specularColor.x << " " << i.second->specularColor.y << " " << i.second->specularColor.z << "\n";
					mtlFile << "Ke " << i.second->emissiveCoeficient.x << " " << i.second->emissiveCoeficient.y << " " << i.second->emissiveCoeficient.z << "\n";
					mtlFile << "Ni " << i.second->opticalDensity << "\n";
					mtlFile << "d " << i.second->transparent << "\n";
					mtlFile << "illum " << i.second->illuminationModels << "\n";
					mtlFile << "\n";
				}
			}
			mtlFile.close();
		}

		return true;
	}

	bool Obj::AddObjSurface(const string& _name, shared_ptr<ObjSurface> _objSurface)
	{
		if (_objSurface == nullptr || _name.size() == 0)
		{
			return false;
		}
		else
		{
			if (surfaces.find(_name) != surfaces.end())
			{
				return false;
			}
			else
			{
				//Не використовую try_emplace оскільки дана функція завжди повертає ітератор на елемен і не відомо чи виконалась операція зі вставлення об'єкту.
				surfaces.emplace(_name, _objSurface);
				return true;
			}
		}
	}

	bool Obj::RenameObjSurface(const string& _name1, const string& _name2)
	{
		if (_name1.size() == 0 || _name2.size() == 0)
		{
			return false;
		}
		else
		{
			auto it = surfaces.find(_name1);
			if (it == surfaces.end())
			{
				return false;
			}
			else
			{
				surfaces.emplace(_name2, move(it->second));
				surfaces.erase(it);
			}
		}
	}

	bool Obj::DeleteObjSurface(const string& _name)
	{
		if (_name.size() == 0)
		{
			return false;
		}
		else
		{
			return surfaces.erase(_name);
		}
	}

	size_t Obj::GetNumberObjSurfaces()
	{
		return surfaces.size();
	}

	vector<string> Obj::GetNamesObjSurface()
	{
		vector<string> names;
		names.reserve(surfaces.size());

		for (auto const& i : surfaces) {
			names.push_back(i.first);
		}

		return move(names);
	}

	shared_ptr<ObjSurface> Obj::GetObjSurface(const string& _name)
	{
		if (_name.size() == 0)
		{
			return nullptr;
		}
		else
		{
			auto it = surfaces.find(_name);
			if (it == surfaces.end())
			{
				return nullptr;
			}
			else
			{
				return it->second;
			}
		}
	}

	map<string, shared_ptr<ObjSurface>>::iterator Obj::GetBeginObjSurfacesMap()
	{
		return surfaces.begin();
	}

	map<string, shared_ptr<ObjSurface>>::iterator Obj::GetEndObjSurfacesMap()
	{
		return surfaces.end();
	}

	vector<string> SplitString(const string& _str, const string& _delim, size_t _numberDivisions)
	{
		vector<string> result;
		size_t startIndex = 0;

		size_t n{ 0 };
		for (size_t found = _str.find(_delim); found != string::npos; found = _str.find(_delim, startIndex))
		{
			if (n == _numberDivisions) break;
			result.emplace_back(_str.begin() + startIndex, _str.begin() + found);
			startIndex = found + _delim.size();
			n++;
		}
		if (startIndex != _str.size())
			result.emplace_back(_str.begin() + startIndex, _str.end());

		return result;
	}
}

