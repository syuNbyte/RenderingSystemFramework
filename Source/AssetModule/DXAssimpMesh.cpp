
// TNLIB
#include"Debug.hpp"

// MyFile
#include"DXAssimpMesh.h"
#include"DirectXUtilityModule/MessageBoxUtility.h"

void DXAssimpSubMeshMaterial::CreateMaterials(aiMaterial* material) {
	m_materials.emplace(
		DXAssimpDataType::MaterialType::Diffuse,
		AssimpLoader::Color::GetDiffuse(material)
	);
	m_materials.emplace(
		DXAssimpDataType::MaterialType::Specular,
		AssimpLoader::Color::GetSpecular(material)
	);
	m_materials.emplace(
		DXAssimpDataType::MaterialType::Ambient,
		AssimpLoader::Color::GetAmbient(material)
	);
	m_materials.emplace(
		DXAssimpDataType::MaterialType::Emissive,
		AssimpLoader::Color::GetEmissive(material)
	);
}

void DXAssimpSubMeshMaterial::CreateTexture(aiMaterial* material, DXAssimpData& assimpData, DirectXDevice* device, DirectXDeviceContext* context) {
	constexpr std::size_t TextureTypeMax = DXAssimpDataType::Texture::Type::Unknown;

	for (int_t i = 0; i < TextureTypeMax; i++) {
		auto type = static_cast<aiTextureType>(i);
		auto typeNum = static_cast<DXAssimpDataType::Texture::Type>(i);
		auto texCount = material->GetTextureCount(type);
		m_textures.emplace(typeNum, std::vector<DXShaderResourceView>());
		m_textures[typeNum].resize(texCount);

		Debug::BracketBegin(DXAssimpDataType::GetTextureTypeName(typeNum));
		Debug::Log("Count[", texCount, "]");
		Debug::Log("Type[", DXAssimpDataType::GetTextureTypeName(typeNum), "]");

		for (int_t c = 0; c < texCount; c++) {
			DXShaderResourceView srv;
			auto path = AssimpLoader::GetAssimpTextureFileName(material, type, c);
			auto respath = assimpData.GetResourcePath() + path;

			Debug::Log("Path[", path, "]");
			Debug::Log("ResourcePath[", respath, "]");
			{
				Debug::SetDengerLogTimeStopFlag(false);
				MessageBoxUtility::SetEnableFlag(false);

				srv.SetFilePath(path);
				srv.CreateTextureFromFile(device, context);

				MessageBoxUtility::SetEnableFlag(true);
				Debug::SetDengerLogTimeStopFlag(true);
			}
			{
				if(!srv.HasTexture()) {
					srv.SetFilePath(respath);
					srv.CreateTextureFromFile(device, context);
				}
			}
			{
				if (srv.HasTexture()) {
					m_textures[typeNum][c] = srv;
				}
			}
		}

		Debug::BracketEnd();
	}
}

DXColor DXAssimpSubMeshMaterial::GetMaterial(DXAssimpDataType::MaterialType type) {
	return m_materials[type];
}

std::vector<DXShaderResourceView> DXAssimpSubMeshMaterial::GetTextures(DXAssimpDataType::Texture::Type type) {
	return m_textures[type];
}

DXColor& DXAssimpSubMeshMaterial::GetMaterialRef(DXAssimpDataType::MaterialType type) {
	return m_materials[type];
}

std::vector<DXShaderResourceView>& DXAssimpSubMeshMaterial::GetTexturesRef(DXAssimpDataType::Texture::Type type) {
	return m_textures[type];
}

std::size_t DXAssimpSubMeshMaterial::GetMaterialCount() {
	return m_materials.size();
}

std::size_t DXAssimpSubMeshMaterial::GetTextureTypeCount(DXAssimpDataType::Texture::Type type) {
	return m_textures[type].size();
}

void DXAssimpSubMesh::CreatePositions(aiMesh* mesh) {
	m_positions = AssimpLoader::GetAssimpVertexPositions(mesh);
}

void DXAssimpSubMesh::CreateNormals(aiMesh* mesh) {
	m_normals = AssimpLoader::GetAssimpVertexNormals(mesh);
}

void DXAssimpSubMesh::CreateTangents(aiMesh* mesh) {
	m_tangents = AssimpLoader::GetAssimpVertexTangents(mesh);
}

void DXAssimpSubMesh::CreateBiTangents(aiMesh* mesh) {
	m_bitangents = AssimpLoader::GetAssimpVertexBiTangents(mesh);
}

void DXAssimpSubMesh::CreateColors(aiMesh* mesh) {
	m_colors = AssimpLoader::Color::GetAssimpVertexColors(mesh);
}

void DXAssimpSubMesh::CreateIndices(aiMesh* mesh) {
	m_indices = AssimpLoader::GetAssimpIndices(mesh);
}

void DXAssimpSubMesh::CreateUVs(aiMesh* mesh) {
	// 8はテクスチャの最大保持数
	for (int_t i = 0; i < 8; i++) {
		if (mesh->HasTextureCoords(i)) {
			m_uvs.emplace(i, AssimpLoader::GetAssimpUVs(mesh, i));
			m_hasUvs[i] = true;
		}
		else {
			m_hasUvs[i] = false;
			break;
		}
	}
}

std::vector<DXVector3> DXAssimpSubMesh::GetPositions() {
	return m_positions;
}

std::vector<DXVector3> DXAssimpSubMesh::GetNormals() {
	return m_normals;
}

std::vector<DXVector3> DXAssimpSubMesh::GetTangents() {
	return m_tangents;
}

std::vector<DXVector3> DXAssimpSubMesh::GetBiTangents() {
	return m_bitangents;
}

std::vector<DXColor> DXAssimpSubMesh::GetColors() {
	return m_colors;
}

std::vector<uint_t> DXAssimpSubMesh::GetIndices() {
	return m_indices;
}

std::vector<DXVector2> DXAssimpSubMesh::GetUVs(uint_t index) {
	return m_uvs[index];
}

std::vector<DXVector3>& DXAssimpSubMesh::GetPositionsRef() {
	return m_positions;
}

std::vector<DXVector3>& DXAssimpSubMesh::GetNormalsRef() {
	return m_normals;
}

std::vector<DXVector3>& DXAssimpSubMesh::GetTangentsRef() {
	return m_tangents;
}

std::vector<DXVector3>& DXAssimpSubMesh::GetBiTangentsRef() {
	return m_bitangents;
}

std::vector<DXColor>& DXAssimpSubMesh::GetColorsRef() {
	return m_colors;
}

std::vector<uint_t>& DXAssimpSubMesh::GetIndicesRef() {
	return m_indices;
}

std::vector<DXVector2>& DXAssimpSubMesh::GetUVsRef(uint_t index) {
	return m_uvs[index];
}

std::size_t DXAssimpSubMesh::GetVerticesCount() {
	return m_positions.size();
}

std::size_t DXAssimpSubMesh::GetPositionsCount() {
	return m_positions.size();
}

std::size_t DXAssimpSubMesh::GetNormalsCount() {
	return m_normals.size();
}

std::size_t DXAssimpSubMesh::GetTangentsCount() {
	return m_tangents.size();
}

std::size_t DXAssimpSubMesh::GetBiTangentsCount() {
	return m_bitangents.size();
}

std::size_t DXAssimpSubMesh::GetColorsCount() {
	return m_colors.size();
}

std::size_t DXAssimpSubMesh::GetIndicesCount() {
	return m_indices.size();
}

std::size_t DXAssimpSubMesh::GetUVCount(uint_t index) {
	return m_uvs[index].size();
}

bool DXAssimpSubMesh::HasUV(uint_t index) {
	return m_hasUvs[index];
}

void DXAssimpMesh::CreateSubMesh(DirectXDevice* device, DirectXDeviceContext* context) {
	auto nodes = m_assimpData.GetNodes();
	auto nodeCount = m_assimpData.GetNodeCount();

	for (int i = 0; i < nodeCount; i++) {
		auto meshs = m_assimpData.GetMeshs(nodes[i]);
		auto materials = m_assimpData.GetMaterials(nodes[i]);
		auto meshCount = m_assimpData.GetMeshCount(nodes[i]);
		for (int_t j = 0; j < meshCount; j++) {
			auto mesh = meshs[j];
			auto material = materials[j];
			// サブメッシュの頂点情報に関する情報のみを取得する
			{
				DXAssimpSubMesh subMesh;
				subMesh.CreatePositions(mesh);
				subMesh.CreateNormals(mesh);
				subMesh.CreateTangents(mesh);
				subMesh.CreateBiTangents(mesh);
				subMesh.CreateColors(mesh);
				subMesh.CreateIndices(mesh);
				subMesh.CreateUVs(mesh);
				m_subMeshs.emplace_back(subMesh);
			}
			// サブメッシュのマテリアルに関する情報のみを取得する
			{
				DXAssimpSubMeshMaterial subMeshMaterial;
				subMeshMaterial.CreateMaterials(material);
				subMeshMaterial.CreateTexture(material, m_assimpData, device, context);
				m_subMeshMaterials.emplace_back(subMeshMaterial);
			}
		}
	}
}

void DXAssimpMesh::Create(DXAssimpData& assimpData, DirectXDevice* device, DirectXDeviceContext* context) {
	Debug::BracketBegin("DXAssmpMesh::Create");

	if (assimpData.IsValid()) {
		m_assimpData = assimpData;
		CreateSubMesh(device, context);
	}
	else {
		Debug::LogError(
			"DXAssimpMesh::Create Method Failed."
		);
#if _DEBUG
		MessageBoxUtility::ErrorMessage(
			"DXAssimpData::Create Method Failed."
		);
#endif
	}

	Debug::BracketEnd();
}

DXAssimpSubMesh DXAssimpMesh::GetSubMesh(uint_t index) {
	return m_subMeshs[index];
}

DXAssimpSubMesh& DXAssimpMesh::GetSubMeshRef(uint_t index) {
	return m_subMeshs[index];
}

uint_t DXAssimpMesh::GetSubMeshCount() {
	return m_subMeshs.size();
}

DXAssimpSubMeshMaterial DXAssimpMesh::GetSubMeshMaterial(uint_t index) {
	return m_subMeshMaterials[index];
}

DXAssimpSubMeshMaterial& DXAssimpMesh::GetSubMeshMaterialRef(uint_t index) {
	return m_subMeshMaterials[index];
}

uint_t DXAssimpMesh::GetSubMeshMaterialCount() {
	return m_subMeshMaterials.size();
}
