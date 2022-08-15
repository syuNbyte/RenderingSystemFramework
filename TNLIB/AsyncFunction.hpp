#pragma once





#include"AsyncFunctionInterface.h"
#include"FutureCash.hpp"





// ���̃N���X����������ɂ͋C�t���Ȃ���������
// std::packed_task�N���X�Ǝ����悤�ȓ��������邪������̕��������o�֐��̃��b�Z�[�W���������̂ł����߂���
#pragma region async_function class



template<class Func>
class async_function {};



template<class Result, class ...FuncArgs >
class async_function<Result( FuncArgs... )> {

public:


	using this_type = async_function<Result( FuncArgs... )>;								// ���g�̃f�[�^�^
	using threadsafe_func_type = threadsafe_function<Result( FuncArgs... )>;		// ���g�ۗ̕L����X���b�h�Z�[�t�֐��N���X�̃f�[�^�^
	using func_type = std::function<Result( FuncArgs... )>;								// ���g�ۗ̕L����X���b�h�Z�[�t�֐��N���X�̊֐��I�u�W�F�N�g�^
	using result_type = Result;																	// ���g�ۗ̕L����X���b�h�Z�[�t�֐��N���X�̊֐��I�u�W�F�N�g�̖߂�l�^
	using args_type = std::tuple<FuncArgs...>;												// ���g�ۗ̕L����X���b�h�Z�[�t�֐��N���X�̊֐��I�u�W�F�N�g�̈����^���܂Ƃ߂�tuple�^
	using future_cash_type = future_cash<result_type>;										// ���ʃf�[�^���L���b�V���i�ێ��j���Ă������߂̃f�[�^�̂܂Ƃ܂�

private:


//===================================
// �����o�ϐ�
//===================================


	// �X���b�h�Z�[�t�Ȋ֐��I�u�W�F�N�g��ۗL���郁���o
	threadsafe_func_type m_threadsafe_func;

	future_cash_type m_futureCash;

public:


//===================================
// �R���X�g���N�^
//===================================


	async_function() noexcept {}


	async_function( func_type func ) {

		m_threadsafe_func = func;

	}


	this_type& operator=( const this_type& async_function ) noexcept {

		m_threadsafe_func = async_function.m_threadsafe_func;

		return *this;

	}


	this_type& operator=( this_type&& async_function ) noexcept {

		m_threadsafe_func = async_function.m_threadsafe_func;

		return *this;

	}


public:


//======================================
// operator
//======================================


	// �֐������s����@�����̃����o�֐���ʂ��Ď��s����֐��̓X���b�h�Z�[�t�ł͂Ȃ�
	result_type operator()( FuncArgs... args ) {

		return m_threadsafe_func.func_exe( args... );

	}


	// �֐����w�肵�����[���`�ŊJ�n����
	void operator()( std::launch launch, FuncArgs... args ) {

		this->operator()( threadsafe_flag::unsafe, launch, args... );

	}



	void operator()( threadsafe_flag isThreadsafe, std::launch launch, FuncArgs... args ) {

		if( isThreadsafe == threadsafe_flag::unsafe ) {

			// ���[���`��async�Ȃ�ێ�����֐��̔񓯊����s���s���Adeferred�Ȃ�ێ�����֐��̒x�����s���s��
			if( launch == std::launch::async ) {

				async( args... );

			}
			else if( launch == std::launch::deferred ) {

				deferred( args... );

			}
			else if( launch == ( std::launch::async | std::launch::deferred ) ) {

				deferred_async( args... );

			}

		}
		else {

			// ���[���`��async�Ȃ�ێ�����֐��̔񓯊����s���s���Adeferred�Ȃ�ێ�����֐��̒x�����s���s��
			if( launch == std::launch::async ) {

				threadsafe_async( args... );

			}
			else if( launch == std::launch::deferred ) {

				threadsafe_deferred( args... );

			}
			else if( launch == ( std::launch::async | std::launch::deferred ) ) {

				deferred_threadsafe_async( args... );

			}

		}

	}

public:


//===================================
// �����o�֐�
//===================================


	// �g�p����֐��I�u�W�F�N�g��ݒ肷��
	void set_func( func_type func ) {

		m_threadsafe_func = func;

	}


	// ���g�ۗ̕L����֐��I�u�W�F�N�g��n���@���������Ă��Ȃ��Ȃ�nullptr��Ԃ�
	func_type own_func() {

		return m_threadsafe_func.own_func();

	}


	// �֐������s����@�����̃����o�֐���ʂ��Ď��s����֐��̓X���b�h�Z�[�t�ł͂Ȃ�
	result_type func_exe( FuncArgs... args ) {

		return m_threadsafe_func.func_exe( args... );

	}


	// �X���b�h�Z�[�t�Ɋ֐������s����
	result_type threadsafe_func_exe( FuncArgs... args ) {

		return m_threadsafe_func.threadsafe_func_exe( args... );

	}

//======================================
// async�n
//======================================
#pragma region async



	// �֐���񓯊����s����
	// ��������s����ۂɂ͑O���async, deferred�̍ۂ̌��ʂ�get_result���\�b�h, get_future���\�b�h, get_shared_future���\�b�h���g�p���Ď擾���Ă����K�v������A�擾���Ă��Ȃ��ꍇ�O��̌��ʂ͔j�������
	void async( FuncArgs... args ) {


		m_futureCash = std::async(
			std::launch::async,
			&threadsafe_func_type::func_exe,
			&m_threadsafe_func,
			args...
		);

	}



	// �֐����X���b�h�Z�[�t�ɔ񓯊����s����
	// �����̃��\�b�h����đS�Ă��s������̓X���b�h�Z�[�t�ɓ��삷�邪�A���̃N���X�ɓn�����֐��I�u�W�F�N�g�Ɠ����֐����O���Ŏ��s�����ꍇ�͖���`�̓���ƂȂ�
	void threadsafe_async( FuncArgs... args ) {


		m_futureCash = std::async(
			std::launch::async,
			&threadsafe_func_type::threadsafe_func_exe,
			&m_threadsafe_func,
			args...
		);

	}

private:


	// deferred_async�����o�֐��ł̂݌Ă΂��֐�
	std::future<result_type> internal_async( FuncArgs... args ) {

		return std::async(
			std::launch::async,
			&threadsafe_func_type::func_exe,
			&m_threadsafe_func,
			args...
		);

	}


	// deferred_threadsafe_async�����o�֐����ł̂݌Ă΂��֐�
	std::future<result_type> internal_threadsafe_async( FuncArgs... args ) {

		return std::async(
			std::launch::async,
			&threadsafe_func_type::threadsafe_func_exe,
			&m_threadsafe_func,
			args...
		);

	}

public:


//===================================================
// defferd_async�n�֐�
//===================================================

//===================================================
// deferred_async�����o�֐�, deferred_threadsafe_async�����o�֐��̓����Ǝg����
//===================================================
// ����
//---------------------------------------------------
// 1.���̓�̃����o�֐��̎��s���ʂ�async�����o�֐�, threadsafe_async�����o�֐��Ɠ����̎��s���e�ƂȂ�
// 2.���̓�̃����o�֐��̒x�����s��deferred_async_exe�֐����g�p���čs����
// 3.���̓�̃����o�֐���deferred_async_exe�֐������s����O�Ɉȉ��̃����o�֐������s�����ꍇ�͎����I��deferred_async_exe�����s������Ɉȉ��̃����o�֐��̏����Ɉڂ�
//   ��deferred_async�n�����o�֐����g�����Ӗ����Ȃ��Ȃ�(����Ȃ�deferred�n�����o�֐������s���Ă���̂Ɠ��e�������ƂȂ邩����ȉ��i�X���b�h�쐬�̃I�[�o�[�w�b�h�������邽��)�j
// --------------------------------------------------
// get_result, get_future, get_shared_future, wait, wait_for, wait_until
// �ȏ�̃����o�֐���deferred_async_exe�����o�֐��̎��s�O�ɌĂԂ�deferred_async_exe�����o�֐��������I�ɌĂ΂��
//===================================================

	// async�����o�֐���x�����s������
	void deferred_async( FuncArgs... args ) {

		m_futureCash = std::async(
			std::launch::deferred,
			&this_type::internal_async,
			this,
			args...
		);

	}


	// threadsafe_async�����o�֐���x�����s������
	void deferred_threadsafe_async( FuncArgs... args ) {

		m_futureCash = std::async(
			std::launch::deferred,
			&this_type::internal_threadsafe_async,
			this,
			args...
		);

	}


	// deferred_async�����o�֐�, deferred_threadsafe_async�����o�֐����g�p�����ۂ�
	// �x�������Ă���񓯊������̎��s���J�n����֐�
	void deferred_async_exe() {

		m_futureCash.deferred_async_exe();

	}

#pragma endregion async


//======================================
// deferred�n
//======================================
#pragma region deferred



	// �֐���x�����s����@��std::future<T>::wait�����o�֐��Eget�����o�֐����Ă΂��܂Ŋ֐��͎��s����Ȃ�
	// ��������s����ۂɂ͑O���async, deferred�̍ۂ̌��ʂ�get_result���\�b�h, share_result���\�b�h���g�p���Ď擾���Ă����K�v������A�擾���Ă��Ȃ��ꍇ�O��̌��ʂ͔j�������
	void deferred( FuncArgs... args ) {

		m_futureCash = std::async(
			std::launch::deferred,
			&threadsafe_func_type::func_exe,
			&m_threadsafe_func,
			args...
		);

	}


	void threadsafe_deferred( FuncArgs... args ) {

		m_futureCash = std::async(
			std::launch::deferred,
			&threadsafe_func_type::threadsafe_func_exe,
			&m_threadsafe_func,
			args...
		);

	}

#pragma endregion deferred


//======================================
// async, deferred���ʂŎg�p���郁���o�֐�
//======================================
#pragma region async, deferred common method


	// ���s���ʂ�discard�i�j���j����
	[[noreturn]] void discard() {

		m_futureCash.discard();

	}


	// ���s�����֐��̖߂�l���󂯎��@�����̊֐���wait�Ɠ����̓��e�����s������Ɋ֐��̎��s���ʂ��擾���܂�
	[[nodiscard]] result_type get_result() {

		return m_futureCash.get_result();

	}


	// future_cash�^��n��
	// ���[�u�Ȃ̂ł���ȍ~�̊Ǘ��𖾂��n�����ƂɂȂ�
	[[nodiscard]] future_cash_type&& get_future_cash() {

		return std::move(m_futureCash);

	}


	// ���ʂ�ێ�����std::future��Ԃ�
	// ���̊֐����Ăяo������ɍēxasync, deferred�����s������
	[[nodiscard]] std::future<result_type>&& get_future() {

		return std::move(m_futureCash.get_future());

	}


	// get_result�̌��ʂ����L����ׂ�std::shared_future<result_type>�I�u�W�F�N�g�Ƃ��擾����
	[[nodiscard]] std::shared_future<result_type> get_shared_future() {

		return m_futureCash.get_shared_future();

	}



	// ���s�����֐��̏������I�����A�֐��̌��ʂ��擾�ł���悤�ɂȂ�܂ő҂��܂�
	void wait() {

		m_futureCash.wait();

	}


	// �w�肵�����Ԃ��o�߂����猻�݂̏����󋵂�Ԃ�
	template< class Rep, class Period >
	[[nodiscard]] std::future_status wait_for( const std::chrono::duration<Rep, Period>& timeout_duration ) {

		return m_futureCash.wait_for( timeout_duration );

	}


	// �w�肵�����ԂɂȂ����猻�݂̏����󋵂�Ԃ�
	template< class Clock, class Duration >
	[[nodiscard]] std::future_status wait_until( const std::chrono::time_point<Clock, Duration>& timeout_time ) {

		return m_futureCash.wait_until( timeout_time );

	}



#pragma endregion async, deferred common method

};





// �w���p�[�֐�
template<class Result, class ...FuncArgs>
async_function<Result( FuncArgs... )>&& make_async_function( std::function<Result( FuncArgs... )>&& function ) {

	return async_function<Result( FuncArgs... )>( function );

}



#pragma endregion async_function class









