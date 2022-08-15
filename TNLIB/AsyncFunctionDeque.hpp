#pragma once





#include<deque>
#include<optional>
#include<cinttypes>

#include"AsyncFunctionInterface.h"
#include"FutureCash.hpp"





// async_function�N���X�̊g���N���X
// async_function�ł͂ł��Ȃ����������֐��𕡐�����s�����ۂɕ����̌��ʂ�ێ��ł���N���X��deque�̕������̗p���Ă���

// C++20�Ŏ�������Ă���STL std::bind_front�̋@�\���g�p�����async_function�N���X���p�����邱�ƂŎ������邱�Ƃ��ł�����
// ���ꂪ�g���Ȃ��̂ŕʃN���X�Ƃ��Ē�`���Ă���
#pragma region async_function_deque



template<class Func>
class async_function_deque;



template<class Result, class ...FuncArgs>
class async_function_deque<Result( FuncArgs... )> : public async_function_interface<Result( FuncArgs... )> {

public:


	using this_type = async_function_deque<Result( FuncArgs... )>;										// ���g�̃f�[�^�^
	using threadsafe_func_type = threadsafe_function<Result( FuncArgs... )>;						// ���g�ۗ̕L����X���b�h�Z�[�t�֐��N���X�̃f�[�^�^
	using func_type = std::function<Result( FuncArgs... )>;												// ���g�ۗ̕L����X���b�h�Z�[�t�֐��N���X�̊֐��I�u�W�F�N�g�^
	using result_type = Result;																					// ���g�ۗ̕L����X���b�h�Z�[�t�֐��N���X�̊֐��I�u�W�F�N�g�̖߂�l�^
	using args_type = std::tuple<FuncArgs...>;																// ���g�ۗ̕L����X���b�h�Z�[�t�֐��N���X�̊֐��I�u�W�F�N�g�̈����^���܂Ƃ߂�tuple�^
	using future_cash_type = future_cash<result_type>;														// ���ʃf�[�^���L���b�V���i�ێ��j���Ă������߂̃f�[�^�̂܂Ƃ܂�
	using deque_type = std::deque<future_cash_type>;														// ���g�̊֐��̎��s���ʂ�ێ�����deque�^
	using iterator = typename deque_type::iterator;															// ���g���ێ�����deque�^�̃C�e���[�^
	using const_iterator = typename deque_type::const_iterator;											// ���g���ێ�����deque�^�̓ǂݎ���p�C�e���[�^
	using reverse_iterator = typename deque_type::reverse_iterator;									// ���g���ێ�����deque�^�̋t�C�e���[�^
	using const_reverse_iterator = typename deque_type::const_reverse_iterator;					// ���g���ێ�����deque�^�̓ǂݎ���p�t�C�e���[�^

private:

	// �X���b�h�Z�[�t�Ȋ֐��I�u�W�F�N�g��ۗL���郁���o
	threadsafe_func_type m_threadsafe_func;

	// async, deferred�����s�����ۂ̖߂�l��future��ێ����郁���o
	deque_type m_futureDeque;

public:


	async_function_deque() noexcept {}


	async_function_deque( func_type func ) {

		m_threadsafe_func = func;

	}


	this_type& operator=( const this_type& async_function_deque ) noexcept {

		m_threadsafe_func = async_function_deque.m_threadsafe_func;

		return *this;

	}


	this_type& operator=( this_type&& async_function_deque ) noexcept {

		m_threadsafe_func = async_function_deque.m_threadsafe_func;

		return *this;

	}

public:


//======================================
// operator
//======================================


	// �֐������s����@�����̃����o�֐���ʂ��Ď��s����֐��̓X���b�h�Z�[�t�ł͂Ȃ�
	result_type operator()( FuncArgs... args ) override {

		return m_threadsafe_func.func_exe( args... );

	}


	// �֐����w�肵�����[���`�ŊJ�n����
	void operator()( std::launch launch, FuncArgs... args ) override {

		this->operator()( threadsafe_flag::unsafe, launch, args... );

	}



	void operator()( threadsafe_flag isThreadsafe, std::launch launch, FuncArgs... args ) override {

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
	void set_func( func_type func ) override {

		m_threadsafe_func = func;

	}


	// ���g�ۗ̕L����֐��I�u�W�F�N�g��n���@���������Ă��Ȃ��Ȃ�nullptr��Ԃ�
	func_type own_func() override {

		return m_threadsafe_func.own_func();

	}


	// �֐������s����@�����̃����o�֐���ʂ��Ď��s����֐��̓X���b�h�Z�[�t�ł͂Ȃ�
	result_type func_exe( FuncArgs... args ) override {

		return m_threadsafe_func.func_exe( args... );

	}


	// �X���b�h�Z�[�t�Ɋ֐������s����
	result_type threadsafe_func_exe( FuncArgs... args ) override {

		return m_threadsafe_func.threadsafe_func_exe( args... );

	}

//======================================
// �v�f�A�N�Z�X�E�v�f�̑���
//======================================
#pragma region access


	// �v�f�ɎQ�ƃA�N�Z�X���s���@��������̕�������
	future_cash_type& operator[]( uint32_t index ) {

		return m_futureDeque[index];

	}


	// �v�f�Ɉ��S�ɎQ�ƃA�N�Z�X���s���@��������̕����ᑬ�Ŕ͈͊O�A�N�Z�X���s�����ꍇout_of_range�G���[�𑗏o����
	future_cash_type& at( uint32_t index ) {

		return m_futureDeque.at( index );

	}


	// �擪�v�f�֎Q�ƃA�N�Z�X���s��
	future_cash_type& front() {

		return m_futureDeque.front();

	}


	// �����v�f�֎Q�ƃA�N�Z�X���s��
	future_cash_type& back() {

		return m_futureDeque.back();

	}


	// �擪�v�f���폜
	void pop_front() {

		m_futureDeque.pop_front();
		
	}


	// �����v�f���폜
	void pop_back() {

		m_futureDeque.pop_back();

	}


	// �擪�v�f���擾���폜
	future_cash_type& get_and_pop_front() {

		auto& front_data = m_futureDeque.front();

		m_futureDeque.pop_front();

		return front_data;

	}


	// �����v�f���擾���폜
	future_cash_type& get_and_pop_back() {

		auto& back_data = m_futureDeque.back();

		m_futureDeque.pop_back();

		return back_data;

	}


	// ���݂̗v�f�����擾
	size_t size() const noexcept {

		return m_futureDeque.size();

	}


	// �v�f�̗L�����m�F����
	bool empty() const noexcept {

		return m_futureDeque.empty();

	}



	// �w�肵�����ʂ��i�[����Ă��邩�ǂ����𔻒肵���ʂ��i�[����Ă���Ȃ��ԍŏ��Ɍ��������f�[�^���i�[����std::optional�^��Ԃ�
	// �������ʂ����݂��Ȃ��ꍇ�͖����l�̊i�[���ꂽstd::optional�^�istd::nullopt�j��Ԃ�
	[[nodiscard]] std::optional<result_type> find( result_type find_value ) {

		// ��i�m�b�istd::chrono���g�p�ł���ŏ��̎��ԁj���g�p����
		return find_wait_for( find_value, std::chrono::nanoseconds( 1 ) );

	}


	// �w�肵�����ʂ��i�[����Ă��邩�ǂ����𔻒肵���ʂ��i�[����Ă���Ȃ��ԍŏ��Ɍ����������̃f�[�^���i�[����std::optional�^�Ԃ�
	// �������ʂ����݂��Ȃ��ꍇ�͖����l�̊i�[���ꂽstd::optional�^�istd::nullopt�j��Ԃ�
	// ���̃����o�֐��͈�x���ʂ��擾���邽�߂Ɉȉ��̎��Ԃ�v����
	// �v�Z���ԁF�w�肵������ * �v�f�����̏����@�����̗v�f���������I�����Ă���ꍇ�ɂ͑������ʂ��擾����邽�ߎ��s����֐��I�u�W�F�N�g�̏������x�Ɉˑ�����
	template< class Rep, class Period >
	[[nodiscard]] std::optional<result_type> find_wait_for( result_type find_value, const std::chrono::duration<Rep, Period>& timeout_duration ) {


		// ��ԍŏ��Ɍ������������l�Ɠ��l�̕��̃C�e���[�^��Ԃ�
		auto iter = std::find_if(
			m_futureDeque.begin(),
			m_futureDeque.end(),
			[ & ]( future_cash_type& value ) {

				auto state = value.get_state();


				if( state == future_cash_status::has_result_state::has_future && value.get_detail_state_wait_for( timeout_duration ) == future_cash_status::has_result_detail_state::has_future_ready ) {

					return find_value == value.get_result();

				}
				else if( state == future_cash_status::has_result_state::has_shared_future && value.get_detail_state_wait_for( timeout_duration ) == future_cash_status::has_result_detail_state::has_shared_future_ready ) {

					return find_value == value.get_result();

				}
				else if( state == future_cash_status::has_result_state::has_result ) {

					return find_value == value.get_result();

				}
				else {

					return false;

				}

			}
		);


		// �������ʂ��󂯎����Ԃ̏ꍇ��
		// ���ʂ��󂯎��A���̌��ʂ�ێ����Ă���future_cash�^���폜����
		if( iter != m_futureDeque.end() ) {

			auto result = iter->get_result();

			m_futureDeque.erase( iter );

			return result;

		}
		else {

			return std::nullopt;

		}

	}



	template<class Clock, class Duration>
	[[nodiscard]] std::optional<result_type> find_wait_until( result_type find_value, const std::chrono::time_point<Clock, Duration>& timeout_time ) {

		// �w�肳�ꂽ���ԂɂȂ�܂Ō��s�̃X���b�h���~���đ҂�
		std::this_thread::sleep_until( timeout_time );


		// �w�肳�ꂽ���Ԃ𒴂����猟�����J�n����
		return find( find_value );

	}


#pragma region iterator


	// ���g�̕ێ�����deque�^�̐擪�v�f���w���C�e���[�^��Ԃ�
	iterator begin() {

		return m_futureDeque.begin();

	}


	// ���g�̕ێ�����deque�^�̖����v�f���w���C�e���[�^��Ԃ�
	iterator end() {

		return m_futureDeque.end();

	}


	// ���g�̕ێ�����deque�^�̐擪�v�f���w���ǂݎ��C�e���[�^��Ԃ�
	const_iterator cbegin() const {

		return m_futureDeque.cbegin();

	}


	// ���g�̕ێ�����deque�^�̖����v�f���w���ǂݎ��C�e���[�^��Ԃ�
	const_iterator cend() const {

		return m_futureDeque.cend();

	}


	// ���g�̕ێ�����deque�^�̐擪�v�f���w���C�e���[�^��Ԃ�
	reverse_iterator rbegin() {

		return m_futureDeque.rbegin();

	}


	// ���g�̕ێ�����deque�^�̖����v�f���w���C�e���[�^��Ԃ�
	reverse_iterator rend() {

		return m_futureDeque.rend();

	}


	// ���g�̕ێ�����deque�^�̖����v�f���w���ǂݎ��C�e���[�^��Ԃ�
	const_reverse_iterator crbegin() const {

		return m_futureDeque.crbegin();

	}

	// ���g�̕ێ�����deque�^�̐擪�v�f���w���ǂݎ��C�e���[�^��Ԃ�

	const_reverse_iterator crend() const {

		return m_futureDeque.crend();

	}



#pragma endregion iterator



#pragma endregion access


//======================================
// async�n
//======================================
#pragma region async



	// �֐���񓯊����s����
	void async( FuncArgs... args ) override {

		m_futureDeque.emplace_back(

			std::async(
				std::launch::async,
				&threadsafe_func_type::func_exe,
				&m_threadsafe_func,
				args...
			)

		);

	}



	void threadsafe_async( FuncArgs... args ) {

		m_futureDeque.emplace_back(

			std::async(
				std::launch::async,
				&threadsafe_func_type::threadsafe_func_exe,
				&m_threadsafe_func,
				args...
			)

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
// deferred_async�n�֐�
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

		m_futureDeque.emplace_back(

			std::async(
				std::launch::deferred,
				&this_type::internal_async,
				this,
				args...
			)

		);

	}


	// threadsafe_async�����o�֐���x�����s������
	void deferred_threadsafe_async( FuncArgs... args ) {

		m_futureDeque.emplace_back(

			std::async(
				std::launch::deferred,
				&this_type::internal_threadsafe_async,
				this,
				args...
			)

		);

	}


	// deferred_async�����o�֐�, deferred_threadsafe_async�����o�֐����g�p�����ۂ�
	// �x�������Ă���񓯊������̎��s���J�n����֐�
	void deferred_async_exe( uint32_t index ) {

		m_futureDeque[index].deferred_async_exe();

	}


	// deferred_async�����o�֐�, deferred_threadsafe_async�����o�֐��ɂ���Ēx�����s����A�܂����s����Ă��Ȃ����̂�S�Ď��s��ԂɈڍs������
	void deferred_async_exe_all() {


		for( auto&& _future_cash : m_futureDeque ) {

			// ���s�҂���future_cash��S�Ď��s��ԂɈڍs����
			if( _future_cash.get_state() == future_cash_status::has_result_state::deferred_async ) {

				_future_cash.deferred_async_exe();

			}

		}


	}


#pragma endregion async


//======================================
// deferred�n
//======================================
#pragma region deferred



	// �֐���x�����s����@��std::future<T>::wait�����o�֐��Eget�����o�֐����Ă΂��܂Ŋ֐��͎��s����Ȃ�
	// ��������s����ۂɂ͑O���async, deferred�̍ۂ̌��ʂ�get_result���\�b�h, share_result���\�b�h���g�p���Ď擾���Ă����K�v������A�擾���Ă��Ȃ��ꍇ�O��̌��ʂ͔j�������
	void deferred( FuncArgs... args ) override {

		m_futureDeque.emplace_back(

			std::async(
				std::launch::deferred,
				&threadsafe_func_type::func_exe,
				&m_threadsafe_func,
				args...
			)

		);

	}


	void threadsafe_deferred( FuncArgs... args ) {


		m_futureDeque.emplace_back(

			std::async(
				std::launch::deferred,
				&threadsafe_func_type::threadsafe_func_exe,
				&m_threadsafe_func,
				args...
			)

		);

	}

#pragma endregion deferred


//======================================
// async, deferred���ʂŎg�p���郁���o�֐�
//======================================
#pragma region async, deferred common method



#pragma region discard method



	// ���s���ʂ�discard�i�j���j����
	void discard( uint32_t index ) {

		m_futureDeque.erase( std::next(m_futureDeque.begin(), index));

	}


	// �w�肵���͈͂̎��s���ʂ�discard�i�j���j����
	void discard_range( uint32_t beginIndex, uint32_t endIndex ) {

		m_futureDeque.erase(
			std::next( m_futureDeque.begin(), beginIndex ),
			std::next( m_futureDeque.begin(), endIndex )
		);

	}


	// �S�Ă̎��s���ʂ�discard�i�j���j����
	void discard_all() {

		m_futureDeque.clear();

	}


	// �w�肵�������ɍ��v����v�f��discard�i�j���j����
	// ��predicate�Ƃ͔��f����֐��Ƃ����Ӗ�
	void discard_if( std::function<bool(future_cash_type&)> predicate ) {

		// �폜�͈͂̐擪�C�e���[�^
		// ���폜�͈͂������ɏW�񂳂��̂ŁA���̐擪�C�e���[�^���疖���C�e���[�^�܂ł͈̔͂��폜����Ƃ悢
		auto discard_begin_iter = std::remove_if( 
			m_futureDeque.begin(),
			m_futureDeque.end(),
			predicate
		);

		// �폜������
		auto discard_num = std::distance( 
			discard_begin_iter,
			m_futureDeque.end()
		);

		//�͈͂��폜
		m_futureDeque.erase(
			discard_begin_iter,
			m_futureDeque.end()
		);


	}



#pragma endregion discard method



#pragma region get member data method



	// ���s�����֐��̖߂�l���󂯎��@�����̊֐���wait�Ɠ����̓��e�����s������Ɋ֐��̎��s���ʂ��擾���܂�
	// ���̊֐��͎擾�������֐��̎��s���I�����Ă��Ȃ��ꍇ
	[[nodiscard]] result_type get_result( uint32_t index ) {

		auto future_result = m_futureDeque[index].get_result();

		return future_result;

	}


	// ���s�����֐��̖߂�l���󂯎��A���̊֐��̎��s���ʂ̃f�[�^���폜����
	[[nodiscard]] result_type get_result_and_discard( uint32_t index ) {

		auto future_result = m_futureDeque[index].get_result();

		discard( index );

		return future_result;

	}


	// ���ʂ�ێ�����fuutre_cash�^��Ԃ�
	[[nodiscard]] future_cash_type&& get_future_cash( uint32_t index ) {

		return std::move(m_futureDeque[index]);

	}


	// ���ʂ�ێ�����std::future��Ԃ�
	[[nodiscard]] std::future<result_type> get_future( uint32_t index ) {

		auto future_result = std::move(m_futureDeque[index].get_future());

		discard( index );

		return future_result;

	}


	// get_result�̌��ʂ����L����ׂ�std::shared_future<result_type>�I�u�W�F�N�g�Ƃ��擾����
	[[nodiscard]] std::shared_future<result_type> get_shared_future( uint32_t index ) {

		return m_futureDeque[index].get_shared_future();

	}



#pragma endregion get member data method



#pragma region wait method



	// ���s�����֐��̏������I�����A�֐��̌��ʂ��擾�ł���悤�ɂȂ�܂ŏ�����҂��܂�
	void wait( uint32_t index ) {

		m_futureDeque[index].wait();

	}


	// ���ݎ��s����Ă��邷�ׂĂ̊֐��̏������I������܂ŏ�����҂��܂�
	void all_wait() {

		for( auto&& _future_cash : m_futureDeque ) {

			_future_cash.wait();

		}

	}



	// �w�肵�����Ԃ��o�߂����猻�݂̏����󋵂�Ԃ�
	template< class Rep, class Period >
	[[nodiscard]] std::future_status wait_for( uint32_t index, const std::chrono::duration<Rep, Period>& timeout_duration ) {

		/*============================================================================
		// �Q�l�T�C�g�Fhttps://ja.cppreference.com/w/cpp/thread/future/wait_for
		//============================================================================
		// ����
		//============================================================================
		// future_status::deferred	: ���ʂ��v�Z���邽�߂̊֐��͂܂��J�n����Ă��܂���
		// future_status::ready		: ���ʂ͏����������Ă��܂�
		// future_status::timeout	: �^�C���A�E�g���o�߂��܂���
		============================================================================*/

		return m_futureDeque[index].wait_for( timeout_duration );

	}


	// �w�肵�����ԂɂȂ����猻�݂̏����󋵂�Ԃ�
	template< class Clock, class Duration >
	[[nodiscard]] std::future_status wait_until( uint32_t index, const std::chrono::time_point<Clock, Duration>& timeout_time ) {

		/*============================================================================
		// �Q�l�T�C�g�Fhttps://ja.cppreference.com/w/cpp/thread/future/wait_until
		//============================================================================
		// ����
		//============================================================================
		// future_status::deferred	: ���ʂ��v�Z���邽�߂̊֐��͂܂��J�n����Ă��܂���
		// future_status::ready		: ���ʂ͏����������Ă��܂�
		// future_status::timeout	: �^�C���A�E�g���o�߂��܂���
		============================================================================*/

		return m_futureDeque[index].wait_until( timeout_time );

	}



#pragma endregion wait method



#pragma endregion async, deferred common method

};



#pragma endregion async_function_deque





#pragma region async_function_deque void specialization



template<class ...FuncArgs>
class async_function_deque<void( FuncArgs... )> : public async_function_interface<void( FuncArgs... )> {

public:


	using this_type = async_function_deque<void( FuncArgs... )>;										// ���g�̃f�[�^�^
	using threadsafe_func_type = threadsafe_function<void( FuncArgs... )>;							// ���g�ۗ̕L����X���b�h�Z�[�t�֐��N���X�̃f�[�^�^
	using func_type = std::function<void( FuncArgs... )>;													// ���g�ۗ̕L����X���b�h�Z�[�t�֐��N���X�̊֐��I�u�W�F�N�g�^
	using result_type = void;																						// ���g�ۗ̕L����X���b�h�Z�[�t�֐��N���X�̊֐��I�u�W�F�N�g�̖߂�l�^
	using args_type = std::tuple<FuncArgs...>;																// ���g�ۗ̕L����X���b�h�Z�[�t�֐��N���X�̊֐��I�u�W�F�N�g�̈����^���܂Ƃ߂�tuple�^
	using future_cash_type = future_cash<result_type>;														// ���ʃf�[�^���L���b�V���i�ێ��j���Ă������߂̃f�[�^�̂܂Ƃ܂�
	using deque_type = std::deque<future_cash_type>;														// ���g�̊֐��̎��s���ʂ�ێ�����deque�^
	using iterator = typename deque_type::iterator;															// ���g���ێ�����deque�^�̃C�e���[�^
	using const_iterator = typename deque_type::const_iterator;											// ���g���ێ�����deque�^�̓ǂݎ���p�C�e���[�^
	using reverse_iterator = typename deque_type::reverse_iterator;									// ���g���ێ�����deque�^�̋t�C�e���[�^
	using const_reverse_iterator = typename deque_type::const_reverse_iterator;					// ���g���ێ�����deque�^�̓ǂݎ���p�t�C�e���[�^

private:

	// �X���b�h�Z�[�t�Ȋ֐��I�u�W�F�N�g��ۗL���郁���o
	threadsafe_func_type m_threadsafe_func;

	// async, deferred�����s�����ۂ̖߂�l��future��ێ����郁���o
	deque_type m_futureDeque;

public:


	async_function_deque() noexcept {}


	async_function_deque( func_type func ) {

		m_threadsafe_func = func;

	}


	this_type& operator=( const this_type& async_function_deque ) noexcept {

		m_threadsafe_func = async_function_deque.m_threadsafe_func;

		return *this;

	}


	this_type& operator=( this_type&& async_function_deque ) noexcept {

		m_threadsafe_func = async_function_deque.m_threadsafe_func;

		return *this;

	}

public:


//======================================
// operator
//======================================


	// �֐������s����@�����̃����o�֐���ʂ��Ď��s����֐��̓X���b�h�Z�[�t�ł͂Ȃ�
	result_type operator()( FuncArgs... args ) override {

		return m_threadsafe_func.func_exe( args... );

	}


	// �֐����w�肵�����[���`�ŊJ�n����
	void operator()( std::launch launch, FuncArgs... args ) override {

		this->operator()( threadsafe_flag::unsafe, launch, args... );

	}



	void operator()( threadsafe_flag isThreadsafe, std::launch launch, FuncArgs... args ) override {

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
	void set_func( func_type func ) override {

		m_threadsafe_func = func;

	}


	// ���g�ۗ̕L����֐��I�u�W�F�N�g��n���@���������Ă��Ȃ��Ȃ�nullptr��Ԃ�
	func_type own_func() override {

		return m_threadsafe_func.own_func();

	}


	// �֐������s����@�����̃����o�֐���ʂ��Ď��s����֐��̓X���b�h�Z�[�t�ł͂Ȃ�
	result_type func_exe( FuncArgs... args ) override {

		return m_threadsafe_func.func_exe( args... );

	}


	// �X���b�h�Z�[�t�Ɋ֐������s����
	result_type threadsafe_func_exe( FuncArgs... args ) override {

		return m_threadsafe_func.threadsafe_func_exe( args... );

	}

//======================================
// �v�f�A�N�Z�X�E�v�f�̑���
//======================================
#pragma region access


	// �v�f�ɎQ�ƃA�N�Z�X���s���@��������̕�������
	future_cash_type& operator[]( uint32_t index ) {

		return m_futureDeque[index];

	}


	// �v�f�Ɉ��S�ɎQ�ƃA�N�Z�X���s���@��������̕����ᑬ�Ŕ͈͊O�A�N�Z�X���s�����ꍇout_of_range�G���[�𑗏o����
	future_cash_type& at( uint32_t index ) {

		return m_futureDeque.at( index );

	}


	// �擪�v�f�֎Q�ƃA�N�Z�X���s��
	future_cash_type& front() {

		return m_futureDeque.front();

	}


	// �����v�f�֎Q�ƃA�N�Z�X���s��
	future_cash_type& back() {

		return m_futureDeque.back();

	}


	// �擪�v�f���폜
	void pop_front() {

		m_futureDeque.pop_front();

	}


	// �����v�f���폜
	void pop_back() {

		m_futureDeque.pop_back();

	}


	// �擪�v�f���擾���폜
	future_cash_type& get_and_pop_front() {

		auto& front_data = m_futureDeque.front();

		m_futureDeque.pop_front();

		return front_data;

	}


	// �����v�f���擾���폜
	future_cash_type& get_and_pop_back() {

		auto& back_data = m_futureDeque.back();

		m_futureDeque.pop_back();

		return back_data;

	}


	// ���݂̗v�f�����擾
	size_t size() const noexcept {

		return m_futureDeque.size();

	}


	// �v�f�̗L�����m�F����
	bool empty() const noexcept {

		return m_futureDeque.empty();

	}



#pragma region iterator


	// ���g�̕ێ�����deque�^�̐擪�v�f���w���C�e���[�^��Ԃ�
	iterator begin() {

		return m_futureDeque.begin();

	}


	// ���g�̕ێ�����deque�^�̖����v�f���w���C�e���[�^��Ԃ�
	iterator end() {

		return m_futureDeque.end();

	}


	// ���g�̕ێ�����deque�^�̐擪�v�f���w���ǂݎ��C�e���[�^��Ԃ�
	const_iterator cbegin() const {

		return m_futureDeque.cbegin();

	}


	// ���g�̕ێ�����deque�^�̖����v�f���w���ǂݎ��C�e���[�^��Ԃ�
	const_iterator cend() const {

		return m_futureDeque.cend();

	}


	// ���g�̕ێ�����deque�^�̐擪�v�f���w���C�e���[�^��Ԃ�
	reverse_iterator rbegin() {

		return m_futureDeque.rbegin();

	}


	// ���g�̕ێ�����deque�^�̖����v�f���w���C�e���[�^��Ԃ�
	reverse_iterator rend() {

		return m_futureDeque.rend();

	}


	// ���g�̕ێ�����deque�^�̖����v�f���w���ǂݎ��C�e���[�^��Ԃ�
	const_reverse_iterator crbegin() const {

		return m_futureDeque.crbegin();

	}

	// ���g�̕ێ�����deque�^�̐擪�v�f���w���ǂݎ��C�e���[�^��Ԃ�

	const_reverse_iterator crend() const {

		return m_futureDeque.crend();

	}



#pragma endregion iterator



#pragma endregion access


//======================================
// async�n
//======================================
#pragma region async



	// �֐���񓯊����s����
	void async( FuncArgs... args ) override {

		m_futureDeque.emplace_back(

			std::async(
				std::launch::async,
				&threadsafe_func_type::func_exe,
				&m_threadsafe_func,
				args...
			)

		);

	}



	void threadsafe_async( FuncArgs... args ) {

		m_futureDeque.emplace_back(

			std::async(
				std::launch::async,
				&threadsafe_func_type::threadsafe_func_exe,
				&m_threadsafe_func,
				args...
			)

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
// deferred_async�n�֐�
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

		m_futureDeque.emplace_back(

			std::async(
				std::launch::deferred,
				&this_type::internal_async,
				this,
				args...
			)

		);

	}


	// threadsafe_async�����o�֐���x�����s������
	void deferred_threadsafe_async( FuncArgs... args ) {

		m_futureDeque.emplace_back(

			std::async(
				std::launch::deferred,
				&this_type::internal_threadsafe_async,
				this,
				args...
			)

		);

	}


	// deferred_async�����o�֐�, deferred_threadsafe_async�����o�֐����g�p�����ۂ�
	// �x�������Ă���񓯊������̎��s���J�n����֐�
	void deferred_async_exe( uint32_t index ) {

		m_futureDeque[index].deferred_async_exe();

	}


	// deferred_async�����o�֐�, deferred_threadsafe_async�����o�֐��ɂ���Ēx�����s����A�܂����s����Ă��Ȃ����̂�S�Ď��s��ԂɈڍs������
	void deferred_async_exe_all() {


		for( auto&& _future_cash : m_futureDeque ) {

			// ���s�҂���future_cash��S�Ď��s��ԂɈڍs����
			if( _future_cash.get_state() == future_cash_status::has_result_state::deferred_async ) {

				_future_cash.deferred_async_exe();

			}

		}


	}


#pragma endregion async


//======================================
// deferred�n
//======================================
#pragma region deferred



	// �֐���x�����s����@��std::future<T>::wait�����o�֐��Eget�����o�֐����Ă΂��܂Ŋ֐��͎��s����Ȃ�
	// ��������s����ۂɂ͑O���async, deferred�̍ۂ̌��ʂ�get_result���\�b�h, share_result���\�b�h���g�p���Ď擾���Ă����K�v������A�擾���Ă��Ȃ��ꍇ�O��̌��ʂ͔j�������
	void deferred( FuncArgs... args ) {

		m_futureDeque.emplace_back(

			std::async(
				std::launch::deferred,
				&threadsafe_func_type::func_exe,
				&m_threadsafe_func,
				args...
			)

		);

	}


	void threadsafe_deferred( FuncArgs... args ) {


		m_futureDeque.emplace_back(

			std::async(
				std::launch::deferred,
				&threadsafe_func_type::threadsafe_func_exe,
				&m_threadsafe_func,
				args...
			)

		);

	}

#pragma endregion deferred


//======================================
// async, deferred���ʂŎg�p���郁���o�֐�
//======================================
#pragma region async, deferred common method



#pragma region discard method



	// ���s���ʂ�discard�i�j���j����
	void discard( uint32_t index ) {

		m_futureDeque.erase( std::next( m_futureDeque.begin(), index ) );

	}


	// �w�肵���͈͂̎��s���ʂ�discard�i�j���j����
	void discard_range( uint32_t beginIndex, uint32_t endIndex ) {

		m_futureDeque.erase(
			std::next( m_futureDeque.begin(), beginIndex ),
			std::next( m_futureDeque.begin(), endIndex )
		);

	}


	// �S�Ă̎��s���ʂ�discard�i�j���j����
	void discard_all() {

		m_futureDeque.clear();

	}


	// �w�肵�������ɍ��v����v�f��discard�i�j���j����
	// ��predicate�Ƃ͔��f����֐��Ƃ����Ӗ�
	void discard_if( std::function<bool( future_cash_type& )> predicate ) {

		// �폜�͈͂̐擪�C�e���[�^
		// ���폜�͈͂������ɏW�񂳂��̂ŁA���̐擪�C�e���[�^���疖���C�e���[�^�܂ł͈̔͂��폜����Ƃ悢
		auto discard_begin_iter = std::remove_if(
			m_futureDeque.begin(),
			m_futureDeque.end(),
			predicate
		);

		// �폜������
		auto discard_num = std::distance(
			discard_begin_iter,
			m_futureDeque.end()
		);

		//�͈͂��폜
		m_futureDeque.erase(
			discard_begin_iter,
			m_futureDeque.end()
		);


	}



#pragma endregion discard method



#pragma region get member data method



	// ���s�����֐��̖߂�l���󂯎��@�����̊֐���wait�Ɠ����̓��e�����s������Ɋ֐��̎��s���ʂ��擾���܂�
	// void�^�Ȃ̂Ŗ߂�l�͂Ȃ����֐��̌��ʂ��擾���邽�߂Ɏg�p����
	[[noreturn]] result_type get_result( uint32_t index ) {

		auto future_result = m_futureDeque[index].get_result();

	}


	// ���s�����֐��̖߂�l���󂯎��A���̊֐��̎��s���ʂ̃f�[�^���폜����
	// void�^�Ȃ̂Ŗ߂�l�͂Ȃ����֐��̌��ʂ��擾���邽�߂Ɏg�p����
	[[noreturn]] result_type get_result_and_discard( uint32_t index ) {

		auto future_result = m_futureDeque[index].get_result();

		discard( index );

	}


	// ���ʂ�ێ�����fuutre_cash�^��Ԃ�
	[[nodiscard]] future_cash_type&& get_future_cash( uint32_t index ) {

		return std::move( m_futureDeque[index] );

	}


	// ���ʂ�ێ�����std::future��Ԃ�
	[[nodiscard]] std::future<result_type> get_future( uint32_t index ) {

		auto future_result = std::move( m_futureDeque[index].get_future() );

		discard( index );

		return future_result;

	}


	// get_result�̌��ʂ����L����ׂ�std::shared_future<result_type>�I�u�W�F�N�g�Ƃ��擾����
	[[nodiscard]] std::shared_future<result_type> get_shared_future( uint32_t index ) {

		return m_futureDeque[index].get_shared_future();

	}



#pragma endregion get member data method



#pragma region wait method



	// ���s�����֐��̏������I�����A�֐��̌��ʂ��擾�ł���悤�ɂȂ�܂ŏ�����҂��܂�
	void wait( uint32_t index ) {

		m_futureDeque[index].wait();

	}


	// ���ݎ��s����Ă��邷�ׂĂ̊֐��̏������I������܂ŏ�����҂��܂�
	void all_wait() {

		for( auto&& _future_cash : m_futureDeque ) {

			_future_cash.wait();

		}

	}



	// �w�肵�����Ԃ��o�߂����猻�݂̏����󋵂�Ԃ�
	template< class Rep, class Period >
	[[nodiscard]] std::future_status wait_for( uint32_t index, const std::chrono::duration<Rep, Period>& timeout_duration ) {

		/*============================================================================
		// �Q�l�T�C�g�Fhttps://ja.cppreference.com/w/cpp/thread/future/wait_for
		//============================================================================
		// ����
		//============================================================================
		// future_status::deferred	: ���ʂ��v�Z���邽�߂̊֐��͂܂��J�n����Ă��܂���
		// future_status::ready		: ���ʂ͏����������Ă��܂�
		// future_status::timeout	: �^�C���A�E�g���o�߂��܂���
		============================================================================*/

		return m_futureDeque[index].wait_for( timeout_duration );

	}


	// �w�肵�����ԂɂȂ����猻�݂̏����󋵂�Ԃ�
	template< class Clock, class Duration >
	[[nodiscard]] std::future_status wait_until( uint32_t index, const std::chrono::time_point<Clock, Duration>& timeout_time ) {

		/*============================================================================
		// �Q�l�T�C�g�Fhttps://ja.cppreference.com/w/cpp/thread/future/wait_until
		//============================================================================
		// ����
		//============================================================================
		// future_status::deferred	: ���ʂ��v�Z���邽�߂̊֐��͂܂��J�n����Ă��܂���
		// future_status::ready		: ���ʂ͏����������Ă��܂�
		// future_status::timeout	: �^�C���A�E�g���o�߂��܂���
		============================================================================*/

		return m_futureDeque[index].wait_until( timeout_time );

	}



#pragma endregion wait method



#pragma endregion async, deferred common method

};




#pragma endregion async_function_deque void specialization





// �w���p�[�֐�
template<class Result, class ...FuncArgs>
async_function_deque<Result( FuncArgs... )>&& make_async_function_deque( std::function<Result( FuncArgs... )>&& function ) {

	return async_function_deque<Result( FuncArgs... )>( function );

}