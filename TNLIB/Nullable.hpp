#pragma once





#include<memory>
#include<optional>

namespace System {

	// 無効値を表すデータ
	using NullType = std::nullopt_t;
	constexpr std::nullopt_t null = std::nullopt;


	// null許容型
	template<class T>
	using NullableType = std::optional<T>;

	// null許容型
	template<class T>
	class Nullable {

	private:

		NullableType<T> m_value;

	public:

		Nullable() {
			m_value = std::make_optional<T>();
		};
		Nullable( const Nullable<T>& value ) {
			m_value = value.m_value;
		};
		Nullable( Nullable<T>&& value ) {
			m_value = value.m_value;
		};
		Nullable( const T& value ) {
			m_value = value;
		};
		Nullable( T&& value ) {
			m_value = value;
		};

	public:

		// 有効な値を持っている場合にtrueを返す
		operator bool() {
			return IsValid();
		}

		T* operator->() {
			return GetValue();
		}

		Nullable<T>& operator=( const T& value ) {
			m_value = value;
			return *this;
		}

		Nullable<T>& operator=( const Nullable<T>& value ) {
			m_value = value.m_value;
			return *this;
		}

	public:

		// 持っているデータが有効な場合にtrue
		virtual bool IsValid() {
			return m_value.has_value();
		}

		// 持っているデータがnullの場合にtrue
		virtual bool IsNull() {
			return !m_value.has_value();
		}

		// 使用可能な値を持っているならその値を返し
		// 無効ならnullptrを返す
		virtual T& GetValue() {
			return m_value.value();
		}

		// 値を設定
		virtual void SetValue(const T& value ) {
			m_value = value;
		}

		// 値を設定
		virtual void SetValue( const Nullable<T>& value ) {
			m_value = value.m_value;
		}

		// 値を設定
		virtual void SetValue( NullType nullType ) {
			m_value = null;
		}

	};

}