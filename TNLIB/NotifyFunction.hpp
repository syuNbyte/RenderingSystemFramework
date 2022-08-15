#pragma once





#include<type_traits>
#include<iostream>

#include"Notify.hpp"
#include"AsyncFunction.hpp"





// �܂�����Ă��Ȃ����牺�����
//// ����̃����o�ϐ���ێ����Ă��邩������
//struct notify_function_member_impl {
//
//	struct enable_if_m_asyncFunc {
//
//		template<class T>
//		static std::true_type check( decltype( T::m_asyncFunc )* );
//
//		template<class T>
//		static std::false_type check(...);
//
//	};
//
//	struct enable_if_m_notifyer {
//
//		template<class T>
//		static std::true_type check( decltype( T::m_notifyer )* );
//
//		template<class T>
//		static std::false_type check( ... );
//
//	};
//
//	struct enable_if_m_notifyGetter {
//
//		template<class T>
//		static std::true_type check( decltype( T::m_notifyGetter )* );
//
//		template<class T>
//		static std::false_type check( ... );
//
//	};
//
//
//
//	template<class T>
//	struct has_m_asyncFunc : public decltype( enable_if_m_asyncFunc::check<T>){};
//
//	template<class T>
//	struct has_m_notifyer : public decltype( enable_if_m_notifyer::check<T> ){};
//
//	template<class T>
//	struct has_m_notifyGetter : public decltype( enable_if_m_notifyGetter::check<T>){};
//
//	template<class T>
//	struct has_all_member : public std::conjunction<has_m_asyncFunc<T>, has_m_notifyer<T>, has_m_notifyGetter<T>>::value {};
//
//};





//============================================================================
// �֐��̎��s���ɒʒm���s���֐�
//============================================================================
#pragma region notify funcition class



template<class Func>
class notify_function {};



template<class Result, class ...FuncArgs >
class notify_function<Result( FuncArgs... )> {

public:


	using this_type = notify_function<Result( FuncArgs... )>;		// ���g�̃f�[�^�^
	using async_func_type = async_function<Result( FuncArgs... )>;	// ���g�ۗ̕L����async_function�^
	using func_type = std::function<Result( FuncArgs... )>;			// ���g�ۗ̕L����֐��I�u�W�F�N�g�^
	using result_type = Result;												// ���g�ۗ̕L����֐��I�u�W�F�N�g�̖߂�l�^
	using args_type = std::tuple<FuncArgs...>;							// ���g�ۗ̕L����֐��I�u�W�F�N�g�̈����^���܂Ƃ߂�tuple�^

private:


//===================================
// �����o�ϐ�
//===================================


	// �֐��̏��������s����async_function�^
	async_func_type m_asyncFunc;

	// �ʒm���󂯎��֐�
	notifyer m_notifyer;
	notify_getter m_notifyGetter;

public:


//===================================
// �R���X�g���N�^
//===================================

	notify_function() noexcept {}

	notify_function( func_type func ) {

		m_asyncFunc.set_func( func );

	}

	notify_function( const notifyer& notifyer, func_type func ) {

		m_notifyGetter = notifyer.get_notifyer();

		m_asyncFunc.set_func( func );

	}

	notify_function( notify_getter notify_getter, func_type func ) {

		m_notifyGetter = notify_getter;

		m_asyncFunc.set_func( func );

	}

public:


	this_type& operator=( const this_type& threadsafe_notify_func ) {

		m_asyncFunc = threadsafe_notify_func.m_asyncFunc;

		return *this;

	}

	this_type& operator=( this_type&& threadsafe_notify_func ) {

		m_asyncFunc = threadsafe_notify_func.m_asyncFunc;

		return *this;

	}


	this_type& operator=( func_type func ) {

		m_asyncFunc.set_func( func );

		return *this;

	}


	// �֐������s����@�����̃����o�֐���ʂ��Ď��s����֐��̓X���b�h�Z�[�t�ł͂Ȃ�
	result_type operator()( FuncArgs... args ) {

		return m_asyncFunc( args... );

	}

public:


//===================================
// �����o�֐�
//===================================

	// �g�p����֐��I�u�W�F�N�g��n��
	void set_func( func_type func ) {

		m_asyncFunc.set_func( func );

	}


	// ���g�ۗ̕L����֐��I�u�W�F�N�g��n���@���������Ă��Ȃ��Ȃ�nullptr��Ԃ�
	func_type own_func() {

		return m_asyncFunc.own_func();

	}


	// �֐������s����@�����̃����o�֐���ʂ��Ď��s����֐��̓X���b�h�Z�[�t�ł͂Ȃ�
	result_type func_exe( FuncArgs... args ) {


#ifdef _DEBUG

		try {

			return m_func( args... );

		}
		catch( const std::bad_function_call& error ) {

			std::cout << "no set function : " << error.what() << std::endl;

		}

#else

		return m_func( args... );

#endif

	}

//===================================================
// �ʒm�n�N���X�󂯎��֐��E�z�z�֐�
//===================================================


	// �ʒm���󂯎��N���X��ݒ肷��
	void set_notify_getter( const notify_getter& notifyGetter ) {

		m_notifyGetter = notifyGetter;

	}


	// �ʒm�𔭐M����N���X���擾����
	notify_getter& get_notifyer() {

		return m_notifyer.get_notifyer();

	}



//===================================================
// �ʒm�󂯎�����s�֐��E���s��ʒm���M�֐�
//===================================================


	// �ʒm������܂őҋ@������Ɋ֐������s����
	[[nodiscard]] result_type notify_wait_func_exe( FuncArgs... args ) {

		auto lock = m_notifyGetter.get_lock();

		m_notifyGetter.wait( lock );

		return m_asyncFunc.func_exe( args... );

	}


	// �ʒm������܂őҋ@������ɃX���b�h�Z�[�t�Ɋ֐������s����
	[[nodiscard]] result_type notify_wait_threadsafe_func_exe( FuncArgs... args ) {

		auto lock = m_notifyGetter.get_lock();

		m_notifyGetter.wait( lock );

		return m_asyncFunc.threadsafe_func_exe( args... );

	}


	// �֐������s������ɒʒm���s��
	[[nodiscard]] result_type func_exe_and_notify( FuncArgs... args ) {

		auto value = m_asyncFunc.func_exe( args... );

		m_notifyer.notify_all();

		return value;

	}


	// �X���b�h�Z�[�t�Ɋ֐������s������ɒʒm���s��
	[[nodiscard]] result_type threadsafe_func_exe_and_notify( FuncArgs... args ) {

		auto value = m_asyncFunc.threadsafe_func_exe( args... );

		m_notifyer.notify_all();

		return value;

	}


	// �ʒm������܂őҋ@������Ɋ֐������s���A�֐��̎��s�I����ɒʒm���s��
	[[nodiscard]] result_type notify_wait_func_exe_and_notify( FuncArgs... args ) {

		auto lock = m_notifyGetter.get_lock();

		m_notifyGetter.wait( lock );

		auto value = m_asyncFunc.func_exe( args... );

		m_notifyer.notify_all();

		return value;

	}


	// �ʒm������܂őҋ@������ɃX���b�h�Z�[�t�Ɋ֐������s���A�֐��̎��s�I����ɒʒm���s��
	[[nodiscard]] result_type notify_wait_threadsafe_func_exe_and_notify( FuncArgs... args ) {

		auto lock = m_notifyGetter.get_lock();

		m_notifyGetter.wait( lock );

		auto value = m_asyncFunc.threadsafe_func_exe( args... );

		m_notifyer.notify_all();

		return value;

	}

};



#pragma endregion notify funcition class





#pragma region notify_function void specialization



template<class ...FuncArgs>
class notify_function<void( FuncArgs... )> {

public:


	using this_type = notify_function<void( FuncArgs... )>;		// ���g�̃f�[�^�^
	using async_func_type = async_function<void( FuncArgs... )>;	// ���g�ۗ̕L����async_function�^
	using func_type = std::function<void( FuncArgs... )>;			// ���g�ۗ̕L����֐��I�u�W�F�N�g�^
	using result_type = void;												// ���g�ۗ̕L����֐��I�u�W�F�N�g�̖߂�l�^
	using args_type = std::tuple<FuncArgs...>;							// ���g�ۗ̕L����֐��I�u�W�F�N�g�̈����^���܂Ƃ߂�tuple�^

private:


//===================================
// �����o�ϐ�
//===================================


	// �֐��̏��������s����async_function�^
	async_func_type m_asyncFunc;

	// �ʒm���󂯎��֐�
	notifyer m_notifyer;
	notify_getter m_notifyGetter;

	// �ʒm��M�N���X��ێ����Ă��邩��ێ�����t���O�^
	bool m_ownNotifyGetter = false;

public:


//===================================
// �R���X�g���N�^
//===================================

	notify_function() noexcept {}

	notify_function( func_type func ) {

		m_asyncFunc.set_func( func );

	}

	notify_function( const notifyer& notifyer, func_type func ) {

		m_notifyGetter = notifyer.get_notifyer();

		m_asyncFunc.set_func( func );

	}

	notify_function( notify_getter notify_getter, func_type func ) {

		m_notifyGetter = notify_getter;

		m_asyncFunc.set_func( func );

	}

public:


	this_type& operator=( const this_type& threadsafe_notify_func ) {

		m_asyncFunc = threadsafe_notify_func.m_asyncFunc;

		return *this;

	}

	this_type& operator=( this_type&& threadsafe_notify_func ) {

		m_asyncFunc = threadsafe_notify_func.m_asyncFunc;

		return *this;

	}


	this_type& operator=( func_type func ) {

		m_asyncFunc.set_func( func );

		return *this;

	}


	// �֐������s����@�����̃����o�֐���ʂ��Ď��s����֐��̓X���b�h�Z�[�t�ł͂Ȃ�
	result_type operator()( FuncArgs... args ) {

		return m_asyncFunc( args... );

	}

public:


//===================================
// �����o�֐�
//===================================

	// �g�p����֐��I�u�W�F�N�g��n��
	void set_func( func_type func ) {

		m_asyncFunc.set_func( func );

	}


	// ���g�ۗ̕L����֐��I�u�W�F�N�g��n���@���������Ă��Ȃ��Ȃ�nullptr��Ԃ�
	func_type own_func() {

		return m_asyncFunc.own_func();

	}


	// �֐������s����@�����̃����o�֐���ʂ��Ď��s����֐��̓X���b�h�Z�[�t�ł͂Ȃ�
	result_type func_exe( FuncArgs... args ) {


#ifdef _DEBUG

		try {

			return m_func( args... );

		}
		catch( const std::bad_function_call& error ) {

			std::cout << "no set function : " << error.what() << std::endl;

		}

#else

		return m_func( args... );

#endif

	}

//===================================================
// �ʒm�n�N���X�󂯎��֐��E�z�z�֐�
//===================================================


	// �ʒm���󂯎��N���X��ݒ肷��
	void set_notify_getter( const notify_getter& notifyGetter ) {

		m_notifyGetter = notifyGetter;

	}


	void set_notify_getter( const notifyer& notifyer ) {

		m_notifyGetter = notifyer.get_notifyer();

	}


	// �ʒm�𔭐M����N���X���擾����
	notify_getter&& get_notifyer() {

		return m_notifyer.get_notifyer();

	}



//===================================================
// �ʒm�󂯎�����s�֐��E���s��ʒm���M�֐�
//===================================================


	// �ʒm������܂ő҂�����Ɋ֐������s����
	result_type notify_wait_func_exe( FuncArgs... args ) {

		wait_object wait_obj( m_notifyGetter );

		m_asyncFunc.func_exe( args... );

	}


	// �ʒm������܂ő҂�����ɃX���b�h�Z�[�t�Ɋ֐������s����
	result_type notify_wait_threadsafe_func_exe( FuncArgs... args ) {

		wait_object wait_obj( m_notifyGetter );

		m_asyncFunc.threadsafe_func_exe( args... );

	}


	// �֐������s������ɒʒm���s��
	[[noreturn]] result_type func_exe_and_notify( FuncArgs... args ) {

		m_asyncFunc.func_exe( args... );

		m_notifyer.notify_all();

	}


	// �X���b�h�Z�[�t�Ɋ֐������s������ɒʒm���s��
	[[noreturn]] result_type threadsafe_func_exe_and_notify( FuncArgs... args ) {

		m_asyncFunc.threadsafe_func_exe( args... );

		m_notifyer.notify_all();

	}


	// �ʒm������܂őҋ@������Ɋ֐������s���A�֐��̎��s�I����ɒʒm���s��
	[[noreturn]] result_type notify_wait_func_exe_and_notify( FuncArgs... args ) {

		auto lock = m_notifyGetter.get_lock();

		m_notifyGetter.wait( lock );

		m_asyncFunc.func_exe( args... );

		m_notifyer.notify_all();

	}


	// �ʒm������܂őҋ@������ɃX���b�h�Z�[�t�Ɋ֐������s���A�֐��̎��s�I����ɒʒm���s��
	[[noreturn]] result_type notify_wait_threadsafe_func_exe_and_notify( FuncArgs... args ) {

		auto lock = m_notifyGetter.get_lock();

		m_notifyGetter.wait( lock );

		m_asyncFunc.threadsafe_func_exe( args... );

		m_notifyer.notify_all();

	}


};



#pragma endregion notify_function void specialization





#pragma region helper function



// �ʒm��҂��Ă���֐��̎��s���s���@�����̊֐��͑������s����
template<class Func, class Result = std::invoke_result_t<Func>, class ...Args >
std::invoke_result_t<Func> notify_wait_func_exe( notifyer& notifyer, Func func, Args... args ) {

	wait_object wait_obj( notifyer );

	return func(args...);

}



//// �ʒm��҂��Ă���֐��̎��s���s��
//template<class Func, class Result = std::invoke_result_t<Func>, class ...Args >
//future_cash<Result>&& notify_wait_async_func_exe( notifyer& notifyer, Func func, Args... args ) {
//
//	async_function<Result(void)>async_func([ & ] {
//
//		wait_object wait_obj( notifyer );
//
//		return func(args...);
//
//	} );
//
//	async_func.async();
//
//	return std::move(async_func;
//
//}



#pragma endregion helper function










