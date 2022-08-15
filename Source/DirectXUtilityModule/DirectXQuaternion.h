#pragma once

#include"Math.hpp"
#include"TypeTraitsEx.hpp"
#include"DirectXDataTypes.h"

#define ALIGN16 _declspec(align(16))

class DXQuaternion : public Math::Quaternion::Quaternion {

public:

	// �p���R���X�g���N�^
	using Math::Quaternion::Quaternion::tQuaternion;

	DXQuaternion( const DirectX::XMFLOAT4& xmFloat4 ) {
		x = xmFloat4.x;
		y = xmFloat4.y;
		z = xmFloat4.z;
		w = xmFloat4.w;
	}

	DXQuaternion(const DirectX::XMVECTOR& xmVector) {
		DirectX::XMFLOAT4 f4;
		XMStoreFloat4(&f4, xmVector);
		x = f4.x;
		y = f4.y;
		z = f4.z;
		w = f4.w;
	}

public:

	// XMFLOAT4�ւ̕ϊ�
	operator DirectX::XMFLOAT4() const noexcept {
		return DirectX::XMFLOAT4(x, y, z, w);
	}

	// XMVector�ւ̕ϊ�
	operator DirectX::XMVECTOR() const noexcept {
		DirectX::XMFLOAT4 f4 = *this;
		return DirectX::XMLoadFloat4(&f4);
	}

public:

	// XMFLOAT4�ւ̕ϊ�
	DirectX::XMFLOAT4 ToXMFLOAT4() const noexcept {
		return DirectX::XMFLOAT4(x, y, z, w);
	}

	// XMVector�ւ̕ϊ�
	DirectX::XMVECTOR ToXMVECTOR() const noexcept {
		DirectX::XMFLOAT4 f4 = *this;
		return DirectX::XMLoadFloat4(&f4);
	}

};

// AlignDataType
using DXQuaternionAlign = ALIGN16 DXQuaternion;