#pragma once

#include"Math.hpp"
#include"TypeTraitsEx.hpp"
#include"DirectXDataTypes.h"

#define ALIGN16 _declspec(align(16))

class DXVector : public Math::Vector::Vector1{};

class DXVectorInt : public Math::Vector::Vector1Int{};

class DXVector2 : public Math::Vector::Vector2 {

public:

	// 継承コンストラクタ
	using Math::Vector::Vector2::Vector2t;

	DXVector2( const DirectX::XMFLOAT2& xmFloat2 ) {
		x = xmFloat2.x;
		y = xmFloat2.y;
	}

	DXVector2( const DirectX::XMVECTOR& xmVector ) {
		DirectX::XMFLOAT2 f2;
		XMStoreFloat2( &f2, xmVector );
		x = f2.x;
		y = f2.y;
	}

public:


	// XMFLOAT2への変換
	operator DirectX::XMFLOAT2() const noexcept {
		return DirectX::XMFLOAT2( x, y );
	}

	// XMVectorへの変換
	operator DirectX::XMVECTOR() const noexcept {
		DirectX::XMFLOAT2 f2 = *this;
		return DirectX::XMLoadFloat2( &f2 );
	}

public:

	// XMFLOAT2への変換
	DirectX::XMFLOAT2 ToXMFLOAT2() const noexcept {
		return DirectX::XMFLOAT2( x, y );
	}

	// XMVectorへの変換
	DirectX::XMVECTOR ToXMVECTOR() const noexcept {
		DirectX::XMFLOAT2 f2 = *this;
		return DirectX::XMLoadFloat2( &f2 );
	}

};

class DXVector2Int : public Math::Vector::Vector2Int {

public:

	// 継承コンストラクタ
	using Math::Vector::Vector2Int::Vector2t;

	DXVector2Int( const DirectX::XMINT2& xmInt2 ) {
		x = xmInt2.x;
		y = xmInt2.y;
	}

	DXVector2Int( const DirectX::XMVECTOR& xmVector ) {
		DirectX::XMINT2 i2;
		XMStoreSInt2( &i2, xmVector );
		x = i2.x;
		y = i2.y;
	}

public:

	// XMINT2への変換
	operator DirectX::XMINT2() const noexcept {
		return DirectX::XMINT2( x, y );
	}

	// XMVectorへの変換
	operator DirectX::XMVECTOR() const noexcept {
		DirectX::XMINT2 i2 = *this;
		return DirectX::XMLoadSInt2( &i2 );
	}

public:

	// XMINT2への変換
	DirectX::XMINT2 ToXMINT2() const noexcept {
		return DirectX::XMINT2( x, y );
	}

	// XMVECTORへの変換
	DirectX::XMVECTOR ToXMVECTOR() const noexcept {
		DirectX::XMINT2 i2 = *this;
		return DirectX::XMLoadSInt2( &i2 );
	}

};

class DXVector3 : public Math::Vector::Vector3 {


public:

	// 継承コンストラクタ
	using Math::Vector::Vector3::Vector3t;

	DXVector3( const DirectX::XMFLOAT3& xmFloat3 ) {
		x = xmFloat3.x;
		y = xmFloat3.y;
		z = xmFloat3.z;
	}

	DXVector3( const DirectX::XMVECTOR& xmVector ) {
		DirectX::XMFLOAT3 f3;
		XMStoreFloat3( &f3, xmVector );
		x = f3.x;
		y = f3.y;
		z = f3.z;
	}

public:

	// XMFLOAT3への変換
	operator DirectX::XMFLOAT3() const noexcept {
		return DirectX::XMFLOAT3( x, y, z );
	}

	// XMVectorへの変換
	operator DirectX::XMVECTOR() const noexcept {
		DirectX::XMFLOAT3 f3 = *this;
		return DirectX::XMLoadFloat3( &f3 );
	}

public:

	// XMFLOAT3への変換
	DirectX::XMFLOAT3 ToXMFLOAT3() const noexcept {
		return DirectX::XMFLOAT3( x, y, z );
	}

	// XMVectorへの変換
	DirectX::XMVECTOR ToXMVECTOR() const noexcept {
		DirectX::XMFLOAT3 f3 = *this;
		return DirectX::XMLoadFloat3( &f3 );
	}

};

class DXVector3Int : public Math::Vector::Vector3Int {

public:
	
	// 継承コンストラクタ
	using Math::Vector::Vector3Int::Vector3t;

	DXVector3Int( const DirectX::XMINT3& xmInt3 ) {
		x = xmInt3.x;
		y = xmInt3.y;
		z = xmInt3.z;
	}

	DXVector3Int( const DirectX::XMVECTOR& xmVector ) {
		DirectX::XMINT3 i3;
		XMStoreSInt3( &i3, xmVector );
		x = i3.x;
		y = i3.y;
		z = i3.z;
	}

public:

	// XMINT3への変換
	operator DirectX::XMINT3() const noexcept {
		return DirectX::XMINT3( x, y, z );
	}

	// XMVectorへの変換
	operator DirectX::XMVECTOR() const noexcept {
		DirectX::XMINT3 i3 = *this;
		return DirectX::XMLoadSInt3( &i3 );
	}

public:

	// XMINT3への変換
	DirectX::XMINT3 ToXMINT3() const noexcept {
		return DirectX::XMINT3( x, y, z );
	}

	// XMVECTORへの変換
	DirectX::XMVECTOR ToXMVECTOR() const noexcept {
		DirectX::XMINT3 i3 = *this;
		return DirectX::XMLoadSInt3( &i3 );
	}

};

class DXVector4 : public Math::Vector::Vector4 {

public:

	// 継承コンストラクタ
	using Math::Vector::Vector4::Vector4t;

	DXVector4( const DirectX::XMFLOAT4& xmFloat4 ) {
		x = xmFloat4.x;
		y = xmFloat4.y;
		z = xmFloat4.z;
		w = xmFloat4.w;
	}

	DXVector4( const DirectX::XMVECTOR& xmVector ) {
		DirectX::XMFLOAT4 f4;
		XMStoreFloat4( &f4, xmVector );
		x = f4.x;
		y = f4.y;
		z = f4.z;
		w = f4.w;
	}

public:

	// XMFLOAT4への変換
	operator DirectX::XMFLOAT4() const noexcept {
		return DirectX::XMFLOAT4( x, y, z, w );
	}

	// XMVectorへの変換
	operator DirectX::XMVECTOR() const noexcept {
		DirectX::XMFLOAT4 f4 = *this;
		return DirectX::XMLoadFloat4( &f4 );
	}

public:

	// XMFLOAT4への変換
	DirectX::XMFLOAT4 ToXMFLOAT4() const noexcept {
		return DirectX::XMFLOAT4( x, y, z, w );
	}

	// XMVectorへの変換
	DirectX::XMVECTOR ToXMVECTOR() const noexcept {
		DirectX::XMFLOAT4 f4 = *this;
		return DirectX::XMLoadFloat4( &f4 );
	}

};

class DXVector4Int : public Math::Vector::Vector4Int {

public:

	// 継承コンストラクタ
	using Math::Vector::Vector4Int::Vector4t;

	DXVector4Int( const DirectX::XMINT4& xmInt4 ) {
		x = xmInt4.x;
		y = xmInt4.y;
		z = xmInt4.z;
		w = xmInt4.w;
	}

	DXVector4Int( const DirectX::XMVECTOR& xmVector ) {
		DirectX::XMINT4 i4;
		XMStoreSInt4( &i4, xmVector );
		x = i4.x;
		y = i4.y;
		z = i4.z;
		w - i4.w;
	}

public:

	// XMINT4への変換
	operator DirectX::XMINT4() const noexcept {
		return DirectX::XMINT4( x, y, z, w );
	}

	// XMVectorへの変換
	operator DirectX::XMVECTOR() const noexcept {
		DirectX::XMINT4 i4 = *this;
		return DirectX::XMLoadSInt4( &i4 );
	}

public:

	// XMINT4への変換
	DirectX::XMINT4 ToXMINT4() const noexcept {
		return DirectX::XMINT4( x, y, z, w );
	}

	// XMVECTORへの変換
	DirectX::XMVECTOR ToXMVECTOR() const noexcept {
		DirectX::XMINT4 i4 = *this;
		return DirectX::XMLoadSInt4( &i4 );
	}

};

// AlignDataType
using DXVectorAlign = ALIGN16 DXVector;
using DXVectorIntAlign = ALIGN16 DXVectorInt;
using DXVector2Align = ALIGN16 DXVector2;
using DXVector2IntAlign = ALIGN16 DXVector2Int;
using DXVector3Align = ALIGN16 DXVector3;
using DXVector3IntAlign = ALIGN16 DXVector3Int;
using DXVector4Align = ALIGN16 DXVector4;
using DXVector4IntAlign = ALIGN16 DXVector4Int;