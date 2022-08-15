#pragma once

#include"Color.hpp"
#include"Math.hpp"
#include"DirectXDataTypes.h"

#define ALIGN16 _declspec(align(16))

class DXColor : public Utility::Color {

public:

	// �p���R���X�g���N�^
	using Utility::Color::tColor;

	DXColor(const DirectX::XMFLOAT4& xmFloat4) {
		r = xmFloat4.x;
		g = xmFloat4.y;
		b = xmFloat4.z;
		a = xmFloat4.w;
	}

	DXColor (const DirectX::XMVECTOR& xmVector) {
		DirectX::XMFLOAT4 f4;
		XMStoreFloat4(&f4, xmVector);
		r = f4.x;
		g = f4.y;
		b = f4.z;
		a = f4.w;
	}

public:

	// XMFLOAT4�ւ̕ϊ�
	operator DirectX::XMFLOAT4() const noexcept {
		return DirectX::XMFLOAT4(r, g, b, a);
	}

	// XMVector�ւ̕ϊ�
	operator DirectX::XMVECTOR() const noexcept {
		DirectX::XMFLOAT4 f4 = *this;
		return DirectX::XMLoadFloat4(&f4);
	}

public:

	// XMFLOAT4�ւ̕ϊ�
	DirectX::XMFLOAT4 ToXMFLOAT4() const noexcept {
		return DirectX::XMFLOAT4(r, g, b, a);
	}

	// XMVector�ւ̕ϊ�
	DirectX::XMVECTOR ToXMVECTOR() const noexcept {
		DirectX::XMFLOAT4 f4 = *this;
		return DirectX::XMLoadFloat4(&f4);
	}

};

class DXHSVColor : public Utility::HSVColor {

public:

		// �p���R���X�g���N�^
	using Utility::HSVColor::tHSVColor;

	DXHSVColor(const DirectX::XMFLOAT4& xmFloat4) {
		h = xmFloat4.x;
		s = xmFloat4.y;
		v = xmFloat4.z;
		a = xmFloat4.w;
	}

	DXHSVColor(const DirectX::XMVECTOR& xmVector) {
		DirectX::XMFLOAT4 f4;
		XMStoreFloat4(&f4, xmVector);
		h = f4.x;
		s = f4.y;
		v = f4.z;
		a = f4.w;
	}

public:

	// XMFLOAT4�ւ̕ϊ�
	operator DirectX::XMFLOAT4() const noexcept {
		return DirectX::XMFLOAT4(h, s, v, a);
	}

	// XMVector�ւ̕ϊ�
	operator DirectX::XMVECTOR() const noexcept {
		DirectX::XMFLOAT4 f4 = *this;
		return DirectX::XMLoadFloat4(&f4);
	}

public:

	// XMFLOAT4�ւ̕ϊ�
	DirectX::XMFLOAT4 ToXMFLOAT4() const noexcept {
		return DirectX::XMFLOAT4(h, s, v, a);
	}

	// XMVector�ւ̕ϊ�
	DirectX::XMVECTOR ToXMVECTOR() const noexcept {
		DirectX::XMFLOAT4 f4 = *this;
		return DirectX::XMLoadFloat4(&f4);
	}

};

// AlignDataType
using DXColorAlign = ALIGN16 DXColor;
using DXHSVColorAlign = ALIGN16 DXHSVColor;