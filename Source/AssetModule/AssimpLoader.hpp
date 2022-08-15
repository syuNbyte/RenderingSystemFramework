#pragma once

// library loading
#pragma comment( lib, "assimp-vc141-mtd.lib" )

// Assimp
#include<assimp\Importer.hpp>
#include<assimp\scene.h>
#include<assimp\postprocess.h>
#include<assimp/cimport.h>

// STL
#include<array>
#include<vector>
#include<map>
#include<memory>
#include<utility>

// TNLIB
#include"Debug.hpp"
#include"SafetyDataType.h"

// MyFile
#include"DirectXUtilityModule/DirectXVector.h"
#include"DirectXUtilityModule/DirectXMatrix.h"
#include"DirectXUtilityModule/DirectXQuaternion.h"
#include"DirectXUtilityModule/DirectXColor.h"

namespace AssimpLoader {

	// aiMesh���̃f�[�^�Ɋւ���������t�@�����X�Fhttp://sir-kimmi.de/assimp/lib_html/structassimp_1_1mesh_1_1ai_mesh.html


	namespace Type {

		// �@���x�N�g���E�ڐ�x�N�g���E�]�ڐ��x�N�g�����܂Ƃ߂�����
		struct AssimpVertexVector {
		public:
			DXVector3 m_normal;		// ���_�̖@���x�N�g��
			DXVector3 m_tangent;	// ���_�̐ڐ��x�N�g��
			DXVector3 m_bitangent;	// ���_�̏]�ڐ��x�N�g��
		public:
			AssimpVertexVector() = default;
			AssimpVertexVector( const AssimpVertexVector& ) = default;
			AssimpVertexVector( AssimpVertexVector&& ) = default;
			AssimpVertexVector( DXVector3 normal, DXVector3 tangent, DXVector3 bitangent )
				:m_normal( normal ), m_tangent( tangent ), m_bitangent( bitangent ) {};

			AssimpVertexVector& operator=( const AssimpVertexVector & ) = default;
			AssimpVertexVector& operator=( AssimpVertexVector && ) = default;
		};

		struct Bone {
			//std::string Name;
			unsigned int idx = -1;			// �{�[���z��̃C���f�b�N�X�ԍ�
			aiMatrix4x4 Matrix;				// �����z�u�s��
			aiMatrix4x4 AnimationMatrix;	// �{�[���s��
			aiMatrix4x4 OffsetMatrix;		// �{�[���I�t�Z�b�g�s��
		};

		struct BoneWeights {
			int_t m_BoneIndex[4];		//	�{���̓{�[���C���f�b�N�X�ŊǗ�����ׂ�
			float_t m_BoneWeight[4];	//	�E�F�C�g�l
			int_t m_BoneNum;				//	�{�[����
			std::string	m_BoneName[4];	//	�{���̓{�[���C���f�b�N�X�ŊǗ�����ׂ�
		};

		struct KeyFrameAnimationParam {
		public:
			aiVector3D m_position;
			aiQuaternion m_rotation;
			aiVector3D m_scaling;
		public:
			KeyFrameAnimationParam() :
				m_position( aiVector3D( 0.0f, 0.0f, 0.0f ) ),
				m_rotation( aiQuaternion( 0.0f, 0.0f, 0.0f, 1.0f ) ),
				m_scaling( aiVector3D( 1.0f, 1.0f, 1.0f ) ) {};
			KeyFrameAnimationParam(const KeyFrameAnimationParam& ) = default;
			KeyFrameAnimationParam( KeyFrameAnimationParam&& ) = default;

			KeyFrameAnimationParam( aiVector3D position, aiQuaternion rotation, aiVector3D scaling )
				: m_position( position ), m_rotation( rotation ), m_scaling( scaling ) {};
		};

		struct Animations {

		public:

			using NodeName = std::string;
			using FrameNumber = int_t;

		public:

			std::unordered_map<NodeName, std::unordered_map<FrameNumber, aiVector3D>> m_positions;
			std::unordered_map<NodeName, std::unordered_map<FrameNumber, aiQuaternion>> m_rotations;
			std::unordered_map<NodeName, std::unordered_map<FrameNumber, aiVector3D>> m_scalings;

		public:

			// �w�肳�ꂽ�m�[�h�����݂��邩�ǂ�����Ԃ�
			bool HasNode( NodeName nodeName ) {
				return ( m_positions.find( nodeName ) != m_positions.end() );
			}

			// �w�肳�ꂽ�m�[�h�����t���[���ԍ��̍��W�p�����[�^�����݂��邩��Ԃ�
			bool HasPositionParam( NodeName nodeName, FrameNumber frame ) {
				auto a = m_positions.find( nodeName );
				if( a != m_positions.end() ) {
					auto b = a->second.find( frame );
					if( b != a->second.end() ) {
						return true;
					}
				}

				return false;
			}

			// �w�肳�ꂽ�m�[�h�����t���[���ԍ��̉�]�p�����[�^�����݂��邩��Ԃ�
			bool HasRotationParam( NodeName nodeName, FrameNumber frame ) {
				auto a = m_rotations.find( nodeName );
				if( a != m_rotations.end() ) {
					auto b = a->second.find( frame );
					if( b != a->second.end() ) {
						return true;
					}
				}

				return false;
			}
			
			// �w�肳�ꂽ�m�[�h�����t���[���ԍ��̊g�k�p�����[�^�����݂��邩��Ԃ�
			bool HasScalingParam( NodeName nodeName, FrameNumber frame ) {
				auto a = m_scalings.find( nodeName );
				if( a != m_scalings.end() ) {
					auto b = a->second.find( frame );
					if( b != a->second.end() ) {
						return true;
					}
				}

				return false;
			}

			// �ێ�������W�p�����[�^���擾����
			// �p�����[�^�����݂��Ȃ���΁AaiVector3D(0,0,0)��Ԃ�
			aiVector3D GetPositionParam( NodeName nodeName, FrameNumber frame ) {
				if( HasPositionParam( nodeName, frame ) ) {
					return m_positions[nodeName][frame];
				}
				else {
					return aiVector3D( 0, 0, 0 );
				}
			}

			// �ێ������]�p�����[�^���擾����
			// �p�����[�^�����݂��Ȃ���΁AaiQuaternion(0,0,0,1)��Ԃ�
			aiQuaternion GetRotationParam( NodeName nodeName, FrameNumber frame ) {
				if( HasRotationParam( nodeName, frame ) ) {
					return m_rotations[nodeName][frame];
				}
				else {
					return aiQuaternion( 0, 0, 0, 1 );
				}
			}

			// �ێ�����g�k�p�����[�^���擾����
			// �p�����[�^�����݂��Ȃ���΁AaiVector3D(1,1,1)��Ԃ�
			aiVector3D GetScalingParam( NodeName nodeName, FrameNumber frame ) {
				if( HasScalingParam( nodeName, frame ) ) {
					return m_scalings[nodeName][frame];
				}
				else {
					return aiVector3D( 1, 1, 1 );
				}
			}

			// �ێ�����A�j���[�V�����f�[�^���擾����
			KeyFrameAnimationParam GetAnimationParam( NodeName nodeName, FrameNumber frame ) {
				return KeyFrameAnimationParam(
					GetPositionParam( nodeName, frame ),
					GetRotationParam( nodeName, frame ),
					GetScalingParam( nodeName, frame )
				);
			}

		};

		struct NewAnimations {

		public:

			using NodeName = std::string;
			using FrameNumber = int_t;

		public:

			std::unordered_map<
				NodeName,
				std::unordered_map<FrameNumber, KeyFrameAnimationParam>
			> m_keyFrames;

		public:

			// �w�肳�ꂽ�m�[�h�����݂��邩�ǂ�����Ԃ�
			bool HasNode( NodeName nodeName ) {
				return ( m_keyFrames.find( nodeName ) != m_keyFrames.end() );
			}

			// �w�肳�ꂽ�m�[�h�����t���[���ԍ������݂��邩��Ԃ�
			bool HasKeyFrame( NodeName nodeName, FrameNumber frame ) {
				auto a = m_keyFrames.find( nodeName );
				if( a != m_keyFrames.end() ) {
					auto b = a->second.find( frame );
					if( b != a->second.end() ) {
						return true;
					}
				}

				return false;
			}

			// �ێ�����A�j���[�V�����f�[�^���擾����
			KeyFrameAnimationParam GetAnimationParam( NodeName nodeName, FrameNumber frame ) {
				if( HasKeyFrame( nodeName, frame )) {
					return m_keyFrames[nodeName][frame];
				}
				else {
					return KeyFrameAnimationParam();
				}
			}

		};

		using Indices = std::vector<uint_t>;
		using UVs = std::vector<DXVector2>;
		using Positions = std::vector<DXVector3>;
		using Normals = std::vector<DXVector3>;
		using Tangents = std::vector<DXVector3>;
		using BiTangents = std::vector<DXVector3>;
		using Colors = std::vector<DXColor>;
		using VertexVectors = std::vector<AssimpVertexVector>;

		// Assimp�̃e�N�X�`���^�C�v�@�����e��aiTextureType�ɏڍׂȐ����������Ă���
		enum class TextureType {
			None			 = aiTextureType::aiTextureType_NONE,			// ����
			Diffuse		 = aiTextureType::aiTextureType_DIFFUSE,		// �f�B�t���[�Y
			Specular		 = aiTextureType::aiTextureType_SPECULAR,		// �X�y�L����
			Ambient		 = aiTextureType::aiTextureType_AMBIENT,		// �A���r�G���g
			Emissive		 = aiTextureType::aiTextureType_EMISSIVE,		// �G�~�b�V�u
			Height		 = aiTextureType::aiTextureType_HEIGHT,		// �n�C�g�}�b�v
			Normal		 = aiTextureType::aiTextureType_NORMALS,		// �@���}�b�v
			Shininess	 = aiTextureType::aiTextureType_SHININESS,	// �P�x�}�b�v
			Opacity		 = aiTextureType::aiTextureType_OPACITY,		// �����x�}�b�v
			Displacement = aiTextureType::aiTextureType_DISPLACEMENT,// �f�B�X�v���C�X�����g�}�b�v
			LightMap		 = aiTextureType::aiTextureType_LIGHTMAP,		// ���C�g�}�b�v
			Refrection	 = aiTextureType::aiTextureType_REFLECTION,	// ���˃}�b�v
			Unknown		 = aiTextureType::aiTextureType_UNKNOWN		// �s��
		};

	}

	inline DXVector2 AiVector2Cast( aiVector2D vec2 ) {
		return DXVector2( vec2.x, vec2.y );
	}

	inline DXVector3 AiVector3Cast( aiVector3D vec3 ) {
		return DXVector3( vec3.x, vec3.y, vec3.z );
	}

	inline DXColor AiColorCast(aiColor4D color) {
		return DXColor(color.r, color.g, color.b, color.a);
	}

	inline DXMatrix4x4 AiMatrix4x4CastAndTranspose( aiMatrix4x4 m ) {
		// aiMatrix4x4�͗�D��s��Ȃ̂�
		// DirectX�p�ɍs�D��s��ɒu��������i�L���X�g�Ɠ]�u�𓯎��ɍs���Ă���j
		return DXMatrix4x4(
			m.a1, m.b1, m.c1, m.d1,
			m.a2, m.b2, m.c2, m.d2,
			m.a3, m.b3, m.c3, m.d3,
			m.a4, m.b4, m.c4, m.d4
		);
	}

	inline DXMatrix4x4 AiMatrix4x4Cast( aiMatrix4x4 m ) {
	// aiMatrix4x4�͗�D��s��Ȃ̂�
	// DirectX�p�ɍs�D��s��ɒu��������i�L���X�g�Ɠ]�u�𓯎��ɍs���Ă���j
		return DXMatrix4x4(
			m.a1, m.a2, m.a3, m.a4,
			m.b1, m.b2, m.b3, m.b4,
			m.c1, m.c2, m.c3, m.c4,
			m.d1, m.d2, m.d3, m.d4
		);
	}

	inline DXQuaternion AiQuaternionCast( aiQuaternion qt ) {
		return DXQuaternion(
			qt.x,
			qt.y,
			qt.z,
			qt.w
		);
	}

	inline aiVector3D AiVector3Lerp( const aiVector3D& a, const aiVector3D& b, float_t t ) {
		return aiVector3D(
			Math::Lerp( a.x, b.x, t ),
			Math::Lerp( a.y, b.y, t ),
			Math::Lerp( a.z, b.z, t )
		);
	}

	inline aiQuaternion AiQuaternionSlerp( const aiQuaternion& a, const aiQuaternion& b, float_t t ) {
		aiQuaternion ret;
		aiQuaternion::Interpolate(
			ret,
			a,
			b,
			t
		);
		return ret;
	}


	// assimpMesh���̃C���f�b�N�X����S�Ď擾
	inline AssimpLoader::Type::Indices GetAssimpIndices( aiMesh* m_assimpMesh ) {

		AssimpLoader::Type::Indices ret;

		// �ʂ̐��������[�v
		for( uint_t i = 0; i < m_assimpMesh->mNumFaces; i++ ) {

			// �ʂ̃f�[�^���擾
			aiFace face = m_assimpMesh->mFaces[i];

			// �ʂ̃C���f�b�N�X�̐��������[�v
			for( uint_t j = 0; j < face.mNumIndices; j++ ) {
				ret.emplace_back( face.mIndices[j] );
			}
		}

		return ret;
	}

	// assimpMesh���̒��_���W��S�Ď擾
	inline AssimpLoader::Type::Positions GetAssimpVertexPositions( aiMesh* assimpMesh ) {

		AssimpLoader::Type::Positions ret;
		ret.resize( assimpMesh->mNumVertices );

		for( int i = 0; i < assimpMesh->mNumVertices; i++ ) {
			ret[i] = AssimpLoader::AiVector3Cast( assimpMesh->mVertices[i] );
		}

		return ret;
	}

	// assimpMesh���̒��_�@����S�Ď擾
	inline AssimpLoader::Type::Normals GetAssimpVertexNormals( aiMesh* assimpMesh ) {

		AssimpLoader::Type::Normals ret;

		if( assimpMesh->HasNormals() ) {
			ret.resize( assimpMesh->mNumVertices );
			for( int i = 0; i < assimpMesh->mNumVertices; i++ ) {
				ret[i] = AssimpLoader::AiVector3Cast( assimpMesh->mNormals[i] );
			}
		}

		// �ێ����Ă��Ȃ��ꍇ�͋�̃f�[�^��Ԃ��̂�empty���\�b�h�Ȃǂ��g�p���Ĕ��肷��Ƃ悢
		return ret;

	}

	// assimpMesh���̒��_�ڐ���S�Ď擾
	inline AssimpLoader::Type::Tangents GetAssimpVertexTangents( aiMesh* assimpMesh ) {

		AssimpLoader::Type::Tangents ret;

		if( assimpMesh->HasTangentsAndBitangents() ) {
			ret.resize( assimpMesh->mNumVertices );
			for( int i = 0; i < assimpMesh->mNumVertices; i++ ) {
				ret[i] = AssimpLoader::AiVector3Cast( assimpMesh->mTangents[i] );
			}
		}

		// �ێ����Ă��Ȃ��ꍇ�͋�̃f�[�^��Ԃ��̂�empty���\�b�h�Ȃǂ��g�p���Ĕ��肷��Ƃ悢
		return ret;

	}

	// assimpMesh���̒��_�]�ڐ���S�Ď擾
	inline AssimpLoader::Type::BiTangents GetAssimpVertexBiTangents( aiMesh* assimpMesh ) {

		AssimpLoader::Type::BiTangents ret;
		if( assimpMesh->HasTangentsAndBitangents() ) {
			ret.resize( assimpMesh->mNumVertices );
			for( int i = 0; i < assimpMesh->mNumVertices; i++ ) {
				ret[i] = AssimpLoader::AiVector3Cast( assimpMesh->mBitangents[i] );
			}
		}

		// �ێ����Ă��Ȃ��ꍇ�͋�̃f�[�^��Ԃ��̂�empty���\�b�h�Ȃǂ��g�p���Ĕ��肷��Ƃ悢
		return ret;

	}

	// assimpMesh���̈�̒��_�f�[�^���C���f�b�N�X���w�肵�Ď擾
	inline AssimpLoader::Type::VertexVectors GetAssimpVertexVectors( aiMesh* assimpMesh ) {

		AssimpLoader::Type::VertexVectors ret;
		ret.resize( assimpMesh->mNumVertices );

		// �擾�������f�[�^�����݂��邩����
		auto hasNormal = assimpMesh->HasNormals();										// ���_�@���x�N�g�������邩����
		auto hasTangentAndBiTangent = assimpMesh->HasTangentsAndBitangents();	// ���_�]�ڐ��x�N�g��,  ���_�ڐ��x�N�g�������邩����

		for( int i = 0; i < assimpMesh->mNumVertices; i++ ) {
			ret[i].m_normal = ( hasNormal ) ? AssimpLoader::AiVector3Cast( assimpMesh->mNormals[i] ) : DXVector3::Zero();							// �@���x�N�g��
			ret[i].m_tangent = ( hasTangentAndBiTangent ) ? AssimpLoader::AiVector3Cast( assimpMesh->mTangents[i] ) : DXVector3::Zero();		// �ڐ��x�N�g��
			ret[i].m_bitangent = ( hasTangentAndBiTangent ) ? AssimpLoader::AiVector3Cast( assimpMesh->mBitangents[i] ) : DXVector3::Zero();	// �]�ڐ��x�N�g��
		}

		return ret;
	}

	// assimpMesh�Ȃ�����e�N�X�`����UV���W���擾����
	inline AssimpLoader::Type::UVs GetAssimpUVs( aiMesh* assimpMesh, int_t index ) {

		// �ȉ��̓�Ɋւ��Ă�UV�̗^����ꂽ���Ɋ֌W����Ǝv���̂ŗv����
		assimpMesh->GetNumUVChannels();
		assimpMesh->mNumUVComponents[0];

		AssimpLoader::Type::UVs ret;

		if( assimpMesh->HasTextureCoords( index ) ) {
			ret.resize( assimpMesh->mNumVertices );
			for( int i = 0; i < assimpMesh->mNumVertices; i++ ) {
				ret[i] = AssimpLoader::AiVector3Cast( assimpMesh->mTextureCoords[index][i] ).ToVector2();
			}
		}

		return ret;
	}

	// �e�N�X�`���t�@�C�������擾
	inline std::string GetAssimpTextureFileName( aiMaterial* assimpMaterial, aiTextureType assimpTextureType, int index ) {
		aiString ai_str;
		assimpMaterial->GetTexture( assimpTextureType, index, &ai_str );
		return ai_str.C_Str();
	}

	// �e�N�X�`���f�[�^���擾
	/*
	inline std::vector<Texture*> GetAssimpTexture(
		aiMesh* assimpMesh,
		aiMaterial* assimpMaterial,
		aiTextureType assimpTextureType,
		std::filesystem::path modelResourceFolderPath
	) {

		std::vector<Texture*> ret;
		auto texCount = assimpMaterial->GetTextureCount( assimpTextureType );

		// �e�N�X�`��������Ȃ�擾����
		for( int_t i = 0; i < texCount; i++ ) {

			std::filesystem::path texFilePath = NewAssimpLoader::GetAssimpTextureFileName( assimpMaterial, assimpTextureType, i );

			{
				auto texPointer = TextureManager::CreateAndGetTexture( texFilePath );
				//if( texPointer != nullptr) {
				if( texPointer->HasTexture() ) {
					ret.emplace_back( texPointer );
					Debug::Log("TextureFilePath[", texFilePath, "]", "Load Success.");
					// �ǂݍ��݂��ł����Ȃ�
					// ���̃R�[�h�u���b�N�ɓ���Ȃ��悤��
					continue;
				}
			}
			{
				// �V�����t�@�C���ւ̃p�X���쐬����
				std::filesystem::path newTexFilePath = modelResourceFolderPath.u8string() + texFilePath.filename().u8string();
				auto texPointer = TextureManager::CreateAndGetTexture( newTexFilePath );
				//if( texPointer != nullptr ) {
				if( texPointer->HasTexture() ) {
					Debug::Log("TextureFilePath[", newTexFilePath, "]", "Load Success.");
					ret.emplace_back( texPointer );
				}
				else {
					Debug::LogWarning( 
						"TextureFilePath[", newTexFilePath, "]\n",
						"Texture couldn't be loaded"
					);
#if _DEBUG
					ErrorMessage( "Texture couldn't be loaded" );
#endif
				}
			}
		}

		return ret;

	}
	*/

	// �g�p����v���~�e�B�u�^���擾
	inline aiPrimitiveType GetAssimpPrimitiveType( aiMesh* assimpMesh ) {
		return static_cast<aiPrimitiveType>(assimpMesh->mPrimitiveTypes);
	}

	namespace Color {

		inline AssimpLoader::Type::Colors GetAssimpVertexColors(aiMesh* assimpMesh) {
			AssimpLoader::Type::Colors ret;
			ret.resize(assimpMesh->mNumVertices);


			for (int i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; i++) {
				if (assimpMesh->HasVertexColors(i)) {
					for (int j = 0; j < assimpMesh->mNumVertices; j++) {
						ret[j] = AssimpLoader::AiColorCast(assimpMesh->mColors[i][j]);
					}
				}
				else {
					continue;
				}
			}

			return ret;
		}

		// �f�B�t���[�Y�F���擾
		inline DXColor GetDiffuse( aiMaterial* assimpMaterial ) {
			aiColor3D color;
			assimpMaterial->Get( AI_MATKEY_COLOR_DIFFUSE, color );
			return DXColor( color.r, color.g, color.b, 1.0f );
		}

		// �X�y�L�����[�F���擾
		inline DXColor GetSpecular( aiMaterial* assimpMaterial ) {
			aiColor3D color;
			float_t power;
			assimpMaterial->Get( AI_MATKEY_COLOR_SPECULAR, color );		// �X�y�L�����[
			assimpMaterial->Get( AI_MATKEY_SHININESS_STRENGTH, power );	// �X�y�L�����[�̋���
			return DXColor( color.r, color.g, color.b, power );
		}

		// �A���r�G���g�F���擾
		inline DXColor GetAmbient( aiMaterial* assimpMaterial ) {
			aiColor3D color;
			assimpMaterial->Get( AI_MATKEY_COLOR_AMBIENT, color );
			return DXColor( color.r, color.g, color.b, 1.0f );
		}

		// �G�~�b�V�u�F���擾
		inline DXColor GetEmissive( aiMaterial* assimpMaterial ) {
			aiColor3D color;
			assimpMaterial->Get( AI_MATKEY_COLOR_EMISSIVE, color );
			return DXColor( color.r, color.g, color.b, 0.0f );
		}

		// ���ːF���擾
		inline DXColor GetReflective(aiMaterial* assimpMaterial) {
			aiColor3D color;
			assimpMaterial->Get(AI_MATKEY_COLOR_REFLECTIVE, color);
			return DXColor(color.r, color.g, color.b, 1.0f);
		}

		// �s�����x���擾
		inline DXColor GetTransparent(aiMaterial* assimpMaterial) {
			aiColor3D color;
			assimpMaterial->Get(AI_MATKEY_COLOR_TRANSPARENT, color);
			return DXColor(color.r, color.g, color.b, 1.0f);
		}

	}


	// �S�Ă�AssimpNode�����o�������I�ȏ���
	inline void _Internal_GetAssimpNode( aiNode* assimpNode, std::vector<aiNode*>& nodes ) {
		for( int_t i = 0; i < assimpNode->mNumChildren; i++ ) {
			nodes.emplace_back( assimpNode->mChildren[i] );
			_Internal_GetAssimpNode( assimpNode->mChildren[i], nodes );
		}
	}

	// ���[�g�m�[�h�ȊO�̑S�Ă�AssimpNode�����o��
	inline std::vector<aiNode*> GetAssimpNode( aiNode* assimpRootNode ) {
		std::vector<aiNode*> ret;
		_Internal_GetAssimpNode( assimpRootNode, ret );
		return ret;
	}

	// ���[�g�m�[�h���܂ޑS�Ă�AssimpNode�����o��
	inline std::vector<aiNode*> GetAssimpNodeEx( aiNode* assimpRootNode ) {
		std::vector<aiNode*> ret;
		ret.emplace_back( assimpRootNode );
		_Internal_GetAssimpNode( assimpRootNode, ret );
		return ret;
	}

	// ����AssimpNode�̎���AssimpMesh��S�Ď擾����
	inline std::vector<aiMesh*> GetAssimpMesh( const aiScene* assimpScene, aiNode* assimpNode ) {
		std::vector<aiMesh*> ret;
		for( int_t i = 0; i < assimpNode->mNumMeshes; i++ ) {
			ret.emplace_back( 
				assimpScene->mMeshes[
					assimpNode->mMeshes[i]
				] 
			);
		}
		return ret;
	}

	// ����AssimpNode�̎���AssimpMaterial��S�Ď擾����
	inline std::vector<aiMaterial*> GetAssimpMaterial( const aiScene* assimpScene, aiNode* assimpNode ) {
		std::vector<aiMaterial*> ret;
		for( int_t i = 0; i < assimpNode->mNumMeshes; i++ ) {
			ret.emplace_back( 
				assimpScene->mMaterials[
					assimpScene->mMeshes[
						assimpNode->mMeshes[i]
					]->mMaterialIndex
				] 
			);
		}
		return ret;
	}

	inline void _Internal_GetAssimpAnimation( 
		aiNode* assimpNode,
		std::unordered_map<std::string, AssimpLoader::Type::Bone>& anims
	) {
		// ������
		for( int_t i = 0; i < assimpNode->mNumChildren; i++ ) {
			anims.emplace(
				assimpNode->mName.C_Str(),
				AssimpLoader::Type::Bone()
			);
			_Internal_GetAssimpAnimation(
				assimpNode->mChildren[i],
				anims
			);
		}
	}

	// TODO �A�j���[�V�����̓ǂݍ��ݏ��������
	inline std::unordered_map<std::string, AssimpLoader::Type::Bone> GetAssimpAnimation( aiNode* assimpRootNode ) {
		std::unordered_map<std::string, AssimpLoader::Type::Bone> ret;
		AssimpLoader::_Internal_GetAssimpAnimation( assimpRootNode, ret );

		// �{�[���̃C���f�b�N�X��ݒ�
		uint_t i = 0;
		for( auto& bone : ret ) bone.second.idx = i++;

		return ret;
	}


	// AssimpMesh����AssimpBone��S�Ď擾����
	inline std::vector<aiBone*> GetAssimpMeshBone( aiMesh* assimpMesh ) {
		std::vector<aiBone*> ret;
		ret.resize( assimpMesh->mNumBones );
		for( int_t i = 0; i < assimpMesh->mNumBones; i++ ) {
			ret[i] = assimpMesh->mBones[i];
		}
		return ret;
	}

	inline std::vector<AssimpLoader::Type::BoneWeights> _Internal_GetAssimpMeshBoneVertices( 
		aiMesh* assimpMesh,
		std::unordered_map<std::string, AssimpLoader::Type::Bone>& mybones
	) {
		std::vector<AssimpLoader::Type::BoneWeights> ret;
		ret.resize( assimpMesh->mNumVertices );
		auto bones = AssimpLoader::GetAssimpMeshBone( assimpMesh );

		for( int_t bi = 0; bi < assimpMesh->mNumBones; bi++ ) {		// bi : bone index
			
			// �I�t�Z�b�g�s���ݒ�
			mybones[bones[bi]->mName.C_Str()].OffsetMatrix = bones[bi]->mOffsetMatrix;

			for( int_t wi = 0; wi < bones[bi]->mNumWeights; wi++ ) {	// wi : weight index
				auto weight = bones[bi]->mWeights;
				auto vi = weight[wi].mVertexId;
				
				auto& boneNum = ret[vi].m_BoneNum;
				const auto boneName = bones[bi]->mName.C_Str();

				ret[vi].m_BoneWeight[boneNum] = weight->mWeight;		// ���_�E�F�C�g
				ret[vi].m_BoneName[boneNum] = boneName;					// ���_�̏�������{�[����
				ret[vi].m_BoneIndex[boneNum] = mybones[boneName].idx;	// ���_�C���f�b�N�X
				++ret[vi].m_BoneNum;
			}
		}

		return ret;
	}

	// ����AssimpScene���̃A�j���[�V�������̒��_����S�Ď擾����
	inline std::vector<AssimpLoader::Type::BoneWeights> _Internal_GetAssimpMeshBoneVertices( const aiScene* assimpScene, aiNode* assimpNode ) {
		std::vector<AssimpLoader::Type::BoneWeights> ret;
		auto meshs = GetAssimpMesh( assimpScene, assimpNode );
		auto meshCount = meshs.size();
		auto anims = GetAssimpAnimation( assimpNode );
		for( int_t i = 0; i < meshCount; i++ ) {
			auto vertices = _Internal_GetAssimpMeshBoneVertices( meshs[i], anims );
			ret.resize( ret.size() + vertices.size() );
			std::copy( vertices.begin(), vertices.end(), std::back_inserter( ret ) );
		}

		return ret;
	}

	// ����AssimpScene���̃A�j���[�V�������̒��_����S�Ď擾����
	inline std::vector<std::vector<AssimpLoader::Type::BoneWeights>> GetAssimpMeshBoneVertices( const aiScene* assimpScene, aiNode* assimpRootNode ) {
		std::vector<std::vector<AssimpLoader::Type::BoneWeights>> ret;

		auto nodes = GetAssimpNode( assimpRootNode );
		for( int_t i = 0; i < nodes.size(); i++ ) {
			ret.emplace_back(_Internal_GetAssimpMeshBoneVertices( assimpScene, nodes[i] ));
		}

		return ret;
	}

		// ����AssimpScene���̃A�j���[�V�������̒��_����S�Ď擾����
	inline std::vector<AssimpLoader::Type::BoneWeights> GetAssimpMeshBoneVertices_( aiNode* assimpNode, aiMesh* assimpMesh ) {
		std::vector<AssimpLoader::Type::BoneWeights> ret;

		auto anims = GetAssimpAnimation( assimpNode );
		auto vertices = _Internal_GetAssimpMeshBoneVertices( assimpMesh, anims );
		ret.resize( ret.size() + vertices.size() );
		std::copy( vertices.begin(), vertices.end(), std::back_inserter( ret ) );

		return ret;
	}



	namespace Bone {

		// ����AssimpScene���̃{�[���̏���S�Ď擾����
		inline std::map<std::string, AssimpLoader::Type::Bone> NewGetAssimpBones( 
			const aiScene* assimpScene,
			aiNode* assimpRootNode
		) {
			std::map<std::string, AssimpLoader::Type::Bone> ret;

			auto nodes = AssimpLoader::GetAssimpNodeEx( assimpRootNode );
			for( auto node : nodes ) {
				ret.emplace(
					node->mName.C_Str(),
					AssimpLoader::Type::Bone()
				);
			}

			for( int_t n = 0; n < nodes.size(); n++ ) {
				auto meshs = AssimpLoader::GetAssimpMesh( assimpScene, nodes[n] );
				for( auto mesh : meshs ) {
					for( int_t b = 0; b < mesh->mNumBones; b++ ) {
						// ���b�V���Ɋ֘A�Â��Ă�{�[�������擾
						auto bone = mesh->mBones[b];
						// �{�[���I�t�Z�b�g�s����擾
						ret[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;
					}
				}
			}

			int_t index = 0;
			for( auto& bone : ret ) {
				bone.second.idx = index++;
			}

			return ret;
		}

		// ����AssimpMesh���̒��_�f�[�^�̃{�[���֘A�̏���S�Ď擾����
		inline std::vector<AssimpLoader::Type::BoneWeights> NewGetAssimpMeshBoneWeights(
			aiMesh* assimpMesh,
			std::map<std::string, AssimpLoader::Type::Bone>& bones // NewAssimpLoader::Bone::NewGetAssimpBone���\�b�h�̖߂�l���g��
		) {
			std::vector<AssimpLoader::Type::BoneWeights> ret;

			// �{�[���ɑΉ����钸�_�f�[�^�̏�����
			// AnimModelData::processMesh[Line : 672]
			for( int_t v = 0; v < assimpMesh->mNumVertices; v++ ) {
				AssimpLoader::Type::BoneWeights boneVert;
				boneVert.m_BoneNum = 0;

				for( int_t b = 0; b < 4; b++ ) {
					boneVert.m_BoneIndex[b] = -1;
					boneVert.m_BoneName[b] = "";
					boneVert.m_BoneWeight[b] = 0.0f;
				}

				ret.push_back( boneVert );
			}


			// �{�[���f�[�^������
			// AnimModelData::processMesh[Line : 682]
			for( int_t b = 0; b < assimpMesh->mNumBones; b++ ) {

				// ���b�V���Ɋ֘A�Â��Ă�{�[�������擾
				auto bone = assimpMesh->mBones[b];
				auto boneName = bone->mName.C_Str();

				// �{�[���Ɋ֘A�Â��Ă��钸�_��I�Ѥ�E�F�C�g�l���Z�b�g����
				for( int_t w = 0; w < bone->mNumWeights; w++ ) {
					auto weight = bone->mWeights[w];
					auto vertexID = bone->mWeights[w].mVertexId;
					auto boneNum = ret[vertexID].m_BoneNum;

					ret[vertexID].m_BoneWeight[boneNum] = weight.mWeight;
					ret[vertexID].m_BoneName[boneNum] = boneName;
					ret[vertexID].m_BoneIndex[boneNum] = bones[boneName].idx;
					ret[vertexID].m_BoneNum++;

					// Bone�̐���4�ȉ��Ȃ���Ȃ�
					// Bone�̐���5�ȏ�Ȃ�A�T�[�g�𑗏o
					assert( ret[vertexID].m_BoneNum <= 4 );
				}

			}

			return ret;
		}

	}



	namespace Animation {

	// ����AssimpScene���ێ�����S�ẴA�j���[�V�����f�[�^�����擾����
		inline std::vector<std::string> GetAssimpAnimationName( const aiScene* assimpScene ) {
			std::vector<std::string> ret;
			auto hasAnimation = assimpScene->HasAnimations();
			auto animationNum = assimpScene->mNumAnimations;
			ret.reserve( animationNum );

			if( hasAnimation ) {
				for( int_t i = 0; i < animationNum; i++ ) {
					ret.push_back( assimpScene->mAnimations[i]->mName.C_Str() );
				}
			}

			return ret;
		}

		// ����AssimpScene���ێ�����S�ẴA�j���[�V�����f�[�^���擾����
		inline std::vector<aiAnimation*> GetAssimpAnimation( const aiScene* assimpScene ) {
			std::vector<aiAnimation*> ret;
			auto hasAnimation = assimpScene->HasAnimations();
			auto animationNum = assimpScene->mNumAnimations;
			ret.reserve( animationNum );

			if( hasAnimation ) {
				for( int_t i = 0; i < animationNum; i++ ) {
					ret.push_back( assimpScene->mAnimations[i] );
				}
			}

			return ret;
		}

		// ����AssimpScene���ێ�����S�ẴA�j���[�V�����f�[�^���擾����
		inline std::unordered_map<std::string, aiAnimation*> GetAssimpAnimationEx( const aiScene* assimpScene ) {
			std::unordered_map<std::string, aiAnimation*> ret;
			auto hasAnimation = assimpScene->HasAnimations();
			auto animationNum = assimpScene->mNumAnimations;
			ret.reserve( animationNum );

			if( hasAnimation ) {
				for( int_t i = 0; i < animationNum; i++ ) {
					ret.emplace(
						assimpScene->mAnimations[i]->mName.C_Str(),
						assimpScene->mAnimations[i]
					);
				}
			}

			return ret;
		}

		// ����AssimpAnimation���ێ�����A�j���[�V�����m�[�h��S�Ď擾����
		inline std::vector<aiNodeAnim*> GetAssimpAnimationNode( aiAnimation* assimpAnim ) {
			std::vector<aiNodeAnim*> ret;
			auto channelNum = assimpAnim->mNumChannels;
			ret.reserve( channelNum );

			for( int_t i = 0; i < channelNum; i++ ) {
				ret.push_back( assimpAnim->mChannels[i] );
			}

			return ret;
		}

		// ����AssimpAnimationNode���̃L�[�t���[����񂩂���W�p�����[�^�݂̂��擾����
		// @ret
		// ���v�f���t���[���ԍ�
		// ���v�f���t���[���ԍ��ɑΉ�������W�p�����[�^
		inline std::unordered_map<int_t, aiVector3D> GetAssimpAnimationPositionParam( aiNodeAnim* assimpAnimNode ) {
			std::unordered_map<int_t, aiVector3D> ret;

			for( int_t i = 0; i < assimpAnimNode->mNumPositionKeys; i++ ) {
				ret.emplace(
					assimpAnimNode->mPositionKeys[i].mTime,
					assimpAnimNode->mPositionKeys[i].mValue
				);
			}

			return ret;
		}

		// ����AssimpAnimationNode���̃L�[�t���[����񂩂��]�p�����[�^�݂̂��擾����
		// @ret
		// ���v�f���t���[���ԍ�
		// ���v�f���t���[���ԍ��ɑΉ�������W�p�����[�^
		inline std::unordered_map<int_t, aiQuaternion> GetAssimpAnimationRotationParam( aiNodeAnim* assimpAnimNode ) {
			std::unordered_map<int_t, aiQuaternion> ret;

			for( int_t i = 0; i < assimpAnimNode->mNumRotationKeys; i++ ) {
				ret.emplace(
					assimpAnimNode->mRotationKeys[i].mTime,
					assimpAnimNode->mRotationKeys[i].mValue
				);
			}

			return ret;
		}

		// ����AssimpAnimationNode���̃L�[�t���[����񂩂�g�k�p�����[�^�݂̂��擾����
		// @ret
		// ���v�f���t���[���ԍ�
		// ���v�f���t���[���ԍ��ɑΉ�������W�p�����[�^
		inline std::unordered_map<int_t, aiVector3D> GetAssimpAnimationScalingParam( aiNodeAnim* assimpAnimNode ) {
			std::unordered_map<int_t, aiVector3D> ret;

			for( int_t i = 0; i < assimpAnimNode->mNumScalingKeys; i++ ) {
				ret.emplace(
					assimpAnimNode->mScalingKeys[i].mTime,
					assimpAnimNode->mScalingKeys[i].mValue
				);
			}

			return ret;
		}

		inline std::unordered_map<
			std::string,
			std::vector<AssimpLoader::Type::KeyFrameAnimationParam>
		> GetAssimpKeyFrameAnimationParams( aiAnimation* assimpAnimation ) {
			std::unordered_map< std::string, std::vector<AssimpLoader::Type::KeyFrameAnimationParam>> ret;
			auto nodes = AssimpLoader::Animation::GetAssimpAnimationNode( assimpAnimation );
			auto maxFrame = static_cast<int_t>(assimpAnimation->mDuration);

			// �m�[�h���Ƃ���ɑΉ�����std::vector<KeyFrame>�̗v�f���m��
			for( auto node : nodes ) {
				ret.emplace(
					node->mNodeName.C_Str(),
					std::vector<AssimpLoader::Type::KeyFrameAnimationParam>( maxFrame + 1 )
				);
			}

			// �m�[�h���̗v�f���擾
			for( auto node : nodes ) {
				auto nodeName = node->mNodeName.C_Str();
				auto numPositionKey = node->mNumPositionKeys - 1;
				auto numRotationKey = node->mNumRotationKeys - 1;
				auto numScalingKey = node->mNumScalingKeys - 1;

				if( numPositionKey > 0 ) {
					if( maxFrame == numPositionKey ) {
						for( int_t i = 0; i <= numPositionKey; i++ ) {
							ret[nodeName][node->mPositionKeys[i].mTime].m_position = node->mPositionKeys[i].mValue;
						}
					}
					else {
						// �L�[���Ȃ��������ԏ����ŃL�[�t���[���̐��l�����߂�
						for( int_t i = 0; i < numPositionKey; i++ ) {
							auto diff = static_cast<int_t>(node->mPositionKeys[i + 1].mTime - node->mPositionKeys[i].mTime );
							auto start = node->mPositionKeys[i].mValue;
							auto end = node->mPositionKeys[i + 1].mValue;

							for( int_t j = 0; j < diff; j++ ) {
								ret[nodeName][i + j].m_position = AssimpLoader::AiVector3Lerp( 
									start,
									end,
									Math::InverseLerp(0.0f, static_cast<float_t>(diff), static_cast<float_t>(j))
								);
							}
						}

						// �o�^����Ă���ŏI�t���[�������̃A�j���[�V�����S�̂̍ő�t���[�����ł͂Ȃ��ꍇ
						// �o�^����Ă���ŏI�t���[���̃p�����[�^���A�j���[�V�����S�̂̍ő�t���[���܂ł̊Ԃ̋�̃p�����[�^�̕����ƒu�������鏈��
						if( maxFrame != static_cast< int_t >( node->mPositionKeys[numPositionKey].mTime ) ) {
							auto lastFrame = static_cast< int_t >( node->mPositionKeys[numPositionKey].mTime );

							for( int_t i = lastFrame; i <= maxFrame; i++ ) {
								ret[nodeName][i].m_position = node->mPositionKeys[numPositionKey].mValue;
							}
						}
					}
				}
				else {
					for( int_t i = 0; i <= maxFrame; i++ ) {
						ret[nodeName][i].m_position = node->mPositionKeys[0].mValue;
					}
				}

				if( numRotationKey > 0 ) {
					if( maxFrame == numRotationKey ) {
						for( int_t i = 0; i <= numRotationKey; i++ ) {
							ret[nodeName][node->mRotationKeys[i].mTime].m_rotation = node->mRotationKeys[i].mValue;
						}
					}
					else {
						// �L�[���Ȃ��������ԏ����ŃL�[�t���[���̐��l�����߂�
						for( int_t i = 0; i < numRotationKey; i++ ) {
							auto diff = static_cast< int_t >( node->mRotationKeys[i + 1].mTime - node->mRotationKeys[i].mTime );
							auto start = node->mRotationKeys[i].mValue;
							auto end = node->mRotationKeys[i + 1].mValue;

							for( int_t j = 0; j < diff; j++ ) {
								ret[nodeName][i + j].m_rotation = AssimpLoader::AiQuaternionSlerp(
									start,
									end,
									Math::InverseLerp( 0.0f, static_cast< float_t >( diff ), static_cast< float_t >( j ) )
								);
							}
						}

						// �o�^����Ă���ŏI�t���[�������̃A�j���[�V�����S�̂̍ő�t���[�����ł͂Ȃ��ꍇ
						// �o�^����Ă���ŏI�t���[���̃p�����[�^���A�j���[�V�����S�̂̍ő�t���[���܂ł̊Ԃ̋�̃p�����[�^�̕����ƒu�������鏈��
						if( maxFrame != static_cast< int_t >( node->mRotationKeys[numRotationKey].mTime ) ) {
							auto lastFrame = static_cast< int_t >( node->mRotationKeys[numRotationKey].mTime );

							for( int_t i = lastFrame; i <= maxFrame; i++ ) {
								ret[nodeName][i].m_rotation = node->mRotationKeys[numRotationKey].mValue;
							}
						}
					}
				}
				else {
					for( int_t i = 0; i <= maxFrame; i++ ) {
						ret[nodeName][i].m_rotation = node->mRotationKeys[0].mValue;
					}
				}

				if( numScalingKey > 0 ) {
					if( maxFrame == numScalingKey ) {
						for( int_t i = 0; i <= numScalingKey; i++ ) {
							ret[nodeName][node->mScalingKeys[i].mTime].m_scaling = node->mScalingKeys[i].mValue;
						}
					}
					else {
						// �L�[���Ȃ��������ԏ����ŃL�[�t���[���̐��l�����߂�
						for( int_t i = 0; i < numScalingKey; i++ ) {
							auto diff = static_cast< int_t >( node->mScalingKeys[i + 1].mTime - node->mScalingKeys[i].mTime );
							auto start = node->mScalingKeys[i].mValue;
							auto end = node->mScalingKeys[i + 1].mValue;

							for( int_t j = 0; j < diff; j++ ) {
								ret[nodeName][i + j].m_scaling = AssimpLoader::AiVector3Lerp(
									start,
									end,
									Math::InverseLerp( 0.0f, static_cast< float_t >( diff ), static_cast< float_t >( j ) )
								);
							}
						}

						// �o�^����Ă���ŏI�t���[�������̃A�j���[�V�����S�̂̍ő�t���[�����ł͂Ȃ��ꍇ
						// �o�^����Ă���ŏI�t���[���̃p�����[�^���A�j���[�V�����S�̂̍ő�t���[���܂ł̊Ԃ̋�̃p�����[�^�̕����ƒu�������鏈��
						if( maxFrame != static_cast< int_t >( node->mScalingKeys[numScalingKey].mTime ) ) {
							auto lastFrame = static_cast< int_t >( node->mScalingKeys[numScalingKey].mTime );

							for( int_t i = lastFrame; i <= maxFrame; i++ ) {
								ret[nodeName][i].m_scaling = node->mScalingKeys[numScalingKey].mValue;
							}
						}
					}
				}
				else {
					for( int_t i = 0; i <= maxFrame; i++ ) {
						ret[nodeName][i].m_scaling = node->mScalingKeys[0].mValue;
					}
				}
			}

			return ret;
		}

		inline AssimpLoader::Type::Animations GetAnimations( aiAnimation* assimpAnimation ) {
			AssimpLoader::Type::Animations ret;
			auto nodes = AssimpLoader::Animation::GetAssimpAnimationNode( assimpAnimation );

			for( auto node : nodes ) {
				auto nodeName = node->mNodeName.C_Str();
				ret.m_positions.emplace(
					nodeName,
					AssimpLoader::Animation::GetAssimpAnimationPositionParam( node )
				);
				ret.m_rotations.emplace(
					nodeName,
					AssimpLoader::Animation::GetAssimpAnimationRotationParam( node )
				);
				ret.m_scalings.emplace(
					nodeName,
					AssimpLoader::Animation::GetAssimpAnimationScalingParam( node )
				);
			}

			return ret;
		}
	}



	namespace Test {
		inline void TestMethod() {

		}
	}
}