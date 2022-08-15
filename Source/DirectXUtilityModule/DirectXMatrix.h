#pragma once
#include"Math.hpp"
#include"TypeTraitsEx.hpp"
#include"DirectXDataTypes.h"

#define ALIGN16 _declspec(align(16))

class DXMatrix3x3 : public Math::Matrix::Matrix3x3 {

public:

	// 継承コンストラクタ
	using Math::Matrix::Matrix3x3::Matrix3x3t;

	DXMatrix3x3(const DirectX::XMFLOAT3X3& xmfloat3x3) {
		m11 = xmfloat3x3._11;
		m12 = xmfloat3x3._12;
		m13 = xmfloat3x3._13;
		m21 = xmfloat3x3._21;
		m22 = xmfloat3x3._22;
		m23 = xmfloat3x3._23;
		m31 = xmfloat3x3._31;
		m32 = xmfloat3x3._32;
		m33 = xmfloat3x3._33;
	}

	DXMatrix3x3(const DirectX::XMMATRIX& xmmatrix) {
		DirectX::XMFLOAT3X3 mat;
		XMStoreFloat3x3(&mat, xmmatrix);
		m11 = mat._11;
		m12 = mat._12;
		m13 = mat._13;
		m21 = mat._21;
		m22 = mat._22;
		m23 = mat._23;
		m31 = mat._31;
		m32 = mat._32;
		m33 = mat._33;
	}

public:

	// XMFLOAT3X3への変換
	operator DirectX::XMFLOAT3X3() const noexcept {
		return DirectX::XMFLOAT3X3(
			m11, m12, m13,
			m21, m22, m23,
			m31, m32, m33
		);
	}

	// XMMATRIXへの変換
	operator DirectX::XMMATRIX() const noexcept {
		DirectX::XMFLOAT3X3 mat = *this;
		return DirectX::XMLoadFloat3x3(&mat);
	}

public:

		// XMFLOAT3X3への変換
	DirectX::XMFLOAT3X3 ToXMFLOAT3X3() const noexcept {
		return DirectX::XMFLOAT3X3(
			m11, m12, m13,
			m21, m22, m23,
			m31, m32, m33
		);
	}

	// XMMATRIXへの変換
	DirectX::XMMATRIX ToXMMATRIX() const noexcept {
		DirectX::XMFLOAT3X3 mat = *this;
		return DirectX::XMLoadFloat3x3(&mat);
	}

};

class DXMatrix4x4 : public Math::Matrix::Matrix4x4 {

public:

	// 継承コンストラクタ
	using Math::Matrix::Matrix4x4::Matrix4x4t;

	DXMatrix4x4(const DirectX::XMFLOAT4X4& xmfloat4x4) {
		m11 = xmfloat4x4._11;
		m12 = xmfloat4x4._12;
		m13 = xmfloat4x4._13;
		m14 = xmfloat4x4._14;
		m21 = xmfloat4x4._21;
		m22 = xmfloat4x4._22;
		m23 = xmfloat4x4._23;
		m24 = xmfloat4x4._24;
		m31 = xmfloat4x4._31;
		m32 = xmfloat4x4._32;
		m33 = xmfloat4x4._33;
		m34 = xmfloat4x4._34;
		m41 = xmfloat4x4._41;
		m42 = xmfloat4x4._42;
		m43 = xmfloat4x4._43;
		m44 = xmfloat4x4._44;
	}

	DXMatrix4x4(const DirectX::XMMATRIX& xmmatrix) {
		DirectX::XMFLOAT4X4 mat;
		XMStoreFloat4x4(&mat, xmmatrix);
		m11 = mat._11;
		m12 = mat._12;
		m13 = mat._13;
		m14 = mat._14;
		m21 = mat._21;
		m22 = mat._22;
		m23 = mat._23;
		m24 = mat._24;
		m31 = mat._31;
		m32 = mat._32;
		m33 = mat._33;
		m34 = mat._34;
		m41 = mat._41;
		m42 = mat._42;
		m43 = mat._43;
		m44 = mat._44;
	}

public:

	// XMFLOAT4X4への変換
	operator DirectX::XMFLOAT4X4() const noexcept {
		return DirectX::XMFLOAT4X4(
			m11, m12, m13, m14,
			m21, m22, m23, m24,
			m31, m32, m33, m34,
			m41, m42, m43, m44
		);
	}

	// XMMATRIXへの変換
	operator DirectX::XMMATRIX() const noexcept {
		DirectX::XMFLOAT4X4 mat = *this;
		return DirectX::XMLoadFloat4x4(&mat);
	}

public:

		// XMFLOAT4X4への変換
	DirectX::XMFLOAT4X4 ToXMFLOAT4X4() const noexcept {
		return DirectX::XMFLOAT4X4(
			m11, m12, m13, m14,
			m21, m22, m23, m24,
			m31, m32, m33, m34,
			m41, m42, m43, m44
		);
	}

	// XMMATRIXへの変換
	DirectX::XMMATRIX ToXMMATRIX() const noexcept {
		DirectX::XMFLOAT4X4 mat = *this;
		return DirectX::XMLoadFloat4x4(&mat);
	}

};

// AlignDataType
using DXMatrix3x3Align = ALIGN16 DXMatrix3x3;
using DXMatrix4x4Align = ALIGN16 DXMatrix4x4;