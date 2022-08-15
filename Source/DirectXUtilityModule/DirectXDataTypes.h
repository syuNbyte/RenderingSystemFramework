#pragma once

// DirectXやWindows関連のシステムに使用する標準ヘッダファイルのインクルード
#include<d3d11.h>				// DirectX11のAPIが入っている
#include<d3dcompiler.h>		// シェーダーのコンパイラ
#include<dxgidebug.h>		// IDXGIDebugを使用するため
#include<wrl/client.h>		// デバイスの管理をしやすくするためのDirectX版のスマートポインタのようなもの　※Microsoft::ComPtr : Microsoft::ComponentObjectPointerの略
#include<DirectXMath.h>		// XMFLOAT系のデータ型を使用するため

//DirectXAPI関係のリンク
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// ComPtr(DirectX版のスマートポインタ)
using Microsoft::WRL::ComPtr;

// XMFLOAT系のデータ型を楽に定義するため
using namespace DirectX;

// ComPtrを使用したオブジェクトのエイリアス宣言を行っている
namespace ComObject {

//IDXGI系
	using IDxgiAdapter = ComPtr<IDXGIAdapter>;								// アダプタ
	using IDxgiAdapter_1 = ComPtr<IDXGIAdapter1>;							// アダプタ1
	using IDxgiFactory = ComPtr<IDXGIFactory>;								// ファクトリー
	using IDxgiFactory_1 = ComPtr<IDXGIFactory1>;							// ファクトリー1
	using IDxgiOutput = ComPtr<IDXGIOutput>;									// アウトプット
	using SwapChain = ComPtr<IDXGISwapChain>;									// スワップチェイン
	using IDxgiDebug = ComPtr<IDXGIDebug>;										// デバッグ用オブジェクト
	using IDxgiDebug_1 = ComPtr<IDXGIDebug1>;									// デバッグ用オブジェクトの拡張版（IDXGIDebug型を継承している）

//ID3D11系
	using DirectXDebug = ComPtr<ID3D11Debug>;									// DirectX11のデバッグを行うオブジェクト　※バグが起こった際にログを出してくれるオブジェクトっぽい？
	using DirectXDevice = ComPtr<ID3D11Device>;								// デバイス
	using DirectXDeviceContext = ComPtr<ID3D11DeviceContext>;			// デバイスコンテキスト（イミディエイトコンテキスト(即時)・ディファードコンテキスト(記録した後一括実行)）
	using ImmediateContext = ComPtr<ID3D11DeviceContext>;					// イミディエイトコンテキスト(即時実行するデバイスコンテキスト)
	using DeferredContext = ComPtr<ID3D11DeviceContext>;					// ディファードコンテキスト(実行内容を記録した後一括実行するデバイスコンテキスト）
	using DeferredCommandList = ComPtr<ID3D11CommandList>;				// ディファードコンテキストで使用するコマンドリスト型
	using DirectXBuffer = ComPtr<ID3D11Buffer>;								// 各種バッファ
	using ConstantBuffer = ComPtr<ID3D11Buffer>;								// 定数バッファ
	using VertexBuffer = ComPtr<ID3D11Buffer>;								// 頂点バッファ
	using IndexBuffer = ComPtr<ID3D11Buffer>;									// インデックスバッファ
	using StructuredBuffer = ComPtr<ID3D11Buffer>;							// ストラクチャードバッファ
	using InputLayout = ComPtr<ID3D11InputLayout>;							// 頂点レイアウト
	using VertexShader = ComPtr<ID3D11VertexShader>;						// 頂点シェーダー
	using PixelShader = ComPtr<ID3D11PixelShader>;							// ピクセルシェーダー
	using GeometryShader = ComPtr<ID3D11GeometryShader>;					// ジオメトリシェーダー
	using ComputeShader = ComPtr<ID3D11ComputeShader>;						// コンピュートシェーダー
	using Resource = ComPtr<ID3D11Resource>;									// リソース	(テクスチャ関連)
	using ShaderResourceView = ComPtr<ID3D11ShaderResourceView>;		// シェーダーリソースビュー (テクスチャ関連)
	using ComputeOutputView = ComPtr<ID3D11UnorderedAccessView>;		// アンオーダードアクセスビュー
	using UnorderedAccessView = ComPtr<ID3D11UnorderedAccessView>;		// アンオーダードアクセスビュー
	using RenderTargetView = ComPtr<ID3D11RenderTargetView>;				// レンダーターゲットビュー
	using Texture2D = ComPtr<ID3D11Texture2D>;								// テクスチャ2D
	using DepthStencilBuffer = ComPtr<ID3D11Texture2D>;					// 深度ステンシルバッファ
	using DepthStencilState = ComPtr<ID3D11DepthStencilState>;			// 深度ステンシルステート
	using DepthStencilView = ComPtr<ID3D11DepthStencilView>;				// 深度ステンシルビュー
	using RasterizerState = ComPtr<ID3D11RasterizerState>;				// ラスタライザステート
	using BlendState = ComPtr<ID3D11BlendState>;								// ブレンドステート
	using SamplerState = ComPtr<ID3D11SamplerState>;						// サンプラーステート
	using ClassLinkage = ComPtr<ID3D11ClassLinkage>;						// クラスリンケージ
	using ClassInstance = ComPtr<ID3D11ClassInstance>;						// クラスインスタンス
	using ShaderReflection = ComPtr<ID3D11ShaderReflection>;											// シェーダーリフレクション
	//using ShaderReflectionConstantBuffer = ComPtr<ID3D11ShaderReflectionConstantBuffer>;	// 定数バッファ用のシェーダーリフレクション
	//using ShaderReflectionType = ComPtr<ID3D11ShaderReflectionType>;							// シェーダーリフレクションのタイプ
	//using ShaderReflectionVariable = ComPtr<ID3D11ShaderReflectionVariable>;					// シェーダーリフレクションの変数
}

//IDXGI系
	using IDxgiAdapter = IDXGIAdapter;											// アダプタ
	using IDxgiAdapter_1 = IDXGIAdapter1;										// アダプタ1
	using IDxgiFactory = IDXGIFactory;											// ファクトリー
	using IDxgiFactory_1 = IDXGIFactory1;										// ファクトリー1
	using IDxgiOutput = IDXGIOutput;												// アウトプット
	using SwapChain = IDXGISwapChain;											// スワップチェイン
	using IDxgiDebug = IDXGIDebug;												// デバッグ用オブジェクト
	using IDxgiDebug_1 = IDXGIDebug1;											// デバッグ用オブジェクトの拡張版（IDXGIDebug型を継承している）

//DXGI系
	using DxgiFormat = DXGI_FORMAT;												// フォーマット

// D3D系
	using FeatureLevel = D3D_FEATURE_LEVEL;

//ID3D11系
	using DirectXDevice = ID3D11Device;											// デバイス
	using DirectXDeviceContext = ID3D11DeviceContext;						// デバイスコンテキスト（イミディエイトコンテキスト(即時)・ディファードコンテキスト(記録した後一括実行)）
	using ImmediateContext = ID3D11DeviceContext;							// イミディエイトコンテキスト(即時)
	using DeferredContext = ID3D11DeviceContext;								// ディファードコンテキスト(記録した後一括実行）
	using DeferredCommandList = ID3D11CommandList;							// ディファードコンテキストで使用するコマンドリスト型
	using DirectXBuffer = ID3D11Buffer;											// 各種バッファ
	using ConstantBuffer = ID3D11Buffer;										// 定数バッファ
	using VertexBuffer = ID3D11Buffer;											// 頂点バッファ
	using IndexBuffer = ID3D11Buffer;											// インデックスバッファ
	using StructuredBuffer = ID3D11Buffer;										// ストラクチャードバッファ
	using InputLayout = ID3D11InputLayout;										// 頂点レイアウト
	using VertexShader = ID3D11VertexShader;									// 頂点シェーダー
	using PixelShader = ID3D11PixelShader;										// ピクセルシェーダー
	using GeometryShader = ID3D11GeometryShader;								// ジオメトリシェーダー
	using ComputeShader = ID3D11ComputeShader;								// コンピュートシェーダー
	using Resource = ID3D11Resource;												// リソース	(テクスチャ関連)
	using ShaderResourceView = ID3D11ShaderResourceView;					// シェーダーリソースビュー (テクスチャ関連)
	using ComputeOutputView = ID3D11UnorderedAccessView;					// アンオーダードアクセスビュー
	using UnorderedAccessView = ID3D11UnorderedAccessView;				// アンオーダードアクセスビュー
	using RenderTargetView = ID3D11RenderTargetView;						// レンダーターゲットビュー
	using Texture2D = ID3D11Texture2D;											// テクスチャ2D
	using DepthStencilBuffer = ID3D11Texture2D;								// 深度ステンシルバッファ
	using DepthStencilState = ID3D11DepthStencilState;						// 深度ステンシルステート
	using DepthStencilView = ID3D11DepthStencilView;						// 深度ステンシルビュー
	using RasterizerState = ID3D11RasterizerState;							// ラスタライザステート
	using BlendState = ID3D11BlendState;										// ブレンドステート
	using SamplerState = ID3D11SamplerState;									// サンプラーステート
	using MappedSubResource = D3D11_MAPPED_SUBRESOURCE;					// マップドサブリソース
	using ViewPort = D3D11_VIEWPORT;												// ビューポート
	using SubResourceData = D3D11_SUBRESOURCE_DATA;							// サブリソースデータ
	using ClassLinkage = ID3D11ClassLinkage;									// クラスリンケージ
	using ClassInstance = ID3D11ClassInstance;								// クラスインスタンス
	using ShaderReflection = ID3D11ShaderReflection;										// シェーダーリフレクション
	using ShaderReflectionConstantBuffer = ID3D11ShaderReflectionConstantBuffer;	// 定数バッファ用のシェーダーリフレクション
	using ShaderReflectionType = ID3D11ShaderReflectionType;								// シェーダーリフレクションのタイプ
	using ShaderReflectionVariable = ID3D11ShaderReflectionVariable;					// シェーダーリフレクションの変数

//===================================================
// 特殊なデータ型
//===================================================
// 参考文献：https://e-words.jp/w/BLOB.html
//======================================================================================================
// BLOB：BinaryLargeObject：不定長のバイナリデータ（大きさの定まっていないバイナリデータ）
//======================================================================================================
	using DirectXBLOB = ID3DBlob;													// 自己定義名称省略型
	using DirectXBinaryObject = ID3DBlob;										// 自己定義名称省略型
	using DirectXBinaryLargeObject = ID3DBlob;								// 自己定義名称無省略型

//～DESC系 // tips : DESCはDescription（説明）の略っぽい
	using DxgiAdapterDesc = DXGI_ADAPTER_DESC;
	using DxgiAdapterDesc_1 = DXGI_ADAPTER_DESC1;
	using InputElementDesc = D3D11_INPUT_ELEMENT_DESC;						// 入力属性（頂点データに持たせるデータ）
	using RasterizerDesc = D3D11_RASTERIZER_DESC;							// ラスタライザデスク
	using Texture2DDesc = D3D11_TEXTURE2D_DESC;								// テクスチャ2Dデスク、深度バッファデスク
	using DepthBufferDesc = D3D11_TEXTURE2D_DESC;							// テクスチャ2Dデスク、深度バッファデスク
	using ShaderResourceViewDesc = D3D11_SHADER_RESOURCE_VIEW_DESC;	// シェーダーリソースビューデスク
	using SamplerDesc = D3D11_SAMPLER_DESC;									// サンプラーデスク
	using BlendDesc = D3D11_BLEND_DESC;											// ブレンドデスク
	using RenderTargetViewDesc = D3D11_RENDER_TARGET_VIEW_DESC;			// レンダーターゲットビューデスク
	using RenderTargetBlendDesc = D3D11_RENDER_TARGET_BLEND_DESC;		// レンダーターゲットブレンドデスク　※D3D11_BLEND_DESC.RenderTarget[0]と同義
	using DepthStencilDesc = D3D11_DEPTH_STENCIL_DESC; 					// 深度ステンシルデスク
	using DepthStencilViewDesc = D3D11_DEPTH_STENCIL_VIEW_DESC;			// 深度ステンシルビューデスク
	using SwapChainDesc = DXGI_SWAP_CHAIN_DESC;								// スワップチェインデスク
	using ModeDesc = DXGI_MODE_DESC;												// モードデスク
	using BufferDesc = D3D11_BUFFER_DESC;										// バッファデスク
	using UnorderedAccessViewDesc = D3D11_UNORDERED_ACCESS_VIEW_DESC;	// アンオーダードアクセスビューデスク

// トポロジー
//enum D3D_PRIMITIVE_TOPOLOGY内の D3D11の部分のみを抜粋したもの
enum class PrimitiveTopology {


	D3D11_UNDEFINED = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED,
	D3D11_POINTLIST = D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
	D3D11_LINELIST = D3D_PRIMITIVE_TOPOLOGY_LINELIST,
	D3D11_LINESTRIP = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
	D3D11_TRIANGLELIST = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	D3D11_TRIANGLESTRIP = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	D3D11_LINELIST_ADJ = D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ,
	D3D11_LINESTRIP_ADJ = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ,
	D3D11_TRIANGLELIST_ADJ = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ,
	D3D11_TRIANGLESTRIP_ADJ = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ,

	D3D11_1_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST,
	D3D11_2_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST,
	D3D11_3_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
	D3D11_4_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST,
	D3D11_5_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST,
	D3D11_6_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST,
	D3D11_7_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST,
	D3D11_8_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST,
	D3D11_9_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST,
	D3D11_10_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST,
	D3D11_11_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST,
	D3D11_12_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST,
	D3D11_13_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST,
	D3D11_14_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST,
	D3D11_15_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST,
	D3D11_16_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST,
	D3D11_17_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST,
	D3D11_18_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST,
	D3D11_19_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST,
	D3D11_20_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST,
	D3D11_21_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST,
	D3D11_22_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST,
	D3D11_23_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST,
	D3D11_24_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST,
	D3D11_25_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST,
	D3D11_26_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST,
	D3D11_27_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST,
	D3D11_28_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST,
	D3D11_29_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST,
	D3D11_30_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST,
	D3D11_31_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST,
	D3D11_32_CONTROL_POINT_PATCHLIST = D3D_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST


};

// 16バイトアラインメントマクロ
#define ALIGN16 _declspec(align(16))

//16バイトアラインメントが行われた基本的なデータ型
using ALIGN_INT = ALIGN16 int;					// 元の大きさ：4byte
using ALIGN_UINT = ALIGN16 unsigned int;		// 元の大きさ：4byte
using ALIGN_FLOAT = ALIGN16 float;				// 元の大きさ：4byte
using ALIGN_DOUBLE = ALIGN16 double;			// 元の大きさ：8byte

//16バイトアラインメントが行われたDirectXMathに入っているデータ型
using ALIGN_XMFLOAT2 = ALIGN16 XMFLOAT2;		// 元の大きさ：8byte
using ALIGN_XMFLOAT3 = ALIGN16 XMFLOAT3;		// 元の大きさ：12byte
using ALIGN_XMFLOAT4 = ALIGN16 XMFLOAT4;		// 元の大きさ：16byte
using ALIGN_XMMATRIX = ALIGN16 XMMATRIX;		// 知らない
using ALIGN_XMVECTOR = ALIGN16 XMVECTOR;		// 知らない
using ALIGN_XMFLOAT4X4 = ALIGN16 XMFLOAT4X4;	// 知らない

// 安全にComPtrを解放するためのマクロ
#define SafeReleaseCom( comObject ) {\
	if (comObject) {\
		comObject.Reset();\
	}\
}