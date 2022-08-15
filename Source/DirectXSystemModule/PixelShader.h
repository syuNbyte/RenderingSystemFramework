#pragma once

// STL
#include<string>
#include<unordered_map>

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"SafetyDataType.h"

// DirectXのピクセルシェーダークラスが継承するインターフェイスクラス
class IDXPixelShader {
public:
	virtual void SetFilePath(const std::string& path) = 0;
	virtual void SetEntryFuncName(const std::string& funcName) = 0;
	virtual void SetVersion(const std::string& version) = 0;
	virtual void Create(DirectXDevice* device) = 0;
};

// DirectXのピクセルシェーダーを扱うクラス
class DXPixelShader : public IDXPixelShader {
protected:
	ComObject::PixelShader m_ps;
protected:
	std::string m_path;								// シェーダーファイルのパス
	std::string m_funcName;							// シェーダーファイルのエントリ関数名
	std::string m_version;							// シェーダーファイルの使用するシェーダーモデルバージョン
public:
	DXPixelShader();
	~DXPixelShader();
protected:
	bool IsCsoFile();
	void CompileShader(void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob);
	void ReadCompiledShaderFile(void*& shaderObject, std::size_t& shaderObjectSize);
	void CompileShaderFromFile(void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob);
	void CreatePixelShader(DirectXDevice* device, void*& shaderObject, std::size_t& shaderObjectSize);
public:
	void Create(DirectXDevice* device);					// 設定されている情報でピクセルシェーダーを作成する
	void CreateAndCompile(DirectXDevice* device);	// 設定されている情報でシェーダーファイルをコンパイルしてからピクセルシェーダーを作成する
	void SetFilePath(const std::string& path);
	void SetEntryFuncName(const std::string& funcName);
	void SetVersion(const std::string& version);
public:
	void PSSetShader(DirectXDeviceContext* context);
};


// クラスリンケージ用に調整されたDirectXの頂点シェーダーを扱うクラス
class DXPixelShaderLink : public IDXPixelShader {
protected:
	ComObject::PixelShader m_ps;																	// ピクセルシェーダー
	ComObject::ClassLinkage m_classLinkage;													// クラスリンケージ
	std::unordered_map<std::string, ComObject::ClassInstance> m_classInstances;	// クラスインスタンス
protected:
	std::string m_path;																				// シェーダーファイルのパス
	std::string m_funcName;																			// シェーダーファイルのエントリ関数名
	std::string m_version;																			// シェーダーファイルの使用するシェーダーモデルバージョン
public:
	DXPixelShaderLink();
	~DXPixelShaderLink();
protected:
	bool IsCsoFile();
	void CompileShader(void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob);
	void ReadCompiledShaderFile(void*& shaderObject, std::size_t& shaderObjectSize);
	void CompileShaderFromFile(void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob);
	void CreateClassLinkage(DirectXDevice* device);
	void CreatePixelShader(DirectXDevice* device, void*& shaderObject, std::size_t& shaderObjectSize);
public:
	void Create(DirectXDevice* device);						// 設定されている情報でピクセルシェーダーを作成する
	void CreateAndCompile(DirectXDevice* device);		// 設定されている情報でシェーダーファイルをコンパイルしてからピクセルシェーダーを作成する
	void CreateClassInstance(const std::string& key);	// 動的シェーダーリンクのクラスインスタンスを作成する
	void GetClassInstance(const std::string& key);		// 動的シェーダーリンクのクラスインスタンスを取得する
	void SetFilePath(const std::string& path);
	void SetEntryFuncName(const std::string& funcName);
	void SetVersion(const std::string& version);
public:
	void PSSetShader(DirectXDeviceContext* context, const std::string& key);
};

/*
* ===========================
* DXVertexShader Example
* ===========================
* 
* // インスタンスの作成
* DXVertexShader ps;
*
* // 事前情報の設定
* ps.SetFilePath("main_ps.hlsl");
* ps.SetEntryFuncName("main");
* ps.SetVersion("ps_5_0");
*
* // 作成
* ps.Create(DirectXManager::GetDevice());
*
* // 使用
* ps.psSetShader(DirectXManager::GetImmediateContext().Get());
*
*/



/*
* ===========================
* DXVertexShaderLink Example
* ===========================
* 
* // インスタンスの作成
* DXVertexShader ps;
*
* // 事前情報の設定
* ps.SetFilePath("main_ps.hlsl");
* ps.SetEntryFuncName("main");
* ps.SetVersion("ps_5_0");
*
* // 作成
* ps.Create(DirectXManager::GetDevice());
*
* // クラスインスタンスの作成
* // データメンバー（メンバ変数）がある場合はGetClassInstanceメソッドを使用する
* // データメンバーがない場合はCreateClassInstanceメソッドを使用する
* ps.CreateClassInstance(
*	"HLSLシェーダー内のInterfaceを継承するデータメンバーを持たないクラス名"
* );
* ps.GetClassInstance(
*	"HLSLシェーダー内のInterfaceを継承するデータメンバーを持つクラスが定数バッファ内に宣言されている場合、そのインスタンス名"
*  // 宣言されていない場合は定数バッファ内にインスタンスの宣言をする必要がある
* );
*
* // 使用
* ※1：CreateClassInstanceメソッド/GetClassInstanceメソッドを使用した際の引数の文字列と同じ文字列
* ps.psSetShader(DirectXManager::GetImmediateContext().Get(), "※1");
*
*/