
// STL
#include<array>

// TNLIB
#include"Debug.hpp"
#include"Color.hpp"

// MyFile
#include"RenderingExample.h"
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
#include"DeferredRenderingCamera.h"

// DXMath
#include"DirectXUtilityModule/DirectXVector.h"
#include"DirectXUtilityModule/DirectXMatrix.h"
#include"DirectXUtilityModule/DirectXColor.h"

// DirectXTexture
#include"DirectXSystemModule/Texture.h"

void RenderingTest() {

	Debug::SetDengerLogTimeStopFlag(false);
	Debug::SetDispOnOutputWindow(true);


	Debug::BracketBegin("RenderingTest Method");

	// 頂点シェーダー
	{
		// 通常の頂点シェーダー
		{
			DXVertexShader vs;
			std::array<InputElementDesc, 3> elements{ {
					InputElements::PositionElement,
					InputElements::NormalElement,
					InputElements::ColorElement,
			} };


			vs.SetFilePath("ShaderFile/main_vs.hlsl");// VC++ Projectファイルのディレクトリから見たhlslファイルのディレクトリ
			vs.SetEntryFuncName("main");					// hlslファイルに設定されているエントリ関数名
			vs.SetVersion("vs_5_0");						// hlslファイルに設定されているシェーダーモデルのバージョン
			vs.SetInputElements(elements.data(), elements.size());
			vs.Create(DirectXManager::GetDevice().Get());
			vs.VSSetShader(DirectXManager::GetImmediateContext().Get());
			vs.IASetInputLayout(DirectXManager::GetImmediateContext().Get());

			Debug::Parse::AllLog();
		}

		// 動的シェーダーリンクに対応した頂点シェーダー
		{
			DXVertexShaderLink vs;
			std::array<InputElementDesc, 3> elements{ {
					InputElements::PositionElement,
					InputElements::NormalElement,
					InputElements::ColorElement,
			} };


			vs.SetFilePath("ShaderFile/DynamicShaderLinkage_vs.hlsl");	// VC++ Projectファイルのディレクトリから見たhlslファイルのディレクトリ
			vs.SetEntryFuncName("main");											// hlslファイルに設定されているエントリ関数名
			vs.SetVersion("vs_5_0");												// hlslファイルに設定されているシェーダーモデルのバージョン
			vs.SetInputElements(elements.data(), elements.size());
			vs.Create(DirectXManager::GetDevice().Get());
			vs.CreateClassInstance("VSRed");
			vs.CreateClassInstance("VSGreen");
			vs.VSSetShader(DirectXManager::GetImmediateContext().Get(), "VSRed");
			vs.VSSetShader(DirectXManager::GetImmediateContext().Get(), "VSGreen");
			vs.IASetInputLayout(DirectXManager::GetImmediateContext().Get());

			Debug::Parse::AllLog();
		}
	}

	// ピクセルシェーダー
	{
		// 通常のピクセルシェーダー
		{
			DXPixelShader ps;

			ps.SetFilePath("ShaderFile/main_ps.hlsl");
			ps.SetEntryFuncName("main");
			ps.SetVersion("ps_5_0");
			ps.Create(DirectXManager::GetDevice().Get());
			ps.PSSetShader(DirectXManager::GetImmediateContext().Get());

			Debug::Parse::AllLog();
		}

		// 動的シェーダーリンクに対応したピクセルシェーダー
		{
			DXPixelShaderLink ps;

			ps.SetFilePath("ShaderFile/DynamicShaderLinkage_ps.hlsl");
			ps.SetEntryFuncName("main");
			ps.SetVersion("ps_5_0");
			ps.Create(DirectXManager::GetDevice().Get());
			ps.CreateClassInstance("PSRed");
			ps.CreateClassInstance("PSGreen");
			ps.PSSetShader(DirectXManager::GetImmediateContext().Get(), "PSRed");
			ps.PSSetShader(DirectXManager::GetImmediateContext().Get(), "PSGreen");

			Debug::Parse::AllLog();
		}
	}

	// 定数バッファ
	{
		// MapUnmap
		{
			DXMatrix4x4 mat;
			DXConstantBuffer cb;

			cb.SetRegisterNumber(0);
			cb.SetBufferSize(mat.MemSize());
			cb.CreateMapUnmap(DirectXManager::GetDevice().Get());
			cb.Update(DirectXManager::GetImmediateContext().Get(), reinterpret_cast<void*>(&mat)); // reinterpret_castしなくても使用は可能
		}

		// UpdateSubResource
		{
			DXMatrix4x4 mat;
			DXConstantBuffer cb;

			cb.SetRegisterNumber(0);
			cb.SetBufferSize(mat.MemSize());
			cb.CreateUpdateSubResource(DirectXManager::GetDevice().Get());
			cb.Update(DirectXManager::GetImmediateContext().Get(), reinterpret_cast<void*>(&mat));
		}
	}

	// 頂点バッファ
	{
		struct Vertex {
			DXVector3 position;
			DXVector3 normal;
			DXVector4 color;
		};

		std::array<Vertex, 10> vertices;
		for (int_t i = 0; i < 10; i++) {
			vertices[i].position(0.0f);
			vertices[i].normal(0.0f);
			vertices[i].color(0.0f);
		}

		DXVertexBuffer vb;
		vb.SetStride(sizeof(Vertex));
		vb.SetQuantity(vertices.size());
		vb.Create(DirectXManager::GetDevice().Get(), vertices.data());
		vb.IASetVertexBuffer(DirectXManager::GetImmediateContext().Get());
	}

	// インデックスバッファ
	{
		std::array<uint_t, 10> indices;
		for (int_t i = 0; i < 10; i++) indices[i] = i;

		DXIndexBuffer ib;
		ib.SetQuantity(10);
		ib.Create(DirectXManager::GetDevice().Get(), indices.data());
		ib.IASetIndexBuffer(DirectXManager::GetImmediateContext().Get());
	}

	Debug::BracketEnd();






}

void RenderingTriangle() {
	Debug::SetDengerLogTimeStopFlag(false);
	Debug::SetDispOnOutputWindow(false);
	Debug::SetDispOnConsoleWindow(false);
	//Debug::SetLogSaveFlag(false);

	Debug::BracketBegin("Rendering Pass");

	static DXVertexShader vs;
	static DXPixelShader ps;

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
				MainCamera::Initialize();
			}

			// 頂点シェーダーの初期化
			{
				std::array<InputElementDesc, 4> elements{ {
						InputElements::PositionElement,
						InputElements::NormalElement,
						InputElements::TexCoordElement,
						InputElements::ColorElement,
				} };

				vs.SetFilePath("ShaderFile/main_vs.hlsl");// VC++ Projectファイルのディレクトリから見たhlslファイルのディレクトリ
				vs.SetEntryFuncName("main");					// hlslファイルに設定されているエントリ関数名
				vs.SetVersion("vs_5_0");						// hlslファイルに設定されているシェーダーモデルのバージョン
				vs.SetInputElements(elements.data(), elements.size());
				vs.Create(DirectXManager::GetDevice().Get());
				vs.VSSetShader(DirectXManager::GetImmediateContext().Get());
				vs.IASetInputLayout(DirectXManager::GetImmediateContext().Get());
			}

			// ピクセルシェーダーの初期化
			{
				ps.SetFilePath("ShaderFile/main_ps.hlsl");
				ps.SetEntryFuncName("main");
				ps.SetVersion("ps_5_0");
				ps.Create(DirectXManager::GetDevice().Get());
				ps.PSSetShader(DirectXManager::GetImmediateContext().Get());
			}

			initialize = true;

			Debug::BracketEnd();

		}
	}

	// Rendering PreProcess
	{
		DirectXDeviceContext* context = DirectXManager::GetImmediateContext().Get();
		MainCamera::SetViewMatrixConstantBuffer(context);
		MainCamera::SetPerspectiveProjectionMatrixConstantBuffer(context);
		MainCamera::VSSetConstantBuffer(context);
		MainCamera::PSSetConstantBuffer(context);

		static float_t x = 1.0f;
		x += 0.01f;

		MainCamera::GetCamera().SetZoom(1.0);
		MainCamera::GetCamera().SetPosition(DXVector3(0.0f, +1.0f, -5.0f));
		MainCamera::GetCamera().SetViewDirection(DXVector3(0.0f, 0.0f, +1.0f));
		MainCamera::GetCamera().UpdateViewMatrix();
		MainCamera::GetCamera().UpdateProjectionMatrix();

		{
			static DXVector3 vec;
			static DXVector2 dir;
			if (DirectInput::GetKeyboardPress(DIK_W)) {
				vec.y += 0.01f;
			}
			if (DirectInput::GetKeyboardPress(DIK_S)) {
				vec.y -= 0.01f;
			}
			if (DirectInput::GetKeyboardPress(DIK_D)) {
				vec.x += 0.01f;
			}
			if (DirectInput::GetKeyboardPress(DIK_A)) {
				vec.x -= 0.01f;
			}
			if (DirectInput::GetKeyboardPress(DIK_Q)) {
				vec.z += 0.01f;
			}
			if (DirectInput::GetKeyboardPress(DIK_E)) {
				vec.z -= 0.01f;
			}
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
			MainCamera::GetCamera().SetRelativePosition(vec);
			MainCamera::GetCamera().SetRelativeViewDirection(dir.ToVector3());
			MainCamera::GetCamera().UpdateViewMatrix();
		}

		CBWorld::SetWorldMatrix(
			Math::Matrix::Affine::Scaling(DXVector3(2, 2, 2))
			*
			Math::Matrix::Affine::RotationAll(DXVector3(0, 0, 0), Math::RotationOrder::XYZ)
			*
			Math::Matrix::Affine::Transform(DXVector3(0, 0, 0))
		);
		CBWorld::UpdateConstantBuffer(context);
		CBWorld::VSSetConstantBuffer(context);
		CBWorld::PSSetConstantBuffer(context);

		context->OMSetRenderTargets(1, DXBackBuffer::GetRTV().GetAddressOf(), DXDepthStencilDefault::GetDSV().Get());
		context->RSSetState(DirectXManager::GetRasterizerState().Get());
		vs.VSSetShader(DirectXManager::GetImmediateContext().Get());
		vs.IASetInputLayout(DirectXManager::GetImmediateContext().Get());
		ps.PSSetShader(DirectXManager::GetImmediateContext().Get());
		DirectXManager::GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		DXBlendContainer::GetAlphaBlend().OMSetBlendState(context);
	}

	// Rendering
	{
		struct Vertex {
			DXVector3 pos;
			DXVector3 normal;
			DXVector2 uv;
			DXVector4 color;
		};

		struct Mesh {
			DXVertexBuffer m_buf;
			std::array<Vertex, 4> m_vertices;
		};

		Mesh mesh;
		DXColor color = DXColor(Math::Random01(), Math::Random01(), Math::Random01(), Math::Random01());
		mesh.m_vertices[0].pos.Set(-1.0f, +1.0f, 0.0f);
		mesh.m_vertices[1].pos.Set(+1.0f, +1.0f, 0.0f);
		mesh.m_vertices[2].pos.Set(-1.0f, -1.0f, 0.0f);
		mesh.m_vertices[3].pos.Set(+1.0f, -1.0f, 0.0f);
		mesh.m_vertices[0].uv.Set(0.0f, 0.0f);
		mesh.m_vertices[1].uv.Set(1.0f, 0.0f);
		mesh.m_vertices[2].uv.Set(0.0f, 1.0f);
		mesh.m_vertices[3].uv.Set(1.0f, 1.0f);
		mesh.m_vertices[0].color.vec = DXColor(Math::Random01(), Math::Random01(), Math::Random01(), Math::Random01()).rgba;
		mesh.m_vertices[1].color.vec = DXColor(Math::Random01(), Math::Random01(), Math::Random01(), Math::Random01()).rgba;
		mesh.m_vertices[2].color.vec = DXColor(Math::Random01(), Math::Random01(), Math::Random01(), Math::Random01()).rgba;
		mesh.m_vertices[3].color.vec = DXColor(Math::Random01(), Math::Random01(), Math::Random01(), Math::Random01()).rgba;
		mesh.m_buf.SetQuantity(4);
		mesh.m_buf.SetOffset(0);
		mesh.m_buf.SetStride(sizeof(Vertex));
		mesh.m_buf.Create(DirectXManager::GetDevice().Get(), mesh.m_vertices.data());
		mesh.m_buf.UpdateVertexBuffer(DirectXManager::GetImmediateContext().Get(), mesh.m_vertices.data());
		mesh.m_buf.IASetVertexBuffer(DirectXManager::GetImmediateContext().Get());
		DirectXManager::GetImmediateContext()->Draw(mesh.m_vertices.size(), 0);
	}

	// Rendering PostProcess
	{
		// フレームフリップ
		//DirectXManager::GetSwapChain()->Present(0, 0);
	}

	Debug::BracketEnd();
	Debug::SetLogSaveFlag(true);
	Debug::SetDispOnConsoleWindow(true);
	Debug::SetDispOnOutputWindow(true);
}

void RenderingTexture() {
	Debug::SetDengerLogTimeStopFlag(false);
	Debug::SetDispOnOutputWindow(false);
	Debug::SetDispOnConsoleWindow(false);
	//Debug::SetLogSaveFlag(false);

	Debug::BracketBegin("Rendering Pass");

	static DXVertexShader vs;
	static DXPixelShader ps;
	//static DXTexture tex;
	static DXShaderResourceView tex;

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
				MainCamera::Initialize();
			}

			// 頂点シェーダーの初期化
			{
				std::array<InputElementDesc, 4> elements{ {
						InputElements::PositionElement,
						InputElements::NormalElement,
						InputElements::TexCoordElement,
						InputElements::ColorElement,
				} };

				vs.SetFilePath("ShaderFile/Texture_vs.hlsl");// VC++ Projectファイルのディレクトリから見たhlslファイルのディレクトリ
				vs.SetEntryFuncName("main");					// hlslファイルに設定されているエントリ関数名
				vs.SetVersion("vs_5_0");						// hlslファイルに設定されているシェーダーモデルのバージョン
				vs.SetInputElements(elements.data(), elements.size());
				vs.Create(DirectXManager::GetDevice().Get());
				vs.VSSetShader(DirectXManager::GetImmediateContext().Get());
				vs.IASetInputLayout(DirectXManager::GetImmediateContext().Get());
			}

			// ピクセルシェーダーの初期化
			{
				ps.SetFilePath("ShaderFile/Texture_ps.hlsl");
				ps.SetEntryFuncName("main");
				ps.SetVersion("ps_5_0");
				ps.Create(DirectXManager::GetDevice().Get());
				ps.PSSetShader(DirectXManager::GetImmediateContext().Get());
			}

			// テクスチャの作成
			{
				auto device = DirectXManager::GetDevice().Get();
				auto context = DirectXManager::GetImmediateContext().Get();
				tex.SetFilePath("Assets/Example.png");
				tex.SetRegistNumber(0);
				tex.CreateTextureFromFile(device, context);
			}

			initialize = true;

			Debug::BracketEnd();

		}
	}

	// Rendering PreProcess
	{
		DirectXDeviceContext* context = DirectXManager::GetImmediateContext().Get();
		MainCamera::SetViewMatrixConstantBuffer(context);
		MainCamera::SetPerspectiveProjectionMatrixConstantBuffer(context);
		MainCamera::VSSetConstantBuffer(context);
		MainCamera::PSSetConstantBuffer(context);

		static float_t x = 1.0f;
		x += 0.01f;

		MainCamera::GetCamera().SetZoom(1.0);
		MainCamera::GetCamera().SetPosition(DXVector3(0.0f, +1.0f, -5.0f));
		MainCamera::GetCamera().SetViewDirection(DXVector3(0.0f, 0.0f, +1.0f));
		MainCamera::GetCamera().UpdateViewMatrix();
		MainCamera::GetCamera().UpdateProjectionMatrix();

		{
			static DXVector3 vec;
			static DXVector2 dir;
			if (DirectInput::GetKeyboardPress(DIK_W)) {
				vec.y += 0.01f;
			}
			if (DirectInput::GetKeyboardPress(DIK_S)) {
				vec.y -= 0.01f;
			}
			if (DirectInput::GetKeyboardPress(DIK_D)) {
				vec.x += 0.01f;
			}
			if (DirectInput::GetKeyboardPress(DIK_A)) {
				vec.x -= 0.01f;
			}
			if (DirectInput::GetKeyboardPress(DIK_Q)) {
				vec.z += 0.01f;
			}
			if (DirectInput::GetKeyboardPress(DIK_E)) {
				vec.z -= 0.01f;
			}
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
			MainCamera::GetCamera().SetRelativePosition(vec);
			MainCamera::GetCamera().SetRelativeViewDirection(dir.ToVector3());
			MainCamera::GetCamera().UpdateViewMatrix();
		}

		static float_t y = 0.0f;
		y += 0.1f;

		CBWorld::SetWorldMatrix(
			Math::Matrix::Affine::Scaling(DXVector3(2, 2, 2))
			*
			Math::Matrix::Affine::RotationAll(DXVector3(0, y, 0), Math::RotationOrder::XYZ)
			*
			Math::Matrix::Affine::Transform(DXVector3(0, 0, 0))
		);
		CBWorld::UpdateConstantBuffer(context);
		CBWorld::VSSetConstantBuffer(context);
		CBWorld::PSSetConstantBuffer(context);

		context->OMSetRenderTargets(1, DXBackBuffer::GetRTV().GetAddressOf(), nullptr);
		context->RSSetState(DirectXManager::GetRasterizerState().Get());
		vs.VSSetShader(DirectXManager::GetImmediateContext().Get());
		vs.IASetInputLayout(DirectXManager::GetImmediateContext().Get());
		ps.PSSetShader(DirectXManager::GetImmediateContext().Get());
		DirectXManager::GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		DXSamplerContainer::GetLinearSampler().PSSetSampler(context);
		DXBlendContainer::GetAlphaBlend().OMSetBlendState(context);
	}

	// Rendering
	{
		struct Vertex {
			DXVector3 pos;
			DXVector3 normal;
			DXVector2 uv;
			DXColor color;
		};

		struct Mesh {
			DXVertexBuffer m_buf;
			std::array<Vertex, 4> m_vertices;
		};

		Mesh mesh;
		DXColor color = DXColor(Math::Random01(), Math::Random01(), Math::Random01(), Math::Random01());
		mesh.m_vertices[0].pos.Set(-1.0f, +1.0f, 0.0f);
		mesh.m_vertices[1].pos.Set(+1.0f, +1.0f, 0.0f);
		mesh.m_vertices[2].pos.Set(-1.0f, -1.0f, 0.0f);
		mesh.m_vertices[3].pos.Set(+1.0f, -1.0f, 0.0f);
		mesh.m_vertices[0].uv.Set(0.0f, 0.0f);
		mesh.m_vertices[1].uv.Set(1.0f, 0.0f);
		mesh.m_vertices[2].uv.Set(0.0f, 1.0f);
		mesh.m_vertices[3].uv.Set(1.0f, 1.0f);
		mesh.m_vertices[0].color.Set(DXColor::Red());
		mesh.m_vertices[1].color.Set(DXColor::Green());
		mesh.m_vertices[2].color.Set(DXColor::Green());
		mesh.m_vertices[3].color.Set(DXColor::Green());
		mesh.m_buf.SetQuantity(4);
		mesh.m_buf.SetOffset(0);
		mesh.m_buf.SetStride(sizeof(Vertex));
		mesh.m_buf.Create(DirectXManager::GetDevice().Get(), mesh.m_vertices.data());
		mesh.m_buf.UpdateVertexBuffer(DirectXManager::GetImmediateContext().Get(), mesh.m_vertices.data());
		mesh.m_buf.IASetVertexBuffer(DirectXManager::GetImmediateContext().Get());
		tex.SetRegistNumber(0);
		tex.VSSetShaderResources(DirectXManager::GetImmediateContext().Get());
		tex.PSSetShaderResources(DirectXManager::GetImmediateContext().Get());
		DirectXManager::GetImmediateContext()->Draw(mesh.m_vertices.size(), 0);
	}

	Debug::BracketEnd();
	Debug::SetLogSaveFlag(true);
	Debug::SetDispOnConsoleWindow(true);
	Debug::SetDispOnOutputWindow(true);
}

void DeferredRendering() {
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
			}

			// 頂点シェーダーの初期化
			{
				std::array<InputElementDesc, 4> elements{ {
						InputElements::PositionElement,
						InputElements::NormalElement,
						InputElements::TexCoordElement,
						InputElements::ColorElement,
				} };

				createVs.SetFilePath("ShaderFile/CreateDeferredTexture_vs.hlsl");// VC++ Projectファイルのディレクトリから見たhlslファイルのディレクトリ
				createVs.SetEntryFuncName("main");					// hlslファイルに設定されているエントリ関数名
				createVs.SetVersion("vs_5_0");						// hlslファイルに設定されているシェーダーモデルのバージョン
				createVs.SetInputElements(elements.data(), elements.size());
				createVs.Create(DirectXManager::GetDevice().Get());
				createVs.VSSetShader(DirectXManager::GetImmediateContext().Get());
				createVs.IASetInputLayout(DirectXManager::GetImmediateContext().Get());
			}

			// ピクセルシェーダーの初期化
			{
				createPs.SetFilePath("ShaderFile/CreateDeferredTexture_ps.hlsl");
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

				loadVs.SetFilePath("ShaderFile/LoadDeferredTexture_vs.hlsl");// VC++ Projectファイルのディレクトリから見たhlslファイルのディレクトリ
				loadVs.SetEntryFuncName("main");					// hlslファイルに設定されているエントリ関数名
				loadVs.SetVersion("vs_5_0");						// hlslファイルに設定されているシェーダーモデルのバージョン
				loadVs.SetInputElements(elements.data(), elements.size());
				loadVs.Create(DirectXManager::GetDevice().Get());
				loadVs.VSSetShader(DirectXManager::GetImmediateContext().Get());
				loadVs.IASetInputLayout(DirectXManager::GetImmediateContext().Get());
			}

			// ピクセルシェーダーの初期化
			{
				loadPs.SetFilePath("ShaderFile/LoadDeferredTexture_ps.hlsl");
				loadPs.SetEntryFuncName("main");
				loadPs.SetVersion("ps_5_0");
				loadPs.Create(DirectXManager::GetDevice().Get());
				loadPs.PSSetShader(DirectXManager::GetImmediateContext().Get());
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

		MainCamera::GetCamera().SetZoom(1.0);
		MainCamera::GetCamera().SetPosition(DXVector3(0.0f, +1.0f, -5.0f));
		MainCamera::GetCamera().SetViewDirection(DXVector3(0.0f, 0.0f, +1.0f));
		MainCamera::GetCamera().UpdateViewMatrix();
		MainCamera::GetCamera().UpdateProjectionMatrix();

		{
			static DXVector3 vec;
			static DXVector2 dir;
			if (DirectInput::GetKeyboardPress(DIK_W)) {
				vec.y += 0.01f;
			}
			if (DirectInput::GetKeyboardPress(DIK_S)) {
				vec.y -= 0.01f;
			}
			if (DirectInput::GetKeyboardPress(DIK_D)) {
				vec.x += 0.01f;
			}
			if (DirectInput::GetKeyboardPress(DIK_A)) {
				vec.x -= 0.01f;
			}
			if (DirectInput::GetKeyboardPress(DIK_Q)) {
				vec.z += 0.01f;
			}
			if (DirectInput::GetKeyboardPress(DIK_E)) {
				vec.z -= 0.01f;
			}
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
			MainCamera::GetCamera().SetRelativePosition(vec);
			MainCamera::GetCamera().SetRelativeViewDirection(dir.ToVector3());
			MainCamera::GetCamera().UpdateViewMatrix();
		}

		static float_t y = 0.0f;
		y += 0.1f;

		CBWorld::SetWorldMatrix(
			Math::Matrix::Affine::Scaling(DXVector3(2, 2, 2))
			*
			Math::Matrix::Affine::RotationAll(DXVector3(0, y, 0), Math::RotationOrder::XYZ)
			*
			Math::Matrix::Affine::Transform(DXVector3(0, 0, 0))
		);
		CBWorld::UpdateConstantBuffer(context);
		CBWorld::VSSetConstantBuffer(context);
		CBWorld::PSSetConstantBuffer(context);

		DXDepthStencilDefault::DepthBufferON(context);
		auto rtvs = DXGraphicBuffer::GetRTVs();
		context->OMSetRenderTargets(rtvs.size(), rtvs.data(), DXDepthStencilDefault::GetDSV().Get());
		context->RSSetState(DirectXManager::GetRasterizerState().Get());
		createVs.VSSetShader(context);
		createVs.IASetInputLayout(context);
		createPs.PSSetShader(context);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		DXSamplerContainer::GetLinearSampler().PSSetSampler(context);
		DXBlendContainer::GetAlphaBlend().OMSetBlendState(context);

		Debug::BracketEnd();
	}


	// DeferredTextureCreatePass
	{
		Debug::BracketBegin("CreateDeferredTexture");

		struct Vertex {
			DXVector3 pos;
			DXVector3 normal;
			DXVector2 uv;
			DXColor color;
		};

		struct Mesh {
			DXVertexBuffer m_buf;
			std::array<Vertex, 4> m_vertices;
		};

		Mesh mesh;
		DXColor color = DXColor(Math::Random01(), Math::Random01(), Math::Random01(), Math::Random01());
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
		mesh.m_vertices[0].color.Set(DXColor::Blue());
		mesh.m_vertices[1].color.Set(DXColor::Blue());
		mesh.m_vertices[2].color.Set(DXColor::Blue());
		mesh.m_vertices[3].color.Set(DXColor::Blue());
		mesh.m_buf.SetQuantity(4);
		mesh.m_buf.SetOffset(0);
		mesh.m_buf.SetStride(sizeof(Vertex));
		mesh.m_buf.Create(DirectXManager::GetDevice().Get(), mesh.m_vertices.data());
		mesh.m_buf.UpdateVertexBuffer(DirectXManager::GetImmediateContext().Get(), mesh.m_vertices.data());
		mesh.m_buf.IASetVertexBuffer(DirectXManager::GetImmediateContext().Get());
		DirectXManager::GetImmediateContext()->Draw(mesh.m_vertices.size(), 0);

		Debug::BracketEnd();
	}

	// DeferredTextureCreatePass
	{
		Debug::BracketBegin("CreateDeferredTexture");

		auto context = DirectXManager::GetImmediateContext().Get();
		static float_t x = 0.0f;
		x += 0.1f;

		CBWorld::SetWorldMatrix(
			Math::Matrix::Affine::Scaling(DXVector3(2, 2, 2))
			*
			Math::Matrix::Affine::RotationAll(DXVector3(x, 0, 0), Math::RotationOrder::XYZ)
			*
			Math::Matrix::Affine::Transform(DXVector3(0, 0, 0))
		);
		CBWorld::UpdateConstantBuffer(context);
		CBWorld::VSSetConstantBuffer(context);
		CBWorld::PSSetConstantBuffer(context);

		struct Vertex {
			DXVector3 pos;
			DXVector3 normal;
			DXVector2 uv;
			DXColor color;
		};

		struct Mesh {
			DXVertexBuffer m_buf;
			std::array<Vertex, 4> m_vertices;
		};

		Mesh mesh;
		DXColor color = DXColor(Math::Random01(), Math::Random01(), Math::Random01(), Math::Random01());
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
		mesh.m_vertices[0].color.Set(DXColor::Red());
		mesh.m_vertices[1].color.Set(DXColor::Red());
		mesh.m_vertices[2].color.Set(DXColor::Red());
		mesh.m_vertices[3].color.Set(DXColor::Red());
		mesh.m_buf.SetQuantity(4);
		mesh.m_buf.SetOffset(0);
		mesh.m_buf.SetStride(sizeof(Vertex));
		mesh.m_buf.Create(DirectXManager::GetDevice().Get(), mesh.m_vertices.data());
		mesh.m_buf.UpdateVertexBuffer(DirectXManager::GetImmediateContext().Get(), mesh.m_vertices.data());
		mesh.m_buf.IASetVertexBuffer(DirectXManager::GetImmediateContext().Get());
		DirectXManager::GetImmediateContext()->Draw(mesh.m_vertices.size(), 0);

		Debug::BracketEnd();
	}

	// RenderingBackBufferPass
	{
		Debug::BracketBegin("BackBufferWriting");

		struct Vertex {
			DXVector3 pos;
			DXVector2 uv;
		};

		struct Mesh {
			DXVertexBuffer m_buf;
			std::array<Vertex, 4> m_vertices;
		};

		Mesh mesh;
		DXColor color = DXColor(Math::Random01(), Math::Random01(), Math::Random01(), Math::Random01());
		DXVector2 bfsize = Application::GetWindowSize();
		//mesh.m_vertices[0].pos.Set(-bfsize.x, +bfsize.y, 0.0f);
		//mesh.m_vertices[1].pos.Set(+bfsize.x, +bfsize.y, 0.0f);
		//mesh.m_vertices[2].pos.Set(-bfsize.x, -bfsize.y, 0.0f);
		//mesh.m_vertices[3].pos.Set(+bfsize.x, -bfsize.y, 0.0f);
		mesh.m_vertices[0].pos.Set(-1.0f, +1.0f, 0.0f);
		mesh.m_vertices[1].pos.Set(+1.0f, +1.0f, 0.0f);
		mesh.m_vertices[2].pos.Set(-1.0f, -1.0f, 0.0f);
		mesh.m_vertices[3].pos.Set(+1.0f, -1.0f, 0.0f);
		mesh.m_vertices[0].uv.Set(0.0f, 0.0f);
		mesh.m_vertices[1].uv.Set(1.0f, 0.0f);
		mesh.m_vertices[2].uv.Set(0.0f, 1.0f);
		mesh.m_vertices[3].uv.Set(1.0f, 1.0f);
		mesh.m_buf.SetQuantity(4);
		mesh.m_buf.SetOffset(0);
		mesh.m_buf.SetStride(sizeof(Vertex));
		mesh.m_buf.Create(DirectXManager::GetDevice().Get(), mesh.m_vertices.data());
		mesh.m_buf.UpdateVertexBuffer(DirectXManager::GetImmediateContext().Get(), mesh.m_vertices.data());
		mesh.m_buf.IASetVertexBuffer(DirectXManager::GetImmediateContext().Get());

		{
			bool onshift = false;
			static float bias = 0.5f;
			if (DirectInput::GetKeyboardPress(DIK_LSHIFT)) {
				onshift = true;
			}
			if (DirectInput::GetKeyboardPress(DIK_1)) {
				bias += (onshift) ? 0.005f : 0.001f;
			}
			if (DirectInput::GetKeyboardPress(DIK_2)) {
				bias -= (onshift) ? 0.005f : 0.001f;
			}
			bfsize *= bias;

			Debug::SetDispOnOutputWindow(true);
			Debug::SetDispOnConsoleWindow(true);
			Debug::SetLogSaveFlag(true);
			Debug::Log("Bias : ", bias);
			Debug::Log("Size : ", bfsize);
			Debug::SetDispOnOutputWindow(false);
			Debug::SetDispOnConsoleWindow(false);
			Debug::SetLogSaveFlag(false);
		}

		auto context = DirectXManager::GetImmediateContext().Get();
		CBWorld::SetWorldMatrix(
			Math::Matrix::Affine::Scaling(DXVector3(bfsize.x, bfsize.y, 1))
			*
			Math::Matrix::Affine::RotationAll(DXVector3(0, 0, 0), Math::RotationOrder::XYZ)
			*
			Math::Matrix::Affine::Transform(DXVector3(0, 0, 0))
		);
		CBWorld::UpdateConstantBuffer(context);
		CBWorld::VSSetConstantBuffer(context);
		CBWorld::PSSetConstantBuffer(context);
		DeferredRenderingCamera::SetViewMatrixConstantBuffer(context);
		DeferredRenderingCamera::SetOrthoProjectionMatrixConstantBuffer(context);
		DeferredRenderingCamera::VSSetConstantBuffer(context);
		DeferredRenderingCamera::PSSetConstantBuffer(context);
		//MainCamera::SetOrthoProjectionMatrixConstantBuffer(context);
		context->OMSetRenderTargets(1, DXBackBuffer::GetRTV().GetAddressOf(), nullptr);
		loadVs.VSSetShader(context);
		loadVs.IASetInputLayout(context);
		loadPs.PSSetShader(context);
		context->PSSetShaderResources(0, 1, DXGraphicBuffer::GetAlbedoSRV().GetAddressOf());
		context->PSSetShaderResources(1, 1, DXGraphicBuffer::GetNormalSRV().GetAddressOf());
		context->PSSetShaderResources(2, 1, DXGraphicBuffer::GetPositionSRV().GetAddressOf());
		context->PSSetShaderResources(3, 1, DXGraphicBuffer::GetDepthSRV().GetAddressOf());
		context->Draw(4, 0);

		Debug::BracketEnd();
	}

	// PostProcess
	{
		auto context = DirectXManager::GetImmediateContext().Get();
		DXGraphicBuffer::ClearRenderTargetView(context);
		ShaderResourceView* nullView = nullptr;
		context->PSSetShaderResources(0, 1, &nullView);	// 空のリソースを設定しなおす
		context->PSSetShaderResources(1, 1, &nullView);	// 空のリソースを設定しなおす
		context->PSSetShaderResources(2, 1, &nullView);	// 空のリソースを設定しなおす
		context->PSSetShaderResources(3, 1, &nullView);	// 空のリソースを設定しなおす
	}

	Debug::BracketEnd();
	Debug::SetLogSaveFlag(true);
	Debug::SetDispOnConsoleWindow(true);
	Debug::SetDispOnOutputWindow(true);
}



void Example() {
	Debug::SetDengerLogTimeStopFlag(false);
	Debug::SetDispOnOutputWindow(true);
	Debug::SetDispOnConsoleWindow(false);

	Debug::BracketBegin("Example");

	{
		DXColor color;
		color = DXColor::Green();

		DXVector2 vec2;
		vec2 = DXVector2::PositiveInfinity();
	}

	Debug::BracketEnd();
	Debug::SetLogSaveFlag(true);
}
