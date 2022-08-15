#pragma once
// UnityのColorクラスを模倣するように作成したクラス

#include"Math.hpp"

namespace Utility {

	// 前方宣言
	template<class T>
	class tColor;
	template<class T>
	class tHSVColor;

	// 色クラス（RGB色空間）
	template<class T>
	class tColor {

	public:

		// r, g, b, a要素のデータ型
		using type = T;

	public:

		enum class ParamType {
			R,
			G,
			B,
			A,
			RGBA
		};

	public:

#pragma region member

		union {
			struct {
				T r;
				T g;
				T b;
				T a;
			};
			struct {
				std::array<T, 3> rgb;
			};
			struct {
				std::array<T, 4> rgba;
			};
		};

#pragma endregion member

	public:

#pragma region constructor

		tColor() : r(0), g(0), b(0), a(0) {};
		tColor(const tColor& color) {
			rgba = color.rgba;
		}
		tColor(tColor&& color) {
			rgba = color.rgba;
		}
		tColor(T r, T g, T b, T a) : r(r), g(g), b(b), a(a) {};

		// 型変換を行い要素を構築する（型変換機能付きコンストラクタ）
		template<class U>
		tColor(tColor<U> const& color) {
			r = static_cast<T>(color.r);
			g = static_cast<T>(color.g);
			b = static_cast<T>(color.b);
			a = static_cast<T>(color.a);
		}
#pragma endregion constructor

	public:

#pragma region operator

	//===========================
	// 代入演算子
	//===========================

		template<class U>
		tColor& operator=(tColor<U> const& color) noexcept {
			r = static_cast<T>(color.r);
			g = static_cast<T>(color.g);
			b = static_cast<T>(color.b);
			a = static_cast<T>(color.a);
			return *this;
		}

		tColor& operator=(const tColor& color) noexcept {
			rgba = color.rgba;
			return *this;
		}

	//===========================
	// ()演算子　※数値を設定する
	//===========================

		tColor& operator()(T newR, T newG, T newB, T newA) noexcept {
			r = newR;
			g = newG;
			b = newB;
			a = newA;
			return *this;
		}

		tColor& operator()(const tColor& color) noexcept {
			r = color.r;
			g = color.g;
			b = color.b;
			a = color.a;
		}

	//===========================
	// []演算子　※[0]、[1]、[2]、[3]を使用して r や g や b や a 成分にアクセスします
	//===========================

		float_t& operator[](int_t i) {
			return rgba[i];
		}

	//===========================
	// 算術演算子
	//===========================

		tColor operator+(T scalar) const noexcept {
			return tColor(r + scalar, g + scalar, b + scalar, a + scalar);
		}

		tColor operator+(const tColor& color) const noexcept {
			return tColor(r + color.r, g + color.g, b + color.b, a + color.a);
		}

		tColor operator-(T scalar) const noexcept {
			return tColor(r - scalar, g - scalar, b - scalar, a - scalar);
		}

		tColor operator-(const tColor& color) const noexcept {
			return tColor(r - color.r, g - color.g, b - color.b, a - color.a);
		}

		tColor operator*(T scalar) const noexcept {
			return tColor(r * scalar, g * scalar, b * scalar, a * scalar);
		}

		tColor operator*(const tColor& color) const noexcept {
			return tColor(r * color.r, g * color.g, b * color.b, a * color.a);
		}

		tColor operator/(T scalar) const noexcept {
			return tColor(r / scalar, g / scalar, b / scalar, a / scalar);
		}

		tColor operator/(const tColor& color) const noexcept {
			return tColor(r / color.r, g / color.g, b / color.b, a / color.a);
		}

	//===========================
	// 複合代入演算子
	//===========================

		tColor& operator+=(T scalar) noexcept {
			*this = *this + scalar;
			return *this;
		}

		tColor& operator+=(const tColor& color) noexcept {
			*this = *this + color;
			return *this;
		}

		tColor& operator-=(T scalar) noexcept {
			*this = *this - scalar;
			return *this;
		}

		tColor& operator-=(const tColor& color) noexcept {
			*this = *this - color;
			return *this;
		}

		tColor& operator*=(T scalar) noexcept {
			*this = *this * scalar;
			return *this;
		}

		tColor& operator*=(const tColor& color) noexcept {
			*this = *this * color;
			return *this;
		}

		tColor& operator/=(T scalar) noexcept {
			*this = *this / scalar;
			return *this;
		}

		tColor& operator/=(const tColor& color) noexcept {
			*this = *this / color;
			return *this;
		}

	//===========================
	// 比較演算子
	//===========================

		bool operator==(const tColor& color) const noexcept {
			return Equal<T>(color);
		}

		bool operator!=(const tColor& color) const noexcept {
			return NotEqual<T>(color);
		}

	//===========================
	// ストリーム出力
	//===========================

		friend std::ostream& operator<<(std::ostream& stream, const tColor& color) {
			stream << color.ToString();
			return stream;
		}

#pragma endregion operator

	public:

#pragma region static param

		static constexpr tColor Red() noexcept {
			return tColor(1.0, 0.0, 0.0, 1.0);
		}

		static constexpr tColor Green() noexcept {
			return tColor(0.0, 1.0, 0.0, 1.0);
		}

		static constexpr tColor Blue() noexcept {
			return tColor(0.0, 0.0, 1.0, 1.0);
		}

		static constexpr tColor Cyan() noexcept {
			return tColor(0.0, 1.0, 1.0, 1.0);
		}

		static constexpr tColor Magenta() noexcept {
			return tColor(1.0, 0.0, 1.0, 1.0);
		}

		static constexpr tColor Yellow() noexcept {
			return tColor(1.0, 0.92, 0.016, 1.0);
		}

		static constexpr tColor White() noexcept {
			return tColor(1.0, 1.0, 1.0, 1.0);
		}

		static constexpr tColor Gray() noexcept {
			return tColor(0.5, 0.5, 0.5, 1.0);
		}

		static constexpr tColor Black() noexcept {
			return tColor(0.0, 0.0, 0.0, 1.0);
		}

		static constexpr tColor Clear() noexcept {
			return tColor(0.0, 0.0, 0.0, 0.0);
		}

#pragma endregion static param

	public:

#pragma region static method

		// 色のパラメータごとに線形補間を行う
		static tColor LerpParam(const tColor& a, const tColor& b, T t, tColor::ParamType paramType = tColor::ParamType::RGBA) noexcept {
			switch (paramType) {
				case ParamType::R:
				{
					return tColor(Math::Lerp(a.r, b.r, t), a.g, a.b, a.a);
				}
				break;
				case ParamType::G:
				{
					return tColor(a.r, Math::Lerp(a.g, b.g, t), a.b, a.a);
				}
				break;
				case ParamType::B:
				{
					return tColor(a.r, a.g, Math::Lerp(a.b, b.b, t), a.a);
				}
				break;
				case ParamType::A:
				{
					return tColor(a.r, a.g, a.b, Math::Lerp(a.a, b.a, t));
				}
				break;
				case ParamType::RGBA:
				{
					return tColor::Lerp(a, b, t);
				}
			}
		}

		// RGBA要素の線形補間を行います　※tは0.0～1.0の間にクランプされます
		static tColor Lerp(const tColor& a, const tColor& b, T t) noexcept {
			auto diff = b - a;
			return a + (diff * Math::Clamp01<T>(t));
		}

		// RGBA要素の線形補間を行います
		static tColor LerpUnclamped(const tColor& a, const tColor& b, T t) noexcept {
			auto diff = b - a;
			return a + (diff * t);
		}

		// 色情報の正規化を行います
		// ※ Color(R : 0.0～1.0, G : 0.0～1.0, B : 0.0～1.0, 0.0～1.0)
		static tColor Normalize(const tColor& color) {
			return tColor(
				color.r / 255.0,
				color.g / 255.0,
				color.b / 255.0,
				color.a
			);
		}

		// 色情報の非正規化を行います
		// ※ Color(R : 0.0～255.0, G : 0.0～255.0, B : 0.0～255.0, 0.0～1.0)
		static tColor DeNormalize(const tColor& color) {
			return tColor(
				color.r * 255.0,
				color.g * 255.0,
				color.b * 255.0,
				color.a
			);
		}

		// グレースケールに関する資料：https://qiita.com/yoya/items/96c36b069e74398796f3
		// 様々な色変換に関する資料：https://yanohirota.com/color-converter/
		// RGBをHSVに変換　// https://code-examples.net/ja/q/2e0e49
		// RGBをHSVに変換　// https://gist.github.com/ochilab/5875412
		// HSVをRGBに変換　// https://code-examples.net/ja/q/2e0e49

#pragma endregion static method

	public:

#pragma region method

		// 各色の成分ごとに値を設定
		tColor& Set(T newR, T newG, T newB, T newA) noexcept {
			r = newR;
			g = newG;
			b = newB;
			a = newA;
			return *this;
		}

		// 色の値をコピー
		tColor& Set(const tColor& newColor) noexcept {
			r = newColor.r;
			g = newColor.g;
			b = newColor.b;
			a = newColor.a;
			return *this;
		}

		// 各色の成分ごとに値を加算
		tColor& Add(T addR, T addG, T addB, T addA) noexcept {
			r += addR;
			g += addG;
			b += addB;
			a += addA;
			return *this;
		}

		// 色の値を加算
		tColor& Add(const tColor& addColor) noexcept {
			for (int_t i = 0; i < 4; i++) {
				rgba[i] += addColor.rgba[i];
			}
			return *this;
		}

		// 各色の成分ごとに値を減算
		tColor& Sub(T subR, T subG, T subB, T subA) noexcept {
			r += subR;
			g += subG;
			b += subB;
			a += subA;
			return *this;
		}

		// 色の値を減算
		tColor& Sub(const tColor& subColor) noexcept {
			for (int_t i = 0; i < 4; i++) {
				rgba[i] += subColor.rgba[i];
			}
			return *this;
		}

		// 各色の成分ごとに乗算
		tColor& Mul(T mulR, T mulG, T mulB, T mulA) noexcept {
			r *= mulR;
			g *= mulG;
			b *= mulB;
			a *= mulA;
		}

		// 色の値を乗算
		tColor& Mul(const tColor& mulColor) noexcept {
			for (int_t i = 0; i < 4; i++) {
				rgba[i] *= mulColor.rgba[i];
			}
			return *this;
		}

		// HSV色空間への変換
		template<class U = T>
		tHSVColor<U> ToHSVColor() const noexcept {
			T max = Math::Max(Math::Max(r, g), b);
			T min = Math::Min(Math::Min(r, g), b);
			U h;
			U s;
			U v;

			// H(色相)のパラメータ
			{
				h = max - min;
				if (h > 0.0f) {
					if (max == r) {
						h = (g - b) / h;
						if (h < 0.0f) {
							h += 6.0f;
						}
					}
					else if (max == g) {
						h = 2.0f + (b - r) / h;
					}
					else {
						h = 4.0f + (r - g) / h;
					}
				}
				h /= 6.0f;
			}

			// S(色彩)のパラメータ
			{
				// 円錐モデルの場合
				// s = max - min;

				// 円柱モデルで計算
				s = (max - min);
				if (!Math::IsZero(max)) s / max;
			}

			// V(明度)のパラメータ
			{
				v = max;
			}

			return tHSVColor<U>(
				h,
				s,
				v,
				a
				);
		}

		// データ型を変換したtColor型を返す
		template<class U>
		tColor<U> TypeConvert() const noexcept {
			return tColor<U>(*this);
		}

		// 使用されているデータ型名を取得
		std::string TypeName() {
			return typeid(type).name();
		}

		// ベクトルの値を文字列に変換
		std::string ToString() const noexcept {
			return std::string(
				"Color( "
				+
				Utility::ToString(r)
				+
				", "
				+
				Utility::ToString(g)
				+
				", "
				+
				Utility::ToString(b)
				+
				", "
				+
				Utility::ToString(a)
				+
				" )"
			);
		}

		// 与えられた色情報がこの色情報と正確に等しい場合、trueを返します
		template<class U = T>
		bool Equal(const tColor& color, U epsilon = 0.001) const noexcept {
			for (int_t i = 0; i < 4; i++) {
				if (Math::NotEqual<U>(rgba[i], color.rgba[i], epsilon)) {
					return false;
				}
			}
			return true;
		}

		// 与えられた色情報がこの色情報と正確に等しくない場合、trueを返します
		template<class U = T>
		bool NotEqual(const tColor& color, U epsilon = 0.001) const noexcept {
			for (int_t i = 0; i < 4; i++) {
				if (Math::NotEqual<U>(rgba[i], color.rgba[i], epsilon)) {
					return true;
				}
			}
			return false;
		}
#pragma endregion method


	};

	using Color = tColor<float_t>;

	// 色クラス（HSV色空間）
	template<class T>
	class tHSVColor {

	public:

		// r, g, b, a要素のデータ型
		using type = T;

	public:

		enum class ParamType {
			H,
			S,
			V,
			A,
			HSVA
		};

	public:

#pragma region member

		union {
			struct {
				T h;
				T s;
				T v;
				T a;
			};
			struct {
				std::array<T, 3> hsv;
			};
			struct {
				std::array<T, 4> hsva;
			};
		};

#pragma endregion member

	public:

#pragma region constructor

		tHSVColor() : h(0), s(0), v(0), a(0) {};
		tHSVColor(const tHSVColor& color) {
			hsva = color.hsva;
		}
		tHSVColor(tHSVColor&& color) {
			hsva = color.hsva;
		}
		tHSVColor(T r, T g, T b, T a) : h(r), s(g), v(b), a(a) {};

		// 型変換を行い要素を構築する（型変換機能付きコンストラクタ）
		template<class U>
		tHSVColor(tHSVColor<U> const& color) {
			h = static_cast<T>(color.h);
			s = static_cast<T>(color.s);
			v = static_cast<T>(color.v);
			a = static_cast<T>(color.a);
		}
#pragma endregion constructor

	public:

#pragma region operator

	//===========================
	// 代入演算子
	//===========================

		template<class U>
		tHSVColor& operator=(tHSVColor<U> const& color) noexcept {
			h = static_cast<T>(color.h);
			s = static_cast<T>(color.s);
			v = static_cast<T>(color.v);
			a = static_cast<T>(color.a);
			return *this;
		}

		tHSVColor& operator=(const tHSVColor& color) noexcept {
			hsva = color.hsva;
			return *this;
		}

	//===========================
	// ()演算子　※数値を設定する
	//===========================

		tHSVColor& operator()(T newR, T newG, T newB, T newA) noexcept {
			h = newR;
			s = newG;
			v = newB;
			a = newA;
			return *this;
		}

		tHSVColor& operator()(const tHSVColor& color) noexcept {
			h = color.h;
			s = color.s;
			v = color.v;
			a = color.a;
			return *this;
		}

	//===========================
	// []演算子　※[0]、[1]、[2]、[3]を使用して r や g や b や a 成分にアクセスします
	//===========================

		float_t& operator[](int_t i) {
			return hsva[i];
		}

	//===========================
	// 算術演算子
	//===========================

		tHSVColor operator+(T scalar) const noexcept {
			return tHSVColor(h + scalar, s + scalar, v + scalar, a + scalar);
		}

		tHSVColor operator+(const tHSVColor& color) const noexcept {
			return tHSVColor(h + color.h, s + color.s, v + color.v, a + color.a);
		}

		tHSVColor operator-(T scalar) const noexcept {
			return tHSVColor(h - scalar, s - scalar, v - scalar, a - scalar);
		}

		tHSVColor operator-(const tHSVColor& color) const noexcept {
			return tHSVColor(h - color.h, s - color.s, v - color.v, a - color.a);
		}

		tHSVColor operator*(T scalar) const noexcept {
			return tHSVColor(h * scalar, s * scalar, v * scalar, a * scalar);
		}

		tHSVColor operator*(const tHSVColor& color) const noexcept {
			return tHSVColor(h * color.h, s * color.s, v * color.v, a * color.a);
		}

		tHSVColor operator/(T scalar) const noexcept {
			return tHSVColor(h / scalar, s / scalar, v / scalar, a / scalar);
		}

		tHSVColor operator/(const tHSVColor& color) const noexcept {
			return tHSVColor(h / color.h, s / color.s, v / color.v, a / color.a);
		}

	//===========================
	// 複合代入演算子
	//===========================

		tHSVColor& operator+=(T scalar) noexcept {
			*this = *this + scalar;
			return *this;
		}

		tHSVColor& operator+=(const tHSVColor& color) noexcept {
			*this = *this + color;
			return *this;
		}

		tHSVColor& operator-=(T scalar) noexcept {
			*this = *this - scalar;
			return *this;
		}

		tHSVColor& operator-=(const tHSVColor& color) noexcept {
			*this = *this - color;
			return *this;
		}

		tHSVColor& operator*=(T scalar) noexcept {
			*this = *this * scalar;
			return *this;
		}

		tHSVColor& operator*=(const tHSVColor& color) noexcept {
			*this = *this * color;
			return *this;
		}

		tHSVColor& operator/=(T scalar) noexcept {
			*this = *this / scalar;
			return *this;
		}

		tHSVColor& operator/=(const tHSVColor& color) noexcept {
			*this = *this / color;
			return *this;
		}

	//===========================
	// 比較演算子
	//===========================

		bool operator==(const tHSVColor& color) const noexcept {
			return Equal<T>(color);
		}

		bool operator!=(const tHSVColor& color) const noexcept {
			return NotEqual<T>(color);
		}

	//===========================
	// ストリーム出力
	//===========================

		friend std::ostream& operator<<(std::ostream& stream, const tHSVColor& color) {
			stream << color.ToString();
			return stream;
		}

#pragma endregion operator

	public:

#pragma region static param

		static constexpr tHSVColor Red() noexcept {
			return tHSVColor(0.0, 1.0, 1.0, 1.0);
		}

		static constexpr tHSVColor Green() noexcept {
			return tHSVColor(0.3333, 1.0, 1.0, 1.0);
		}

		static constexpr tHSVColor Blue() noexcept {
			return tHSVColor(0.6667, 1.0, 1.0, 1.0);
		}

		static constexpr tHSVColor Cyan() noexcept {
			return tHSVColor(0.5, 1.0, 1.0, 1.0);
		}

		static constexpr tHSVColor Magenta() noexcept {
			return tHSVColor(0.8333, 1.0, 1.0, 1.0);
		}

		static constexpr tHSVColor Yellow() noexcept {
			return tHSVColor(0.1531, 0.984, 1.0, 1.0);
		}

		static constexpr tHSVColor White() noexcept {
			return tHSVColor(0.0, 0.0, 1.0, 1.0);
		}

		static constexpr tHSVColor Gray() noexcept {
			return tHSVColor(0.0, 0.0, 0.5, 1.0);
		}

		static constexpr tHSVColor Black() noexcept {
			return tHSVColor(0.0, 0.0, 0.0, 1.0);
		}

		static constexpr tHSVColor Clear() noexcept {
			return tHSVColor(0.0, 0.0, 0.0, 0.0);
		}

#pragma endregion static param

	public:

#pragma region static method

		// 色のパラメータごとに線形補間を行う
		static tHSVColor LerpParam(const tHSVColor& a, const tHSVColor& b, T t, tHSVColor::ParamType paramType = tHSVColor::ParamType::HSVA) noexcept {
			switch (paramType) {
				case ParamType::H:
				{
					return tHSVColor(Math::Lerp(a.h, b.h, t), a.s, a.v, a.a);
				}
				break;
				case ParamType::S:
				{
					return tHSVColor(a.h, Math::Lerp(a.s, b.s, t), a.v, a.a);
				}
				break;
				case ParamType::V:
				{
					return tHSVColor(a.h, a.s, Math::Lerp(a.v, b.v, t), a.a);
				}
				break;
				case ParamType::A:
				{
					return tHSVColor(a.h, a.s, a.v, Math::Lerp(a.a, b.a, t));
				}
				break;
				case ParamType::HSVA:
				{
					return tHSVColor::Lerp(a, b, t);
				}
			}
		}

		// RGBA要素の線形補間を行います　※tは0.0～1.0の間にクランプされます
		static tHSVColor Lerp(const tHSVColor& a, const tHSVColor& b, T t) noexcept {
			auto diff = b - a;
			return a + (diff * Math::Clamp01<T>(t));
		}

		// RGBA要素の線形補間を行います
		static tHSVColor LerpUnclamped(const tHSVColor& a, const tHSVColor& b, T t) noexcept {
			auto diff = b - a;
			return a + (diff * t);
		}

		// 色情報の正規化を行います
		// ※ Color(H : 0.0～1.0, G : 0.0～1.0, B : 0.0～1.0, 0.0～1.0)
		static tHSVColor Normalize(const tHSVColor& color) noexcept {
			return tHSVColor(
				color.h / 360.0,
				color.s,
				color.v,
				color.a
			);
		}

		// 色情報の非正規化を行います
		// ※ Color(R : 0.0～360.0, G : 0.0～1.0, B : 0.0～1.0, 0.0～1.0)
		static tHSVColor DeNormalize(const tHSVColor& color) noexcept {
			return tHSVColor(
				color.h * 360.0,
				color.s,
				color.v,
				color.a
			);
		}

#pragma endregion static method

	public:

#pragma region method

		// 各色の成分ごとに値を設定
		tHSVColor& Set(T newH, T newS, T newV, T newA) noexcept {
			h = newH;
			s = newS;
			v = newV;
			a = newA;
			return *this;
		}

		// 色の値をコピー
		tHSVColor& Set(const tHSVColor& newColor) noexcept {
			h = newColor.h;
			s = newColor.s;
			v = newColor.v;
			a = newColor.a;
			return *this;
		}

		// 各色の成分ごとに値を加算
		tHSVColor& Add(T addH, T addS, T addV, T addA) noexcept {
			h += addH;
			s += addS;
			v += addV;
			a += addA;
			return *this;
		}

		// 色の値を加算
		tHSVColor& Add(const tHSVColor& addColor) noexcept {
			for (int_t i = 0; i < 4; i++) {
				hsva[i] += addColor.hsva[i];
			}
			return *this;
		}

		// 各色の成分ごとに値を減算
		tHSVColor& Sub(T subH, T subS, T subV, T subA) noexcept {
			h += subH;
			s += subS;
			v += subV;
			a += subA;
			return *this;
		}

		// 色の値を減算
		tHSVColor& Sub(const tHSVColor& subColor) noexcept {
			for (int_t i = 0; i < 4; i++) {
				hsva[i] += subColor.hsva[i];
			}
			return *this;
		}

		// 各色の成分ごとに乗算
		tHSVColor& Mul(T mulH, T mulS, T mulV, T mulA) noexcept {
			h *= mulH;
			s *= mulS;
			v *= mulV;
			a *= mulA;
		}

		// 色の値を乗算
		tHSVColor& Mul(const tHSVColor& mulColor) noexcept {
			for (int_t i = 0; i < 4; i++) {
				hsva[i] *= mulColor.hsva[i];
			}
			return *this;
		}

		// 色相環における角度を取得する
		T GetHueAngle() const noexcept {
			return h * T(360.0);
		}

		// RGB色空間への変換
		template<class U = T>
		tColor<U> ToRGBColor() const noexcept {
			T hd = h * 360.0f;
			T c = v * s;
			T x = c * (T(1.0) - Math::Abs<T>(Math::Mod<T>(hd / 60.0, 2.0) - 1.0));
			T m = v - c;
			U r = v;
			U g = v;
			U b = v;

			if (Math::GreaterLessEqual<T>(0.0, hd, 60.0, 0.001)) {
				r = c;
				g = x;
				b = U(0.0);
			}
			else if (Math::GreaterLessEqual<T>(0.0, hd, 120.0, 0.001)) {
				r = x;
				g = c;
				b = U(0.0);
			}
			else if (Math::GreaterLessEqual<T>(0.0, hd, 180.0, 0.001)) {
				r = U(0.0);
				g = c;
				b = x;
			}
			else if (Math::GreaterLessEqual<T>(0.0, hd, 240.0, 0.001)) {
				r = U(0.0);
				g = x;
				b = c;
			}
			else if (Math::GreaterLessEqual<T>(0.0, hd, 300.0, 0.001)) {
				r = x;
				g = U(0.0);
				b = c;
			}
			else if (Math::GreaterLessEqual<T>(0.0, hd, 360.0, 0.001)) {
				r = c;
				g = U(0.0);
				b = x;
			}

			return tColor<U>(
				(r + m),
				(g + m),
				(b + m),
				a
				);
		}

		// データ型を変換したtHSVColor型を返す
		template<class U>
		tHSVColor<U> TypeConvert() const noexcept {
			return tHSVColor<U>(*this);
		}

		// 使用されているデータ型名を取得
		std::string TypeName() {
			return typeid(type).name();
		}

		// ベクトルの値を文字列に変換
		std::string ToString() const noexcept {
			return std::string(
				"Color( "
				+
				Utility::ToString(h)
				+
				", "
				+
				Utility::ToString(s)
				+
				", "
				+
				Utility::ToString(v)
				+
				", "
				+
				Utility::ToString(a)
				+
				" )"
			);
		}

		// 与えられた色情報がこの色情報と正確に等しい場合、trueを返します
		template<class U = T>
		bool Equal(const tHSVColor& color, U epsilon = 0.001) const noexcept {
			for (int_t i = 0; i < 4; i++) {
				if (Math::NotEqual<U>(hsva[i], color.hsva[i], epsilon)) {
					return false;
				}
			}
			return true;
		}

		// 与えられた色情報がこの色情報と正確に等しくない場合、trueを返します
		template<class U = T>
		bool NotEqual(const tHSVColor& color, U epsilon = 0.001) const noexcept {
			for (int_t i = 0; i < 4; i++) {
				if (Math::NotEqual<U>(hsva[i], color.hsva[i], epsilon)) {
					return true;
				}
			}
			return false;
		}
#pragma endregion method


	};

	using HSVColor = tHSVColor<float_t>;

}