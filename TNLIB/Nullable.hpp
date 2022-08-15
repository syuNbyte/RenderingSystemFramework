#pragma once





#include<memory>
#include<optional>

namespace System {

	// �����l��\���f�[�^
	using NullType = std::nullopt_t;
	constexpr std::nullopt_t null = std::nullopt;


	// null���e�^
	template<class T>
	using NullableType = std::optional<T>;

	// null���e�^
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

		// �L���Ȓl�������Ă���ꍇ��true��Ԃ�
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

		// �����Ă���f�[�^���L���ȏꍇ��true
		virtual bool IsValid() {
			return m_value.has_value();
		}

		// �����Ă���f�[�^��null�̏ꍇ��true
		virtual bool IsNull() {
			return !m_value.has_value();
		}

		// �g�p�\�Ȓl�������Ă���Ȃ炻�̒l��Ԃ�
		// �����Ȃ�nullptr��Ԃ�
		virtual T& GetValue() {
			return m_value.value();
		}

		// �l��ݒ�
		virtual void SetValue(const T& value ) {
			m_value = value;
		}

		// �l��ݒ�
		virtual void SetValue( const Nullable<T>& value ) {
			m_value = value.m_value;
		}

		// �l��ݒ�
		virtual void SetValue( NullType nullType ) {
			m_value = null;
		}

	};

}