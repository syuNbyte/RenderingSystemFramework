#pragma once

// STL
#include<string>
#include<vector>
#include<unordered_map>

// TNLIB
#include"SafetyDataType.h"

// MyFile
#include"DXAssimpStringHash.h"
#include"AssimpLoader.hpp"

class DXAssimpData {
private:
	const aiScene* m_assimpScene;
	aiNode* m_rootNode;
	std::vector<aiNode*> m_nodes;
	std::unordered_map<aiString, std::vector<aiMesh*>> m_meshs;
	std::unordered_map<aiString, std::vector<aiMaterial*>> m_materials;
private:
	std::string m_path;
	std::string m_resourcePath;
private:
	void LoadMeshs();
	void LoadMaterials();
public:
	void Create(const std::string& path, const std::string& resourcePath);
public:
	const aiScene* GetAssimpScene();
	aiNode* GetRootNode();
	std::vector<aiNode*> GetNodes();
	std::vector<aiMesh*> GetMeshs(aiNode* node);
	std::vector<aiMaterial*> GetMaterials(aiNode* node);
	std::string GetPath();
	std::string GetResourcePath();
	uint_t GetNodeCount();
	uint_t GetMeshCount(aiNode* node);
	uint_t GetMaterialCount(aiNode* node);
	bool IsValid();
};