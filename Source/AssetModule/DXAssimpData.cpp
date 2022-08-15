
// TNLIB
#include"Debug.hpp"

// MyFile
#include"DXAssimpData.h"
#include"DirectXUtilityModule/MessageBoxUtility.h"

void DXAssimpData::LoadMeshs() {
	auto nodeCount = m_nodes.size();
	for (int_t i = 0; i < nodeCount; i++) {
		m_meshs.emplace(
			m_nodes[i]->mName,
			AssimpLoader::GetAssimpMesh(m_assimpScene, m_nodes[i])
		);
	}
}

void DXAssimpData::LoadMaterials() {
	auto nodeCount = m_nodes.size();
	for (int_t i = 0; i < nodeCount; i++) {
		m_materials.emplace(
			m_nodes[i]->mName,
			AssimpLoader::GetAssimpMaterial(m_assimpScene, m_nodes[i])
		);
	}
}

void DXAssimpData::Create(const std::string& path, const std::string& resourcePath) {
	Debug::BracketBegin("DXAssimpData::Create");

	m_assimpScene = aiImportFile(
		path.c_str(),
		aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality
	);

	if (m_assimpScene != nullptr) {
		m_path = path;
		m_resourcePath = resourcePath;
		m_rootNode = m_assimpScene->mRootNode;
		m_nodes = AssimpLoader::GetAssimpNode(m_rootNode);
		LoadMeshs();
		LoadMaterials();
		Debug::Log("DXAssimp::Create Method Success");
	}
	else {
		Debug::LogError(
			"DXAssimpData::Create",
			"AssimpLibrary::aiImportFile Method Failed."
		);
#if _DEBUG
		MessageBoxUtility::ErrorMessage(
			"DXAssimpData::Create\n\
			AssimpLibrary::aiImportFile Method Failed."
		);
#endif
	}

	Debug::BracketEnd();
}

const aiScene* DXAssimpData::GetAssimpScene() {
	return m_assimpScene;
}

aiNode* DXAssimpData::GetRootNode() {
	return m_rootNode;
}

std::vector<aiNode*> DXAssimpData::GetNodes() {
	return m_nodes;
}

std::vector<aiMesh*> DXAssimpData::GetMeshs(aiNode* node) {
	return m_meshs[node->mName];
}

std::vector<aiMaterial*> DXAssimpData::GetMaterials(aiNode* node) {
	return m_materials[node->mName];
}

std::string DXAssimpData::GetPath() {
	return m_path;
}

std::string DXAssimpData::GetResourcePath() {
	return m_resourcePath;
}

uint_t DXAssimpData::GetNodeCount() {
	return m_nodes.size();
}

uint_t DXAssimpData::GetMeshCount(aiNode* node) {
	return m_meshs[node->mName].size();
}

uint_t DXAssimpData::GetMaterialCount(aiNode* node) {
	return m_materials[node->mName].size();
}

bool DXAssimpData::IsValid() {
	return (m_assimpScene != nullptr);
}