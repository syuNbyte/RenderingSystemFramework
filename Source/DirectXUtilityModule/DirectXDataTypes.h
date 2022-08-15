#pragma once

// DirectX��Windows�֘A�̃V�X�e���Ɏg�p����W���w�b�_�t�@�C���̃C���N���[�h
#include<d3d11.h>				// DirectX11��API�������Ă���
#include<d3dcompiler.h>		// �V�F�[�_�[�̃R���p�C��
#include<dxgidebug.h>		// IDXGIDebug���g�p���邽��
#include<wrl/client.h>		// �f�o�C�X�̊Ǘ������₷�����邽�߂�DirectX�ł̃X�}�[�g�|�C���^�̂悤�Ȃ��́@��Microsoft::ComPtr : Microsoft::ComponentObjectPointer�̗�
#include<DirectXMath.h>		// XMFLOAT�n�̃f�[�^�^���g�p���邽��

//DirectXAPI�֌W�̃����N
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// ComPtr(DirectX�ł̃X�}�[�g�|�C���^)
using Microsoft::WRL::ComPtr;

// XMFLOAT�n�̃f�[�^�^���y�ɒ�`���邽��
using namespace DirectX;

// ComPtr���g�p�����I�u�W�F�N�g�̃G�C���A�X�錾���s���Ă���
namespace ComObject {

//IDXGI�n
	using IDxgiAdapter = ComPtr<IDXGIAdapter>;								// �A�_�v�^
	using IDxgiAdapter_1 = ComPtr<IDXGIAdapter1>;							// �A�_�v�^1
	using IDxgiFactory = ComPtr<IDXGIFactory>;								// �t�@�N�g���[
	using IDxgiFactory_1 = ComPtr<IDXGIFactory1>;							// �t�@�N�g���[1
	using IDxgiOutput = ComPtr<IDXGIOutput>;									// �A�E�g�v�b�g
	using SwapChain = ComPtr<IDXGISwapChain>;									// �X���b�v�`�F�C��
	using IDxgiDebug = ComPtr<IDXGIDebug>;										// �f�o�b�O�p�I�u�W�F�N�g
	using IDxgiDebug_1 = ComPtr<IDXGIDebug1>;									// �f�o�b�O�p�I�u�W�F�N�g�̊g���ŁiIDXGIDebug�^���p�����Ă���j

//ID3D11�n
	using DirectXDebug = ComPtr<ID3D11Debug>;									// DirectX11�̃f�o�b�O���s���I�u�W�F�N�g�@���o�O���N�������ۂɃ��O���o���Ă����I�u�W�F�N�g���ۂ��H
	using DirectXDevice = ComPtr<ID3D11Device>;								// �f�o�C�X
	using DirectXDeviceContext = ComPtr<ID3D11DeviceContext>;			// �f�o�C�X�R���e�L�X�g�i�C�~�f�B�G�C�g�R���e�L�X�g(����)�E�f�B�t�@�[�h�R���e�L�X�g(�L�^������ꊇ���s)�j
	using ImmediateContext = ComPtr<ID3D11DeviceContext>;					// �C�~�f�B�G�C�g�R���e�L�X�g(�������s����f�o�C�X�R���e�L�X�g)
	using DeferredContext = ComPtr<ID3D11DeviceContext>;					// �f�B�t�@�[�h�R���e�L�X�g(���s���e���L�^������ꊇ���s����f�o�C�X�R���e�L�X�g�j
	using DeferredCommandList = ComPtr<ID3D11CommandList>;				// �f�B�t�@�[�h�R���e�L�X�g�Ŏg�p����R�}���h���X�g�^
	using DirectXBuffer = ComPtr<ID3D11Buffer>;								// �e��o�b�t�@
	using ConstantBuffer = ComPtr<ID3D11Buffer>;								// �萔�o�b�t�@
	using VertexBuffer = ComPtr<ID3D11Buffer>;								// ���_�o�b�t�@
	using IndexBuffer = ComPtr<ID3D11Buffer>;									// �C���f�b�N�X�o�b�t�@
	using StructuredBuffer = ComPtr<ID3D11Buffer>;							// �X�g���N�`���[�h�o�b�t�@
	using InputLayout = ComPtr<ID3D11InputLayout>;							// ���_���C�A�E�g
	using VertexShader = ComPtr<ID3D11VertexShader>;						// ���_�V�F�[�_�[
	using PixelShader = ComPtr<ID3D11PixelShader>;							// �s�N�Z���V�F�[�_�[
	using GeometryShader = ComPtr<ID3D11GeometryShader>;					// �W�I���g���V�F�[�_�[
	using ComputeShader = ComPtr<ID3D11ComputeShader>;						// �R���s���[�g�V�F�[�_�[
	using Resource = ComPtr<ID3D11Resource>;									// ���\�[�X	(�e�N�X�`���֘A)
	using ShaderResourceView = ComPtr<ID3D11ShaderResourceView>;		// �V�F�[�_�[���\�[�X�r���[ (�e�N�X�`���֘A)
	using ComputeOutputView = ComPtr<ID3D11UnorderedAccessView>;		// �A���I�[�_�[�h�A�N�Z�X�r���[
	using UnorderedAccessView = ComPtr<ID3D11UnorderedAccessView>;		// �A���I�[�_�[�h�A�N�Z�X�r���[
	using RenderTargetView = ComPtr<ID3D11RenderTargetView>;				// �����_�[�^�[�Q�b�g�r���[
	using Texture2D = ComPtr<ID3D11Texture2D>;								// �e�N�X�`��2D
	using DepthStencilBuffer = ComPtr<ID3D11Texture2D>;					// �[�x�X�e���V���o�b�t�@
	using DepthStencilState = ComPtr<ID3D11DepthStencilState>;			// �[�x�X�e���V���X�e�[�g
	using DepthStencilView = ComPtr<ID3D11DepthStencilView>;				// �[�x�X�e���V���r���[
	using RasterizerState = ComPtr<ID3D11RasterizerState>;				// ���X�^���C�U�X�e�[�g
	using BlendState = ComPtr<ID3D11BlendState>;								// �u�����h�X�e�[�g
	using SamplerState = ComPtr<ID3D11SamplerState>;						// �T���v���[�X�e�[�g
	using ClassLinkage = ComPtr<ID3D11ClassLinkage>;						// �N���X�����P�[�W
	using ClassInstance = ComPtr<ID3D11ClassInstance>;						// �N���X�C���X�^���X
	using ShaderReflection = ComPtr<ID3D11ShaderReflection>;											// �V�F�[�_�[���t���N�V����
	//using ShaderReflectionConstantBuffer = ComPtr<ID3D11ShaderReflectionConstantBuffer>;	// �萔�o�b�t�@�p�̃V�F�[�_�[���t���N�V����
	//using ShaderReflectionType = ComPtr<ID3D11ShaderReflectionType>;							// �V�F�[�_�[���t���N�V�����̃^�C�v
	//using ShaderReflectionVariable = ComPtr<ID3D11ShaderReflectionVariable>;					// �V�F�[�_�[���t���N�V�����̕ϐ�
}

//IDXGI�n
	using IDxgiAdapter = IDXGIAdapter;											// �A�_�v�^
	using IDxgiAdapter_1 = IDXGIAdapter1;										// �A�_�v�^1
	using IDxgiFactory = IDXGIFactory;											// �t�@�N�g���[
	using IDxgiFactory_1 = IDXGIFactory1;										// �t�@�N�g���[1
	using IDxgiOutput = IDXGIOutput;												// �A�E�g�v�b�g
	using SwapChain = IDXGISwapChain;											// �X���b�v�`�F�C��
	using IDxgiDebug = IDXGIDebug;												// �f�o�b�O�p�I�u�W�F�N�g
	using IDxgiDebug_1 = IDXGIDebug1;											// �f�o�b�O�p�I�u�W�F�N�g�̊g���ŁiIDXGIDebug�^���p�����Ă���j

//DXGI�n
	using DxgiFormat = DXGI_FORMAT;												// �t�H�[�}�b�g

// D3D�n
	using FeatureLevel = D3D_FEATURE_LEVEL;

//ID3D11�n
	using DirectXDevice = ID3D11Device;											// �f�o�C�X
	using DirectXDeviceContext = ID3D11DeviceContext;						// �f�o�C�X�R���e�L�X�g�i�C�~�f�B�G�C�g�R���e�L�X�g(����)�E�f�B�t�@�[�h�R���e�L�X�g(�L�^������ꊇ���s)�j
	using ImmediateContext = ID3D11DeviceContext;							// �C�~�f�B�G�C�g�R���e�L�X�g(����)
	using DeferredContext = ID3D11DeviceContext;								// �f�B�t�@�[�h�R���e�L�X�g(�L�^������ꊇ���s�j
	using DeferredCommandList = ID3D11CommandList;							// �f�B�t�@�[�h�R���e�L�X�g�Ŏg�p����R�}���h���X�g�^
	using DirectXBuffer = ID3D11Buffer;											// �e��o�b�t�@
	using ConstantBuffer = ID3D11Buffer;										// �萔�o�b�t�@
	using VertexBuffer = ID3D11Buffer;											// ���_�o�b�t�@
	using IndexBuffer = ID3D11Buffer;											// �C���f�b�N�X�o�b�t�@
	using StructuredBuffer = ID3D11Buffer;										// �X�g���N�`���[�h�o�b�t�@
	using InputLayout = ID3D11InputLayout;										// ���_���C�A�E�g
	using VertexShader = ID3D11VertexShader;									// ���_�V�F�[�_�[
	using PixelShader = ID3D11PixelShader;										// �s�N�Z���V�F�[�_�[
	using GeometryShader = ID3D11GeometryShader;								// �W�I���g���V�F�[�_�[
	using ComputeShader = ID3D11ComputeShader;								// �R���s���[�g�V�F�[�_�[
	using Resource = ID3D11Resource;												// ���\�[�X	(�e�N�X�`���֘A)
	using ShaderResourceView = ID3D11ShaderResourceView;					// �V�F�[�_�[���\�[�X�r���[ (�e�N�X�`���֘A)
	using ComputeOutputView = ID3D11UnorderedAccessView;					// �A���I�[�_�[�h�A�N�Z�X�r���[
	using UnorderedAccessView = ID3D11UnorderedAccessView;				// �A���I�[�_�[�h�A�N�Z�X�r���[
	using RenderTargetView = ID3D11RenderTargetView;						// �����_�[�^�[�Q�b�g�r���[
	using Texture2D = ID3D11Texture2D;											// �e�N�X�`��2D
	using DepthStencilBuffer = ID3D11Texture2D;								// �[�x�X�e���V���o�b�t�@
	using DepthStencilState = ID3D11DepthStencilState;						// �[�x�X�e���V���X�e�[�g
	using DepthStencilView = ID3D11DepthStencilView;						// �[�x�X�e���V���r���[
	using RasterizerState = ID3D11RasterizerState;							// ���X�^���C�U�X�e�[�g
	using BlendState = ID3D11BlendState;										// �u�����h�X�e�[�g
	using SamplerState = ID3D11SamplerState;									// �T���v���[�X�e�[�g
	using MappedSubResource = D3D11_MAPPED_SUBRESOURCE;					// �}�b�v�h�T�u���\�[�X
	using ViewPort = D3D11_VIEWPORT;												// �r���[�|�[�g
	using SubResourceData = D3D11_SUBRESOURCE_DATA;							// �T�u���\�[�X�f�[�^
	using ClassLinkage = ID3D11ClassLinkage;									// �N���X�����P�[�W
	using ClassInstance = ID3D11ClassInstance;								// �N���X�C���X�^���X
	using ShaderReflection = ID3D11ShaderReflection;										// �V�F�[�_�[���t���N�V����
	using ShaderReflectionConstantBuffer = ID3D11ShaderReflectionConstantBuffer;	// �萔�o�b�t�@�p�̃V�F�[�_�[���t���N�V����
	using ShaderReflectionType = ID3D11ShaderReflectionType;								// �V�F�[�_�[���t���N�V�����̃^�C�v
	using ShaderReflectionVariable = ID3D11ShaderReflectionVariable;					// �V�F�[�_�[���t���N�V�����̕ϐ�

//===================================================
// ����ȃf�[�^�^
//===================================================
// �Q�l�����Fhttps://e-words.jp/w/BLOB.html
//======================================================================================================
// BLOB�FBinaryLargeObject�F�s�蒷�̃o�C�i���f�[�^�i�傫���̒�܂��Ă��Ȃ��o�C�i���f�[�^�j
//======================================================================================================
	using DirectXBLOB = ID3DBlob;													// ���Ȓ�`���̏ȗ��^
	using DirectXBinaryObject = ID3DBlob;										// ���Ȓ�`���̏ȗ��^
	using DirectXBinaryLargeObject = ID3DBlob;								// ���Ȓ�`���̖��ȗ��^

//�`DESC�n // tips : DESC��Description�i�����j�̗����ۂ�
	using DxgiAdapterDesc = DXGI_ADAPTER_DESC;
	using DxgiAdapterDesc_1 = DXGI_ADAPTER_DESC1;
	using InputElementDesc = D3D11_INPUT_ELEMENT_DESC;						// ���͑����i���_�f�[�^�Ɏ�������f�[�^�j
	using RasterizerDesc = D3D11_RASTERIZER_DESC;							// ���X�^���C�U�f�X�N
	using Texture2DDesc = D3D11_TEXTURE2D_DESC;								// �e�N�X�`��2D�f�X�N�A�[�x�o�b�t�@�f�X�N
	using DepthBufferDesc = D3D11_TEXTURE2D_DESC;							// �e�N�X�`��2D�f�X�N�A�[�x�o�b�t�@�f�X�N
	using ShaderResourceViewDesc = D3D11_SHADER_RESOURCE_VIEW_DESC;	// �V�F�[�_�[���\�[�X�r���[�f�X�N
	using SamplerDesc = D3D11_SAMPLER_DESC;									// �T���v���[�f�X�N
	using BlendDesc = D3D11_BLEND_DESC;											// �u�����h�f�X�N
	using RenderTargetViewDesc = D3D11_RENDER_TARGET_VIEW_DESC;			// �����_�[�^�[�Q�b�g�r���[�f�X�N
	using RenderTargetBlendDesc = D3D11_RENDER_TARGET_BLEND_DESC;		// �����_�[�^�[�Q�b�g�u�����h�f�X�N�@��D3D11_BLEND_DESC.RenderTarget[0]�Ɠ��`
	using DepthStencilDesc = D3D11_DEPTH_STENCIL_DESC; 					// �[�x�X�e���V���f�X�N
	using DepthStencilViewDesc = D3D11_DEPTH_STENCIL_VIEW_DESC;			// �[�x�X�e���V���r���[�f�X�N
	using SwapChainDesc = DXGI_SWAP_CHAIN_DESC;								// �X���b�v�`�F�C���f�X�N
	using ModeDesc = DXGI_MODE_DESC;												// ���[�h�f�X�N
	using BufferDesc = D3D11_BUFFER_DESC;										// �o�b�t�@�f�X�N
	using UnorderedAccessViewDesc = D3D11_UNORDERED_ACCESS_VIEW_DESC;	// �A���I�[�_�[�h�A�N�Z�X�r���[�f�X�N

// �g�|���W�[
//enum D3D_PRIMITIVE_TOPOLOGY���� D3D11�̕����݂̂𔲐���������
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

// 16�o�C�g�A���C�������g�}�N��
#define ALIGN16 _declspec(align(16))

//16�o�C�g�A���C�������g���s��ꂽ��{�I�ȃf�[�^�^
using ALIGN_INT = ALIGN16 int;					// ���̑傫���F4byte
using ALIGN_UINT = ALIGN16 unsigned int;		// ���̑傫���F4byte
using ALIGN_FLOAT = ALIGN16 float;				// ���̑傫���F4byte
using ALIGN_DOUBLE = ALIGN16 double;			// ���̑傫���F8byte

//16�o�C�g�A���C�������g���s��ꂽDirectXMath�ɓ����Ă���f�[�^�^
using ALIGN_XMFLOAT2 = ALIGN16 XMFLOAT2;		// ���̑傫���F8byte
using ALIGN_XMFLOAT3 = ALIGN16 XMFLOAT3;		// ���̑傫���F12byte
using ALIGN_XMFLOAT4 = ALIGN16 XMFLOAT4;		// ���̑傫���F16byte
using ALIGN_XMMATRIX = ALIGN16 XMMATRIX;		// �m��Ȃ�
using ALIGN_XMVECTOR = ALIGN16 XMVECTOR;		// �m��Ȃ�
using ALIGN_XMFLOAT4X4 = ALIGN16 XMFLOAT4X4;	// �m��Ȃ�

// ���S��ComPtr��������邽�߂̃}�N��
#define SafeReleaseCom( comObject ) {\
	if (comObject) {\
		comObject.Reset();\
	}\
}