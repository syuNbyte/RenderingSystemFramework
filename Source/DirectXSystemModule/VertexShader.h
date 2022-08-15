#pragma once

// STL
#include<string>
#include<vector>
#include<unordered_map>

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"SafetyDataType.h"

// DirectXの頂点シェーダークラスが継承するインターフェイスクラス
class IDXVertexShader {
public:
	virtual void SetFilePath(const std::string& path) = 0;
	virtual void SetEntryFuncName(const std::string& funcName) = 0;
	virtual void SetVersion(const std::string& version) = 0;
	virtual void SetInputElements(InputElementDesc* elements, const std::size_t& size) = 0;
	virtual void Create(DirectXDevice* device) = 0;
};

// DirectXの頂点シェーダーを扱うクラス
class DXVertexShader : public IDXVertexShader {
protected:
	ComObject::VertexShader m_vs;					// 頂点シェーダー
	ComObject::InputLayout m_layout;				// 入力レイアウト
	std::vector<InputElementDesc> m_elements;	// 頂点に持たせる情報群
protected:
	std::string m_path;								// シェーダーファイルのパス
	std::string m_funcName;							// シェーダーファイルのエントリ関数名
	std::string m_version;							// シェーダーファイルの使用するシェーダーモデルバージョン
public:
	DXVertexShader();
	~DXVertexShader();
protected:
	bool IsCsoFile();
	void CompileShader(void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob);
	void ReadCompiledShaderFile(void*& shaderObject, std::size_t& shaderObjectSize);
	void CompileShaderFromFile(void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob);
	void CreateVertexShader(DirectXDevice* device, void*& shaderObject, std::size_t& shaderObjectSize);
	void CreateInputLayout(DirectXDevice* device, void*& shaderObject, std::size_t& shaderObjectSize);
	void ReleaseBlobData(DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob);
public:
	void Create(DirectXDevice* device);					// 設定されている情報で頂点シェーダーを作成する
	void CreateAndCompile(DirectXDevice* device);	// 設定されている情報でシェーダーファイルをコンパイルしてから頂点シェーダーを作成する
	void SetFilePath(const std::string& path);
	void SetEntryFuncName(const std::string& funcName);
	void SetVersion(const std::string& version);
	void SetInputElements(InputElementDesc* elements, const std::size_t& size);
public:
	void VSSetShader(DirectXDeviceContext* context);
	void IASetInputLayout(DirectXDeviceContext* context);
};


// クラスリンケージ用に調整されたDirectXの頂点シェーダーを扱うクラス
class DXVertexShaderLink : public IDXVertexShader {
protected:
	ComObject::VertexShader m_vs;																	// 頂点シェーダー
	ComObject::InputLayout m_layout;																// 入力レイアウト
	std::vector<InputElementDesc> m_elements;													// 頂点に持たせる情報群
	ComObject::ClassLinkage m_classLinkage;													// クラスリンケージ
	std::unordered_map<std::string, ComObject::ClassInstance> m_classInstances;	// クラスインスタンス
protected:
	std::string m_path;																				// シェーダーファイルのパス
	std::string m_funcName;																			// シェーダーファイルのエントリ関数名
	std::string m_version;																			// シェーダーファイルの使用するシェーダーモデルバージョン
public:
	DXVertexShaderLink();
	~DXVertexShaderLink();
protected:
	bool IsCsoFile();
	void CompileShader(void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob);
	void ReadCompiledShaderFile(void*& shaderObject, std::size_t& shaderObjectSize);
	void CompileShaderFromFile(void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob);
	void CreateClassLinkage(DirectXDevice* device);
	void CreateVertexShader(DirectXDevice* device, void*& shaderObject, std::size_t& shaderObjectSize);
	void CreateInputLayout(DirectXDevice* device, void*& shaderObject, std::size_t& shaderObjectSize);
	void ReleaseBlobData(DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob);
public:
	void Create(DirectXDevice* device);							// 設定されている情報で頂点シェーダーを作成する
	void CreateAndCompile(DirectXDevice* device);			// 設定されている情報でシェーダーファイルをコンパイルしてから頂点シェーダーを作成する
	void CreateClassInstance(const std::string& key);		// 動的シェーダーリンクのクラスインスタンスを作成する
	void GetClassInstance(const std::string& key);			// 動的シェーダーリンクのクラスインスタンスを取得する
	void SetFilePath(const std::string& path);
	void SetEntryFuncName(const std::string& funcName);
	void SetVersion(const std::string& version);
	void SetInputElements(InputElementDesc* elements, const std::size_t& size);
public:
	void VSSetShader(DirectXDeviceContext* context, const std::string& key);
	void IASetInputLayout(DirectXDeviceContext* context);
};

/*
* ===========================
* DXVertexShader Example
* ===========================
* 
* // 入力属性の定義
* std::array<InputElementDesc, 3> elements{ {
*		InputElements::PositionElement,
*		InputElements::NormalElement,
*		InputElements::ColorElement,
* } };
* 
* // インスタンスの作成
* DXVertexShader vs;
* 
* // 事前情報の設定
* vs.SetFilePath("main_vs.hlsl");
* vs.SetEntryFuncName("main");
* vs.SetVersion("vs_5_0");
* vs.SetInputElements(elements.data(), elements.size());
* 
* // 作成
* vs.Create(DirectXManager::GetDevice());
* 
* // 使用
* vs.VSSetShader(DirectXManager::GetImmediateContext().Get());
* vs.IASstInputLayout(DirectXManager::GetImmediateContext().Get());
* 
*/



/*
* ===========================
* DXVertexShaderLink Example
* ===========================
* 
* * // 入力属性の定義
* std::array<InputElementDesc, 3> elements{ {
*		InputElements::PositionElement,
*		InputElements::NormalElement,
*		InputElements::ColorElement,
* } };
* 
* // インスタンスの作成
* DXVertexShader vs;
* 
* // 事前情報の設定
* vs.SetFilePath("main_vs.hlsl");
* vs.SetEntryFuncName("main");
* vs.SetVersion("vs_5_0");
* vs.SetInputElements(elements.data(), elements.size());
* 
* // 作成
* vs.Create(DirectXManager::GetDevice());
* 
* // クラスインスタンスの作成
* // データメンバー（メンバ変数）がある場合はGetClassInstanceメソッドを使用する
* // データメンバーがない場合はCreateClassInstanceメソッドを使用する
* vs.CreateClassInstance(
*	"HLSLシェーダー内のInterfaceを継承するデータメンバーを持たないクラス名"
* );
* vs.GetClassInstance(
*	"HLSLシェーダー内のInterfaceを継承するデータメンバーを持つクラスが定数バッファ内に宣言されている場合、そのインスタンス名"
*  // 宣言されていない場合は定数バッファ内にインスタンスの宣言をする必要がある
* );
* 
* // 使用
* ※1：CreateClassInstanceメソッド/GetClassInstanceメソッドを使用した際の引数の文字列と同じ文字列
* vs.VSSetShader(DirectXManager::GetImmediateContext().Get(), "※1");
* vs.IASstInputLayout(DirectXManager::GetImmediateContext().Get(), "※1");
*
*/