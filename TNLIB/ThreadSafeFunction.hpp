#pragma once





#include<mutex>
#include<functional>





//============================================================================
// �X���b�h�Z�[�t�Ȋ֐��I�u�W�F�N�g��ێ�����N���X
//============================================================================
#pragma region MultiThread Function class



// �X���b�h�Z�[�t�Ɋ֐������s����ۂɎg�p����^�O�^�@���^�O�^�͎��ۂɎg�p���邱�Ƃ͂Ȃ�
enum class threadsafe_flag {

	safe,		// �X���b�h�Z�[�t�ȏ�ԂŎ��s����
	unsafe,	// ��X���b�h�Z�[�t�ȏ�ԂŎ��s����

};





template<class Func>
class threadsafe_function {};



template<class Result, class ...FuncArgs >
class threadsafe_function<Result( FuncArgs... )> {

public:


	using this_type = threadsafe_function<Result( FuncArgs... )>;	// ���g�̃f�[�^�^
	using func_type = std::function<Result( FuncArgs... )>;			// ���g�ۗ̕L����֐��I�u�W�F�N�g�^
	using result_type = Result;												// ���g�ۗ̕L����֐��I�u�W�F�N�g�̖߂�l�^
	using args_type = std::tuple<FuncArgs...>;							// ���g�ۗ̕L����֐��I�u�W�F�N�g�̈����^���܂Ƃ߂�tuple�^

private:


	func_type m_func = nullptr;
	std::mutex m_mutex;

public:


//===================================
// �R���X�g���N�^
//===================================

	threadsafe_function() noexcept {}

	threadsafe_function( func_type func ) {

		m_func = func;

	}

public:


	this_type& operator=( const this_type& threadsafe_func ) {

		m_func = threadsafe_func.m_func;

		return *this;

	}

	this_type& operator=( this_type&& threadsafe_func ) {

		m_func = threadsafe_func.m_func;

		return *this;

	}


	this_type& operator=( func_type func ) {

		m_func = func;

		return *this;

	}


	// �֐������s����@�����̃����o�֐���ʂ��Ď��s����֐��̓X���b�h�Z�[�t�ł͂Ȃ�
	result_type operator()( FuncArgs... args ) {

		return m_func( args... );

	}


	// �X���b�h�Z�[�t�Ɋ֐������s����
	result_type operator()( threadsafe_flag isThreadsafe, FuncArgs... args ) {

		if( isThreadsafe == threadsafe_flag::unsafe ) {

			return func_exe( args... );

		}
		else {

			return threadsafe_func_exe( args... );

		}

	}

public:


//===================================
// �����o�֐�
//===================================

	// �g�p����֐��I�u�W�F�N�g��n��
	void set_func( func_type func ) {

		m_func = func;

	}


	// ���g�ۗ̕L����֐��I�u�W�F�N�g��n���@���������Ă��Ȃ��Ȃ�nullptr��Ԃ�
	func_type own_func() {

		return m_func;

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


	// �X���b�h�Z�[�t�ȏ�ԂŊ֐������s����
	result_type threadsafe_func_exe( FuncArgs... args ) {

		// RAII�N���X�Ńf�X�g���N�^��unlock�����s����N���X�̂��߃X�R�[�v�𔲂���Ǝ����I�Ƀ��b�N���������
		std::lock_guard<std::mutex> lock( m_mutex );

		return func_exe( args... );

	}

};



#pragma endregion MultiThread Function class