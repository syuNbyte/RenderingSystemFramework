
// STL
#include<array>

// TNLIB
#include"ScopedTimer.hpp"
#include"Debug.hpp"
#include"Color.hpp"

// MyFile
#include"DeferredRenderingExample.h"
#include"Application.h"
#include"DirectXSystemModule/DirectXManager.h"
#include"DirectXSystemModule/InputElements.h"
#include"DirectXSystemModule/VertexShader.h"
#include"DirectXSystemModule/PixelShader.h"
#include"DirectXSystemModule/ConstantBuffer.h"
#include"DirectXSystemModule/VertexBuffer.h"
#include"DirectXSystemModule/IndexBuffer.h"
#include"DirectXSystemModule/ShaderResourceView.h"
#include"DirectXSystemModule/RasterizerContainer.h"
#include"DirectXSystemModule/BlendContainer.h"
#include"DirectXSystemModule/SamplerContainer.h"
#include"DirectXSystemModule/ViewPort.h"
#include"DirectInput.h"

// Rendering
#include"RenderingSystemModule/BackBuffer.h"
#include"RenderingSystemModule/GraphicBuffer.h"
#include"DirectXSystemModule/DepthStencil.h"
#include"ConstantBuffer/CBCameraParam.h"
#include"ConstantBuffer/CBWorld.h"
#include"ConstantBuffer/CBMaterial.h"
#include"ConstantBuffer/CBDirectionalLight.h"
#include"MainCamera.h"
#include"MainDirectionalLight.h"
#include"DeferredRenderingCamera.h"

// Assets
#include"AssetModule/DXAssimpData.h"
#include"AssetModule/DXAssimpMesh.h"

// DXMath
#include"DirectXUtilityModule/DirectXVector.h"
#include"DirectXUtilityModule/DirectXMatrix.h"
#include"DirectXUtilityModule/DirectXColor.h"

// DirectXTexture
#include"DirectXSystemModule/Texture.h"

#pragma region struct or class

// この列挙定数はビットマスク用です
enum class TextureBitMask : int_t {
	None = 0x0,
	Diffuse = 0x1,
	Normal = 0x2,
	MaskAll = 0x3
};

struct Vertex {
	DXVector3 pos;
	DXVector3 normal;
	DXVector2 uv;
	DXColor color;
};

struct RectMesh {
	DXVertexBuffer m_buf;
	std::array<Vertex, 4> m_vertices;
};

struct Mesh {
	DXVertexBuffer m_vbuf;
	DXIndexBuffer m_ibuf;
	std::vector<Vertex> m_vertices;
	std::vector<uint_t> m_indices;
};

struct BackBufferVertex {
	DXVector3 pos;
	DXVector2 uv;
};

struct BackBufferRectMesh {
	DXVertexBuffer m_buf;
	std::array<BackBufferVertex, 4> m_vertices;
};

struct AssimpVertex {
	DXVector3 pos;
	DXVector3 normal;
	DXVector3 tangent;
	DXVector3 bitangent;
	DXVector2 uv;
	DXColor color;
};

struct AssimpSubMeshMaterial {
	DXShaderResourceView m_diffuse;
	DXShaderResourceView m_normal;
};

struct AssimpSubMesh {
	DXVertexBuffer m_vbuf;
	DXIndexBuffer m_ibuf;
	std::vector<AssimpVertex> m_vertices;
	std::vector<uint_t> m_indices;
};

struct AssimpMesh {
	std::vector<AssimpSubMesh> m_subMesh;
	std::vector<AssimpSubMeshMaterial> m_subMeshMaterial;
};

struct AssimpMaterial {
public:
	DXVertexShaderLink m_vs;
	DXPixelShaderLink m_ps;
	float_t roughness;
	float_t metalness;
	float_t specular;
public:
	void SetMaterial() {
		auto context = DirectXManager::GetImmediateContext().Get();
		CBMaterial::SetRoughness(roughness);
		CBMaterial::SetMetalness(metalness);
		CBMaterial::SetSpecular(specular);
		CBMaterial::UpdateConstantBuffer(context);
		CBMaterial::VSSetConstantBuffer(context);
		CBMaterial::PSSetConstantBuffer(context);
	}
};

struct Transform {
public:
	DXVector3 pos;
	DXVector3 rot;
	DXVector3 sca;
public:
	Transform() {
		pos = DXVector3::Zero();
		rot = DXVector3::Zero();
		sca = DXVector3::One();
	}
public:
	void Reset() {
		pos = DXVector3::Zero();
		rot = DXVector3::Zero();
		sca = DXVector3::One();
	}

	void SetWorldMatrix() {
		auto scale = Math::Matrix::Affine::Scaling(sca);
		auto rotation = Math::Matrix::Affine::RotationAll(rot, Math::RotationOrder::XYZ);
		auto trans = Math::Matrix::Affine::Transform(pos);
		auto matrix = scale * rotation * trans;
		auto context = DirectXManager::GetImmediateContext().Get();
		CBWorld::SetWorldMatrix(matrix);
		CBWorld::UpdateConstantBuffer(context);
		CBWorld::VSSetConstantBuffer(context);
		CBWorld::PSSetConstantBuffer(context);
	}
};

struct RectTransform {
public:
	DXVector3 pos;
	DXVector3 rot;
	DXVector3 sca;
public:
	RectTransform() {
		pos = DXVector3::Zero();
		rot = DXVector3::Zero();
		sca = DXVector3::One();
	}

	void SetWorldMatrix() {
		auto scale = Math::Matrix::Affine::Scaling(sca);
		auto rotation = Math::Matrix::Affine::RotationAll(rot, Math::RotationOrder::XYZ);
		auto trans = Math::Matrix::Affine::Transform(pos);
		auto matrix = scale * rotation * trans;
		auto context = DirectXManager::GetImmediateContext().Get();
		CBWorld::SetWorldMatrix(matrix);
		CBWorld::UpdateConstantBuffer(context);
		CBWorld::VSSetConstantBuffer(context);
		CBWorld::PSSetConstantBuffer(context);
	}
};

#pragma endregion struct or class

// プロトタイプ宣言
#pragma region prototype declaration

void CreateDeferredTextureShader(
	DXVertexShader& vs,
	std::string path,
	std::string entry
);

void CreateVertexShader(
	DXVertexShader& vs,
	std::string path,
	std::string entry
);

void CreatePixelShader(
	DXPixelShader& ps,
	std::string path,
	std::string entry
);

void CreateVertexShader(
	IDXVertexShader* vs,
	std::string path,
	std::string entry
);

void CreatePixelShader(
	IDXPixelShader* ps,
	std::string path,
	std::string entry
);

void InitializeSingleton();

void InitializeCamera();

void InitializeConstantBufferParam();

void Initialize(
	DXVertexShader& createVs,
	DXPixelShader& createPs,
	std::string createVsPath,
	std::string createPsPath,
	DXVertexShader& loadVs,
	DXPixelShader& loadPs,
	std::string loadVsPath,
	std::string loadPsPath
);

void SetBackBufferSizeMesh();

void SetBackBuffer();

void RenderBackBuffer(DXVertexShader& loadVs, DXPixelShader& loadPs);

void SetGraphicBuffer(DXVertexShader& createVs, DXPixelShader& createPs);

// 動的シェーダーリンクを使用している
void SetGraphicBufferLink(DXVertexShader& createVs, DXPixelShaderLink& createPs, const std::string& psLinkKey);

void SetGraphicBufferSRV();

void SetGraphicBufferNull();

void SetWorldMatrix(const DXMatrix4x4& matrix);

void SetMaterial(float_t roughness, float_t metalness, float_t specular);

void SetDirectionalLight(
	const DXVector3& lightDirection,
	const DXColor& directionalLightColor,
	const DXColor& ambientLightColor
);

DXVector3 InputCameraPosition();

DXVector2 InputCameraDirection();

RectMesh CreateMesh(const DXColor& color);

BackBufferRectMesh CreateBackBufferMesh();

AssimpSubMesh CreateAssimpSubMesh(DXAssimpData& asdata, DXAssimpMesh& asmesh);

AssimpMesh CreateAssimpMesh(DXAssimpData& asdata, DXAssimpMesh& asmesh, DXColor color);

void AssimpSubMeshDraw(DirectXDeviceContext* context, AssimpSubMesh& assimpMesh);

void AssimpMeshDraw(DirectXDeviceContext* context, AssimpMesh& assimpMesh);

void AssimpMeshDraw(
	DirectXDeviceContext* context,
	DXVertexShader& createVs,
	DXPixelShaderLink& createPs,
	AssimpMesh& assimpMesh
);

void AssimpMeshShadowMapDraw(
	DirectXDeviceContext* context,
	DXVertexShader& createShadowMapVs,
	DXPixelShader& createShadowMapPs,
	AssimpMesh& assimpMesh
);

#pragma endregion prototype declaration


#pragma region method

void CreateDeferredTextureShader(
	DXVertexShader& vs,
	std::string path,
	std::string entry
) {
	// 頂点シェーダーの初期化
	std::array<InputElementDesc, 6> elements{ {
			InputElements::PositionElement,
			InputElements::NormalElement,
			InputElements::TangentElement,
			InputElements::BiTangentElement,
			InputElements::TexCoordElement,
			InputElements::ColorElement,
	} };

	vs.SetFilePath(path);							// VC++ Projectファイルのディレクトリから見たhlslファイルのディレクトリ
	vs.SetEntryFuncName(entry);					// hlslファイルに設定されているエントリ関数名
	vs.SetVersion("vs_5_0");						// hlslファイルに設定されているシェーダーモデルのバージョン
	vs.SetInputElements(elements.data(), elements.size());
	vs.Create(DirectXManager::GetDevice().Get());
	vs.VSSetShader(DirectXManager::GetImmediateContext().Get());
	vs.IASetInputLayout(DirectXManager::GetImmediateContext().Get());
}

void CreateVertexShader(
	DXVertexShader& vs,
	std::string path,
	std::string entry
) {
	// 頂点シェーダーの初期化
	std::array<InputElementDesc, 2> elements{ {
			InputElements::PositionElement,
			InputElements::TexCoordElement
	} };

	vs.SetFilePath(path);							// VC++ Projectファイルのディレクトリから見たhlslファイルのディレクトリ
	vs.SetEntryFuncName(entry);					// hlslファイルに設定されているエントリ関数名
	vs.SetVersion("vs_5_0");						// hlslファイルに設定されているシェーダーモデルのバージョン
	vs.SetInputElements(elements.data(), elements.size());
	vs.Create(DirectXManager::GetDevice().Get());
}

void CreatePixelShader(
	DXPixelShader& ps,
	std::string path,
	std::string entry
) {
	// ピクセルシェーダーの初期化
	ps.SetFilePath(path);
	ps.SetEntryFuncName(entry);
	ps.SetVersion("ps_5_0");
	ps.Create(DirectXManager::GetDevice().Get());
}

void CreateVertexShader(
	IDXVertexShader* vs,
	std::string path,
	std::string entry
) {
		// 頂点シェーダーの初期化
	std::array<InputElementDesc, 2> elements{ {
			InputElements::PositionElement,
			InputElements::TexCoordElement
	} };

	vs->SetFilePath(path);							// VC++ Projectファイルのディレクトリから見たhlslファイルのディレクトリ
	vs->SetEntryFuncName(entry);					// hlslファイルに設定されているエントリ関数名
	vs->SetVersion("vs_5_0");						// hlslファイルに設定されているシェーダーモデルのバージョン
	vs->SetInputElements(elements.data(), elements.size());
	vs->Create(DirectXManager::GetDevice().Get());
}

void CreatePixelShader(
	IDXPixelShader* ps,
	std::string path,
	std::string entry
) {
		// ピクセルシェーダーの初期化
	ps->SetFilePath(path);
	ps->SetEntryFuncName(entry);
	ps->SetVersion("ps_5_0");
	ps->Create(DirectXManager::GetDevice().Get());
}

void InitializeSingleton() {
	// シングルトンクラスの初期化
	auto device = DirectXManager::GetDevice().Get();

	// 二度初期化されると解放エラーが出力ウィンドウに発生する
	CBCameraParam::Initialize(device);
	CBWorld::Initialize(device);
	CBMaterial::Initialize(device);
	CBDirectionalLight::Initialize(device);
	DXGraphicBuffer::Initialize(device);
	DXGraphicBufferResult::Initailize(device);
	MainCamera::Initialize();
	MainDirectionalLight::Initialize(device);
	DeferredRenderingCamera::Initialize();
}

void InitializeCamera() {
	// カメラの初期化
	{
		MainCamera::GetCamera().SetZoom(1.0);
		MainCamera::GetCamera().SetPosition(DXVector3(0.0f, +1.0f, -5.0f));
		MainCamera::GetCamera().SetViewDirection(DXVector3(0.0f, 0.0f, +1.0f));
		MainCamera::GetCamera().UpdateViewMatrix();
		MainCamera::GetCamera().UpdateProjectionMatrix();
	}
}

void InitializeConstantBufferParam() {
	CBCameraParam::SetViewPortSize(Application::GetWindowSize());
}

void Initialize(
	DXVertexShader& createVs,
	DXPixelShader& createPs,
	std::string createVsPath,
	std::string createPsPath,
	DXVertexShader& loadVs,
	DXPixelShader& loadPs,
	std::string loadVsPath,
	std::string loadPsPath
) {
	Debug::BracketBegin("Initialize");

// シングルトンクラスの初期化
	{
		auto device = DirectXManager::GetDevice().Get();
		// 二度初期化されると解放エラーが出力ウィンドウに発生する
		CBCameraParam::Initialize(device);
		CBWorld::Initialize(device);
		CBMaterial::Initialize(device);
		CBDirectionalLight::Initialize(device);
		DXGraphicBuffer::Initialize(device);
		MainCamera::Initialize();
		DeferredRenderingCamera::Initialize();
	}

	// 頂点シェーダーの初期化
	{
		std::array<InputElementDesc, 6> elements{ {
				InputElements::PositionElement,
				InputElements::NormalElement,
				InputElements::TangentElement,
				InputElements::BiTangentElement,
				InputElements::TexCoordElement,
				InputElements::ColorElement,
		} };

		createVs.SetFilePath(createVsPath);// VC++ Projectファイルのディレクトリから見たhlslファイルのディレクトリ
		createVs.SetEntryFuncName("main");					// hlslファイルに設定されているエントリ関数名
		createVs.SetVersion("vs_5_0");						// hlslファイルに設定されているシェーダーモデルのバージョン
		createVs.SetInputElements(elements.data(), elements.size());
		createVs.Create(DirectXManager::GetDevice().Get());
		createVs.VSSetShader(DirectXManager::GetImmediateContext().Get());
		createVs.IASetInputLayout(DirectXManager::GetImmediateContext().Get());
	}

	// ピクセルシェーダーの初期化
	{
		createPs.SetFilePath(createPsPath);
		createPs.SetEntryFuncName("main");
		createPs.SetVersion("ps_5_0");
		createPs.Create(DirectXManager::GetDevice().Get());
		createPs.PSSetShader(DirectXManager::GetImmediateContext().Get());
	}

	// 頂点シェーダーの初期化
	{
		std::array<InputElementDesc, 2> elements{ {
				InputElements::PositionElement,
				InputElements::TexCoordElement
		} };

		loadVs.SetFilePath(loadVsPath);// VC++ Projectファイルのディレクトリから見たhlslファイルのディレクトリ
		loadVs.SetEntryFuncName("main");					// hlslファイルに設定されているエントリ関数名
		loadVs.SetVersion("vs_5_0");						// hlslファイルに設定されているシェーダーモデルのバージョン
		loadVs.SetInputElements(elements.data(), elements.size());
		loadVs.Create(DirectXManager::GetDevice().Get());
		loadVs.VSSetShader(DirectXManager::GetImmediateContext().Get());
		loadVs.IASetInputLayout(DirectXManager::GetImmediateContext().Get());
	}

	// ピクセルシェーダーの初期化
	{
		loadPs.SetFilePath(loadPsPath);
		loadPs.SetEntryFuncName("main");
		loadPs.SetVersion("ps_5_0");
		loadPs.Create(DirectXManager::GetDevice().Get());
		loadPs.PSSetShader(DirectXManager::GetImmediateContext().Get());
	}

	// カメラの初期化
	{
		MainCamera::GetCamera().SetZoom(1.0);
		MainCamera::GetCamera().SetPosition(DXVector3(0.0f, +1.0f, -5.0f));
		MainCamera::GetCamera().SetViewDirection(DXVector3(0.0f, 0.0f, +1.0f));
		MainCamera::GetCamera().UpdateViewMatrix();
		MainCamera::GetCamera().UpdateProjectionMatrix();
	}

	// Assimpを使用したメッシュの作成
	{
		std::string primitiveDirectory = "Assets/PrimitiveModel/";
		//assimpData.Create(
		//	primitiveDirectory + "IcoSphere/IcoSphere.fbx",
		//	""
		//);
		//assimpMesh.Create(&assimpData);
	}
}

void SetBackBufferSizeMesh() {
	auto device = DirectXManager::GetDevice().Get();
	auto context = DirectXManager::GetImmediateContext().Get();
	const DXVector2 bfsize = Application::GetWindowSize();
	//const DXVector2 bfsize = MainDirectionalLight::GetLight().GetScreenSize();
	const DXVector2 aspect = DXVector2(1.0f, bfsize.y / bfsize.x);
	const auto scale = Math::Matrix::Affine::Scaling(DXVector3(bfsize.x, bfsize.y, 1));
	const auto rotation = Math::Matrix::Affine::RotationAll(DXVector3(0, 0, 0), Math::RotationOrder::XYZ);
	const auto trans = Math::Matrix::Affine::Transform(DXVector3(bfsize.x / 2.0f, bfsize.y / 2.0f, 0));
	//const auto scale = Math::Matrix::Affine::Scaling(DXVector3(bfsize.x / 2.0f, bfsize.y / 2.0f, 1));
	//const auto rotation = Math::Matrix::Affine::RotationAll(DXVector3(0, 0, 0), Math::RotationOrder::XYZ);
	//const auto trans = Math::Matrix::Affine::Transform(DXVector3(bfsize.x *(1.0f / 4.0f), bfsize.y * (1.0f / 4.0f), 0));
	CBWorld::SetWorldMatrix(scale * rotation * trans);
	CBWorld::UpdateConstantBuffer(context);
	CBWorld::VSSetConstantBuffer(context);
	CBWorld::PSSetConstantBuffer(context);

	auto mesh = CreateBackBufferMesh();
	mesh.m_buf.SetQuantity(4);
	mesh.m_buf.SetOffset(0);
	mesh.m_buf.SetStride(sizeof(BackBufferVertex));
	mesh.m_buf.Create(device, mesh.m_vertices.data());
	mesh.m_buf.UpdateVertexBuffer(context, mesh.m_vertices.data());
	mesh.m_buf.IASetVertexBuffer(context);
}

void SetBackBuffer() {
}

void RenderBackBuffer(DXVertexShader& loadVs, DXPixelShader& loadPs) {
	auto device = DirectXManager::GetDevice().Get();
	auto context = DirectXManager::GetImmediateContext().Get();
	const DXVector2 bfsize = Application::GetWindowSize();
	const DXVector2 aspect = DXVector2(1.0f, bfsize.y / bfsize.x);
	const auto scale = Math::Matrix::Affine::Scaling(DXVector3(bfsize.x, bfsize.y, 1));
	const auto rotation = Math::Matrix::Affine::RotationAll(DXVector3(0, 0, 0), Math::RotationOrder::XYZ);
	const auto trans = Math::Matrix::Affine::Transform(DXVector3(bfsize.x / 2.0f, bfsize.y / 2.0f, 0));
	CBWorld::SetWorldMatrix(scale * rotation * trans);
	CBWorld::UpdateConstantBuffer(context);
	CBWorld::VSSetConstantBuffer(context);
	CBWorld::PSSetConstantBuffer(context);

	auto mesh = CreateBackBufferMesh();
	mesh.m_buf.SetQuantity(4);
	mesh.m_buf.SetOffset(0);
	mesh.m_buf.SetStride(sizeof(BackBufferVertex));
	mesh.m_buf.Create(device, mesh.m_vertices.data());
	mesh.m_buf.UpdateVertexBuffer(context, mesh.m_vertices.data());
	mesh.m_buf.IASetVertexBuffer(context);

	context->OMSetRenderTargets(1, DXBackBuffer::GetRTV().GetAddressOf(), nullptr);
	loadVs.VSSetShader(context);
	loadVs.IASetInputLayout(context);
	loadPs.PSSetShader(context);
	SetGraphicBufferSRV();
	context->Draw(4, 0);
}

void SetGraphicBuffer(
	DXVertexShader& createVs,
	DXPixelShader& createPs
) {
	auto context = DirectXManager::GetImmediateContext().Get();
	DXDepthStencilDefault::DepthBufferON(context);
	auto rtvs = DXGraphicBuffer::GetRTVs();
	context->OMSetRenderTargets(rtvs.size(), rtvs.data(), DXDepthStencilDefault::GetDSV().Get());
	context->RSSetState(DirectXManager::GetRasterizerState().Get());
	createVs.VSSetShader(context);
	createVs.IASetInputLayout(context);
	createPs.PSSetShader(context);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	DXSamplerContainer::GetLinearSampler().PSSetSampler(context);
	DXBlendContainer::GetAlphaBlend().OMSetBlendState(context);
}

void SetGraphicBufferLink(
	DXVertexShader& createVs,
	DXPixelShaderLink& createPs,
	const std::string& psLinkKey
) {
	auto context = DirectXManager::GetImmediateContext().Get();
	DXDepthStencilDefault::DepthBufferON(context);
	auto rtvs = DXGraphicBuffer::GetRTVs();
	context->OMSetRenderTargets(rtvs.size(), rtvs.data(), DXDepthStencilDefault::GetDSV().Get());
	context->RSSetState(DirectXManager::GetRasterizerState().Get());
	createVs.VSSetShader(context);
	createVs.IASetInputLayout(context);
	createPs.PSSetShader(context, psLinkKey);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	DXSamplerContainer::GetLinearSampler().PSSetSampler(context);
	DXBlendContainer::GetAlphaBlend().OMSetBlendState(context);
}

void SetGraphicBufferSRV() {
	auto context = DirectXManager::GetImmediateContext().Get();
	context->VSSetShaderResources(0, 1, DXGraphicBuffer::GetAlbedoSRV().GetAddressOf());
	context->VSSetShaderResources(1, 1, DXGraphicBuffer::GetNormalSRV().GetAddressOf());
	context->VSSetShaderResources(2, 1, DXGraphicBuffer::GetPositionSRV().GetAddressOf());
	context->VSSetShaderResources(3, 1, DXGraphicBuffer::GetDepthSRV().GetAddressOf());
	context->VSSetShaderResources(4, 1, DXGraphicBuffer::GetMaterialSRV().GetAddressOf());
	context->VSSetShaderResources(5, 1, DXGraphicBuffer::GetBrightnessSRV().GetAddressOf());
	context->PSSetShaderResources(0, 1, DXGraphicBuffer::GetAlbedoSRV().GetAddressOf());
	context->PSSetShaderResources(1, 1, DXGraphicBuffer::GetNormalSRV().GetAddressOf());
	context->PSSetShaderResources(2, 1, DXGraphicBuffer::GetPositionSRV().GetAddressOf());
	context->PSSetShaderResources(3, 1, DXGraphicBuffer::GetDepthSRV().GetAddressOf());
	context->PSSetShaderResources(4, 1, DXGraphicBuffer::GetMaterialSRV().GetAddressOf());
	context->PSSetShaderResources(5, 1, DXGraphicBuffer::GetBrightnessSRV().GetAddressOf());
}

void SetGraphicBufferNull() {
	auto context = DirectXManager::GetImmediateContext().Get();
	ShaderResourceView* nullView = nullptr;
	context->VSSetShaderResources(0, 1, &nullView);	// 空のリソースを設定しなおす
	context->VSSetShaderResources(1, 1, &nullView);	// 空のリソースを設定しなおす
	context->VSSetShaderResources(2, 1, &nullView);	// 空のリソースを設定しなおす
	context->VSSetShaderResources(3, 1, &nullView);	// 空のリソースを設定しなおす
	context->VSSetShaderResources(4, 1, &nullView);	// 空のリソースを設定しなおす
	context->VSSetShaderResources(5, 1, &nullView);	// 空のリソースを設定しなおす
	context->VSSetShaderResources(6, 1, &nullView);	// 空のリソースを設定しなおす
	context->PSSetShaderResources(0, 1, &nullView);	// 空のリソースを設定しなおす
	context->PSSetShaderResources(1, 1, &nullView);	// 空のリソースを設定しなおす
	context->PSSetShaderResources(2, 1, &nullView);	// 空のリソースを設定しなおす
	context->PSSetShaderResources(3, 1, &nullView);	// 空のリソースを設定しなおす
	context->PSSetShaderResources(4, 1, &nullView);	// 空のリソースを設定しなおす
	context->PSSetShaderResources(5, 1, &nullView);	// 空のリソースを設定しなおす
	context->PSSetShaderResources(6, 1, &nullView);	// 空のリソースを設定しなおす
}

void SetWorldMatrix(const DXMatrix4x4& matrix) {
	auto context = DirectXManager::GetImmediateContext().Get();
	CBWorld::SetWorldMatrix(matrix);
	CBWorld::UpdateConstantBuffer(context);
	CBWorld::VSSetConstantBuffer(context);
	CBWorld::PSSetConstantBuffer(context);
}

void SetMaterial(float_t roughness, float_t metalness, float_t specular) {
	auto context = DirectXManager::GetImmediateContext().Get();
	CBMaterial::SetRoughness(roughness);
	CBMaterial::SetMetalness(metalness);
	CBMaterial::SetSpecular(specular);
	CBMaterial::UpdateConstantBuffer(context);
	CBMaterial::VSSetConstantBuffer(context);
	CBMaterial::PSSetConstantBuffer(context);
}

void SetDirectionalLignt(
	const DXVector3& lightDirection,
	const DXColor& directionalLightColor,
	const DXColor& ambientLightColor
) {
	auto context = DirectXManager::GetImmediateContext().Get();

	// ビュー行列とプロジェクション行列を取得する
	// MainDirectionalLight::GetLight().GetViewMatrix();
	// MainDirectionalLight::GetLight().GetProjectionMatrix();

	MainDirectionalLight::SetLightDirectionConstantBuffer(context);
	MainDirectionalLight::SetLightColorConstantBuffer(context);
	MainDirectionalLight::SetAmbientColorConstantBuffer(context);
	MainDirectionalLight::SetViewMatrixConstantBuffer(context);
	MainDirectionalLight::SetProjectionMatrixConstantBuffer(context);
	MainDirectionalLight::VSSetConstantBuffer(context);
	MainDirectionalLight::PSSetConstantBuffer(context);

}

DXVector3 InputCameraPosition() {
	DXVector3 vec;
	float_t add = 0.01f;
	if (DirectInput::GetKeyboardPress(DIK_LSHIFT)) {
		add *= 5.0f;
	}
	if (DirectInput::GetKeyboardPress(DIK_W)) {
		vec.y += add;
	}
	if (DirectInput::GetKeyboardPress(DIK_S)) {
		vec.y -= add;
	}
	if (DirectInput::GetKeyboardPress(DIK_D)) {
		vec.x += add;
	}
	if (DirectInput::GetKeyboardPress(DIK_A)) {
		vec.x -= add;
	}
	if (DirectInput::GetKeyboardPress(DIK_Q)) {
		vec.z += add;
	}
	if (DirectInput::GetKeyboardPress(DIK_E)) {
		vec.z -= add;
	}
	return vec;
}

DXVector2 InputCameraDirection() {
	DXVector2 dir;
	if (DirectInput::GetKeyboardPress(DIK_UP)) {
		dir.y += 0.01f;
	}
	if (DirectInput::GetKeyboardPress(DIK_DOWN)) {
		dir.y -= 0.01f;
	}
	if (DirectInput::GetKeyboardPress(DIK_RIGHT)) {
		dir.x += 0.01f;
	}
	if (DirectInput::GetKeyboardPress(DIK_LEFT)) {
		dir.x -= 0.01f;
	}
	return dir;
}

RectMesh CreateMesh(const DXColor& color) {
	RectMesh mesh;
	mesh.m_vertices[0].pos.Set(-1.0f, +1.0f, 0.0f);
	mesh.m_vertices[1].pos.Set(+1.0f, +1.0f, 0.0f);
	mesh.m_vertices[2].pos.Set(-1.0f, -1.0f, 0.0f);
	mesh.m_vertices[3].pos.Set(+1.0f, -1.0f, 0.0f);
	mesh.m_vertices[0].uv.Set(0.0f, 0.0f);
	mesh.m_vertices[1].uv.Set(1.0f, 0.0f);
	mesh.m_vertices[2].uv.Set(0.0f, 1.0f);
	mesh.m_vertices[3].uv.Set(1.0f, 1.0f);
	mesh.m_vertices[0].normal.Set(0.0f, 0.0f, 1.0f);
	mesh.m_vertices[1].normal.Set(0.0f, 0.0f, 1.0f);
	mesh.m_vertices[2].normal.Set(0.0f, 0.0f, 1.0f);
	mesh.m_vertices[3].normal.Set(0.0f, 0.0f, 1.0f);
	mesh.m_vertices[0].color.Set(color);
	mesh.m_vertices[1].color.Set(color);
	mesh.m_vertices[2].color.Set(color);
	mesh.m_vertices[3].color.Set(color);
	return mesh;
}

BackBufferRectMesh CreateBackBufferMesh() {
	BackBufferRectMesh mesh;
	mesh.m_vertices[0].pos.Set(-0.5f, +0.5f, 0.0f);
	mesh.m_vertices[1].pos.Set(+0.5f, +0.5f, 0.0f);
	mesh.m_vertices[2].pos.Set(-0.5f, -0.5f, 0.0f);
	mesh.m_vertices[3].pos.Set(+0.5f, -0.5f, 0.0f);
	mesh.m_vertices[0].uv.Set(0.0f, 0.0f);
	mesh.m_vertices[1].uv.Set(1.0f, 0.0f);
	mesh.m_vertices[2].uv.Set(0.0f, 1.0f);
	mesh.m_vertices[3].uv.Set(1.0f, 1.0f);
	return mesh;
}

AssimpSubMesh CreateAssimpSubMesh(DXAssimpData& asdata, DXAssimpMesh& asmesh) {
	auto device = DirectXManager::GetDevice().Get();
	auto context = DirectXManager::GetImmediateContext().Get();
	auto assimpMesh = AssimpSubMesh();
	{
		auto meshCount = asmesh.GetSubMeshCount();
		for (int_t i = 0; i < meshCount; i++) {
			auto& subMesh = asmesh.GetSubMeshRef(i);
			auto vcount = subMesh.GetVerticesCount();
			auto icount = subMesh.GetIndicesCount();
			assimpMesh.m_vertices.resize(vcount);
			assimpMesh.m_indices.resize(icount);
			for (int_t j = 0; j < vcount; j++) {
				AssimpVertex v;
				v.pos = subMesh.GetPositionsRef()[j];
				v.normal = subMesh.GetNormalsRef()[j];
				v.tangent = subMesh.GetTangentsRef()[j];
				v.bitangent = subMesh.GetBiTangentsRef()[j];
				v.color = DXColor::Red();
				assimpMesh.m_vertices[j] = v;
			}
			for (int_t j = 0; j < icount; j++) {
				assimpMesh.m_indices[j] = subMesh.GetIndicesRef()[j];
			}
		}
	}

	{
		assimpMesh.m_vbuf.SetOffset(0);
		assimpMesh.m_vbuf.SetStride(sizeof(AssimpVertex));
		assimpMesh.m_vbuf.SetQuantity(assimpMesh.m_vertices.size());
		assimpMesh.m_vbuf.Create(device, assimpMesh.m_vertices.data());
	}
	{
		assimpMesh.m_ibuf.SetOffset(0);
		assimpMesh.m_ibuf.SetQuantity(assimpMesh.m_indices.size());
		assimpMesh.m_ibuf.Create(device, assimpMesh.m_indices.data());
	}
	return assimpMesh;
}

AssimpMesh CreateAssimpMesh(DXAssimpData& asdata, DXAssimpMesh& asmesh, DXColor color) {
	auto device = DirectXManager::GetDevice().Get();
	auto context = DirectXManager::GetImmediateContext().Get();
	auto assimpMesh = AssimpMesh();
	{
		auto meshCount = asmesh.GetSubMeshCount();
		assimpMesh.m_subMesh.resize(meshCount);
		for (int_t i = 0; i < meshCount; i++) {
			auto& subMesh = asmesh.GetSubMeshRef(i);
			auto vcount = subMesh.GetVerticesCount();
			auto icount = subMesh.GetIndicesCount();
			assimpMesh.m_subMesh[i].m_vertices.resize(vcount);
			assimpMesh.m_subMesh[i].m_indices.resize(icount);
			// 頂点データの作成
			for (int_t j = 0; j < vcount; j++) {
				AssimpVertex v;
				v.pos = subMesh.GetPositionsRef()[j];
				v.normal = subMesh.GetNormalsRef()[j];
				v.tangent = subMesh.GetTangentsRef()[j];
				v.bitangent = subMesh.GetBiTangentsRef()[j];
				v.color = color;
				v.uv = (subMesh.HasUV(0) && subMesh.GetUVCount(0) == vcount) ? subMesh.GetUVsRef(0)[j] : DXVector2::Zero();
				assimpMesh.m_subMesh[i].m_vertices[j] = v;
			}
			// インデックスデータの作成
			for (int_t j = 0; j < icount; j++) {
				assimpMesh.m_subMesh[i].m_indices[j] = subMesh.GetIndicesRef()[j];
			}
		}
	}

	// テクスチャデータの作成
	{
		auto meshCount = asmesh.GetSubMeshCount();
		assimpMesh.m_subMeshMaterial.resize(meshCount);
		for (int_t i = 0; i < meshCount; i++) {
			AssimpSubMeshMaterial mateiral;
			auto& subMeshMaterial = asmesh.GetSubMeshMaterialRef(i);
			if (subMeshMaterial.GetTextureTypeCount(DXAssimpDataType::Texture::Type::Diffuse) > 0) {
				assimpMesh.m_subMeshMaterial[i].m_diffuse = subMeshMaterial.GetTexturesRef(DXAssimpDataType::Texture::Type::Diffuse)[0];
			}
			if (subMeshMaterial.GetTextureTypeCount(DXAssimpDataType::Texture::Type::Normal) > 0) {
				assimpMesh.m_subMeshMaterial[i].m_normal = subMeshMaterial.GetTexturesRef(DXAssimpDataType::Texture::Type::Normal)[0];
			}
		}
	}

	// 頂点バッファの作成
	{
		auto meshCount = asmesh.GetSubMeshCount();
		for (int_t i = 0; i < meshCount; i++) {
			assimpMesh.m_subMesh[i].m_vbuf.SetOffset(0);
			assimpMesh.m_subMesh[i].m_vbuf.SetStride(sizeof(AssimpVertex));
			assimpMesh.m_subMesh[i].m_vbuf.SetQuantity(assimpMesh.m_subMesh[i].m_vertices.size());
			assimpMesh.m_subMesh[i].m_vbuf.Create(device, assimpMesh.m_subMesh[i].m_vertices.data());
		}
	}
	// インデックスバッファの作成
	{
		auto meshCount = asmesh.GetSubMeshCount();
		for (int_t i = 0; i < meshCount; i++) {
			assimpMesh.m_subMesh[i].m_ibuf.SetOffset(0);
			assimpMesh.m_subMesh[i].m_ibuf.SetQuantity(assimpMesh.m_subMesh[i].m_indices.size());
			assimpMesh.m_subMesh[i].m_ibuf.Create(device, assimpMesh.m_subMesh[i].m_indices.data());
		}
	}
	return assimpMesh;
}

void AssimpSubMeshDraw(DirectXDeviceContext* context, AssimpSubMesh& assimpMesh) {
	assimpMesh.m_vbuf.IASetVertexBuffer(context);
	assimpMesh.m_ibuf.IASetIndexBuffer(context);
	context->DrawIndexed(assimpMesh.m_indices.size(), 0, 0);
}

void AssimpMeshDraw(DirectXDeviceContext* context, AssimpMesh& assimpMesh) {
	auto meshCount = assimpMesh.m_subMesh.size();
	for (int_t i = 0; i < meshCount; i++) {
		assimpMesh.m_subMesh[i].m_vbuf.IASetVertexBuffer(context);
		assimpMesh.m_subMesh[i].m_ibuf.IASetIndexBuffer(context);
		context->DrawIndexed(assimpMesh.m_subMesh[i].m_indices.size(), 0, 0);
	}
}

void AssimpMeshDraw(
	DirectXDeviceContext* context,
	DXVertexShader& createVs,
	DXPixelShaderLink& createPs,
	AssimpMesh& assimpMesh
) {
	auto meshCount = assimpMesh.m_subMesh.size();
	auto bitmask = int_t(0);
	for (int_t i = 0; i < meshCount; i++) {
		assimpMesh.m_subMesh[i].m_vbuf.IASetVertexBuffer(context);
		assimpMesh.m_subMesh[i].m_ibuf.IASetIndexBuffer(context);
		if (assimpMesh.m_subMeshMaterial[i].m_diffuse.HasTexture()) {
			assimpMesh.m_subMeshMaterial[i].m_diffuse.SetRegistNumber(0);
			assimpMesh.m_subMeshMaterial[i].m_diffuse.VSSetShaderResources(context);
			assimpMesh.m_subMeshMaterial[i].m_diffuse.PSSetShaderResources(context);
			bitmask |= static_cast<int_t>(TextureBitMask::Diffuse);
		}
		if (assimpMesh.m_subMeshMaterial[i].m_normal.HasTexture()) {
			assimpMesh.m_subMeshMaterial[i].m_normal.SetRegistNumber(1);
			assimpMesh.m_subMeshMaterial[i].m_normal.VSSetShaderResources(context);
			assimpMesh.m_subMeshMaterial[i].m_normal.PSSetShaderResources(context);
			bitmask |= static_cast<int_t>(TextureBitMask::Normal);
		}
		/*
		* ここでbitmask変数とTextureBitMaskを使用してどのビットが立っているかを判定し
		* その後、テクスチャのビットによって動的シェーダリンクを行う
		*/
		std::string use = "";
		switch (static_cast<TextureBitMask>(bitmask)) {
			case TextureBitMask::None:
			{
				use = "VertexColorAndVertexNormal";
			}
			break;
			case TextureBitMask::Diffuse:
			{
				use = "TextureColorAndVertexNormal";
			}
			break;
			case TextureBitMask::Normal:
			{
				use = "VertexColorAndNormalMapNormal";
			}
			break;
			case TextureBitMask::MaskAll:
			{
				use = "TextureColorAndNormalMapNormal";
			}
			break;
		}
		SetGraphicBufferLink(createVs, createPs, use);
		context->DrawIndexed(assimpMesh.m_subMesh[i].m_indices.size(), 0, 0);
	}
}

void AssimpMeshShadowMapDraw(
	DirectXDeviceContext* context,
	DXVertexShader& createShadowMapVs,
	DXPixelShader& createShadowMapPs,
	AssimpMesh& assimpMesh
) {
	{
	// シャドウマップ用のビューポート
		DXViewPort vp;
		vp.SetViewPortTopLeftPos(DXVector2(0.0f, 0.0f));
		vp.SetViewPortBottomRightPos(DXVector2(MainDirectionalLight::GetRenderTarget().GetRenderTargetSize()));
		vp.SetViewPortDepthRange(0.0f, 1.0f);
		vp.RSSetViewPort(context);
	}
	context->OMSetRenderTargets(
		1,
		MainDirectionalLight::GetRenderTarget().GetRTV().GetAddressOf(),
		MainDirectionalLight::GetDepthStencil().GetDSV().Get()
	);
	createShadowMapVs.VSSetShader(context);
	createShadowMapVs.IASetInputLayout(context);
	createShadowMapPs.PSSetShader(context);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DXSamplerContainer::GetLinearSampler().PSSetSampler(context);
	DXBlendContainer::GetAlphaBlend().OMSetBlendState(context);



	auto meshCount = assimpMesh.m_subMesh.size();
	for (int_t i = 0; i < meshCount; i++) {
		assimpMesh.m_subMesh[i].m_vbuf.IASetVertexBuffer(context);
		assimpMesh.m_subMesh[i].m_ibuf.IASetIndexBuffer(context);
		context->DrawIndexed(assimpMesh.m_subMesh[i].m_indices.size(), 0, 0);
	}

	{
		// 通常バッファ用のビューポート
		DXViewPort vp;
		vp.SetViewPortTopLeftPos(DXVector2(0.0f, 0.0f));
		vp.SetViewPortBottomRightPos(DXVector2(Application::GetWindowSize()));
		vp.SetViewPortDepthRange(0.0f, 1.0f);
		vp.RSSetViewPort(context);
	}
}

#pragma endregion method

//===================================================//



void DefaultExample() {
	//Debug::Log("\
	//////////////////////\n\
	/////DefaultExample///\n\
	//////////////////////\n\
	//");
	Debug::SetDengerLogTimeStopFlag(false);
	Debug::SetDispOnOutputWindow(false);
	Debug::SetDispOnConsoleWindow(false);
	Debug::SetLogSaveFlag(false);

	Debug::BracketBegin("Rendering Pass");

	static DXVertexShader vs;
	static DXPixelShader ps;
	static DXVertexShader createVs;	// DeferredRenderingで使用するテクスチャを作成するシェーダー
	static DXPixelShaderLink createPs;	// DeferredRenderingで使用するテクスチャを作成するシェーダー
	static DXVertexShader loadVs;		// 事前に作成されたテクスチャを読み込みDeferredRenderingを行うシェーダー
	static DXPixelShaderLink loadPs;		// 事前に作成されたテクスチャを読み込みDeferredRenderingを行うシェーダー
	static DXVertexShader sendVs;		// バックバッファに結果を送るためだけのシェーダー
	static DXPixelShader sendPs;		// バックバッファに結果を送るためだけのシェーダー
	static DXShaderResourceView tex;	// テクスチャ
	static DXShaderResourceView ntex;// 法線テクスチャ


	static DXAssimpData asdata;
	static DXAssimpMesh asmesh;
	static AssimpMesh assimpMesh;
	static AssimpMaterial material;
	static Transform transform;

	// Initialize
	{
		static bool initialize = false;
		if (initialize == false) {

			Debug::BracketBegin("Initialize");

			InitializeSingleton();

			CreateDeferredTextureShader(
				createVs,
				"ShaderFile/CreateDeferredTexture_vs.hlsl",
				"main"
			);

			CreatePixelShader(
				&createPs,
				"ShaderFile/CreateDeferredTexture_ps.hlsl",
				"main"
			);

			createPs.CreateClassInstance("VertexColorAndVertexNormal");
			createPs.CreateClassInstance("VertexColorAndNormalMapNormal");
			createPs.CreateClassInstance("TextureColorAndVertexNormal");
			createPs.CreateClassInstance("TextureColorAndNormalMapNormal");

			CreateVertexShader(
				loadVs,
				"ShaderFile/LoadDeferredTexture_vs.hlsl",
				"main"
			);

			CreatePixelShader(
				&loadPs,
				"ShaderFile/LoadDeferredTexture_ps.hlsl",
				"main"
			);

			loadPs.CreateClassInstance("AlbedoLoader");
			loadPs.CreateClassInstance("NormalLoader");
			loadPs.CreateClassInstance("PositionLoader");
			loadPs.CreateClassInstance("DepthLoader");
			loadPs.CreateClassInstance("MaterialLoader");
			loadPs.CreateClassInstance("BrightnessLoader");

			CreateVertexShader(
				sendVs,
				"ShaderFile/SendToBackBuffer_vs.hlsl",
				"main"
			);

			CreatePixelShader(
				&sendPs,
				"ShaderFile/SendToBackBuffer_ps.hlsl",
				"main"
			);

			// テクスチャの初期化
			{
				auto device = DirectXManager::GetDevice().Get();
				auto context = DirectXManager::GetImmediateContext().Get();
				tex.SetFilePath("Assets/Example.png");
				tex.SetRegistNumber(0);
				tex.CreateTextureFromFile(device, context);
				ntex.SetFilePath("Assets/Example_Normal.png");
				ntex.SetRegistNumber(0);
				ntex.CreateTextureFromFile(device, context);
			}

			// カメラの初期化
			{
				MainCamera::GetCamera().SetZoom(1.0);
				MainCamera::GetCamera().SetPosition(DXVector3(0.0f, +1.0f, -5.0f));
				MainCamera::GetCamera().SetViewDirection(DXVector3(0.0f, 0.0f, +1.0f));
				MainCamera::GetCamera().UpdateViewMatrix();
				MainCamera::GetCamera().UpdateProjectionMatrix();
			}

			// Assimpを使用したメッシュの作成
			{
				auto device = DirectXManager::GetDevice().Get();
				auto context = DirectXManager::GetImmediateContext().Get();

				//asdata.Create("Assets/PrimitiveModel/UvSphere/HighQuarity/UvSphere.fbx", "");
				//asdata.Create("Assets/PrimitiveModel/UvSphere/MiddleQuarity/UvSphere.fbx", "");
				//asdata.Create("Assets/PrimitiveModel/UvSphere/LowQuarity/UvSphere.fbx", "");
				//asdata.Create("Assets/PrimitiveModel/Cube/Cube.fbx", "");
				asdata.Create("Assets/PrimitiveModel/Monkey/Monkey.fbx", "");
				//asdata.Create("Assets/PrimitiveModel//.fbx", "");
				//asdata.Create("Assets/PrimitiveModel//.fbx", "");
				//asdata.Create("Assets/PrimitiveModel//.fbx", "");
				asmesh.Create(asdata, device, context);
				assimpMesh = CreateAssimpMesh(asdata, asmesh, DXColor::Red());
			}

			initialize = true;

			Debug::BracketEnd();

		}
	}

	// Rendering PreProcess
	{
		Debug::BracketBegin("PreProcess");

		DirectXDeviceContext* context = DirectXManager::GetImmediateContext().Get();
		MainCamera::SetViewMatrixConstantBuffer(context);
		MainCamera::SetPerspectiveProjectionMatrixConstantBuffer(context);
		MainCamera::VSSetConstantBuffer(context);
		MainCamera::PSSetConstantBuffer(context);

		static float_t x = 1.0f;
		x += 0.01f;


		auto vec = InputCameraPosition();
		auto dir = InputCameraDirection();
		MainCamera::GetCamera().SetRelativePosition(vec);
		MainCamera::GetCamera().SetRelativeViewDirection(dir.ToVector3());
		MainCamera::GetCamera().UpdateViewMatrix();

		Debug::BracketEnd();
	}


	// DeferredTextureCreatePass
	{
		Debug::BracketBegin("CreateDeferredTexture");

		DirectXDeviceContext* context = DirectXManager::GetImmediateContext().Get();
		//tex.SetRegistNumber(0);
		//tex.VSSetShaderResources(DirectXManager::GetImmediateContext().Get());
		//tex.PSSetShaderResources(DirectXManager::GetImmediateContext().Get());
		//ntex.SetRegistNumber(1);
		//ntex.VSSetShaderResources(DirectXManager::GetImmediateContext().Get());
		//ntex.PSSetShaderResources(DirectXManager::GetImmediateContext().Get());
		static float_t y = 0.0f;
		y += 0.1f;
		transform.rot.y = y;
		transform.SetWorldMatrix();
		AssimpMeshDraw(context, createVs, createPs, assimpMesh);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		Debug::BracketEnd();
	}

	static std::string use = "AlbedoLoader";
	{
		use = (DirectInput::GetKeyboardTrigger(DIK_F2)) ? "AlbedoLoader" : use;
		use = (DirectInput::GetKeyboardTrigger(DIK_F3)) ? "NormalLoader" : use;
		use = (DirectInput::GetKeyboardTrigger(DIK_F4)) ? "PositionLoader" : use;
		use = (DirectInput::GetKeyboardTrigger(DIK_F5)) ? "DepthLoader" : use;
		use = (DirectInput::GetKeyboardTrigger(DIK_F6)) ? "MaterialLoader" : use;
		use = (DirectInput::GetKeyboardTrigger(DIK_F7)) ? "BrightnessLoader" : use;
	}

	// RenderingBackBufferPass
	{
		auto context = DirectXManager::GetImmediateContext().Get();
		SetBackBufferSizeMesh();
		context->OMSetRenderTargets(1, DXGraphicBufferResult::GetResultRTV1().GetAddressOf(), nullptr);
		loadVs.IASetInputLayout(context);
		loadVs.VSSetShader(context);
		loadPs.PSSetShader(context, use);
		SetGraphicBufferSRV();
		context->Draw(4, 0);
	}

		// バックバッファに転送する
	{
		auto context = DirectXManager::GetImmediateContext().Get();
		SetBackBufferSizeMesh();
		context->OMSetRenderTargets(1, DXBackBuffer::GetRTV().GetAddressOf(), nullptr);
		sendVs.IASetInputLayout(context);
		sendVs.VSSetShader(context);
		sendPs.PSSetShader(context);
		context->VSSetShaderResources(6, 1, DXGraphicBufferResult::GetResultSRV1().GetAddressOf());
		context->PSSetShaderResources(6, 1, DXGraphicBufferResult::GetResultSRV1().GetAddressOf());
		context->Draw(4, 0);
		ShaderResourceView* nullView = nullptr;
		context->VSSetShaderResources(6, 1, &nullView);
		context->PSSetShaderResources(6, 1, &nullView);
	}

	// PostProcess
	{
		auto context = DirectXManager::GetImmediateContext().Get();
		DXGraphicBuffer::ClearRenderTargetView(context);
		SetGraphicBufferNull();
	}

	Debug::BracketEnd();
	Debug::SetLogSaveFlag(true);
	Debug::SetDispOnConsoleWindow(true);
	Debug::SetDispOnOutputWindow(true);
}



//===================================================//



void PhysicsBasedRendering() {
	Debug::SetDengerLogTimeStopFlag(false);
	//Debug::SetDispOnOutputWindow(true);
	Debug::SetDispOnOutputWindow(false);
	Debug::SetDispOnConsoleWindow(false);
	Debug::SetLogSaveFlag(false);
	//Debug::SetLogSaveFlag(false);

	Debug::BracketBegin("Rendering Pass");

	static DXVertexShader vs;
	static DXPixelShader ps;
	static DXVertexShader createVs;	// DeferredRenderingで使用するテクスチャを作成するシェーダー
	static DXPixelShader createPs;	// DeferredRenderingで使用するテクスチャを作成するシェーダー
	static DXVertexShader loadVs;		// 事前に作成されたテクスチャを読み込みDeferredRenderingを行うシェーダー
	static DXPixelShader loadPs;		// 事前に作成されたテクスチャを読み込みDeferredRenderingを行うシェーダー
	static DXShaderResourceView tex;	// テクスチャ
	static DXShaderResourceView ntex;// 法線テクスチャ

	static DXAssimpData asdata;
	static DXAssimpMesh asmesh;
	static AssimpMesh assimpMesh;
	static AssimpMaterial material;
	static Transform transform;

	// Initialize
	{
		static bool initialize = false;
		if (initialize == false) {
			Debug::SetDispOnOutputWindow(true);
			Debug::SetDispOnConsoleWindow(true);
			Debug::SetLogSaveFlag(true);
			Debug::BracketBegin("Initialize");

			// シングルトンクラスの初期化
			{
				auto device = DirectXManager::GetDevice().Get();
				// 二度初期化されると解放エラーが出力ウィンドウに発生する
				CBWorld::Initialize(device);
				CBMaterial::Initialize(device);
				CBDirectionalLight::Initialize(device);
				CBCameraParam::Initialize(device);
				DXGraphicBuffer::Initialize(device);
				MainCamera::Initialize();
			}

			Initialize(
				createVs,
				createPs,
				"ShaderFile/CreateDeferredTexture_vs.hlsl",
				"ShaderFile/CreateDeferredTexture_ps.hlsl",
				loadVs,
				loadPs,
				"ShaderFile/PseudoPBR_vs.hlsl",
				"ShaderFile/PseudoPBR_ps.hlsl"
			);

			// テクスチャの初期化
			{
				auto device = DirectXManager::GetDevice().Get();
				auto context = DirectXManager::GetImmediateContext().Get();
				//tex.SetFilePath("Assets/Example.png");
				tex.SetFilePath("Assets/Tile.png");
				tex.SetRegistNumber(0);
				tex.CreateTextureFromFile(device, context);
				//ntex.SetFilePath("Assets/Example_Normal.png");
				ntex.SetFilePath("Assets/Tile_Normal.png");
				ntex.SetRegistNumber(0);
				ntex.CreateTextureFromFile(device, context);
			}

			// メッシュの読み込み
			{
				auto device = DirectXManager::GetDevice().Get();
				auto context = DirectXManager::GetImmediateContext().Get();

				//asdata.Create("Assets/PrimitiveModel/UvSphere/HighQuarity/UvSphere.fbx", "");
				//asdata.Create("Assets/PrimitiveModel/UvSphere/MiddleQuarity/UvSphere.fbx", "");
				//asdata.Create("Assets/PrimitiveModel/UvSphere/LowQuarity/UvSphere.fbx", "");
				//asdata.Create("Assets/PrimitiveModel/Monkey/Monkey.fbx", "");
				//asdata.Create("Assets/PrimitiveModel//.fbx", "");
				//asdata.Create("Assets/PrimitiveModel//.fbx", "");
				//asdata.Create("Assets/PrimitiveModel//.fbx", "");
				asdata.Create("Assets/PrimitiveModel/Cube/Cube.fbx", "");
				asmesh.Create(asdata, device, context);
				assimpMesh = CreateAssimpMesh(asdata, asmesh, DXColor::Red());
			}

			// カメラの初期化
			{
				MainCamera::GetCamera().SetZoom(1.0);
				MainCamera::GetCamera().SetPosition(DXVector3(0.0f, +1.0f, -5.0f));
				MainCamera::GetCamera().SetViewDirection(DXVector3(0.0f, 0.0f, +1.0f));
				MainCamera::GetCamera().UpdateViewMatrix();
				MainCamera::GetCamera().UpdateProjectionMatrix();
			}

			// 定数バッファの初期パラメータ
			{
				auto context = DirectXManager::GetImmediateContext().Get();
				CBCameraParam::SetViewPortSize(Application::GetWindowSize());
				CBCameraParam::UpdateConstantBuffer(context);
				CBCameraParam::VSSetConstantBuffer(context);
				CBCameraParam::PSSetConstantBuffer(context);
			}

			initialize = true;

			Debug::BracketEnd();
			Debug::SetDispOnOutputWindow(false);
			Debug::SetDispOnConsoleWindow(false);
			Debug::SetLogSaveFlag(false);
		}
	}

	// Rendering PreProcess
	{
		Debug::BracketBegin("PreProcess");

		DirectXDeviceContext* context = DirectXManager::GetImmediateContext().Get();
		MainCamera::SetViewMatrixConstantBuffer(context);
		MainCamera::SetPerspectiveProjectionMatrixConstantBuffer(context);
		MainCamera::VSSetConstantBuffer(context);
		MainCamera::PSSetConstantBuffer(context);

		static float_t x = 1.0f;
		x += 0.01f;

		{
			DXVector3 vec = InputCameraPosition();
			DXVector2 dir = InputCameraDirection();
			MainCamera::GetCamera().SetRelativePosition(vec);
			MainCamera::GetCamera().SetRelativeViewDirection(dir.ToVector3());
			MainCamera::GetCamera().UpdateViewMatrix();
		}

		SetDirectionalLignt(
			DXVector3::Back() + DXVector3::Right(),
			DXColor::White(),
			DXColor(0.4f, 0.4f, 0.4f, 1.0f)
		);

		SetGraphicBuffer(
			createVs,
			createPs
		);

		Debug::BracketEnd();
	}

	// DeferredTextureCreatePass
	{

		static DXVector3 irot;
		static DXVector3 imat;
		DXVector3 input;
		DXVector3 mat;
		{
			bool shift = false;
			bool ctrl = false;
			if (DirectInput::GetKeyboardPress(DIK_LSHIFT)) {
				shift = true;
			}
			if (DirectInput::GetKeyboardPress(DIK_LCONTROL)) {
				ctrl = true;
			}
			if (DirectInput::GetKeyboardPress(DIK_1)) {
				input.x += 0.01f;
			}
			if (DirectInput::GetKeyboardPress(DIK_2)) {
				input.y += 0.01f;
			}
			if (DirectInput::GetKeyboardPress(DIK_3)) {
				input.z += 0.001f;
			}
			if (DirectInput::GetKeyboardPress(DIK_7)) {
				mat.x += 0.0001f;
			}
			if (DirectInput::GetKeyboardPress(DIK_8)) {
				mat.y += 0.0001f;
			}
			if (DirectInput::GetKeyboardPress(DIK_9)) {
				mat.z += 0.0001f;
			}

			if (shift) {
				input *= 20.0f;
				mat *= 20.0f;
			}
			if (ctrl) {
				input.Inverse();
				mat.Inverse();
			}

			irot += input;
			imat += mat;

			imat.x = Math::Clamp01(imat.x);
			imat.y = Math::Clamp01(imat.y);
			imat.z = Math::Clamp01(imat.z);
		}

		DirectXDeviceContext* context = DirectXManager::GetImmediateContext().Get();
		static float y = 0;
		y += 0.01f;
		transform.rot.y = y;
		transform.SetWorldMatrix();
		material.roughness = imat.x;
		material.metalness = imat.y;
		material.specular = imat.z;
		material.SetMaterial();

		tex.SetRegistNumber(0);
		tex.VSSetShaderResources(context);
		tex.PSSetShaderResources(context);
		ntex.SetRegistNumber(1);
		ntex.VSSetShaderResources(context);
		ntex.PSSetShaderResources(context);
		AssimpMeshDraw(context, assimpMesh);
		//AssimpSubMeshDraw(context, assimpMesh.m_subMesh[1]);
	}

	// RenderingBackBufferPass
	{
		Debug::BracketBegin("BackBufferWriting");

		RenderBackBuffer(loadVs, loadPs);

		Debug::BracketEnd();
	}

	// PostProcess
	{
		auto context = DirectXManager::GetImmediateContext().Get();
		DXGraphicBuffer::ClearRenderTargetView(context);
		SetGraphicBufferNull();
	}

	{
		ScopedTimer t;
		t.Begin();
		while (DirectInput::GetKeyboardPress(DIK_0)) {
			if (t.Second() > 2.0f) break;
		}
		t.End();
	}

	Debug::BracketEnd();
	Debug::SetLogSaveFlag(true);
	Debug::SetDispOnConsoleWindow(true);
	Debug::SetDispOnOutputWindow(true);
}




//===================================================//




void LaplacianFilter() {
	Debug::SetDengerLogTimeStopFlag(false);
//Debug::SetDispOnOutputWindow(true);
	Debug::SetDispOnOutputWindow(false);
	Debug::SetDispOnConsoleWindow(false);
	Debug::SetLogSaveFlag(false);
	//Debug::SetLogSaveFlag(false);

	Debug::BracketBegin("Rendering Pass");

	static DXVertexShader vs;
	static DXPixelShader ps;
	static DXVertexShader createVs;	// DeferredRenderingで使用するテクスチャを作成するシェーダー
	static DXPixelShader createPs;	// DeferredRenderingで使用するテクスチャを作成するシェーダー
	static DXVertexShader loadVs;		// 事前に作成されたテクスチャを読み込みDeferredRenderingを行うシェーダー
	static DXPixelShader loadPs;		// 事前に作成されたテクスチャを読み込みDeferredRenderingを行うシェーダー

	static DXAssimpData asdata;
	static DXAssimpMesh asmesh;
	static AssimpMesh assimpMesh;
	static AssimpMaterial material;
	static Transform transform;

	// Initialize
	{
		static bool initialize = false;
		if (initialize == false) {

			Debug::BracketBegin("Initialize");

			// シングルトンクラスの初期化
			{
				auto device = DirectXManager::GetDevice().Get();
				// 二度初期化されると解放エラーが出力ウィンドウに発生する
				CBWorld::Initialize(device);
				CBMaterial::Initialize(device);
				CBDirectionalLight::Initialize(device);
				CBCameraParam::Initialize(device);
				DXGraphicBuffer::Initialize(device);
			}

			Initialize(
				createVs,
				createPs,
				"ShaderFile/CreateDeferredTexture_vs.hlsl",
				"ShaderFile/CreateDeferredTexture_ps.hlsl",
				loadVs,
				loadPs,
				"ShaderFile/LaplacianFilter_vs.hlsl",
				"ShaderFile/LaplacianFilter_ps.hlsl"
			);

			// カメラの初期化
			{
				CBCameraParam::SetViewPortSize(Application::GetWindowSize());
				MainCamera::Initialize();
				MainCamera::GetCamera().SetZoom(1.0);
				MainCamera::GetCamera().SetPosition(DXVector3(0.0f, +1.0f, -5.0f));
				MainCamera::GetCamera().SetViewDirection(DXVector3(0.0f, 0.0f, +1.0f));
				MainCamera::GetCamera().UpdateViewMatrix();
				MainCamera::GetCamera().UpdateProjectionMatrix();
			}

			// メッシュの作成
			{
				auto device = DirectXManager::GetDevice().Get();
				auto context = DirectXManager::GetImmediateContext().Get();

				//asdata.Create("Assets/PrimitiveModel/UvSphere/HighQuarity/UvSphere.fbx", "");
				//asdata.Create("Assets/PrimitiveModel/UvSphere/MiddleQuarity/UvSphere.fbx", "");
				asdata.Create("Assets/PrimitiveModel/UvSphere/LowQuarity/UvSphere.fbx", "");
				//asdata.Create("Assets/PrimitiveModel/Cube/Cube.fbx", "");
				//asdata.Create("Assets/PrimitiveModel/Monkey/Monkey.fbx", "");
				//asdata.Create("Assets/PrimitiveModel//.fbx", "");
				//asdata.Create("Assets/PrimitiveModel//.fbx", "");
				//asdata.Create("Assets/PrimitiveModel//.fbx", "");
				asmesh.Create(asdata, device, context);
				assimpMesh = CreateAssimpMesh(asdata, asmesh, DXColor::Red());
			}

			initialize = true;

			Debug::BracketEnd();

		}
	}

	// Rendering PreProcess
	{
		Debug::BracketBegin("PreProcess");

		DirectXDeviceContext* context = DirectXManager::GetImmediateContext().Get();
		MainCamera::SetViewMatrixConstantBuffer(context);
		MainCamera::SetPerspectiveProjectionMatrixConstantBuffer(context);
		MainCamera::VSSetConstantBuffer(context);
		MainCamera::PSSetConstantBuffer(context);

		{
			DXVector3 vec = InputCameraPosition();
			DXVector2 dir = InputCameraDirection();
			MainCamera::GetCamera().SetRelativePosition(vec);
			MainCamera::GetCamera().SetRelativeViewDirection(dir.ToVector3());
			MainCamera::GetCamera().UpdateViewMatrix();
		}


		SetGraphicBuffer(createVs, createPs);

		Debug::BracketEnd();
	}


	// DeferredTextureCreatePass
	{
		Debug::BracketBegin("CreateDeferredTexture");
		DirectXDeviceContext* context = DirectXManager::GetImmediateContext().Get();

		static float_t y = 0.0f;
		y += 0.1f;
		transform.rot.y = y;
		transform.SetWorldMatrix();
		material.roughness = 1.0f;
		material.metalness = 1.0f;
		material.specular = 1.0f;
		material.SetMaterial();
		AssimpMeshDraw(context, assimpMesh);

		Debug::BracketEnd();
	}

	// RenderingBackBufferPass
	{
		Debug::BracketBegin("BackBufferWriting");

		RenderBackBuffer(loadVs, loadPs);


		Debug::BracketEnd();
	}

	// PostProcess
	{
		auto context = DirectXManager::GetImmediateContext().Get();
		DXGraphicBuffer::ClearRenderTargetView(context);
		SetGraphicBufferNull();
	}

	Debug::BracketEnd();
	Debug::SetLogSaveFlag(true);
	Debug::SetDispOnConsoleWindow(true);
	Debug::SetDispOnOutputWindow(true);
}



//===================================================//



void GaussianFilter() {
	Debug::SetDengerLogTimeStopFlag(false);
//Debug::SetDispOnOutputWindow(true);
	Debug::SetDispOnOutputWindow(false);
	Debug::SetDispOnConsoleWindow(false);
	Debug::SetLogSaveFlag(false);
	//Debug::SetLogSaveFlag(false);

	Debug::BracketBegin("Rendering Pass");

	static DXVertexShader vs;
	static DXPixelShader ps;
	static DXVertexShader createVs;	// DeferredRenderingで使用するテクスチャを作成するシェーダー
	static DXPixelShader createPs;	// DeferredRenderingで使用するテクスチャを作成するシェーダー
	static DXVertexShader loadVs;		// 事前に作成されたテクスチャを読み込みDeferredRenderingを行うシェーダー
	static DXPixelShader loadPs;		// 事前に作成されたテクスチャを読み込みDeferredRenderingを行うシェーダー
	static DXShaderResourceView tex;	// テクスチャ
	static DXShaderResourceView ntex;// 法線テクスチャ

	static DXAssimpData asdata;
	static DXAssimpMesh asmesh;
	static AssimpMesh assimpMesh;
	static AssimpMaterial material;
	static Transform transform;

	// Initialize
	{
		static bool initialize = false;
		if (initialize == false) {

			Debug::BracketBegin("Initialize");

			// シングルトンクラスの初期化
			{
				auto device = DirectXManager::GetDevice().Get();
				// 二度初期化されると解放エラーが出力ウィンドウに発生する
				CBWorld::Initialize(device);
				CBMaterial::Initialize(device);
				CBDirectionalLight::Initialize(device);
				CBCameraParam::Initialize(device);
				DXGraphicBuffer::Initialize(device);
				MainCamera::Initialize();
				DeferredRenderingCamera::Initialize();
			}

			Initialize(
				createVs,
				createPs,
				"ShaderFile/CreateDeferredTexture_vs.hlsl",
				"ShaderFile/CreateDeferredTexture_ps.hlsl",
				loadVs,
				loadPs,
				"ShaderFile/GaussianFilter_vs.hlsl",
				"ShaderFile/GaussianFilter_ps.hlsl"
			);

			// テクスチャの初期化
			{
				auto device = DirectXManager::GetDevice().Get();
				auto context = DirectXManager::GetImmediateContext().Get();
				//tex.SetFilePath("Assets/Example.png");
				tex.SetFilePath("Assets/Tile.png");
				tex.SetRegistNumber(0);
				tex.CreateTextureFromFile(device, context);
				//ntex.SetFilePath("Assets/Example_Normal.png");
				ntex.SetFilePath("Assets/Tile_Normal.png");
				ntex.SetRegistNumber(0);
				ntex.CreateTextureFromFile(device, context);
			}

			// メッシュの読み込み
			{
				auto device = DirectXManager::GetDevice().Get();
				auto context = DirectXManager::GetImmediateContext().Get();
				//asdata.Create("Assets/PrimitiveModel/UvSphere/HighQuarity/UvSphere.fbx", "");
				//asdata.Create("Assets/PrimitiveModel/UvSphere/MiddleQuarity/UvSphere.fbx", "");
				//asdata.Create("Assets/PrimitiveModel/UvSphere/LowQuarity/UvSphere.fbx", "");
				//asdata.Create("Assets/PrimitiveModel/Monkey/Monkey.fbx", "");
				//asdata.Create("Assets/PrimitiveModel//.fbx", "");
				//asdata.Create("Assets/PrimitiveModel//.fbx", "");
				//asdata.Create("Assets/PrimitiveModel//.fbx", "");
				asdata.Create("Assets/PrimitiveModel/Cube/Cube.fbx", "");
				asmesh.Create(asdata, device, context);
				assimpMesh = CreateAssimpMesh(asdata, asmesh, DXColor::Red());
			}

			// カメラの初期化
			{
				MainCamera::GetCamera().SetZoom(1.0);
				MainCamera::GetCamera().SetPosition(DXVector3(0.0f, +1.0f, -5.0f));
				MainCamera::GetCamera().SetViewDirection(DXVector3(0.0f, 0.0f, +1.0f));
				MainCamera::GetCamera().UpdateViewMatrix();
				MainCamera::GetCamera().UpdateProjectionMatrix();
			}

			// 定数バッファの初期パラメータ
			{
				auto context = DirectXManager::GetImmediateContext().Get();
				CBCameraParam::SetViewPortSize(Application::GetWindowSize());
				CBCameraParam::UpdateConstantBuffer(context);
				CBCameraParam::VSSetConstantBuffer(context);
				CBCameraParam::PSSetConstantBuffer(context);
			}

			initialize = true;

			Debug::BracketEnd();

		}
	}

	// Rendering PreProcess
	{
		Debug::BracketBegin("PreProcess");

		DirectXDeviceContext* context = DirectXManager::GetImmediateContext().Get();
		MainCamera::SetViewMatrixConstantBuffer(context);
		MainCamera::SetPerspectiveProjectionMatrixConstantBuffer(context);
		MainCamera::VSSetConstantBuffer(context);
		MainCamera::PSSetConstantBuffer(context);

		static float_t x = 1.0f;
		x += 0.01f;

		{
			DXVector3 vec = InputCameraPosition();
			DXVector2 dir = InputCameraDirection();
			MainCamera::GetCamera().SetRelativePosition(vec);
			MainCamera::GetCamera().SetRelativeViewDirection(dir.ToVector3());
			MainCamera::GetCamera().UpdateViewMatrix();
		}

		SetDirectionalLignt(
			DXVector3::Back() + DXVector3::Right(),
			DXColor::White(),
			DXColor(0.4f, 0.4f, 0.4f, 1.0f)
		);

		SetGraphicBuffer(
			createVs,
			createPs
		);

		Debug::BracketEnd();
	}


	// DeferredTextureCreatePass
	{
		Debug::BracketBegin("CreateDeferredTexture");

		DirectXDeviceContext* context = DirectXManager::GetImmediateContext().Get();
		static float y = 0;
		y += 0.01f;
		transform.rot.y = y;
		transform.SetWorldMatrix();
		material.roughness = 1.0f;
		material.metalness = 1.0f;
		material.specular = 1.0f;
		material.SetMaterial();

		tex.SetRegistNumber(0);
		tex.VSSetShaderResources(context);
		tex.PSSetShaderResources(context);
		ntex.SetRegistNumber(1);
		ntex.VSSetShaderResources(context);
		ntex.PSSetShaderResources(context);
		AssimpMeshDraw(context, assimpMesh);

		Debug::BracketEnd();
	}


	// RenderingBackBufferPass
	{
		Debug::BracketBegin("BackBufferWriting");

		RenderBackBuffer(loadVs, loadPs);

		Debug::BracketEnd();
	}

	// PostProcess
	{
		auto context = DirectXManager::GetImmediateContext().Get();
		DXGraphicBuffer::ClearRenderTargetView(context);
		SetGraphicBufferNull();
	}

	Debug::BracketEnd();
	Debug::SetLogSaveFlag(true);
	Debug::SetDispOnConsoleWindow(true);
	Debug::SetDispOnOutputWindow(true);
}



//===================================================//



void RenderingPassExample() {
	//Debug::Log("\
	//////////////////////\n\
	/////RenderingPassExample///\n\
	//////////////////////\n\
	//");
	Debug::SetDengerLogTimeStopFlag(false);
	Debug::SetDispOnOutputWindow(false);
	Debug::SetDispOnConsoleWindow(false);
	Debug::SetLogSaveFlag(false);

	static DXVertexShader createVs;				// DeferredRenderingで使用するテクスチャを作成するシェーダー
	static DXPixelShaderLink createPs;			// DeferredRenderingで使用するテクスチャを作成するシェーダー
	static DXVertexShader createShadowMapVs;	// シャドウマップの作成を行うシェーダー
	static DXPixelShader createShadowMapPs;	// シャドウマップの作成を行うシェーダー
	static DXVertexShader shadingVs;				// 適当にシェーディングする 
	static DXPixelShaderLink shadingPs;			// 適当にシェーディングする
	static DXVertexShader rimVs;					// リムライト
	static DXPixelShaderLink rimPs;				// リムライト
	static DXVertexShader tonemapVs;				// トーンマッピング
	static DXPixelShader tonemapPs;				// トーンマッピング
	static DXVertexShader outlineVs;				// アウトラインシェーダー
	static DXPixelShaderLink outlinePs;			// アウトラインシェーダー
	static DXVertexShaderLink blurVs;			//	ブラーシェーダー
	static DXPixelShaderLink blurPs;				//	ブラーシェーダー
	static DXVertexShader radialBlurVs;			// 放射状ブラー
	static DXPixelShaderLink radialBlurPs;		// 放射状ブラー
	static DXVertexShader colorConvertVs;		// 事前に作成されたレンダリング結果をバックバッファに送るシェーダー　※送る際にガンマ補正を行う
	static DXPixelShaderLink colorConvertPs;	// 事前に作成されたレンダリング結果をバックバッファに送るシェーダー　※送る際にガンマ補正を行う
	static DXVertexShader mosaicVs;				// モザイク
	static DXPixelShaderLink mosaicPs;			// モザイク
	static DXVertexShader sendVs;					// バックバッファに結果を送るためだけのシェーダー
	static DXPixelShader sendPs;					// バックバッファに結果を送るためだけのシェーダー
	static DXShaderResourceView tex;				// テクスチャ
	static DXShaderResourceView ntex;			// 法線テクスチャ


	static DXAssimpData asdata;
	static DXAssimpMesh asmesh;
	static AssimpMesh assimpMesh;
	static AssimpMaterial material;
	static Transform transform;

	static DXAssimpData _asdata;
	static DXAssimpMesh _asmesh;
	static AssimpMesh _assimpMesh;

	static DXAssimpData skydomeData;
	static DXAssimpMesh skydomeMesh;
	static AssimpMesh skydome;

	// Initialize
	{
		static bool initialize = false;
		if (initialize == false) {
			Debug::SetDengerLogTimeStopFlag(true);
			Debug::SetDispOnOutputWindow(true);
			Debug::SetDispOnConsoleWindow(true);
			Debug::SetLogSaveFlag(true);

			Debug::BracketBegin("Initialize");

			InitializeSingleton();
			InitializeCamera();
			InitializeConstantBufferParam();

			CreateDeferredTextureShader(
				createVs,
				"ShaderFile/CreateDeferredTexture_vs.hlsl",
				"main"
			);

			CreatePixelShader(
				&createPs,
				"ShaderFile/CreateDeferredTexture_ps.hlsl",
				"main"
			);

			createPs.CreateClassInstance("VertexColorAndVertexNormal");
			createPs.CreateClassInstance("VertexColorAndNormalMapNormal");
			createPs.CreateClassInstance("TextureColorAndVertexNormal");
			createPs.CreateClassInstance("TextureColorAndNormalMapNormal");

			CreateDeferredTextureShader(
				createShadowMapVs,
				"ShaderFile/CreateShadowMapTexture_vs.hlsl",
				"main"
			);

			CreatePixelShader(
				createShadowMapPs,
				"ShaderFile/CreateShadowMapTexture_ps.hlsl",
				"main"
			);

			CreateVertexShader(
				shadingVs,
				"ShaderFile/Shading_vs.hlsl",
				"main"
			);

			CreatePixelShader(
				&shadingPs,
				"ShaderFile/Shading_ps.hlsl",
				"main"
			);

			shadingPs.CreateClassInstance("NoShading");
			shadingPs.CreateClassInstance("LambertShading");
			shadingPs.CreateClassInstance("LambertAndPhongShading");
			shadingPs.CreateClassInstance("PhysicsBasedRendering");
			shadingPs.CreateClassInstance("DisneyBasePhysicsBasedRendering");
			shadingPs.CreateClassInstance("TestPBR");

			//CreateVertexShader(
			//	shadingVs,
			//	"ShaderFile/PhysicsBasedRendering_vs.hlsl",
			//	"main"
			//);

			//CreatePixelShader(
			//	shadingPs,
			//	"ShaderFile/PhysicsBasedRendering_ps.hlsl",
			//	"main"
			//);

			CreateVertexShader(
				rimVs,
				"ShaderFile/RimLight_vs.hlsl",
				"main"
			);

			CreatePixelShader(
				&rimPs,
				"ShaderFile/RimLight_ps.hlsl",
				"main"
			);

			rimPs.CreateClassInstance("RimLight");
			rimPs.CreateClassInstance("TestRimLight");
			rimPs.CreateClassInstance("NoLight");

			CreateVertexShader(
				tonemapVs,
				"ShaderFile/CreateToneMap_vs.hlsl",
				"main"
			);

			CreatePixelShader(
				tonemapPs,
				"ShaderFile/CreateToneMap_ps.hlsl",
				"main"
			);

			CreateVertexShader(
				outlineVs,
				"ShaderFile/LaplacianFilter_vs.hlsl",
				"main"
			);

			CreatePixelShader(
				&outlinePs,
				"ShaderFile/LaplacianFilter_ps.hlsl",
				"main"
			);

			outlinePs.CreateClassInstance("LaplacianFilter");
			outlinePs.CreateClassInstance("NoFilter");

			CreateVertexShader(
				&blurVs,
				"ShaderFile/GaussianFilter_vs.hlsl",
				"main"
			);

			blurVs.CreateClassInstance("GausBlurX");
			blurVs.CreateClassInstance("GausBlurY");
			blurVs.CreateClassInstance("NoBlur");

			CreatePixelShader(
				&blurPs,
				"ShaderFile/GaussianFilter_ps.hlsl",
				"main"
			);

			blurPs.CreateClassInstance("ApplyBlur");
			blurPs.CreateClassInstance("NoBlur");

			CreateVertexShader(
				radialBlurVs,
				"ShaderFile/RadialBlur_vs.hlsl",
				"main"
			);

			CreatePixelShader(
				&radialBlurPs,
				"ShaderFile/RadialBlur_ps.hlsl",
				"main"
			);

			radialBlurPs.CreateClassInstance("ApplyBlur");
			radialBlurPs.CreateClassInstance("NoBlur");

			CreateVertexShader(
				colorConvertVs,
				"ShaderFile/ConvertColor_vs.hlsl",
				"main"
			);

			CreatePixelShader(
				&colorConvertPs,
				"ShaderFile/ConvertColor_ps.hlsl",
				"main"
			);

			colorConvertPs.CreateClassInstance("GammaConvert");
			colorConvertPs.CreateClassInstance("GrayConvert");
			colorConvertPs.CreateClassInstance("WhiteBlackConvert");
			colorConvertPs.CreateClassInstance("SepiaConvert");
			colorConvertPs.CreateClassInstance("NegativeConvert");
			colorConvertPs.CreateClassInstance("NoConvert");

			CreateVertexShader(
				mosaicVs,
				"ShaderFile/MosaicFilter_vs.hlsl",
				"main"
			);

			CreatePixelShader(
				&mosaicPs,
				"ShaderFile/MosaicFilter_ps.hlsl",
				"main"
			);

			mosaicPs.CreateClassInstance("MosaicFilter");
			mosaicPs.CreateClassInstance("NoFilter");

			CreateVertexShader(
				sendVs,
				"ShaderFile/SendToBackBuffer_vs.hlsl",
				"main"
			);

			CreatePixelShader(
				&sendPs,
				"ShaderFile/SendToBackBuffer_ps.hlsl",
				"main"
			);

			// テクスチャの初期化
			{
				auto device = DirectXManager::GetDevice().Get();
				auto context = DirectXManager::GetImmediateContext().Get();
				tex.SetFilePath("Assets/Example.png");
				tex.SetRegistNumber(0);
				tex.CreateTextureFromFile(device, context);
				ntex.SetFilePath("Assets/Example_Normal.png");
				ntex.SetRegistNumber(0);
				ntex.CreateTextureFromFile(device, context);
			}

			// カメラの初期化
			{
				MainCamera::GetCamera().SetZoom(1.0);
				MainCamera::GetCamera().SetPosition(DXVector3(0.0f, +1.0f, -5.0f));
				MainCamera::GetCamera().SetViewDirection(DXVector3(0.0f, 0.0f, +1.0f));
				MainCamera::GetCamera().UpdateViewMatrix();
				MainCamera::GetCamera().UpdateProjectionMatrix();
			}

			// Assimpを使用したメッシュの作成
			{
				auto device = DirectXManager::GetDevice().Get();
				auto context = DirectXManager::GetImmediateContext().Get();

				//asdata.Create("Assets/PrimitiveModel/UvSphere/HighQuarity/UvSphere.fbx", "");
				//asdata.Create("Assets/PrimitiveModel/UvSphere/MiddleQuarity/UvSphere.fbx", "");
				asdata.Create("Assets/PrimitiveModel/UvSphere/LowQuarity/UvSphere.fbx", "");
				//asdata.Create("Assets/PrimitiveModel/Cube/Cube.fbx", "");
				//asdata.Create("Assets/PrimitiveModel/Monkey/Monkey.fbx", "");
				//asdata.Create("Assets/PrimitiveModel/Quad/Quad.fbx", "");
				//asdata.Create("Assets/PrimitiveModel//.fbx", "");
				//asdata.Create("Assets/PrimitiveModel//.fbx", "");
				//asdata.Create("Assets/PrimitiveModel//.fbx", "");
				asmesh.Create(asdata, device, context);
				assimpMesh = CreateAssimpMesh(asdata, asmesh, DXColor::White());

				_asdata.Create("Assets/PrimitiveModel/Quad/Quad.fbx", "");
				_asmesh.Create(_asdata, device, context);
				_assimpMesh = CreateAssimpMesh(_asdata, _asmesh, DXColor::Lerp(DXColor::Blue(), DXColor::Black(), 0.75f));

				skydomeData.Create("Assets/SkyDome/SkyDome_0/SkyDome_0.fbx", "Assets/SkyDome/SkyDome_0/");
				skydomeMesh.Create(skydomeData, device, context);
				skydome = CreateAssimpMesh(skydomeData, skydomeMesh, DXColor::White());
			}

			{
				auto device = DirectXManager::GetDevice().Get();
				auto context = DirectXManager::GetImmediateContext().Get();
				auto meshCount = assimpMesh.m_subMesh.size();
				for (int_t i = 0; i < meshCount; i++) {
					assimpMesh.m_subMeshMaterial[i].m_diffuse.SetFilePath("Assets/stones.bmp");
					//assimpMesh.m_subMeshMaterial[i].m_diffuse.SetFilePath("Assets/Tile.png");
					assimpMesh.m_subMeshMaterial[i].m_diffuse.CreateTextureFromFile(device, context);
				}
				for (int_t i = 0; i < meshCount; i++) {
					assimpMesh.m_subMeshMaterial[i].m_normal.SetFilePath("Assets/stones_NM_height.png");
					//assimpMesh.m_subMeshMaterial[i].m_normal.SetFilePath("Assets/Tile_Normal.png");
					assimpMesh.m_subMeshMaterial[i].m_normal.CreateTextureFromFile(device, context);
				}
			}

			initialize = true;

			//Debug::BracketEnd();
			//Debug::SetDengerLogTimeStopFlag(false);
			//Debug::SetDispOnOutputWindow(false);
			//Debug::SetDispOnConsoleWindow(false);
			//Debug::SetLogSaveFlag(false);
		}
	}

	// Rendering PreProcess
	{
		Debug::BracketBegin("PreProcess");

		DirectXDeviceContext* context = DirectXManager::GetImmediateContext().Get();
		MainCamera::SetCameraPositionConstantBuffer(context);
		MainCamera::SetCameraViewDirectionConstantBuffer(context);
		MainCamera::SetViewMatrixConstantBuffer(context);
		MainCamera::SetPerspectiveProjectionMatrixConstantBuffer(context);
		MainCamera::VSSetConstantBuffer(context);
		MainCamera::PSSetConstantBuffer(context);

		static float_t x = 1.0f;
		x += 0.01f;


		auto vec = InputCameraPosition();
		auto dir = InputCameraDirection();
		MainCamera::GetCamera().SetRelativePosition(vec);
		MainCamera::GetCamera().SetRelativeViewDirection(dir.ToVector3());
		MainCamera::GetCamera().UpdateViewMatrix();





		Debug::BracketEnd();
	}


	// DeferredTextureCreatePass
	{
		Debug::BracketBegin("CreateDeferredTexture");

		DirectXDeviceContext* context = DirectXManager::GetImmediateContext().Get();

		// Assimpはトライアングルリストで描画する必要がある
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//tex.SetRegistNumber(0);
		//tex.VSSetShaderResources(DirectXManager::GetImmediateContext().Get());
		//tex.PSSetShaderResources(DirectXManager::GetImmediateContext().Get());
		//ntex.SetRegistNumber(1);
		//ntex.VSSetShaderResources(DirectXManager::GetImmediateContext().Get());
		//ntex.PSSetShaderResources(DirectXManager::GetImmediateContext().Get());
		SetDirectionalLignt(
			DXVector3::Back() + DXVector3::Right(),
			DXColor::White(),
			DXColor(0.4f, 0.4f, 0.4f, 1.0f)
		);
		static float_t y = 0.0f;
		y += 0.1f;

		// シャドウマップへの書き込み
		{
			transform.Reset();
			transform.pos.y = +2.0f;
			transform.rot.y = y;
			transform.SetWorldMatrix();
			material.roughness = 0.5f;
			material.metalness = 0.0f;
			material.specular = 1.0f;
			material.SetMaterial();
			AssimpMeshShadowMapDraw(context, createShadowMapVs, createShadowMapPs, assimpMesh);

			transform.pos.x = 5.0f;
			transform.SetWorldMatrix();
			material.roughness = 1.0f;
			material.metalness = 0.0f;
			material.specular = 1.0f;
			material.SetMaterial();
			AssimpMeshShadowMapDraw(context, createShadowMapVs, createShadowMapPs, assimpMesh);

			transform.pos.x = -5.0f;
			transform.SetWorldMatrix();
			material.roughness = 0.0f;
			material.metalness = 0.0f;
			material.specular = 1.0f;
			material.SetMaterial();
			AssimpMeshShadowMapDraw(context, createShadowMapVs, createShadowMapPs, assimpMesh);

			transform.pos = DXVector3::Zero();
			transform.pos.y = -2.0f;
			transform.rot.y = y;
			transform.SetWorldMatrix();
			material.roughness = 0.5f;
			material.metalness = 1.0f;
			material.specular = 1.0f;
			material.SetMaterial();
			AssimpMeshShadowMapDraw(context, createShadowMapVs, createShadowMapPs, assimpMesh);

			transform.pos.x = 5.0f;
			transform.SetWorldMatrix();
			material.roughness = 1.0f;
			material.metalness = 1.0f;
			material.specular = 1.0f;
			material.SetMaterial();
			AssimpMeshShadowMapDraw(context, createShadowMapVs, createShadowMapPs, assimpMesh);

			transform.pos.x = -5.0f;
			transform.SetWorldMatrix();
			material.roughness = 0.0f;
			material.metalness = 1.0f;
			material.specular = 1.0f;
			material.SetMaterial();
			AssimpMeshShadowMapDraw(context, createShadowMapVs, createShadowMapPs, assimpMesh);

			//transform.Reset();
			//transform.pos.y = -10.0f;
			//transform.rot.x = -90.0f;
			//transform.sca.Set(100.0f);
			//transform.SetWorldMatrix();
			//material.roughness = 0.0f;
			//material.metalness = 1.0f;
			//material.specular = 1.0f;
			//material.SetMaterial();
			//AssimpMeshShadowMapDraw(context, createShadowMapVs, createShadowMapPs, _assimpMesh);
		}
		// シャドウマップを設定
		{
			SetGraphicBufferLink(createVs, createPs, "VertexColorAndVertexNormal");
			context->VSSetShaderResources(2, 1, MainDirectionalLight::GetRenderTarget().GetSRV().GetAddressOf());
			context->PSSetShaderResources(2, 1, MainDirectionalLight::GetRenderTarget().GetSRV().GetAddressOf());
			//context->VSSetShaderResources(2, 1, MainDirectionalLight::GetDepthStencil().GetSRV().GetAddressOf());
			//context->PSSetShaderResources(2, 1, MainDirectionalLight::GetDepthStencil().GetSRV().GetAddressOf());
		}
		// 通常のモデルの描画
		{
			transform.Reset();
			transform.pos.y = +2.0f;
			transform.rot.y = y;
			transform.SetWorldMatrix();
			material.roughness = 0.5f;
			material.metalness = 0.0f;
			material.specular = 1.0f;
			material.SetMaterial();
			AssimpMeshDraw(context, createVs, createPs, assimpMesh);

			transform.pos.x = 5.0f;
			transform.SetWorldMatrix();
			material.roughness = 1.0f;
			material.metalness = 0.0f;
			material.specular = 1.0f;
			material.SetMaterial();
			AssimpMeshDraw(context, createVs, createPs, assimpMesh);

			transform.pos.x = -5.0f;
			transform.SetWorldMatrix();
			material.roughness = 0.0f;
			material.metalness = 0.0f;
			material.specular = 1.0f;
			material.SetMaterial();
			AssimpMeshDraw(context, createVs, createPs, assimpMesh);

			transform.pos = DXVector3::Zero();
			transform.pos.y = -2.0f;
			transform.rot.y = y;
			transform.SetWorldMatrix();
			material.roughness = 0.5f;
			material.metalness = 1.0f;
			material.specular = 1.0f;
			material.SetMaterial();
			AssimpMeshDraw(context, createVs, createPs, assimpMesh);

			transform.pos.x = 5.0f;
			transform.SetWorldMatrix();
			material.roughness = 1.0f;
			material.metalness = 1.0f;
			material.specular = 1.0f;
			material.SetMaterial();
			AssimpMeshDraw(context, createVs, createPs, assimpMesh);

			transform.pos.x = -5.0f;
			transform.SetWorldMatrix();
			material.roughness = 0.0f;
			material.metalness = 1.0f;
			material.specular = 1.0f;
			material.SetMaterial();
			AssimpMeshDraw(context, createVs, createPs, assimpMesh);

			transform.Reset();
			transform.pos.y = -10.0f;
			transform.rot.x = -90.0f;
			transform.sca.Set(100.0f);
			transform.SetWorldMatrix();
			material.roughness = 1.0f;
			material.metalness = 1.0f;
			material.specular = 1.0f;
			material.SetMaterial();
			AssimpMeshDraw(context, createVs, createPs, _assimpMesh);
		}


		//Transform trans;
		//trans.pos.x = 0.0f;
		//trans.pos.z = +10.0f;
		//trans.sca.Set(1000.0f);
		//trans.SetWorldMatrix();
		//material.roughness = 1.0f;
		//material.metalness = 0.0f;
		//material.specular = 100.0f;
		//material.SetMaterial();
		//AssimpMeshDraw(context, createVs, createPs, _assimpMesh);

		// スカイドームにはシェーディング・一部ポストエフェクト以外のポストエフェクトを適用したくない
		// ポストエフェクトの適用有無を判別するためのマテリアルを用意する
		//Transform _trans;
		//_trans.sca.Set(10.0f);
		//_trans.SetWorldMatrix();
		//material.roughness = 1.0f;
		//material.metalness = 0.0f;
		//material.specular = 100.0f;
		//material.SetMaterial();
		//AssimpMeshDraw(context, createVs, createPs, skydome);


		// テクスチャの描画（ポストプロセス）はトライアングルストリップで描画する必要がある
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		Debug::BracketEnd();
	}

	// ポストエフェクトのON/OFFフラグ
	bool onControl = false;			// このフラグのみ毎回初期化する
	static bool shading = false;
	static bool rim = false;
	static bool outline = false;
	static bool gamma = false;
	static bool gausX = false;
	static bool gausY = false;
	static bool radial = false;
	static bool mosaic = false;

	// キー操作分岐用のフラグ切り替え
	onControl = DirectInput::GetKeyboardPress(DIK_LCONTROL) ? !onControl : onControl;

	// ON/OFFフラグの切り替え
	if (!onControl) {
		shading = DirectInput::GetKeyboardTrigger(DIK_F2) ? !shading : shading;
		rim = DirectInput::GetKeyboardTrigger(DIK_F3) ? !rim : rim;
		outline = DirectInput::GetKeyboardTrigger(DIK_F4) ? !outline : outline;
		gamma = DirectInput::GetKeyboardTrigger(DIK_F5) ? !gamma : gamma;
	}
	if (onControl) {
		gausX = DirectInput::GetKeyboardTrigger(DIK_F2) ? !gausX : gausX;
		gausY = DirectInput::GetKeyboardTrigger(DIK_F3) ? !gausY : gausY;
		radial = DirectInput::GetKeyboardTrigger(DIK_F4) ? !radial : radial;
		mosaic = DirectInput::GetKeyboardTrigger(DIK_F5) ? !mosaic : mosaic;
	}

	// 多分
	// レンダーターゲットビュー（書き込み先）が設定されているのに
	// シェーダーリソースビュー（読み込み先）も設定されているのがエラーの原因でした
	// 簡潔に言うと、書き込もうとしてるけど読み込もうともしてるからってことです。
	// DXGraphicBufferResultのレンダーターゲットビューを書き込み先にしているのに
	// DXGraphicBufferResultのシェーダーリソースビューを読み込んでいるから（SetGraphicBufferSRV関数内で）

	// シェーディング計算
	{
		auto context = DirectXManager::GetImmediateContext().Get();
		SetBackBufferSizeMesh();
		context->OMSetRenderTargets(1, DXGraphicBufferResult::GetWriteRenderTarget().GetRTV().GetAddressOf(), nullptr);
		shadingVs.IASetInputLayout(context);
		shadingVs.VSSetShader(context);
		if (shading) {
			//shadingPs.PSSetShader(context, "LambertAndPhongShading");
			//shadingPs.PSSetShader(context, "PhysicsBasedRendering");
			//shadingPs.PSSetShader(context, "DisneyBasePhysicsBasedRendering");
			shadingPs.PSSetShader(context, "TestPBR");
		}
		else {
			shadingPs.PSSetShader(context, "NoShading");
		}
		SetGraphicBufferSRV();
		context->Draw(4, 0);
	}

	// 読み書きするレンダーターゲットを切り替え
	DXGraphicBufferResult::SwitchRenderTarget();

	// リムライト（輪郭ライト）計算
	{
		auto context = DirectXManager::GetImmediateContext().Get();
		SetBackBufferSizeMesh();
		context->OMSetRenderTargets(1, DXGraphicBufferResult::GetWriteRenderTarget().GetRTV().GetAddressOf(), nullptr);
		rimVs.IASetInputLayout(context);
		rimVs.VSSetShader(context);
		if (rim) {
			rimPs.PSSetShader(context, "RimLight");
			//rimPs.PSSetShader(context, "TestRimLight");
		}
		else {
			rimPs.PSSetShader(context, "NoLight");
		}
		SetGraphicBufferSRV();
		context->VSSetShaderResources(6, 1, DXGraphicBufferResult::GetReadRenderTarget().GetSRV().GetAddressOf());
		context->PSSetShaderResources(6, 1, DXGraphicBufferResult::GetReadRenderTarget().GetSRV().GetAddressOf());
		context->Draw(4, 0);
		ShaderResourceView* nullView = nullptr;
		context->VSSetShaderResources(6, 1, &nullView);
		context->PSSetShaderResources(6, 1, &nullView);
	}

	DXGraphicBufferResult::SwitchRenderTarget();

	// トーンマッピングを行う（輝度テクスチャの作成）
	{
		auto context = DirectXManager::GetImmediateContext().Get();
		ShaderResourceView* nullView = nullptr;
		context->VSSetShaderResources(5, 1, &nullView);
		context->PSSetShaderResources(5, 1, &nullView);
		SetBackBufferSizeMesh();
		context->ClearRenderTargetView(DXGraphicBuffer::GetBrightnessRTV().Get(), DXColor::Clear().rgba.data());
		context->OMSetRenderTargets(1, DXGraphicBuffer::GetBrightnessRTV().GetAddressOf(), nullptr);
		tonemapVs.IASetInputLayout(context);
		tonemapVs.VSSetShader(context);
		tonemapPs.PSSetShader(context);
		context->VSSetShaderResources(6, 1, DXGraphicBufferResult::GetReadRenderTarget().GetSRV().GetAddressOf());
		context->PSSetShaderResources(6, 1, DXGraphicBufferResult::GetReadRenderTarget().GetSRV().GetAddressOf());
		context->Draw(4, 0);
	}



	// ガウスブラーの計算
	{

		// ブラー用のサンプラーを設定
		// これが必要な理由
		// デフォルトで使用されているサンプラーはテクスチャの範囲(0.0f～1.0f)の範囲外を参照した場合
		// 現在設定されているテクスチャを複製した者を参照する
		// そのためブラーをした際に参照したピクセルが[0.0f～1.0f]の範囲外だった場合にはテクスチャの対象となる座標を取得してしまう
		// 例：[U : 1.1f, V : 0.0f]のピクセルを参照した場合[U : 0.1f , V : 0.0f]のピクセルが使用される
		{
			DXSampler sampler;
			SamplerDesc desc;
			
			{
				ZeroMemory(&desc, sizeof(SamplerDesc));
				desc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;	// ブラーをかけるときにADDRESS_BORDERモードだと周りがボーダーカラーになってしまうのでADDRESS_MIRRORで反転させて端の色と同じ色でブラーが端まできれいにできるようなった
				desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;	// ブラーをかけるときにADDRESS_BORDERモードだと周りがボーダーカラーになってしまうのでADDRESS_MIRRORで反転させて端の色と同じ色でブラーが端まできれいにできるようなった
				desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;	// ブラーをかけるときにADDRESS_BORDERモードだと周りがボーダーカラーになってしまうのでADDRESS_MIRRORで反転させて端の色と同じ色でブラーが端まできれいにできるようなった
				//desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER;	// 以下で指定されている範囲外選択時の色を設定する
				//desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER;	// 以下で指定されている範囲外選択時の色を設定する
				//desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER;	// 以下で指定されている範囲外選択時の色を設定する
				desc.BorderColor[0] = 0.0f;	// D3D11_TEXTURE_ADDRESS_BORDER時に使用される色情報（R）
				desc.BorderColor[1] = 0.0f;	// D3D11_TEXTURE_ADDRESS_BORDER時に使用される色情報（G）
				desc.BorderColor[2] = 0.0f;	// D3D11_TEXTURE_ADDRESS_BORDER時に使用される色情報（B）
				desc.BorderColor[3] = 0.0f;	// D3D11_TEXTURE_ADDRESS_BORDER時に使用される色情報（A）
			}

			{
				auto device = DirectXManager::GetDevice().Get();
				auto context = DirectXManager::GetImmediateContext().Get();
				sampler.SetRegistNumber(0);
				sampler.Create(device, desc);
				sampler.VSSetSampler(context);
				sampler.PSSetSampler(context);
			}
		}

		// X方向ブラー
		{
			auto context = DirectXManager::GetImmediateContext().Get();
			SetBackBufferSizeMesh();
			context->OMSetRenderTargets(1, DXGraphicBufferResult::GetWriteRenderTarget().GetRTV().GetAddressOf(), nullptr);
			blurVs.IASetInputLayout(context);
			if (gausX) {
				blurVs.VSSetShader(context, "GausBlurX");
				blurPs.PSSetShader(context, "ApplyBlur");
			}
			else {
				blurVs.VSSetShader(context, "NoBlur");
				blurPs.PSSetShader(context, "NoBlur");
			}
			SetGraphicBufferSRV();
			context->VSSetShaderResources(6, 1, DXGraphicBufferResult::GetReadRenderTarget().GetSRV().GetAddressOf());
			context->PSSetShaderResources(6, 1, DXGraphicBufferResult::GetReadRenderTarget().GetSRV().GetAddressOf());
			context->Draw(4, 0);
			ShaderResourceView* nullView = nullptr;
			context->VSSetShaderResources(6, 1, &nullView);
			context->PSSetShaderResources(6, 1, &nullView);
		}

		DXGraphicBufferResult::SwitchRenderTarget();

		// Y方向ブラー
		{
			auto context = DirectXManager::GetImmediateContext().Get();
			SetBackBufferSizeMesh();
			context->OMSetRenderTargets(1, DXGraphicBufferResult::GetWriteRenderTarget().GetRTV().GetAddressOf(), nullptr);
			//context->OMSetRenderTargets(1, DXBackBuffer::GetRTV().GetAddressOf(), nullptr);
			blurVs.IASetInputLayout(context);
			if (gausY) {
				blurVs.VSSetShader(context, "GausBlurY");
				blurPs.PSSetShader(context, "ApplyBlur");
			}
			else {
				blurVs.VSSetShader(context, "NoBlur");
				blurPs.PSSetShader(context, "NoBlur");
			}
			SetGraphicBufferSRV();
			context->VSSetShaderResources(6, 1, DXGraphicBufferResult::GetReadRenderTarget().GetSRV().GetAddressOf());
			context->PSSetShaderResources(6, 1, DXGraphicBufferResult::GetReadRenderTarget().GetSRV().GetAddressOf());
			context->Draw(4, 0);
			ShaderResourceView* nullView = nullptr;
			context->VSSetShaderResources(6, 1, &nullView);
			context->PSSetShaderResources(6, 1, &nullView);
		}
	}

	DXGraphicBufferResult::SwitchRenderTarget();

	// 放射状ブラー
	{
		auto context = DirectXManager::GetImmediateContext().Get();
		SetBackBufferSizeMesh();
		context->OMSetRenderTargets(1, DXGraphicBufferResult::GetWriteRenderTarget().GetRTV().GetAddressOf(), nullptr);
		radialBlurVs.IASetInputLayout(context);
		radialBlurVs.VSSetShader(context);
		if (radial) {
			radialBlurPs.PSSetShader(context, "ApplyBlur");
		}
		else {
			radialBlurPs.PSSetShader(context, "NoBlur");
		}
		SetGraphicBufferSRV();
		context->VSSetShaderResources(6, 1, DXGraphicBufferResult::GetReadRenderTarget().GetSRV().GetAddressOf());
		context->PSSetShaderResources(6, 1, DXGraphicBufferResult::GetReadRenderTarget().GetSRV().GetAddressOf());
		context->Draw(4, 0);
		ShaderResourceView* nullView = nullptr;
		context->VSSetShaderResources(6, 1, &nullView);
		context->PSSetShaderResources(6, 1, &nullView);
	}

	DXGraphicBufferResult::SwitchRenderTarget();

	// 適当にラプラシアンフィルターの計算
	{
		auto context = DirectXManager::GetImmediateContext().Get();
		SetBackBufferSizeMesh();
		context->OMSetRenderTargets(1, DXGraphicBufferResult::GetWriteRenderTarget().GetRTV().GetAddressOf(), nullptr);
		outlineVs.IASetInputLayout(context);
		outlineVs.VSSetShader(context);
		if (outline) {
			outlinePs.PSSetShader(context, "LaplacianFilter");
		}
		else {
			outlinePs.PSSetShader(context, "NoFilter");
		}
		SetGraphicBufferSRV();
		context->VSSetShaderResources(6, 1, DXGraphicBufferResult::GetReadRenderTarget().GetSRV().GetAddressOf());
		context->PSSetShaderResources(6, 1, DXGraphicBufferResult::GetReadRenderTarget().GetSRV().GetAddressOf());
		context->Draw(4, 0);
		ShaderResourceView* nullView = nullptr;
		context->VSSetShaderResources(6, 1, &nullView);
		context->PSSetShaderResources(6, 1, &nullView);
	}

	DXGraphicBufferResult::SwitchRenderTarget();

	// ガンマ補正
	{
		//RenderBackBuffer(toGammaVs, toGammaPs);
		auto context = DirectXManager::GetImmediateContext().Get();
		SetBackBufferSizeMesh();
		context->OMSetRenderTargets(1, DXGraphicBufferResult::GetWriteRenderTarget().GetRTV().GetAddressOf(), nullptr);
		colorConvertVs.IASetInputLayout(context);
		colorConvertVs.VSSetShader(context);
		if (gamma) {
			colorConvertPs.PSSetShader(context, "GammaConvert");
		}
		else {
			colorConvertPs.PSSetShader(context, "NoConvert");
		}
		context->VSSetShaderResources(6, 1, DXGraphicBufferResult::GetReadRenderTarget().GetSRV().GetAddressOf());
		context->PSSetShaderResources(6, 1, DXGraphicBufferResult::GetReadRenderTarget().GetSRV().GetAddressOf());
		context->Draw(4, 0);
		ShaderResourceView* nullView = nullptr;
		context->VSSetShaderResources(6, 1, &nullView);
		context->PSSetShaderResources(6, 1, &nullView);
	}

	DXGraphicBufferResult::SwitchRenderTarget();

	{
		auto context = DirectXManager::GetImmediateContext().Get();
		SetBackBufferSizeMesh();
		context->OMSetRenderTargets(1, DXGraphicBufferResult::GetWriteRenderTarget().GetRTV().GetAddressOf(), nullptr);
		mosaicVs.IASetInputLayout(context);
		mosaicVs.VSSetShader(context);
		if (mosaic) {
			mosaicPs.PSSetShader(context, "MosaicFilter");
		}
		else {
			mosaicPs.PSSetShader(context, "NoFilter");
		}
		context->VSSetShaderResources(6, 1, DXGraphicBufferResult::GetReadRenderTarget().GetSRV().GetAddressOf());
		context->PSSetShaderResources(6, 1, DXGraphicBufferResult::GetReadRenderTarget().GetSRV().GetAddressOf());
		context->Draw(4, 0);
		ShaderResourceView* nullView = nullptr;
		context->VSSetShaderResources(6, 1, &nullView);
		context->PSSetShaderResources(6, 1, &nullView);
	}

	DXGraphicBufferResult::SwitchRenderTarget();

	// バックバッファに転送する
	{
		auto context = DirectXManager::GetImmediateContext().Get();
		SetBackBufferSizeMesh();
		context->OMSetRenderTargets(1, DXBackBuffer::GetRTV().GetAddressOf(), nullptr);
		sendVs.IASetInputLayout(context);
		sendVs.VSSetShader(context);
		sendPs.PSSetShader(context);
		context->VSSetShaderResources(6, 1, DXGraphicBufferResult::GetReadRenderTarget().GetSRV().GetAddressOf());
		context->PSSetShaderResources(6, 1, DXGraphicBufferResult::GetReadRenderTarget().GetSRV().GetAddressOf());
		{
			// ライトから見た深度テクスチャの描画
			//context->VSSetShaderResources(6, 1, MainDirectionalLight::GetRenderTarget().GetSRV().GetAddressOf());
			//context->PSSetShaderResources(6, 1, MainDirectionalLight::GetRenderTarget().GetSRV().GetAddressOf());
			//context->VSSetShaderResources(6, 1, MainDirectionalLight::GetDepthStencil().GetSRV().GetAddressOf());
			//context->PSSetShaderResources(6, 1, MainDirectionalLight::GetDepthStencil().GetSRV().GetAddressOf());
		}
		context->Draw(4, 0);
		ShaderResourceView* nullView = nullptr;
		context->VSSetShaderResources(6, 1, &nullView);
		context->PSSetShaderResources(6, 1, &nullView);
	}

	// 後処理
	{
		auto context = DirectXManager::GetImmediateContext().Get();
		DXGraphicBuffer::ClearRenderTargetView(context);
		DXGraphicBufferResult::ClearRenderTargetView(context);
		MainDirectionalLight::GetRenderTarget().Clear(context, DXColor::Clear());
		MainDirectionalLight::GetDepthStencil().Clear(context);
		SetGraphicBufferNull();
	}

	Debug::BracketEnd();
	Debug::SetLogSaveFlag(true);
	Debug::SetDispOnConsoleWindow(true);
	Debug::SetDispOnOutputWindow(true);
}



//===================================================//