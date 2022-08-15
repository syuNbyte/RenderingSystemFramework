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

	// aiMesh内のデータに関する公式リファレンス：http://sir-kimmi.de/assimp/lib_html/structassimp_1_1mesh_1_1ai_mesh.html


	namespace Type {

		// 法線ベクトル・接戦ベクトル・従接線ベクトルをまとめたもの
		struct AssimpVertexVector {
		public:
			DXVector3 m_normal;		// 頂点の法線ベクトル
			DXVector3 m_tangent;	// 頂点の接線ベクトル
			DXVector3 m_bitangent;	// 頂点の従接線ベクトル
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
			unsigned int idx = -1;			// ボーン配列のインデックス番号
			aiMatrix4x4 Matrix;				// 初期配置行列
			aiMatrix4x4 AnimationMatrix;	// ボーン行列
			aiMatrix4x4 OffsetMatrix;		// ボーンオフセット行列
		};

		struct BoneWeights {
			int_t m_BoneIndex[4];		//	本来はボーンインデックスで管理するべき
			float_t m_BoneWeight[4];	//	ウェイト値
			int_t m_BoneNum;				//	ボーン数
			std::string	m_BoneName[4];	//	本来はボーンインデックスで管理するべき
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

			// 指定されたノードが存在するかどうかを返す
			bool HasNode( NodeName nodeName ) {
				return ( m_positions.find( nodeName ) != m_positions.end() );
			}

			// 指定されたノード名かつフレーム番号の座標パラメータが存在するかを返す
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

			// 指定されたノード名かつフレーム番号の回転パラメータが存在するかを返す
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
			
			// 指定されたノード名かつフレーム番号の拡縮パラメータが存在するかを返す
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

			// 保持する座標パラメータを取得する
			// パラメータが存在しなければ、aiVector3D(0,0,0)を返す
			aiVector3D GetPositionParam( NodeName nodeName, FrameNumber frame ) {
				if( HasPositionParam( nodeName, frame ) ) {
					return m_positions[nodeName][frame];
				}
				else {
					return aiVector3D( 0, 0, 0 );
				}
			}

			// 保持する回転パラメータを取得する
			// パラメータが存在しなければ、aiQuaternion(0,0,0,1)を返す
			aiQuaternion GetRotationParam( NodeName nodeName, FrameNumber frame ) {
				if( HasRotationParam( nodeName, frame ) ) {
					return m_rotations[nodeName][frame];
				}
				else {
					return aiQuaternion( 0, 0, 0, 1 );
				}
			}

			// 保持する拡縮パラメータを取得する
			// パラメータが存在しなければ、aiVector3D(1,1,1)を返す
			aiVector3D GetScalingParam( NodeName nodeName, FrameNumber frame ) {
				if( HasScalingParam( nodeName, frame ) ) {
					return m_scalings[nodeName][frame];
				}
				else {
					return aiVector3D( 1, 1, 1 );
				}
			}

			// 保持するアニメーションデータを取得する
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

			// 指定されたノードが存在するかどうかを返す
			bool HasNode( NodeName nodeName ) {
				return ( m_keyFrames.find( nodeName ) != m_keyFrames.end() );
			}

			// 指定されたノード名かつフレーム番号が存在するかを返す
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

			// 保持するアニメーションデータを取得する
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

		// Assimpのテクスチャタイプ　※内容はaiTextureTypeに詳細な説明が書いてある
		enum class TextureType {
			None			 = aiTextureType::aiTextureType_NONE,			// 無し
			Diffuse		 = aiTextureType::aiTextureType_DIFFUSE,		// ディフューズ
			Specular		 = aiTextureType::aiTextureType_SPECULAR,		// スペキュラ
			Ambient		 = aiTextureType::aiTextureType_AMBIENT,		// アンビエント
			Emissive		 = aiTextureType::aiTextureType_EMISSIVE,		// エミッシブ
			Height		 = aiTextureType::aiTextureType_HEIGHT,		// ハイトマップ
			Normal		 = aiTextureType::aiTextureType_NORMALS,		// 法線マップ
			Shininess	 = aiTextureType::aiTextureType_SHININESS,	// 輝度マップ
			Opacity		 = aiTextureType::aiTextureType_OPACITY,		// 透明度マップ
			Displacement = aiTextureType::aiTextureType_DISPLACEMENT,// ディスプレイスメントマップ
			LightMap		 = aiTextureType::aiTextureType_LIGHTMAP,		// ライトマップ
			Refrection	 = aiTextureType::aiTextureType_REFLECTION,	// 反射マップ
			Unknown		 = aiTextureType::aiTextureType_UNKNOWN		// 不明
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
		// aiMatrix4x4は列優先行列なので
		// DirectX用に行優先行列に置き換える（キャストと転置を同時に行っている）
		return DXMatrix4x4(
			m.a1, m.b1, m.c1, m.d1,
			m.a2, m.b2, m.c2, m.d2,
			m.a3, m.b3, m.c3, m.d3,
			m.a4, m.b4, m.c4, m.d4
		);
	}

	inline DXMatrix4x4 AiMatrix4x4Cast( aiMatrix4x4 m ) {
	// aiMatrix4x4は列優先行列なので
	// DirectX用に行優先行列に置き換える（キャストと転置を同時に行っている）
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


	// assimpMesh内のインデックス情報を全て取得
	inline AssimpLoader::Type::Indices GetAssimpIndices( aiMesh* m_assimpMesh ) {

		AssimpLoader::Type::Indices ret;

		// 面の数だけループ
		for( uint_t i = 0; i < m_assimpMesh->mNumFaces; i++ ) {

			// 面のデータを取得
			aiFace face = m_assimpMesh->mFaces[i];

			// 面のインデックスの数だけループ
			for( uint_t j = 0; j < face.mNumIndices; j++ ) {
				ret.emplace_back( face.mIndices[j] );
			}
		}

		return ret;
	}

	// assimpMesh内の頂点座標を全て取得
	inline AssimpLoader::Type::Positions GetAssimpVertexPositions( aiMesh* assimpMesh ) {

		AssimpLoader::Type::Positions ret;
		ret.resize( assimpMesh->mNumVertices );

		for( int i = 0; i < assimpMesh->mNumVertices; i++ ) {
			ret[i] = AssimpLoader::AiVector3Cast( assimpMesh->mVertices[i] );
		}

		return ret;
	}

	// assimpMesh内の頂点法線を全て取得
	inline AssimpLoader::Type::Normals GetAssimpVertexNormals( aiMesh* assimpMesh ) {

		AssimpLoader::Type::Normals ret;

		if( assimpMesh->HasNormals() ) {
			ret.resize( assimpMesh->mNumVertices );
			for( int i = 0; i < assimpMesh->mNumVertices; i++ ) {
				ret[i] = AssimpLoader::AiVector3Cast( assimpMesh->mNormals[i] );
			}
		}

		// 保持していない場合は空のデータを返すのでemptyメソッドなどを使用して判定するとよい
		return ret;

	}

	// assimpMesh内の頂点接線を全て取得
	inline AssimpLoader::Type::Tangents GetAssimpVertexTangents( aiMesh* assimpMesh ) {

		AssimpLoader::Type::Tangents ret;

		if( assimpMesh->HasTangentsAndBitangents() ) {
			ret.resize( assimpMesh->mNumVertices );
			for( int i = 0; i < assimpMesh->mNumVertices; i++ ) {
				ret[i] = AssimpLoader::AiVector3Cast( assimpMesh->mTangents[i] );
			}
		}

		// 保持していない場合は空のデータを返すのでemptyメソッドなどを使用して判定するとよい
		return ret;

	}

	// assimpMesh内の頂点従接線を全て取得
	inline AssimpLoader::Type::BiTangents GetAssimpVertexBiTangents( aiMesh* assimpMesh ) {

		AssimpLoader::Type::BiTangents ret;
		if( assimpMesh->HasTangentsAndBitangents() ) {
			ret.resize( assimpMesh->mNumVertices );
			for( int i = 0; i < assimpMesh->mNumVertices; i++ ) {
				ret[i] = AssimpLoader::AiVector3Cast( assimpMesh->mBitangents[i] );
			}
		}

		// 保持していない場合は空のデータを返すのでemptyメソッドなどを使用して判定するとよい
		return ret;

	}

	// assimpMesh内の一つの頂点データをインデックスを指定して取得
	inline AssimpLoader::Type::VertexVectors GetAssimpVertexVectors( aiMesh* assimpMesh ) {

		AssimpLoader::Type::VertexVectors ret;
		ret.resize( assimpMesh->mNumVertices );

		// 取得したいデータが存在するか判定
		auto hasNormal = assimpMesh->HasNormals();										// 頂点法線ベクトルがあるか判定
		auto hasTangentAndBiTangent = assimpMesh->HasTangentsAndBitangents();	// 頂点従接線ベクトル,  頂点接線ベクトルがあるか判定

		for( int i = 0; i < assimpMesh->mNumVertices; i++ ) {
			ret[i].m_normal = ( hasNormal ) ? AssimpLoader::AiVector3Cast( assimpMesh->mNormals[i] ) : DXVector3::Zero();							// 法線ベクトル
			ret[i].m_tangent = ( hasTangentAndBiTangent ) ? AssimpLoader::AiVector3Cast( assimpMesh->mTangents[i] ) : DXVector3::Zero();		// 接線ベクトル
			ret[i].m_bitangent = ( hasTangentAndBiTangent ) ? AssimpLoader::AiVector3Cast( assimpMesh->mBitangents[i] ) : DXVector3::Zero();	// 従接線ベクトル
		}

		return ret;
	}

	// assimpMeshないからテクスチャのUV座標を取得する
	inline AssimpLoader::Type::UVs GetAssimpUVs( aiMesh* assimpMesh, int_t index ) {

		// 以下の二つに関してはUVの与えられた数に関係すると思うので要検証
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

	// テクスチャファイル名を取得
	inline std::string GetAssimpTextureFileName( aiMaterial* assimpMaterial, aiTextureType assimpTextureType, int index ) {
		aiString ai_str;
		assimpMaterial->GetTexture( assimpTextureType, index, &ai_str );
		return ai_str.C_Str();
	}

	// テクスチャデータを取得
	/*
	inline std::vector<Texture*> GetAssimpTexture(
		aiMesh* assimpMesh,
		aiMaterial* assimpMaterial,
		aiTextureType assimpTextureType,
		std::filesystem::path modelResourceFolderPath
	) {

		std::vector<Texture*> ret;
		auto texCount = assimpMaterial->GetTextureCount( assimpTextureType );

		// テクスチャがあるなら取得する
		for( int_t i = 0; i < texCount; i++ ) {

			std::filesystem::path texFilePath = NewAssimpLoader::GetAssimpTextureFileName( assimpMaterial, assimpTextureType, i );

			{
				auto texPointer = TextureManager::CreateAndGetTexture( texFilePath );
				//if( texPointer != nullptr) {
				if( texPointer->HasTexture() ) {
					ret.emplace_back( texPointer );
					Debug::Log("TextureFilePath[", texFilePath, "]", "Load Success.");
					// 読み込みができたなら
					// 下のコードブロックに入らないように
					continue;
				}
			}
			{
				// 新しくファイルへのパスを作成する
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

	// 使用するプリミティブ型を取得
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

		// ディフューズ色を取得
		inline DXColor GetDiffuse( aiMaterial* assimpMaterial ) {
			aiColor3D color;
			assimpMaterial->Get( AI_MATKEY_COLOR_DIFFUSE, color );
			return DXColor( color.r, color.g, color.b, 1.0f );
		}

		// スペキュラー色を取得
		inline DXColor GetSpecular( aiMaterial* assimpMaterial ) {
			aiColor3D color;
			float_t power;
			assimpMaterial->Get( AI_MATKEY_COLOR_SPECULAR, color );		// スペキュラー
			assimpMaterial->Get( AI_MATKEY_SHININESS_STRENGTH, power );	// スペキュラーの強さ
			return DXColor( color.r, color.g, color.b, power );
		}

		// アンビエント色を取得
		inline DXColor GetAmbient( aiMaterial* assimpMaterial ) {
			aiColor3D color;
			assimpMaterial->Get( AI_MATKEY_COLOR_AMBIENT, color );
			return DXColor( color.r, color.g, color.b, 1.0f );
		}

		// エミッシブ色を取得
		inline DXColor GetEmissive( aiMaterial* assimpMaterial ) {
			aiColor3D color;
			assimpMaterial->Get( AI_MATKEY_COLOR_EMISSIVE, color );
			return DXColor( color.r, color.g, color.b, 0.0f );
		}

		// 反射色を取得
		inline DXColor GetReflective(aiMaterial* assimpMaterial) {
			aiColor3D color;
			assimpMaterial->Get(AI_MATKEY_COLOR_REFLECTIVE, color);
			return DXColor(color.r, color.g, color.b, 1.0f);
		}

		// 不当明度を取得
		inline DXColor GetTransparent(aiMaterial* assimpMaterial) {
			aiColor3D color;
			assimpMaterial->Get(AI_MATKEY_COLOR_TRANSPARENT, color);
			return DXColor(color.r, color.g, color.b, 1.0f);
		}

	}


	// 全てのAssimpNodeを取り出す内部的な処理
	inline void _Internal_GetAssimpNode( aiNode* assimpNode, std::vector<aiNode*>& nodes ) {
		for( int_t i = 0; i < assimpNode->mNumChildren; i++ ) {
			nodes.emplace_back( assimpNode->mChildren[i] );
			_Internal_GetAssimpNode( assimpNode->mChildren[i], nodes );
		}
	}

	// ルートノード以外の全てのAssimpNodeを取り出す
	inline std::vector<aiNode*> GetAssimpNode( aiNode* assimpRootNode ) {
		std::vector<aiNode*> ret;
		_Internal_GetAssimpNode( assimpRootNode, ret );
		return ret;
	}

	// ルートノードを含む全てのAssimpNodeを取り出す
	inline std::vector<aiNode*> GetAssimpNodeEx( aiNode* assimpRootNode ) {
		std::vector<aiNode*> ret;
		ret.emplace_back( assimpRootNode );
		_Internal_GetAssimpNode( assimpRootNode, ret );
		return ret;
	}

	// このAssimpNodeの持つAssimpMeshを全て取得する
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

	// このAssimpNodeの持つAssimpMaterialを全て取得する
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
		// 初期化
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

	// TODO アニメーションの読み込み処理を作る
	inline std::unordered_map<std::string, AssimpLoader::Type::Bone> GetAssimpAnimation( aiNode* assimpRootNode ) {
		std::unordered_map<std::string, AssimpLoader::Type::Bone> ret;
		AssimpLoader::_Internal_GetAssimpAnimation( assimpRootNode, ret );

		// ボーンのインデックスを設定
		uint_t i = 0;
		for( auto& bone : ret ) bone.second.idx = i++;

		return ret;
	}


	// AssimpMesh内のAssimpBoneを全て取得する
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
			
			// オフセット行列を設定
			mybones[bones[bi]->mName.C_Str()].OffsetMatrix = bones[bi]->mOffsetMatrix;

			for( int_t wi = 0; wi < bones[bi]->mNumWeights; wi++ ) {	// wi : weight index
				auto weight = bones[bi]->mWeights;
				auto vi = weight[wi].mVertexId;
				
				auto& boneNum = ret[vi].m_BoneNum;
				const auto boneName = bones[bi]->mName.C_Str();

				ret[vi].m_BoneWeight[boneNum] = weight->mWeight;		// 頂点ウェイト
				ret[vi].m_BoneName[boneNum] = boneName;					// 頂点の所属するボーン名
				ret[vi].m_BoneIndex[boneNum] = mybones[boneName].idx;	// 頂点インデックス
				++ret[vi].m_BoneNum;
			}
		}

		return ret;
	}

	// このAssimpScene内のアニメーション時の頂点情報を全て取得する
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

	// このAssimpScene内のアニメーション時の頂点情報を全て取得する
	inline std::vector<std::vector<AssimpLoader::Type::BoneWeights>> GetAssimpMeshBoneVertices( const aiScene* assimpScene, aiNode* assimpRootNode ) {
		std::vector<std::vector<AssimpLoader::Type::BoneWeights>> ret;

		auto nodes = GetAssimpNode( assimpRootNode );
		for( int_t i = 0; i < nodes.size(); i++ ) {
			ret.emplace_back(_Internal_GetAssimpMeshBoneVertices( assimpScene, nodes[i] ));
		}

		return ret;
	}

		// このAssimpScene内のアニメーション時の頂点情報を全て取得する
	inline std::vector<AssimpLoader::Type::BoneWeights> GetAssimpMeshBoneVertices_( aiNode* assimpNode, aiMesh* assimpMesh ) {
		std::vector<AssimpLoader::Type::BoneWeights> ret;

		auto anims = GetAssimpAnimation( assimpNode );
		auto vertices = _Internal_GetAssimpMeshBoneVertices( assimpMesh, anims );
		ret.resize( ret.size() + vertices.size() );
		std::copy( vertices.begin(), vertices.end(), std::back_inserter( ret ) );

		return ret;
	}



	namespace Bone {

		// このAssimpScene内のボーンの情報を全て取得する
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
						// メッシュに関連づいてるボーン情報を取得
						auto bone = mesh->mBones[b];
						// ボーンオフセット行列を取得
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

		// このAssimpMesh内の頂点データのボーン関連の情報を全て取得する
		inline std::vector<AssimpLoader::Type::BoneWeights> NewGetAssimpMeshBoneWeights(
			aiMesh* assimpMesh,
			std::map<std::string, AssimpLoader::Type::Bone>& bones // NewAssimpLoader::Bone::NewGetAssimpBoneメソッドの戻り値を使う
		) {
			std::vector<AssimpLoader::Type::BoneWeights> ret;

			// ボーンに対応する頂点データの初期化
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


			// ボーンデータ初期化
			// AnimModelData::processMesh[Line : 682]
			for( int_t b = 0; b < assimpMesh->mNumBones; b++ ) {

				// メッシュに関連づいてるボーン情報を取得
				auto bone = assimpMesh->mBones[b];
				auto boneName = bone->mName.C_Str();

				// ボーンに関連づいている頂点を選び､ウェイト値をセットする
				for( int_t w = 0; w < bone->mNumWeights; w++ ) {
					auto weight = bone->mWeights[w];
					auto vertexID = bone->mWeights[w].mVertexId;
					auto boneNum = ret[vertexID].m_BoneNum;

					ret[vertexID].m_BoneWeight[boneNum] = weight.mWeight;
					ret[vertexID].m_BoneName[boneNum] = boneName;
					ret[vertexID].m_BoneIndex[boneNum] = bones[boneName].idx;
					ret[vertexID].m_BoneNum++;

					// Boneの数が4つ以下なら問題なし
					// Boneの数が5つ以上ならアサートを送出
					assert( ret[vertexID].m_BoneNum <= 4 );
				}

			}

			return ret;
		}

	}



	namespace Animation {

	// このAssimpSceneが保持する全てのアニメーションデータ名を取得する
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

		// このAssimpSceneが保持する全てのアニメーションデータを取得する
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

		// このAssimpSceneが保持する全てのアニメーションデータを取得する
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

		// このAssimpAnimationが保持するアニメーションノードを全て取得する
		inline std::vector<aiNodeAnim*> GetAssimpAnimationNode( aiAnimation* assimpAnim ) {
			std::vector<aiNodeAnim*> ret;
			auto channelNum = assimpAnim->mNumChannels;
			ret.reserve( channelNum );

			for( int_t i = 0; i < channelNum; i++ ) {
				ret.push_back( assimpAnim->mChannels[i] );
			}

			return ret;
		}

		// このAssimpAnimationNode内のキーフレーム情報から座標パラメータのみを取得する
		// @ret
		// 第一要素がフレーム番号
		// 第二要素がフレーム番号に対応する座標パラメータ
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

		// このAssimpAnimationNode内のキーフレーム情報から回転パラメータのみを取得する
		// @ret
		// 第一要素がフレーム番号
		// 第二要素がフレーム番号に対応する座標パラメータ
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

		// このAssimpAnimationNode内のキーフレーム情報から拡縮パラメータのみを取得する
		// @ret
		// 第一要素がフレーム番号
		// 第二要素がフレーム番号に対応する座標パラメータ
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

			// ノード名とそれに対応するstd::vector<KeyFrame>の要素を確保
			for( auto node : nodes ) {
				ret.emplace(
					node->mNodeName.C_Str(),
					std::vector<AssimpLoader::Type::KeyFrameAnimationParam>( maxFrame + 1 )
				);
			}

			// ノード内の要素を取得
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
						// キーがない部分を補間処理でキーフレームの数値を求める
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

						// 登録されている最終フレームがこのアニメーション全体の最大フレーム数ではない場合
						// 登録されている最終フレームのパラメータをアニメーション全体の最大フレームまでの間の空のパラメータの部分と置き換える処理
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
						// キーがない部分を補間処理でキーフレームの数値を求める
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

						// 登録されている最終フレームがこのアニメーション全体の最大フレーム数ではない場合
						// 登録されている最終フレームのパラメータをアニメーション全体の最大フレームまでの間の空のパラメータの部分と置き換える処理
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
						// キーがない部分を補間処理でキーフレームの数値を求める
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

						// 登録されている最終フレームがこのアニメーション全体の最大フレーム数ではない場合
						// 登録されている最終フレームのパラメータをアニメーション全体の最大フレームまでの間の空のパラメータの部分と置き換える処理
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