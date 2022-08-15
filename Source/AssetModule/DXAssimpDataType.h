// Assimp/material.hに定義されていること

// STL
#include<string>

// TNLIB
#include"SafetyDataType.h"

class DXAssimpDataType {
public:

	// シェーディングを方法
	enum ShadingMode {
		Flat,			// フラット
		Gouraud,		// グーロー
		Phong,			// フォン
		Blinn,			// ブリンフォン
		Toon,			// トゥーン
		OrenNayar,		// オーレンネイヤー
		Minnaert,		// ランバートシェーディングを使用したうえで暗さを考慮したシェーディング
		CookTorrance,	// クックトランス
		NoShading,		// シェーディングしない
		Fresnel			// フレネル
	};

	// マテリアルのタイプ
	enum MaterialType {
		Diffuse,
		Specular,
		Ambient,
		Emissive
	};

	// テクスチャに関する定義
	class Texture {
	public:
		// テクスチャのタイプ
		// 参照元：aiTextureType
		enum Type {
			None,			// テクスチャは存在しない
			Diffuse,		// ディフューズ
			Specular,		// スペキュラ
			Ambient,		// アンビエント
			Emissive,		// エミッシブ
			Height,			// 高さマップ
			Normal,			// 法線マップ
			Shininess,		// 輝度マップ
			Opacity,		// 透明度マップ
			Displacement,	// 凹凸マップ
			Light,			// ライトマップ
			Reflection,		// 反射マップ
			Unknown			// 識別できないテクスチャ・テクスチャタイプの数
		};

		// テクスチャの使用方法
		// 参照元：aiTextureFlags
		enum Flag {
			Invert,			// テクスチャの数値を1から引いた値を使用する
			UseAlpha,		// アルファ値を使用する
			IgnoreAlpha,	// アルファ値を使用しない
		};

		// 描画時のブレンディング方法
		// T1が元の色	（Source)
		// T2が自身の色	（Destination）
		// 参照元：aiTextureOp
		enum Operation {
			Mul,			// 乗算合成ブレンド						// T = T1 * T2
			Add,			// 加算合成ブレンド						// T = T1 + T2
			Sub,			// 減算合成ブレンド						// T = T1 - T2
			Div,			// 除算合成ブレンド						// T = T1 / T2
			SmoothAdd,		// 加算合成ブレンド - 乗算合成ブレンド	// T = (T1 + T2) - (T1 * T2)
			SignedAdd		// よくわからん							// T = T1 + (T2 - 0.5)
		};

		// マッピング時のUV座標の扱われ方
		// [0.0～1.0]の範囲外のUV座標をどのように扱うかを定義します。
		// 参照元：aiTextureMapMode
		enum MapMode {
			Wrap,			// テクスチャ座標u|vは、u%1|v%1に変換されます。
			Clamp,			// [0.0～1.0]以外のテクスチャ座標はの外側のテクスチャ座標は、最も近い有効な値にクランプされます。
			Decal,			// あるピクセルのテクスチャ座標が[0...1]の外側にある場合、そのピクセルにはテクスチャは適用されません。
			Mirror			// テクスチャ座標 u|v は、(u-(u%1.0))%2.0 が 0.0 ならば u%1.0|v%1.0 となり、それ以外なら 1.0-(u%1.0)|1.0-(v%1.0) となります。
		};

		// テクスチャマッピングが行われていない場合に
		// UV座標を生成する関数（aiProcess_GenUVCoords関数）で使用するマッピング方法
		// 参照元：aiTextureMapping
		enum Mapping {
			Uv,				// 
			Sphere,			// 球
			Cylinder,		// 円筒
			Box,			// 直方体
			Plane,			// 平面
			Other			// 未定義
		};
	};

public:
	static std::string GetTextureTypeName(int_t typeEnum);
};

#define DXAssimpMaxTextureType = DXAssimpDataType::TextureType::Unknown;