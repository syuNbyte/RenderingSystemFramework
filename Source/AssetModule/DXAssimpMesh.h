#pragma once

// STL
#include<string>
#include<vector>
#include<unordered_map>

// MyFile
#include"DXAssimpData.h"
#include"DXAssimpDataType.h"
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"DirectXUtilityModule/DirectXVector.h"
#include"DirectXUtilityModule/DirectXColor.h"
#include"DirectXSystemModule/ShaderResourceView.h"

class DXAssimpSubMeshMaterial {
private:
	std::unordered_map<DXAssimpDataType::MaterialType, DXColor> m_materials;										// �}�e���A���f�[�^
	std::unordered_map<DXAssimpDataType::Texture::Type, std::vector<DXShaderResourceView>> m_textures;		// �e�N�X�`���f�[�^
public:
	void CreateMaterials(aiMaterial* material);
	void CreateTexture(aiMaterial* material, DXAssimpData& assimpData, DirectXDevice* device, DirectXDeviceContext* context);
public:
	DXColor GetMaterial(DXAssimpDataType::MaterialType type);
	std::vector<DXShaderResourceView> GetTextures(DXAssimpDataType::Texture::Type type);
	DXColor& GetMaterialRef(DXAssimpDataType::MaterialType type);
	std::vector<DXShaderResourceView>& GetTexturesRef(DXAssimpDataType::Texture::Type type);
public:
	std::size_t GetMaterialCount();
	std::size_t GetTextureTypeCount(DXAssimpDataType::Texture::Type type);
};

class DXAssimpSubMesh {
private:
	std::vector<DXVector3> m_positions;																							// ���_���W
	std::vector<DXVector3> m_normals;																							// ���_�@��
	std::vector<DXVector3> m_tangents;																							// ���_�ڐ�
	std::vector<DXVector3> m_bitangents;																						// ���_�]�ڐ�
	std::vector<DXColor> m_colors;																								// ���_�F
	std::vector<uint_t> m_indices;																								// �C���f�b�N�X�f�[�^
	std::unordered_map<uint_t, std::vector<DXVector2>> m_uvs;															// UV���W
	std::array<bool, 8> m_hasUvs;																									// �ǂ̔ԍ���UV���W�������Ă��邩��ێ�����
public:
	void CreatePositions(aiMesh* mesh);
	void CreateNormals(aiMesh* mesh);
	void CreateTangents(aiMesh* mesh);
	void CreateBiTangents(aiMesh* mesh);
	void CreateColors(aiMesh* mesh);
	void CreateIndices(aiMesh* mesh);
	void CreateUVs(aiMesh* mesh);
	//void CreateTextures(aiMesh* mesh, aiMaterial* material, const std::string& resourcePath);
public:
	std::vector<DXVector3> GetPositions();
	std::vector<DXVector3> GetNormals();
	std::vector<DXVector3> GetTangents();
	std::vector<DXVector3> GetBiTangents();
	std::vector<DXColor> GetColors();
	std::vector<uint_t> GetIndices();
	std::vector<DXVector2> GetUVs(uint_t index);
	std::vector<DXVector3>& GetPositionsRef();
	std::vector<DXVector3>& GetNormalsRef();
	std::vector<DXVector3>& GetTangentsRef();
	std::vector<DXVector3>& GetBiTangentsRef();
	std::vector<DXColor>& GetColorsRef();
	std::vector<uint_t>& GetIndicesRef();
	std::vector<DXVector2>& GetUVsRef(uint_t index);
public:
	std::size_t GetVerticesCount();
	std::size_t GetPositionsCount();
	std::size_t GetNormalsCount();
	std::size_t GetTangentsCount();
	std::size_t GetBiTangentsCount();
	std::size_t GetColorsCount();
	std::size_t GetIndicesCount();
	std::size_t GetUVCount(uint_t index);
public:
	bool HasUV(uint_t index);
};

class DXAssimpMesh {
private:
	DXAssimpData m_assimpData;
	std::vector<DXAssimpSubMesh> m_subMeshs;
	std::vector<DXAssimpSubMeshMaterial> m_subMeshMaterials;
private:
	void CreateSubMesh(DirectXDevice* device, DirectXDeviceContext* context);
public:
	void Create(DXAssimpData& assimpData, DirectXDevice* device, DirectXDeviceContext* context);
public:
	DXAssimpSubMesh GetSubMesh(uint_t index);
	DXAssimpSubMesh& GetSubMeshRef(uint_t index);
	uint_t GetSubMeshCount();
public:
	DXAssimpSubMeshMaterial GetSubMeshMaterial(uint_t index);
	DXAssimpSubMeshMaterial& GetSubMeshMaterialRef(uint_t index);
	uint_t GetSubMeshMaterialCount();
};