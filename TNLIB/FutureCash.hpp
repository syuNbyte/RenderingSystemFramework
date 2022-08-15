#pragma once





#include<future>
#include<optional>
#include<type_traits>
#include<iostream>





#pragma region future_cash



namespace future_cash_status {

	// �ǂ̃����o�ϐ������ʃf�[�^��ێ����Ă���̂���m�邽�߂̃N���X
	enum class has_result_state {

		has_future,				// future�^�����o�ϐ����f�[�^��ێ����Ă���E�ێ��\��
		has_shared_future,	// shared_future�^�����o�ϐ����f�[�^��ێ����Ă���E�ێ��\��
		has_result,				// result�^�����o�ϐ����f�[�^��ێ����Ă���E�ێ��\��
		no_result,				// �ǂ̃����o�ϐ������ʃf�[�^��ێ����Ă��Ȃ��ivoid�^�̓��ꉻ���Ɏg�p���Ă���j
		deferred_async			// �x�����sasync�֐��̎��s�҂���ԁA���s��ԂɈڍs�����ꍇ�ɂ�has_future�ɕω�����

	};

	// �ڍׂȕێ��f�[�^�@���ڍׂȃf�[�^��get_state�����o�֐��̎g�p���ɂ̂ݎg�p�����
	enum class has_result_detail_state {

		has_future_deferred,			// future�^�����o���f�[�^��ێ��\�肾���Adeferred�֐������s���Ă��Ȃ�
		has_future_ready,				// future�^�����o�����ʃf�[�^��ێ����Ă���
		has_future_timeout,			// future�^�����o���������f�[�^��ێ��\�肾���������I�����Ă��Ȃ�
		has_shared_future_deferred,// shared_future�^�����o���f�[�^��ێ����Ă��邪�Adeferred�֐������s���Ă��Ȃ�
		has_shared_future_ready,	// shared_future�^�����o�����ʃf�[�^��ێ����Ă���
		has_shared_future_timeout,	// shared_future�^�����o���������f�[�^��ێ��\�肾���������I�����Ă��Ȃ�
		has_result,						// result�^�����o�ϐ����f�[�^��ێ����Ă���E�ێ��\��
		no_result,						// �ǂ̃����o�ϐ������ʃf�[�^��ێ����Ă��Ȃ��ivoid�^�̓��ꉻ���Ɏg�p���Ă���j
		deferred_async					// �x�����sasync�֐��̎��s�҂���ԁA���s��ԂɈڍs�����ꍇ�ɂ�has_future�ɕω�����

	};


}





// ���ʃf�[�^���L���b�V���i�ێ��j���Ă������߂̃f�[�^�̂܂Ƃ܂�
// 
template<class ResultType>
class future_cash {

private:


	// static_assert�͏������莮��false�̏ꍇ�ɓ��삷�邽�߃��^�֐��̌��ʂ�not���Z���Ă���
	// std::optional�^���g�p����ۂ�object�^�̗v���𖞂����K�v�����邽��
	// object�^�̗v��
	// 1.�֐��ł͂Ȃ�
	// 2.void�ł͂Ȃ�
	// 3.�Q�ƌ^�ł͂Ȃ�
	static_assert(
		std::is_object_v<ResultType>,
		"result_type is not object type."
		);
	static_assert(
		std::is_object_v<ResultType>,
		"If you use future_cash class, the return type must be object type."
		);
	static_assert(
		!std::is_function_v<ResultType>,
		"The result_type was function. std::optional<T = function_type> cannot be used."
		);
	static_assert(
		!std::is_void_v<ResultType>,
		"The result_type was void. std::optional<T = void> cannot be used."
		);
	static_assert(
		!std::is_reference_v<ResultType>,
		"The result_type was reference type. std::optional<T = reference_type> cannot be used."
		);

public:


	using result_type = ResultType;
	using future_type = std::future<result_type>;
	using shared_future_type = std::shared_future<result_type>;
	using optional_type = std::optional<result_type>;
	using deferred_async_future_type = std::future<future_type>;

private:


	// �֐��̎��s���ʂ�ێ����Ă������߂̃N���X
	future_type m_future;

	// ���̃N���X�̕ێ�����future��share���s�����ۂ�share��Ԃ�ێ����邽�߂̃����o�ϐ�
	shared_future_type m_sharedFuture;

	// find�ȂǂŃf�[�^����������ۂɈ�x�O���Ɏ����Ă���K�v�����������߂Ƀf�[�^��ێ����Ă������߂̃����o�ϐ�
	// ��m_future, m_sharedFuture��get�����o�֐������s���Č��ʂ���x�擾����ƍēxget�����o�֐������s���邱�Ƃ͕s�\�Ȃ̂�
	optional_type m_result = std::nullopt;

	// �ǂ̃����o�ϐ������ʃf�[�^��ێ����Ă���̂���Ԃ�
	future_cash_status::has_result_state m_has_result = future_cash_status::has_result_state::no_result;



	// async_function�N���X, async_function_deque�N���X�Ŏ��s�����
	// deferred_async, deferred_threadsafe_async�����o�֐��i�x�����s�����async�֐��j�Ŏg�p����郁���o�ϐ�
	deferred_async_future_type m_deferredAsyncFuture;

public:


	future_cash() noexcept {};

	future_cash( future_type&& future )
		: m_future( std::move( future ) ), m_has_result( future_cash_status::has_result_state::has_future ) {};

	future_cash( deferred_async_future_type&& deferredAsyncFuture )
		: m_deferredAsyncFuture( std::move( deferredAsyncFuture ) ), m_has_result(future_cash_status::has_result_state::deferred_async) {};

public:


#pragma region get member data method



	// ���s�����֐��̖߂�l���󂯎��@�����̊֐���wait�Ɠ����̓��e�����s������Ɋ֐��̎��s���ʂ��擾���܂�
	// ���̊֐��͎擾�������֐��̎��s���I�����Ă��Ȃ��ꍇ
	result_type get_result() {

#ifdef _DEBUG


		try {

			switch( m_has_result ) {

				case future_cash_status::has_result_state::has_future:
				{

					if( m_future.valid() == true ) {

						m_result = m_future.get();

						m_has_result = future_cash_status::has_result_state::has_result;

						return m_result.value();

					}
					else {

						// �݌v��̃~�X�ɂ���ēK�؂ȃ^�O��ݒ�ł��Ă��Ȃ�
						throw std::logic_error(
							"Set the appropriate future_cash::has_result_state.This is an error due to a design error."
						);

					}

				}break;
				case future_cash_status::has_result_state::has_shared_future:
				{

					if( m_future.valid() == true ) {

						m_result = m_sharedFuture.get();

						m_has_result = future_cash_status::has_result_state::has_result;

						return m_result.value();

					}
					else {

						// �݌v��̃~�X�ɂ���ēK�؂ȃ^�O��ݒ�ł��Ă��Ȃ�
						throw std::logic_error(
							"Set the appropriate future_cash::has_result_state.This is an error due to a design error."
						);

					}

				}break;
				case future_cash_status::has_result_state::has_result:
				{

					return m_result.value();

				}break;
				case future_cash_status::has_result_state::deferred_async:
				{

					if( m_deferredAsyncFuture.valid() == true ) {

						deferred_async_exe();

						// �ċA�i������x���̊֐����ĂԁA���̎��s���ɂ�has_future�̃^�O���ݒ肳��Ă��邽�ߏ������e���ύX�����j
						return get_result();

					}
					else {

						// �݌v��̃~�X�ɂ���ēK�؂ȃ^�O��ݒ�ł��Ă��Ȃ�
						throw std::logic_error(
							"Set the appropriate future_cash::has_result_state.This is an error due to a design error."
						);

					}

				}break;
				case future_cash_status::has_result_state::no_result:
				{

					throw std::runtime_error(
						"not execute function"
					);

				}

			}

		}
		catch( const std::logic_error& logicError ) {

			std::cout << "error : " << logicError.what() << std::endl;

		}

#else


		switch( m_has_result ) {

			case future_cash_status::has_result_state::has_future:
			{

				m_result = m_future.get();

				m_has_result = future_cash_status::has_result_state::has_result;

				return m_result.value();

			}break;
			case future_cash_status::has_result_state::has_shared_future:
			{

				m_result = m_sharedFuture.get();

				m_has_result = future_cash_status::has_result_state::has_result;

				return m_result.value();

			}break;
			case future_cash_status::has_result_state::has_result:
			{

				return m_result.value();

			}break;
			case future_cash_status::has_result_state::deferred_async:
			{

				if( m_deferredAsyncFuture.valid() == true ) {

					deferred_async_exe();

					// �ċA�i������x���̊֐����ĂԁA���̎��s���ɂ�has_future�̃^�O���ݒ肳��Ă��邽�ߏ������e���ύX�����j
					return get_result();

				}

			}break;

		}

#endif

	}


	// ���ʂ�ێ�����std::future��Ԃ�
	[[nodiscard]] future_type get_future() {

#ifdef _DEBUG


		try {

			// async, deferred�̌��ʂ�����m_future�����o���܂��Ȃɂ���������擾����Ă��Ȃ���Ԃ̏ꍇ
			// ��std::future::get���\�b�h, std::future::share���\�b�h����x���Ă΂�Ă��Ȃ���Ԃ̂Ƃ���true
			if( m_future.valid() == true ) {

				m_has_result = future_cash_status::has_result_state::no_result;

				return std::move( m_future );

			}
			else if( m_deferredAsyncFuture.valid() == true ) {

				m_has_result = future_cash_status::has_result_state::no_result;

				return std::move( m_deferredAsyncFuture.get() );

			}
			else {

				// �ێ�����future�^��empty������
				throw std::runtime_error( "The future type was empty." );

			}

		}
		catch( const std::runtime_error& runtimeError ) {

			std::cout << "error : " << runtimeError.what() << std::endl;

			// empty�����߂�l�͕Ԃ�
			return std::move( m_future );

		}

#else


		if( m_future.valid() == true ) {

			m_has_result = future_cash_status::has_result_state::no_result;

			return std::move( m_future );

		}
		else if( m_deferredAsyncFuture.valid() == true ) {

			m_has_result = future_cash_status::has_result_state::no_result;

			return std::move( m_deferredAsyncFuture.get() );

		}

#endif

	}


	// get_result�̌��ʂ����L����ׂ�std::shared_future<result_type>�I�u�W�F�N�g�Ƃ��擾����
	[[nodiscard]] shared_future_type get_shared_future() {


		if( m_has_result == future_cash_status::has_result_state::has_future ) {

			m_sharedFuture = m_future.share();

			m_has_result = future_cash_status::has_result_state::has_shared_future;

		}
		else if( m_has_result == future_cash_status::has_result_state::deferred_async ) {

			m_sharedFuture = m_deferredAsyncFuture.get().share();

			m_has_result = future_cash_status::has_result_state::has_shared_future;

		}


#ifdef _DEBUG


		try {

			// async, deferred�̌��ʂ�����m_future�����o���܂��Ȃɂ���������擾����Ă��Ȃ���Ԃ̏ꍇ
			// ��std::future::get���\�b�h, std::future::share���\�b�h����x���Ă΂�Ă��Ȃ���Ԃ̂Ƃ���true
			if( m_sharedFuture.valid() == true ) {

				return m_sharedFuture;

			}
			else {

				// �ێ�����future�^�Eshared_future�^��empty������
				throw std::runtime_error( "Both future type and shared_future type were empty." );

			}

		}
		catch( const std::runtime_error& runtimeError ) {

			std::cout << "error : " << runtimeError.what() << std::endl;

			// empty�����߂�l�͕Ԃ�
			return m_sharedFuture;

		}

#else

		if( m_sharedFuture.valid() == true ) {

			return m_sharedFuture;

		}

#endif


	}


	// �x�����s���s��
	void deferred_async_exe() {

		if( m_deferredAsyncFuture.valid() == true ) {

			m_future = std::move( m_deferredAsyncFuture.get() );

			m_has_result = future_cash_status::has_result_state::has_future;

		}


	}


	// ���s���ʂ�discard�i�j���j����
	[[noreturn]] void discard() const {

		m_future = std::future<result_type>();
		m_sharedFuture = std::shared_future<result_type>();
		m_result = std::nullopt;
		m_has_result = future_cash_status::has_result_state::no_result;
		m_deferredAsyncFuture = std::future<std::future<result_type>>();

	}


#pragma endregion get member data method



#pragma region future, shared_future valid method



// �ێ�����future�^�����o�ϐ����L�����ǂ�����Ԃ�
	bool valid_future() {

		return m_future.valid();

	}


	// �ێ�����shared_future�^�����o�ϐ����L�����ǂ�����Ԃ�
	bool valid_shared_future() {

		return m_sharedFuture.valid();

	}


	// �ێ�����deferred_async�p��future�^�����o�ϐ����L�����ǂ�����Ԃ�
	bool valid_deferred_async() {

		return m_deferredAsyncFuture.valid();

	}


#pragma endregion future, shared_future valid method



#pragma region wait method



	// ���s�����֐��̏������I�����A�֐��̌��ʂ��擾�ł���悤�ɂȂ�܂ő҂��܂�
	void wait() {

#ifdef _DEBUG

		try {

			// async, deferred�̌��ʂ�����m_future�����o���܂��Ȃɂ���������擾����Ă��Ȃ���Ԃ̏ꍇ
			// ��std::future::get���\�b�h, std::future::share���\�b�h����x���Ă΂�Ă��Ȃ���Ԃ̂Ƃ���true
			if( m_future.valid() == true ) {

				m_future.wait();

			}
			else if( m_sharedFuture.valid() == true ) {

				m_sharedFuture.wait();

			}
			else if( m_deferredAsyncFuture.valid() == true ) {

				deferred_async_exe();

				// �ċA
				wait();

			}
			else {

				// �ێ�����future�^�Eshared_future�^��empty������
				throw std::runtime_error( "Both future type and shared_future type were empty." );

			}

		}
		catch( const std::runtime_error& runtimeError ) {

			std::cout << "error : " << runtimeError.what() << std::endl;

		}

#else

		if( m_future.valid() == true ) {

			m_future.wait();

		}
		else if( m_sharedFuture.valid() == true ) {

			m_sharedFuture.wait();

		}
		else if( m_deferredAsyncFuture.valid() == true ) {

			deferred_async_exe();

			wait();

		}

#endif

	}


	// �w�肵�����Ԃ��o�߂����猻�݂̏����󋵂�Ԃ�
	template< class Rep, class Period >
	[[nodiscard]] std::future_status wait_for( const std::chrono::duration<Rep, Period>& timeout_duration ) {

		/*============================================================================
		// �Q�l�T�C�g�Fhttps://ja.cppreference.com/w/cpp/thread/future/wait_for
		//============================================================================
		// ����
		//============================================================================
		// future_status::deferred	: ���ʂ��v�Z���邽�߂̊֐��͂܂��J�n����Ă��܂���
		// future_status::ready		: ���ʂ͏����������Ă��܂�
		// future_status::timeout	: �^�C���A�E�g���o�߂��܂���
		============================================================================*/

#ifdef _DEBUG

		try {

			if( m_future.valid() == true ) {

				return m_future.wait_for( timeout_duration );

			}
			else if( m_sharedFuture.valid() == true ) {

				return m_sharedFuture.wait_for( timeout_duration );

			}
			else if( m_deferredAsyncFuture.valid() == true ) {

				deferred_async_exe();

				// �ċA
				return wait_for( timeout_duration );

			}
			else {

				// �ێ�����future�^�Eshared_future�^��empty������
				throw std::runtime_error( "Both future type and shared_future type were empty." );

			}

		}
		catch( const std::runtime_error& runtimeError ) {

			std::cout << "error : " << runtimeError.what() << std::endl;

		}

#else

		if( m_future.valid() == true ) {

			return m_future.wait_for( timeout_duration );

		}
		else if( m_sharedFuture.valid() == true ) {

			return m_sharedFuture.wait_for( timeout_duration );

		}
		else if( m_deferredAsyncFuture.valid() == true ) {

			deferred_async_exe();

			// �ċA
			return wait_for( timeout_duration );

		}

#endif

	}


	// �w�肵�����ԂɂȂ����猻�݂̏����󋵂�Ԃ�
	template< class Clock, class Duration >
	[[nodiscard]] std::future_status wait_until( const std::chrono::time_point<Clock, Duration>& timeout_time ) {

		/*============================================================================
		// �Q�l�T�C�g�Fhttps://ja.cppreference.com/w/cpp/thread/future/wait_for
		//============================================================================
		// ����
		//============================================================================
		// future_status::deferred	: ���ʂ��v�Z���邽�߂̊֐��͂܂��J�n����Ă��܂���
		// future_status::ready		: ���ʂ͏����������Ă��܂�
		// future_status::timeout	: �^�C���A�E�g���o�߂��܂���
		============================================================================*/

#ifdef _DEBUG

		try {

			if( m_future.valid() == true ) {

				return m_future.wait_until( timeout_time );

			}
			else if( m_sharedFuture.valid() == true ) {

				return m_sharedFuture.wait_until( timeout_time );

			}
			else if( m_deferredAsyncFuture.valid() == true ) {

				deferred_async_exe();

				// �ċA
				return wait_until( timeout_time );

			}
			else {

				// �ێ�����future�^�Eshared_future�^��empty������
				throw std::runtime_error( "Both future type and shared_future type were empty." );

			}

		}
		catch( const std::runtime_error& runtimeError ) {

			std::cout << "error : " << runtimeError.what() << std::endl;

		}

#else

		if( m_future.valid() == true ) {

			return m_future.wait_until( timeout_time );

		}
		else if( m_sharedFuture.valid() == true ) {

			return m_sharedFuture.wait_until( timeout_time );

		}
		else if( m_deferredAsyncFuture.valid() == true ) {

			deferred_async_exe();

			// �ċA
			return wait_until( timeout_time );

		}

#endif

	}



#pragma endregion wait method



#pragma region get state method



	// �������ʂ̕ێ���Ԃ��擾����
	future_cash_status::has_result_state get_state() {

		return m_has_result;

	}


	// �������ʂ̏ڍוێ���Ԃ��擾����
	future_cash_status::has_result_detail_state get_detail_state() {

		return get_detail_state_wait_for( std::chrono::nanoseconds( 1 ) );

	}


	// �������ʂ̏ڍוێ���Ԃ���莞�Ԍo�ߌ�Ɏ擾����
	template< class Rep, class Period >
	future_cash_status::has_result_detail_state get_detail_state_wait_for( const std::chrono::duration<Rep, Period>& timeout_duration ) {


#ifdef _DEBUG

		// ��i�m�b�istd::chrono���g�p�ł���ŏ��̎��ԁj���g�p����
		if( m_has_result == future_cash_status::has_result_state::has_future ) {

			if( m_future.valid() == true ) {


				switch( m_future.wait_for( timeout_duration ) ) {

					case std::future_status::deferred:
					{
						return future_cash_status::has_result_detail_state::has_future_deferred;
					}break;
					case std::future_status::ready:
					{
						return future_cash_status::has_result_detail_state::has_future_ready;
					}break;
					case std::future_status::timeout:
					{
						return future_cash_status::has_result_detail_state::has_future_timeout;
					}break;

				}


			}
			else {

				// �݌v��̃~�X�ɂ���ēK�؂ȃ^�O��ݒ�ł��Ă��Ȃ�
				throw std::logic_error(
					"Set the appropriate future_cash::has_result_state.This is an error due to a design error."
				);

			}

		}
		else if( m_has_result == future_cash_status::has_result_state::has_shared_future ) {

			if( m_sharedFuture.valid() == true ) {


				switch( m_sharedFuture.wait_for( timeout_duration ) ) {

					case std::future_status::deferred:
					{
						return future_cash_status::has_result_detail_state::has_shared_future_deferred;
					}break;
					case std::future_status::ready:
					{
						return future_cash_status::has_result_detail_state::has_shared_future_ready;
					}break;
					case std::future_status::timeout:
					{
						return future_cash_status::has_result_detail_state::has_shared_future_timeout;
					}break;

				}

			}
			else {

				// �݌v��̃~�X�ɂ���ēK�؂ȃ^�O��ݒ�ł��Ă��Ȃ�
				throw std::logic_error(
					"Set the appropriate future_cash::has_result_state.This is an error due to a design error."
				);

			}

		}
		else if( m_has_result == future_cash_status::has_result_state::has_result ) {

			return future_cash_status::has_result_detail_state::has_result;

		}
		else if( m_has_result == future_cash_status::has_result_state::no_result ) {

			return future_cash_status::has_result_detail_state::no_result;

		}
		else if( m_has_result == future_cash_status::has_result_state::deferred_async ) {

			return future_cash_status::has_result_detail_state::deferred_async;

		}

#else


				// ��i�m�b�istd::chrono���g�p�ł���ŏ��̎��ԁj���g�p����
		if( m_has_result == future_cash_status::has_result_state::has_future ) {

			if( m_future.valid() == true ) {


				switch( m_future.wait_for( timeout_duration ) ) {

					case std::future_status::deferred:
					{
						return future_cash_status::has_result_detail_state::has_future_deferred;
					}break;
					case std::future_status::ready:
					{
						return future_cash_status::has_result_detail_state::has_future_ready;
					}break;
					case std::future_status::timeout:
					{
						return future_cash_status::has_result_detail_state::has_future_timeout;
					}break;

				}


			}

		}
		else if( m_has_result == future_cash_status::has_result_state::has_shared_future ) {

			if( m_sharedFuture.valid() == true ) {


				switch( m_sharedFuture.wait_for( timeout_duration ) ) {

					case std::future_status::deferred:
					{
						return future_cash_status::has_result_detail_state::has_shared_future_deferred;
					}break;
					case std::future_status::ready:
					{
						return future_cash_status::has_result_detail_state::has_shared_future_ready;
					}break;
					case std::future_status::timeout:
					{
						return future_cash_status::has_result_detail_state::has_shared_future_timeout;
					}break;

				}


			}

		}
		else if( m_has_result == future_cash_status::has_result_state::has_result ) {

			return future_cash_status::has_result_detail_state::has_result;

		}
		else if( m_has_result == future_cash_status::has_result_state::no_result ) {

			return future_cash_status::has_result_detail_state::no_result;

		}
		else if( m_has_result == future_cash_status::has_result_state::deferred_async ) {

			return future_cash_status::has_result_detail_state::deferred_async;

		}

#endif


	}


	// �������ʂ̏ڍוێ���Ԃ��w�肵�����ԂɂȂ�����擾����
	template< class Clock, class Duration >
	future_cash_status::has_result_detail_state get_detail_state_wait_until( const std::chrono::time_point<Clock, Duration>& timeout_time ) {

		std::this_thread::sleep_until( timeout_time );

		return get_detail_state();

	}



#pragma endregion get state method


};



#pragma endregion future_cash





#pragma region future_cash void specialization



template<>
class future_cash<void> {

public:


	using result_type = void;
	using future_type = std::future<result_type>;
	using shared_future_type = std::shared_future<result_type>;
	using deferred_async_future_type = std::future<future_type>;

private:


	// �֐��̎��s���ʂ�ێ����Ă������߂̃N���X
	future_type m_future;

	// ���̃N���X�̕ێ�����future��share���s�����ۂ�share��Ԃ�ێ����邽�߂̃����o�ϐ�
	shared_future_type m_sharedFuture;

	// �ǂ̃����o�ϐ������ʃf�[�^��ێ����Ă���̂���Ԃ�
	future_cash_status::has_result_state m_has_result = future_cash_status::has_result_state::has_future;



	// async_function�N���X, async_function_deque�N���X�Ŏ��s�����
	// deferred_async, deferred_threadsafe_async�����o�֐��i�x�����s�����async�֐��j�Ŏg�p����郁���o�ϐ�
	deferred_async_future_type m_deferredAsyncFuture;

public:



	future_cash() noexcept {};

	future_cash( future_type&& future )
		: m_future( std::move( future ) ), m_has_result( future_cash_status::has_result_state::has_future ) {};

	future_cash( deferred_async_future_type&& deferredAsyncFuture )
		: m_deferredAsyncFuture( std::move( deferredAsyncFuture ) ), m_has_result( future_cash_status::has_result_state::deferred_async ) {};



public:


#pragma region get member data method



	// ���s�����֐��̖߂�l���󂯎��@�����̊֐���wait�Ɠ����̓��e�����s������Ɋ֐��̎��s���ʂ��擾���܂�
	// ���̊֐��͎擾�������֐��̎��s���I�����Ă��Ȃ��ꍇ
	result_type get_result() {

#ifdef _DEBUG


		try {

			switch( m_has_result ) {

				case future_cash_status::has_result_state::has_future:
				{

					if( m_future.valid() == true ) {

						m_future.get();

						m_has_result = future_cash_status::has_result_state::no_result;

					}
					else {

						// �݌v��̃~�X�ɂ���ēK�؂ȃ^�O��ݒ�ł��Ă��Ȃ�
						throw std::logic_error(
							"Set the appropriate future_cash::has_result_state.This is an error due to a design error."
						);

					}

				}break;
				case future_cash_status::has_result_state::has_shared_future:
				{

					if( m_sharedFuture.valid() == true ) {

						m_sharedFuture.get();

						m_has_result = future_cash_status::has_result_state::no_result;

					}
					else {

						// �݌v��̃~�X�ɂ���ēK�؂ȃ^�O��ݒ�ł��Ă��Ȃ�
						throw std::logic_error(
							"Set the appropriate future_cash::has_result_state.This is an error due to a design error."
						);

					}

				}break;
				case future_cash_status::has_result_state::deferred_async:
				{

					if( m_deferredAsyncFuture.valid() == true ) {

						deferred_async_exe();

						// �ċA�i������x���̊֐����ĂԁA���̎��s���ɂ�has_future�̃^�O���ݒ肳��Ă��邽�ߏ������e���ύX�����j
						return get_result();

					}
					else {

						// �݌v��̃~�X�ɂ���ēK�؂ȃ^�O��ݒ�ł��Ă��Ȃ�
						throw std::logic_error(
							"Set the appropriate future_cash::has_result_state.This is an error due to a design error."
						);

					}

				}break;
				case future_cash_status::has_result_state::no_result:
				{

					throw std::runtime_error(
						"not execute function"
					);

				}


			}

		}
		catch( const std::logic_error& logicError ) {

			std::cout << "error : " << logicError.what() << std::endl;

		}

#else


		switch( m_has_result ) {

			case future_cash_status::has_result_state::has_future:
			{

				if( m_future.valid() == true ) {

					m_future.get();

					m_has_result = future_cash_status::has_result_state::no_result;

				}

			}break;
			case future_cash_status::has_result_state::has_shared_future:
			{

				if( m_sharedFuture.valid() == true ) {

					m_sharedFuture.get();

					m_has_result = future_cash_status::has_result_state::no_result;

				}

			}break;
			case future_cash_status::has_result_state::deferred_async:
			{

				if( m_deferredAsyncFuture.valid() == true ) {

					deferred_async_exe();

					// �ċA�i������x���̊֐����ĂԁA���̎��s���ɂ�has_future�̃^�O���ݒ肳��Ă��邽�ߏ������e���ύX�����j
					return get_result();

				}

			}break;

		}

#endif

	}


	// ���ʂ�ێ�����std::future��Ԃ�
	[[nodiscard]] future_type get_future() {

#ifdef _DEBUG


		try {

			// async, deferred�̌��ʂ�����m_future�����o���܂��Ȃɂ���������擾����Ă��Ȃ���Ԃ̏ꍇ
			// ��std::future::get���\�b�h, std::future::share���\�b�h����x���Ă΂�Ă��Ȃ���Ԃ̂Ƃ���true
			if( m_future.valid() == true ) {

				return std::move( m_future );

			}
			else if( m_deferredAsyncFuture.valid() == true ) {

				return std::move( m_deferredAsyncFuture.get() );

			}
			else {

				// �ێ�����future�^��empty������
				throw std::runtime_error( "The future type was empty." );

			}

		}
		catch( const std::runtime_error& runtimeError ) {

			std::cout << "error : " << runtimeError.what() << std::endl;

		}

#else


		if( m_future.valid() == true ) {

			return std::move( m_future );

		}
		else if( m_deferredAsyncFuture.valid() == true ) {

			return std::move( m_deferredAsyncFuture.get() );

		}

#endif

	}


	// get_result�̌��ʂ����L����ׂ�std::shared_future<result_type>�I�u�W�F�N�g�Ƃ��擾����
	[[nodiscard]] shared_future_type get_shared_future() {


		if( m_has_result == future_cash_status::has_result_state::has_future ) {

			m_sharedFuture = m_future.share();

			m_has_result = future_cash_status::has_result_state::has_shared_future;

		}
		else if( m_has_result == future_cash_status::has_result_state::deferred_async ) {

			m_sharedFuture = m_deferredAsyncFuture.get().share();

			m_has_result = future_cash_status::has_result_state::has_shared_future;

		}


#ifdef _DEBUG


		try {

			// async, deferred�̌��ʂ�����m_future�����o���܂��Ȃɂ���������擾����Ă��Ȃ���Ԃ̏ꍇ
			// ��std::future::get���\�b�h, std::future::share���\�b�h����x���Ă΂�Ă��Ȃ���Ԃ̂Ƃ���true
			if( m_sharedFuture.valid() == true ) {

				return m_sharedFuture;

			}
			else {

				// �ێ�����future�^�Eshared_future�^��empty������
				throw std::runtime_error( "Both future type and shared_future type were empty." );

			}

		}
		catch( const std::runtime_error& runtimeError ) {

			std::cout << "error : " << runtimeError.what() << std::endl;

		}

#else

		if( m_sharedFuture.valid() == true ) {

			return m_sharedFuture;

		}

#endif


	}


	// �x�����s���s��
	void deferred_async_exe() {

		m_future = std::move( m_deferredAsyncFuture.get() );

		m_has_result = future_cash_status::has_result_state::has_future;

	}


	// ���s���ʂ�discard�i�j���j����
	[[noreturn]] void discard() {

		m_future = std::future<result_type>();
		m_sharedFuture = std::shared_future<result_type>();
		m_has_result = future_cash_status::has_result_state::no_result;
		m_deferredAsyncFuture = std::future<std::future<result_type>>();

	}


#pragma endregion get member data method



#pragma region future, shared_future valid method



// �ێ�����future�^�����o�ϐ����L�����ǂ�����Ԃ�
	bool valid_future() {

		return m_future.valid();

	}


	// �ێ�����shared_future�^�����o�ϐ����L�����ǂ�����Ԃ�
	bool valid_shared_future() {

		return m_sharedFuture.valid();

	}


	// �ێ�����deferred_async�p��future�^�����o�ϐ����L�����ǂ�����Ԃ�
	bool valid_deferred_async() {

		return m_deferredAsyncFuture.valid();

	}


#pragma endregion future, shared_future valid method



#pragma region wait method



	// ���s�����֐��̏������I�����A�֐��̌��ʂ��擾�ł���悤�ɂȂ�܂ő҂��܂�
	void wait() {

#ifdef _DEBUG

		try {

			// async, deferred�̌��ʂ�����m_future�����o���܂��Ȃɂ���������擾����Ă��Ȃ���Ԃ̏ꍇ
			// ��std::future::get���\�b�h, std::future::share���\�b�h����x���Ă΂�Ă��Ȃ���Ԃ̂Ƃ���true
			if( m_future.valid() == true ) {

				m_future.wait();

			}
			else if( m_sharedFuture.valid() == true ) {

				m_sharedFuture.wait();

			}
			else if( m_deferredAsyncFuture.valid() == true ) {

				deferred_async_exe();

				// �ċA
				wait();

			}
			else {

				// �ێ�����future�^�Eshared_future�^��empty������
				throw std::runtime_error( "Both future type and shared_future type were empty." );

			}

		}
		catch( const std::runtime_error& runtimeError ) {

			std::cout << "error : " << runtimeError.what() << std::endl;

		}

#else

		if( m_future.valid() == true ) {

			m_future.wait();

		}
		else if( m_sharedFuture.valid() == true ) {

			m_sharedFuture.wait();

		}
		else if( m_deferredAsyncFuture.valid() == true ) {

			deferred_async_exe();

			// �ċA
			wait();

		}

#endif

	}


	// �w�肵�����Ԃ��o�߂����猻�݂̏����󋵂�Ԃ�
	template< class Rep, class Period >
	[[nodiscard]] std::future_status wait_for( const std::chrono::duration<Rep, Period>& timeout_duration ) {

		/*============================================================================
		// �Q�l�T�C�g�Fhttps://ja.cppreference.com/w/cpp/thread/future/wait_for
		//============================================================================
		// ����
		//============================================================================
		// future_status::deferred	: ���ʂ��v�Z���邽�߂̊֐��͂܂��J�n����Ă��܂���
		// future_status::ready		: ���ʂ͏����������Ă��܂�
		// future_status::timeout	: �^�C���A�E�g���o�߂��܂���
		============================================================================*/

#ifdef _DEBUG

		try {

			if( m_future.valid() == true ) {

				return m_future.wait_for( timeout_duration );

			}
			else if( m_sharedFuture.valid() == true ) {

				return m_sharedFuture.wait_for( timeout_duration );

			}
			else if( m_deferredAsyncFuture.valid() == true ) {

				deferred_async_exe();

				// �ċA
				return wait_for( timeout_duration );

			}
			else {

				// �ێ�����future�^�Eshared_future�^��empty������
				throw std::runtime_error( "Both future type and shared_future type were empty." );

			}

		}
		catch( const std::runtime_error& runtimeError ) {

			std::cout << "error : " << runtimeError.what() << std::endl;

		}

#else

		if( m_future.valid() == true ) {

			return m_future.wait_for( timeout_duration );

		}
		else if( m_sharedFuture.valid() == true ) {

			return m_sharedFuture.wait_for( timeout_duration );

		}
		else if( m_deferredAsyncFuture.valid() == true ) {

			deferred_async_exe();

			// �ċA
			return wait_for( timeout_duration );

		}

#endif

	}


	// �w�肵�����ԂɂȂ����猻�݂̏����󋵂�Ԃ�
	template< class Clock, class Duration >
	[[nodiscard]] std::future_status wait_until( const std::chrono::time_point<Clock, Duration>& timeout_time ) {

		/*============================================================================
		// �Q�l�T�C�g�Fhttps://ja.cppreference.com/w/cpp/thread/future/wait_for
		//============================================================================
		// ����
		//============================================================================
		// future_status::deferred	: ���ʂ��v�Z���邽�߂̊֐��͂܂��J�n����Ă��܂���
		// future_status::ready		: ���ʂ͏����������Ă��܂�
		// future_status::timeout	: �^�C���A�E�g���o�߂��܂���
		============================================================================*/

#ifdef _DEBUG

		try {

			if( m_future.valid() == true ) {

				return m_future.wait_until( timeout_time );

			}
			else if( m_sharedFuture.valid() == true ) {

				return m_sharedFuture.wait_until( timeout_time );

			}
			else if( m_deferredAsyncFuture.valid() == true ) {

				deferred_async_exe();

				// �ċA
				return wait_until( timeout_time );

			}
			else {

				// �ێ�����future�^�Eshared_future�^��empty������
				throw std::runtime_error( "Both future type and shared_future type were empty." );

			}

		}
		catch( const std::runtime_error& runtimeError ) {

			std::cout << "error : " << runtimeError.what() << std::endl;

		}

#else

		if( m_future.valid() == true ) {

			return m_future.wait_until( timeout_time );

		}
		else if( m_sharedFuture.valid() == true ) {

			return m_sharedFuture.wait_until( timeout_time );

		}
		else if( m_deferredAsyncFuture.valid() == true ) {

			deferred_async_exe();

			// �ċA
			return wait_until( timeout_time );

		}

#endif

	}



#pragma endregion wait method



#pragma region get state method



	// �������ʂ̕ێ���Ԃ��擾����
	future_cash_status::has_result_state get_state() {

		return m_has_result;

	}


	// �������ʂ̏ڍוێ���Ԃ��擾����
	future_cash_status::has_result_detail_state get_detail_state() {

		return get_detail_state_wait_for( std::chrono::nanoseconds( 1 ) );

	}


	// �������ʂ̏ڍוێ���Ԃ���莞�Ԍo�ߌ�Ɏ擾����
	template< class Rep, class Period >
	future_cash_status::has_result_detail_state get_detail_state_wait_for( const std::chrono::duration<Rep, Period>& timeout_duration ) {


#ifdef _DEBUG

		// ��i�m�b�istd::chrono���g�p�ł���ŏ��̎��ԁj���g�p����
		if( m_has_result == future_cash_status::has_result_state::has_future ) {

			if( m_future.valid() == true ) {


				switch( m_future.wait_for( timeout_duration ) ) {

					case std::future_status::deferred:
					{
						return future_cash_status::has_result_detail_state::has_future_deferred;
					}break;
					case std::future_status::ready:
					{
						return future_cash_status::has_result_detail_state::has_future_ready;
					}break;
					case std::future_status::timeout:
					{
						return future_cash_status::has_result_detail_state::has_future_timeout;
					}break;

				}


			}
			else {

				// �݌v��̃~�X�ɂ���ēK�؂ȃ^�O��ݒ�ł��Ă��Ȃ�
				throw std::logic_error(
					"Set the appropriate future_cash::has_result_state.This is an error due to a design error."
				);

			}

		}
		else if( m_has_result == future_cash_status::has_result_state::has_shared_future ) {

			if( m_sharedFuture.valid() == true ) {


				switch( m_sharedFuture.wait_for( timeout_duration ) ) {

					case std::future_status::deferred:
					{
						return future_cash_status::has_result_detail_state::has_shared_future_deferred;
					}break;
					case std::future_status::ready:
					{
						return future_cash_status::has_result_detail_state::has_shared_future_ready;
					}break;
					case std::future_status::timeout:
					{
						return future_cash_status::has_result_detail_state::has_shared_future_timeout;
					}break;

				}

			}
			else {

				// �݌v��̃~�X�ɂ���ēK�؂ȃ^�O��ݒ�ł��Ă��Ȃ�
				throw std::logic_error(
					"Set the appropriate future_cash::has_result_state.This is an error due to a design error."
				);

			}

		}
		else if( m_has_result == future_cash_status::has_result_state::has_result ) {

			return future_cash_status::has_result_detail_state::has_result;

		}
		else if( m_has_result == future_cash_status::has_result_state::no_result ) {

			return future_cash_status::has_result_detail_state::no_result;

		}
		else if( m_has_result == future_cash_status::has_result_state::deferred_async ) {

			return future_cash_status::has_result_detail_state::deferred_async;

		}

#else


				// ��i�m�b�istd::chrono���g�p�ł���ŏ��̎��ԁj���g�p����
		if( m_has_result == future_cash_status::has_result_state::has_future ) {

			if( m_future.valid() == true ) {


				switch( m_future.wait_for( timeout_duration ) ) {

					case std::future_status::deferred:
					{
						return future_cash_status::has_result_detail_state::has_future_deferred;
					}break;
					case std::future_status::ready:
					{
						return future_cash_status::has_result_detail_state::has_future_ready;
					}break;
					case std::future_status::timeout:
					{
						return future_cash_status::has_result_detail_state::has_future_timeout;
					}break;

				}


			}

		}
		else if( m_has_result == future_cash_status::has_result_state::has_shared_future ) {

			if( m_sharedFuture.valid() == true ) {


				switch( m_sharedFuture.wait_for( timeout_duration ) ) {

					case std::future_status::deferred:
					{
						return future_cash_status::has_result_detail_state::has_shared_future_deferred;
					}break;
					case std::future_status::ready:
					{
						return future_cash_status::has_result_detail_state::has_shared_future_ready;
					}break;
					case std::future_status::timeout:
					{
						return future_cash_status::has_result_detail_state::has_shared_future_timeout;
					}break;

				}


			}

		}
		else if( m_has_result == future_cash_status::has_result_state::has_result ) {

			return future_cash_status::has_result_detail_state::has_result;

		}
		else if( m_has_result == future_cash_status::has_result_state::no_result ) {

			return future_cash_status::has_result_detail_state::no_result;

		}
		else if( m_has_result == future_cash_status::has_result_state::deferred_async ) {

			return future_cash_status::has_result_detail_state::deferred_async;

		}

#endif


	}


	// �������ʂ̏ڍוێ���Ԃ��w�肵�����ԂɂȂ�����擾����
	template< class Clock, class Duration >
	future_cash_status::has_result_detail_state get_detail_state_wait_until( const std::chrono::time_point<Clock, Duration>& timeout_time ) {

		std::this_thread::sleep_until( timeout_time );

		return get_detail_state();

	}



#pragma endregion get state method


};



#pragma endregion future_cash void specialization