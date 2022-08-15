#pragma once





#include<future>
#include"AsyncFunction.hpp"




#define TRUE 1
#define FALSE 0




// notify�n�N���X�ƂȂ��Ă��邪�Asignal�n�N���X�ł����������@������͖��̂Ɋւ��Ă̂��Ƃł�

// std::shared_future<void>���g�p���Ă��̃N���X�Q�͐݌v����Ă��邪
// std::condition_variable�^�Estd::condition_variable_any�^���g�p���Ă̍쐬���\�Ȃ̂ōœK����std::condition_variable�^�Ȃ̂ł����
// ��������x�[�X�Ƃ����N���X�݌v�ɑg�ݑւ��邱�Ƃ��s��
#pragma region notify class



#pragma region notifyer class



// �ʒm�𑗂邽�߂����̃N���X
// �ォ��C�Â����̂����A���̃N���X��C++20�Ŏ�������Ă���STL std::stop_source�Ǝ����悤�ȏ������s���Ă��邽��
// ����STL�Ɏ����悤�Ȑ݌v�̃N���X�����
class o_notifyer {

private:


	// �C�ӂ̃^�C�~���O�Œʒm���郁���o�֐�notify�ł���std::shared_future�ւ̎Q�Ƃ����ʃX���b�h
	std::promise<void> m_notifyer;

public:


	// �C�ӂ̃^�C�~���O�Œʒm���s���A���g���n���Ă���notifyer���~�߂Ă��鑼�X���b�h�̏������N����
	void notify() {

		m_notifyer.set_value();

	}


	// �ʒm���s�������o�֐�notify����ʒm�������ۂɒʒm���󂯎��I�u�W�F�N�g��n�������o�֐�
	std::shared_future<void> get_notifyer() {

		return m_notifyer.get_future();

	}

};



#pragma endregion notifyer class





#pragma region notify_getter class



// notifyer�N���X��������ʒm���󂯎�邽�߂����̃N���X
// notifyer�N���X�ƈꏏ�Ōォ��C�Â����̂����A���̃N���X��C++20�Ŏ�������Ă���STL std::stop_token�Ǝ����悤�ȏ������s���Ă��邽��
// ����STL�Ɏ����悤�Ȑ݌v�̃N���X�����
class o_notify_getter {

private:


	// ����async_function�I�u�W�F�N�g��notify���󂯎�邽�߂�notifyer��ێ����Ă������߂̃����o
	std::shared_future<void> m_notifyer;

public:


	o_notify_getter() noexcept {};


	o_notify_getter( o_notifyer& notifyer ) {

		m_notifyer = notifyer.get_notifyer();

	}


	o_notify_getter( std::shared_future<void> notifyer ) {

		m_notifyer = notifyer;

	}

public:


	void set_notifyer( o_notifyer& notifyer ) {

		m_notifyer = notifyer.get_notifyer();

	}


	void set_notifyer( std::shared_future<void> notifyer ) {

		m_notifyer = notifyer;

	}


	const std::shared_future<void>& use_notifyer() {

		return m_notifyer;

	}


	// notify�҂�
	void wait() {

		m_notifyer.wait();

	}


	// �w�肵�����Ԃ��o�߂����猻�݂̏����󋵂�Ԃ�
	template< class Rep, class Period >
	std::future_status wait_for( const std::chrono::duration<Rep, Period>& timeout_duration ) {

		/*============================================================================
		// �Q�l�T�C�g�Fhttps://ja.cppreference.com/w/cpp/thread/future/wait_for
		//============================================================================
		// ����
		//============================================================================
		// future_status::deferred	: ���ʂ��v�Z���邽�߂̊֐��͂܂��J�n����Ă��܂���
		// future_status::ready		: ���ʂ͏����������Ă��܂�
		// future_status::timeout	: �^�C���A�E�g���o�߂��܂���
		============================================================================*/

		return m_notifyer.wait_for( timeout_duration );

	}


	// �w�肵�����ԂɂȂ����猻�݂̏����󋵂�Ԃ�
	template< class Clock, class Duration >
	std::future_status wait_until( const std::chrono::time_point<Clock, Duration>& timeout_time ) {

		/*============================================================================
		// �Q�l�T�C�g�Fhttps://ja.cppreference.com/w/cpp/thread/future/wait_until
		//============================================================================
		// ����
		//============================================================================
		// future_status::deferred	: ���ʂ��v�Z���邽�߂̊֐��͂܂��J�n����Ă��܂���
		// future_status::ready		: ���ʂ͏����������Ă��܂�
		// future_status::timeout	: �^�C���A�E�g���o�߂��܂���
		============================================================================*/

		return m_notifyer.wait_until( timeout_time );

	}


};



#pragma endregion notify_getter class





#pragma region notify_callback




// m_args�̈�ԍŏ��̗v�f�Ɏ��N���X��&threadsafe_func_type��ǉ��ł���������ł���
#if FALSE





template<class Func>
class notify_callback{};



// notifyer�N���X�Enotify_getter�N���X����STL std::stop_source, std::stop_token�Ɏ����悤�ȏ������s���Ă��邽��
// std::stop_callback�N���X�Ɏ����悤�ȃN���X�������ɐV�����쐬����
template<class Result, class ...FuncArgs>
class notify_callback<Result( FuncArgs... )> {

private:


	typedef std::tuple<threadsafe_function<Result( FuncArgs... )>> threadsafe_tuple_type;

public:


	typedef notify_callback<Result(FuncArgs...)> this_type;					// ���g�̃f�[�^�^
	typedef async_function<Result( this_type )> async_func_type;					// �񓯊����������s����֐��N���X�^
	typedef threadsafe_function<Result( FuncArgs... )> threadsafe_func_type;	// ���g�ۗ̕L����X���b�h�Z�[�t�֐��N���X�̃f�[�^�^
	typedef std::function<Result( FuncArgs... )> func_type;							// ���g�ۗ̕L����X���b�h�Z�[�t�֐��N���X�̊֐��I�u�W�F�N�g�^
	typedef Result result_type;																// ���g�ۗ̕L����X���b�h�Z�[�t�֐��N���X�̊֐��I�u�W�F�N�g�̖߂�l�^
	typedef std::tuple<FuncArgs...> args_type;											// ���g�ۗ̕L����X���b�h�Z�[�t�֐��N���X�̊֐��I�u�W�F�N�g�̈����^���܂Ƃ߂�tuple�^

private:


	async_func_type m_asyncFunction;
	threadsafe_func_type m_threadsafe_func;

	args_type m_args;

	notify_getter m_info_getter;

public:


	notify_callback() noexcept {};


	// RAII�������̗p����R���X�g���N�^
	notify_callback( notify_getter notify_getter, func_type function, FuncArgs... args ) {

		// �R�[���o�b�N����֐���ݒ�@���R�[���o�b�N�֐��F�񓯊��Ŏ��s�����֐�
		set_func( function );

		// �R�[���o�b�N�֐��̎��s���Ɏg�p���������ݒ�
		set_args( args );

		// �ʒm���������ۂɎ󂯎��I�u�W�F�N�g
		m_info_getter = notify_getter;

	}


private:


	
	result_type callback_exe() {

		// �ʒm���Ԃł����őҋ@
		m_info_getter.wait();

		// �ҋ@�������I��������֐������s����
		return std::apply( 
			&decltype(m_threadsafe_func)::this_type::func_exe,
			m_args
		);

	}


	result_type threadsafe_callback_exe() {

		// �ʒm���Ԃł����őҋ@
		m_info_getter.wait();

		// �ҋ@�������I��������X���b�h�Z�[�t�Ȋ֐������s����
		return std::apply(
			&decltype( m_threadsafe_func )::this_type::threadsafe_func_exe,
			m_args
		);

	}

public:


	void set_func( func_type function ) {

		// �֐���ݒ�
		m_threadsafe_func = function;
		m_asyncFunction.set_func( &this_type::callback_exe );

	}


	void set_args( FuncArgs... args ) {

		// std::apply�֐����g�p�����std::tuple�̒��g���֐��ɓW�J���ēn�����Ƃ��ł���炵���Fhttps://qiita.com/seriru13/items/5115e92fa2665d01d2e1#:~:text=std%3A%3Aapply%20%E3%81%AF%E5%AE%9F%E8%A1%8C%E3%81%99%E3%82%8B%E9%96%A2%E6%95%B0%E3%81%A8%E3%82%BF%E3%83%97%E3%83%AB%E3%82%92%E5%BC%95%E6%95%B0%E3%81%A8%E3%81%97%E3%81%A6%E5%8F%96%E3%82%8A%E3%80%81,std%3A%3A__apply_impl%20%E3%82%92%E5%91%BC%E3%81%B3%E5%87%BA%E3%81%97%E3%81%BE%E3%81%99%E3%80%82
		// ������g�p���Čォ��������֐��ɓn���Fhttps://qiita.com/_EnumHack/items/677363eec054d70b298d#index-tuple-idiom-the-indices-trick
		
		
		m_args = std::make_tuple<threadsafe_func_type*, FuncArgs...>( &m_threadsafe_func, args,,, );

	}


	void set_notify_getter( notify_getter notify_getter ) {

		m_info_getter = notify_getter;

	}



	result_type get_result() {

		m_asyncFunction.get_result();

	}



};

#endif




// C++20�Ŏ�������Ă���std::stop_callback�Ɠ����̋@�\�����i�����j
// notifyer�N���X��notify���\�b�h�����s�����^�C�~���O�ł��̃R�[���o�b�N�֐����Ă΂��
class o_notify_callback {

private:


	async_function<void(o_notify_callback*)> m_asyncFunction;

	threadsafe_function<void()> m_threadsafeFunction;

	o_notify_getter m_notifyGetter;

private:


	void callback_exe() {

		m_notifyGetter.wait();

		m_threadsafeFunction();

	}

public:


	o_notify_callback( o_notify_getter notify_getter, std::function<void()> func ) {

		m_notifyGetter = notify_getter;

		m_threadsafeFunction.set_func( func );

		m_asyncFunction.set_func( &o_notify_callback::callback_exe );

		m_asyncFunction.async( this );

	}


};





#pragma endregion notify_callback



#pragma endregion old_notify class










#pragma region new notifyer class




// �O���錾
class notifyer;





class notify_getter {

private:


	using condition_variable_pointer = std::add_pointer_t<std::condition_variable>;
	using consistency_flag_pointer = std::add_pointer_t<bool>;
	using mutex_pointer = std::add_pointer_t<std::mutex>;

	using condition_variable_lvalue = std::add_lvalue_reference_t<std::condition_variable>;
	using consistency_flag_lvalue = std::add_lvalue_reference_t<bool>;
	using mutex_lvalue = std::add_lvalue_reference_t<std::mutex>;

private:


	condition_variable_pointer m_cvPointer;				// notifyer�N���X�̎������ϐ��ւ̃|�C���^�[
	consistency_flag_pointer m_consistencyFlagPointer;	// notifyer�N���X�̎���spurious wakeup�h�~�t���O�̃|�C���^�[
	mutex_pointer m_mutexPointer;								// notifyer�N���X�̎���mutex�^�ւ̃|�C���^�[

public:


	notify_getter() = default;
	notify_getter( const notify_getter& notifyGetter ) = default;
	notify_getter( notify_getter&& notifyGetter ) = default;


	notify_getter( notifyer& notifyer );
	notify_getter( notifyer&& notifyer );


	notify_getter( condition_variable_pointer cvPointer, consistency_flag_pointer consistencyFlagPointer, mutex_pointer mutexPointer );
	notify_getter( condition_variable_lvalue cvPointer, consistency_flag_lvalue consistencyFlagPointer, mutex_lvalue mutexPointer );
	
public:


	notify_getter& operator=( const notify_getter& notifyGetter ) = default;
	notify_getter& operator=( notify_getter&& notifyGetter ) = default;

	notify_getter& operator=( notifyer& notifyer);
	notify_getter& operator=( notifyer&& notifyer);

public:


	// ���̒ʒm��M�֐����L�����ǂ����i�L����notifyer��notify_getter���ǂ����Ƃ����Ӗ��ŁAfalse�Ȃ�ʒm�֐����󂯎��Ă��Ȃ��Ƃ����Ӗ��j
	bool enable() {

#ifdef _DEBUG

		try {

			if( m_mutexPointer != nullptr && m_cvPointer != nullptr && m_consistencyFlagPointer != nullptr ) {

				return true;

			}
			else {

				throw std::runtime_error( "is invalid notify_getter." );

				return false;

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}
		
#else

		return ( m_mutexPointer != nullptr && m_cvPointer != nullptr && m_consistencyFlagPointer != nullptr );

#endif

	}


	// ���ݕێ����Ă���ʒm���󂯎���̃f�[�^���폜����
	void discard() {

		m_mutexPointer = nullptr;
		m_cvPointer = nullptr;
		m_consistencyFlagPointer = nullptr;

	}

public:

//===================================================
// ���b�N�擾�֐�
//===================================================


	// ���g�̕ێ�����std::mutex�ւ̎Q�Ƃ�Ԃ�
	std::mutex& get_mutex() {

#ifdef _DEBUG

		try {

			if( enable() ) {

				return *m_mutexPointer;

			}
			else {

				throw std::runtime_error( "mutex is nullptr." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		return *m_mutexPointer;

#endif
	}


	// ���g�̕ێ�����std::mutex���g�p����std::unique_lock���쐬
	std::unique_lock<std::mutex> get_lock() {

#ifdef _DEBUG

		try {

			if( enable() ) {

				return std::unique_lock<std::mutex>( *m_mutexPointer );

			}
			else {

				throw std::runtime_error( "Failed to acquire the lock." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		return std::unique_lock<std::mutex> ( *m_mutexPointer );

#endif

	}


	// ���g�̕ێ�����std::mutex���g�p���Ēx�����b�N��std::unique_lock���쐬�@
	// ���x�����b�N�̂��ߎ��g�Ń��b�N�֐������s����K�v������
	std::unique_lock<std::mutex> get_defferd_lock() {

#ifdef _DEBUG

		try {

			if( enable() ) {

				return std::unique_lock<std::mutex>( *m_mutexPointer, std::defer_lock );

			}
			else {

				throw std::runtime_error( "Failed to acquire the deferred lock." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		return std::unique_lock<std::mutex>( *m_mutexPointer, std::defer_lock );

#endif

	}


	// ���g�̕ێ�����std::mutex���g�p���Ă�std::unique_lock���쐬
	// �����b�N���擾�ł����true��Ԃ��A�擾�ł��Ȃ����false��Ԃ�
	std::unique_lock<std::mutex> get_try_lock() {

#ifdef _DEBUG

		try {

			if( enable() ) {

				return std::unique_lock<std::mutex>( *m_mutexPointer, std::try_to_lock );

			}
			else {

				throw std::runtime_error( "Failed to acquire the try lock." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		return std::unique_lock<std::mutex>( *m_mutexPointer, std::try_to_lock );

#endif

	}


	// ���g�̕ێ�����std::mutex���g�p���Ă��̊֐����s�ȑO�Ƀ��b�N�擾�ς݂�std::mutex�̃��b�N������std::unique_lock�Ɉڏ������std::unique_lock���쐬�@
	// ����{�I�ɂ͎g�p���Ȃ����b�N�擾���@�����@�\�Ƃ��Ă̒񋟂݂̂��s���Ă����ԂŊ댯�����������b�N�擾���@�ɂȂ�i�ʒm���̑��݂���X���b�h�̃��b�N���͂����Ă��܂����Ƃɑ������邽�ߊ댯�łȃ��b�N�擾���@�j
	std::unique_lock<std::mutex> get_adopt_lock() {

#ifdef _DEBUG

		try {

			if( enable() ) {

				return std::unique_lock<std::mutex>( *m_mutexPointer, std::adopt_lock );

			}
			else {

				throw std::runtime_error( "Failed to acquire the adopt lock." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		return std::unique_lock<std::mutex>( *m_mutexPointer, std::adopt_lock );

#endif

	}


	// ���g�̕ێ����鐮�����t���O�̌��ʂ��擾����@���������t���O�F���̃N���X�̎��ʒm����notifyer�N���X�ɓ��B���Ă��邩�ǂ����������t���O
	bool get_consistency_flag() {

#ifdef _DEBUG

		try {

			if( enable() ) {

				return *m_consistencyFlagPointer;

			}
			else {

				throw std::runtime_error( "condition variable is nullptr." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		return *m_consistencyFlagPointer;

#endif

	}

public:


#pragma region wait 


	// �ʒm���ɏ��������B����܂ł��̃X���b�h�̏�����ҋ@����
	void wait( std::unique_lock<std::mutex>& lock ) {

#ifdef _DEBUG

		try {

			if( enable() ) {

				m_cvPointer->wait( lock, [ this ] { return get_consistency_flag(); } );

				//discard(); // ��xwait���g�p������Ďg�p�s�ɂ���

			}
			else {

				throw std::runtime_error( "Failed to execute the wait method." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		m_cvPointer->wait( lock, [this] { return get_consistency_flag(); } );

#endif

	}


	// �ʒm���ɏ��������B���Apredicate�֐��̎��s���ʂ�true��Ԃ��܂ł��̃X���b�h�̏�����ҋ@����
	template<class Predicate>
	void wait( std::unique_lock<std::mutex>& lock, Predicate predicate ) {

#ifdef _DEBUG

		try {

			if( enable() ) {

				m_cvPointer->wait( lock, [ & ] { return predicate(); } );

			}
			else {

				throw std::runtime_error( "Failed to execute the wait method." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		m_cvPointer->wait( lock, [&] { return predicate(); } );

#endif

	}



#pragma endregion wait 



#pragma region wait_for 


	// �w�肵�����Ԍo�ߌ�ɑҋ@�������I���ł���Ȃ�I�����I���ł��Ȃ��Ȃ�I�����Ȃ�
	// �ҋ@�������I���ł����ꍇ��std::cv_status::no_timeout��Ԃ��A�I���ł��Ȃ������ꍇ��std::cv_status::timeout��Ԃ�
	template< class Rep, class Period >
	std::cv_status wait_for( std::unique_lock<std::mutex>& lock, const std::chrono::duration<Rep, Period>& timeoutDuration ) {

#ifdef _DEBUG

		try {

			if( enable() ) {

				return m_cvPointer->wait_for( lock, timeoutDuration );

			}
			else {

				throw std::runtime_error( "Failed to execute the wait_for method." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		return m_cvPointer->wait_for( lock, timeoutDuration );

#endif

	}



	// �w�肵�����Ԍo�ߌ�ɑҋ@�������I���ł���Ȃ�I�����I���ł��Ȃ��Ȃ�I�����Ȃ�
	// �ҋ@�������I���ł��Ă���q��(predicate)��true�̏ꍇ��true��Ԃ��A����ȊO�̏ꍇ��false��Ԃ�
	template< class Rep, class Period, class Predicate >
	bool wait_for_pred( std::unique_lock<std::mutex>& lock, const std::chrono::duration<Rep, Period>& timeoutDuration, Predicate predicate ) {

#ifdef _DEBUG

		try {

			if( enable() ) {

				return m_cvPointer->wait_for( lock, timeoutDuration, [ & ] { predicate(); } );

			}
			else {

				throw std::runtime_error( "Failed to execute the wait_for_pred method." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		return m_cvPointer->wait_for( lock, timeoutDuration, [&] { predicate(); } );

#endif

	}


	// �w�肵�����Ԍo�ߌ�ɑҋ@�������I���ł���Ȃ�I�����I���ł��Ȃ��Ȃ�I�����Ȃ�
	// �ҋ@�������I���ł����ꍇ��true��Ԃ��A�I���ł��Ȃ������ꍇ��false��Ԃ�
	template< class Rep, class Period >
	bool wait_for_pred( std::unique_lock<std::mutex>& lock, const std::chrono::duration<Rep, Period>& timeoutDuration ) {

#ifdef _DEBUG

		try {

			if( enable() ) {

				return m_cvPointer->wait_for( lock, timeoutDuration, [ this ] { return get_consistency_flag(); } );

			}
			else {

				throw std::runtime_error( "Failed to execute the wait_for_pred method." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		return m_cvPointer->wait_for( lock, timeoutDuration, [ this ] { return get_consistency_flag(); } );

#endif

	}



#pragma endregion wait_for



#pragma region wait_until


	// �w�肵�����Ԃ܂łɑҋ@�������I���ł���Ȃ�I�����I���ł��Ȃ��Ȃ�I�����Ȃ�
	// �ҋ@�������I���ł����ꍇ��std::cv_status::no_timeout��Ԃ��A�I���ł��Ȃ������ꍇ��std::cv_status::timeout��Ԃ�
	template< class Clock, class Duration >
	std::cv_status wait_until( std::unique_lock<std::mutex>& lock, const std::chrono::time_point<Clock, Duration>& timeoutTime ) {

#ifdef _DEBUG

		try {

			if( enable() ) {

				return m_cvPointer->wait_until( lock, timeoutTime );

			}
			else {

				throw std::runtime_error( "Failed to execute the wait_until method." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		return m_cvPointer->wait_until( lock, timeoutTime );

#endif

	}


	// �w�肵�����Ԃ܂łɑҋ@�������I���ł���Ȃ�I�����I���ł��Ȃ��Ȃ�I�����Ȃ�
	// �ҋ@�������I���ł��Ă���q��(predicate)��true�̏ꍇ��true��Ԃ��A����ȊO�̏ꍇ��false��Ԃ�
	template< class Clock, class Duration, class Predicate >
	bool wait_until_pred( std::unique_lock<std::mutex>& lock, const std::chrono::time_point<Clock, Duration>& timeoutTime, Predicate predicate ) {

#ifdef _DEBUG

		try {

			if( enable() ) {

				return m_cvPointer->wait_until( lock, timeoutTime, predicate );

			}
			else {

				throw std::runtime_error( "Failed to execute the wait_until_pred method." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		return m_cvPointer->wait_until( lock, timeoutTime, predicate );

#endif

	}


	// �w�肵�����Ԃ܂łɑҋ@�������I���ł���Ȃ�I�����I���ł��Ȃ��Ȃ�I�����Ȃ�
	// �ҋ@�������I���ł����ꍇ��true��Ԃ��A�I���ł��Ȃ������ꍇ��false��Ԃ�
	template< class Clock, class Duration >
	bool wait_until_pred( std::unique_lock<std::mutex>& lock, const std::chrono::time_point<Clock, Duration>& timeoutTime ) {

#ifdef _DEBUG

		try {

			if( enable() ) {

				return m_cvPointer->wait_until( lock, timeoutTime, [ this ] { return get_consistency_flag(); } );

			}
			else {

				throw std::runtime_error( "Failed to execute the wait_until_pred method." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		return m_cvPointer->wait_until( lock, timeoutTime, [ this ] { return get_consistency_flag(); } );

#endif

	}



#pragma endregion wait_until



};





class notifyer {

private:


	std::condition_variable m_cv;		// �ێ���������ϐ�
	bool m_consistencyFlag = false;	// notify�n�֐����Ă΂ꂽ���Ƃ�ۏ؂��邽�߂̐������t���O
	std::mutex m_mutex;					// �ێ�����mutex

public:


	notify_getter&& get_notifyer() {

		return notify_getter(
			&m_cv,
			&m_consistencyFlag,
			&m_mutex
		);

	}



	std::mutex& get_mutex() {

		return m_mutex;

	}



	std::unique_lock<std::mutex> get_lock() {

		return std::unique_lock<std::mutex>( m_mutex );

	}



	std::unique_lock<std::mutex> get_defferd_lock() {

		return std::unique_lock<std::mutex>( m_mutex, std::defer_lock );

	}



	std::unique_lock<std::mutex> get_try_lock() {

		return std::unique_lock<std::mutex>( m_mutex, std::try_to_lock );

	}



	std::unique_lock<std::mutex> get_adopt_lock() {

		return std::unique_lock<std::mutex>( m_mutex, std::adopt_lock );

	}

public:


	//======================================================================================================
	// ���̃N���X�C���X�^���X��get_notifyer�����o�֐����g�p���č쐬���ꂽ
	// notify_getter�N���X�C���X�^���X�ł�wait�n�֐������s���̃X���b�h���ǂꂩ��̂ݑҋ@�������I��������ʒm�𑗂�
	//======================================================================================================o
	// �����̃����o�֐����g�p����ۂ͈ȉ��̏��Ԃ����K�v������B�@����ɏ��炵�Ȃ��ꍇ�͖���`�̓���ƂȂ�
	// �@���̃N���X�C���X�^���X����get_notifyer�����o�֐���p���č쐬���ꂽ
	// �@notify_getter�N���X�C���X�^���X���g�p����wait�n�֐��̎��s�������s���X���b�h���ɍ쐬����
	// �A���̃����o�֐������s����X���b�h���쐬����
	//======================================================================================================
	// �ȏ�̏��Ԃ����炵�Ȃ��ꍇ�͈ȉ��̂悤�ȃo�O�������\��������
	// �@notify_one����x�������s���Ă��Ȃ��ɂ��ւ�炸�A������wait�n�֐����ҋ@�������I������inotify_all�̂悤�ȋ���������j
	void notify_one() {

		m_consistencyFlag = true;

		m_cv.notify_one();

	}


	// ���̃N���X�C���X�^���X��get_notifyer�����o�֐����g�p���č쐬���ꂽ
	// notify_getter�N���X�C���X�^���X�ł�wait�n�֐������s���̃X���b�h��S�đҋ@�������I��������ʒm�𑗂�
	void notify_all() {

		m_consistencyFlag = true;

		m_cv.notify_all();

	}


	// ���̃����o�֐���std::thread���Ŏ��s���ꂽ�ꍇ�̂ݎ��s�\�A����ȊO�̏ꍇ�͖���`�̓�����N����
	// std::async�֐��̒��Ŏ��s�����ꍇ�͎��s����Ȃ�
	void notify_all_at_thread_exit( std::unique_lock<std::mutex>& lock ) {

		m_consistencyFlag = true;

		std::notify_all_at_thread_exit( m_cv, std::move(lock) );

	}

};



#pragma endregion new notifyer class





#pragma region wait object class



// RAII�N���X�ŒP���ɓn���ꂽnotifyer����ʒm������܂ő҂����̃N���X
// �g������std::lcok_guard�̂悤�Ɏg�p����
class wait_object {

public:


	wait_object() = delete;
	wait_object( const wait_object& ) = delete;
	wait_object( wait_object&& ) = delete;



	explicit wait_object( notifyer& notifyer ) {

		auto notify_getter = notifyer.get_notifyer();

		auto lock = notify_getter.get_lock();

		notify_getter.wait( lock );

	}


	explicit wait_object( notify_getter& notify_getter ) {

		auto lock = notify_getter.get_lock();

		notify_getter.wait( lock );

	}

};



#pragma endregion wait object class









// �N���X���ɃC�����C���W�J�����notifyer�N���X�̒�`�������炸�G���[��f���̂ŁA��ނȂ��t�@�C���ŉ����ɃC�����C���W�J���s���Ă���
#pragma region notify_getter constructor



inline notify_getter::notify_getter( notifyer& notifyer ) {
	*this = notifyer.get_notifyer();
};



inline notify_getter::notify_getter( notifyer&& notifyer ) {
	*this = notifyer.get_notifyer();
};



inline notify_getter::notify_getter( condition_variable_pointer cvPointer, consistency_flag_pointer consistencyFlagPointer, mutex_pointer mutexPointer )
	: m_cvPointer( cvPointer ), m_consistencyFlagPointer( consistencyFlagPointer ), m_mutexPointer( mutexPointer ) {};



inline notify_getter::notify_getter( condition_variable_lvalue cvPointer, consistency_flag_lvalue consistencyFlagPointer, mutex_lvalue mutexPointer )
	: m_cvPointer( &cvPointer ), m_consistencyFlagPointer( &consistencyFlagPointer ), m_mutexPointer( &mutexPointer ) {};



#pragma endregion notify_getter constructor



#pragma region notify_getter operator=



inline notify_getter& notify_getter::operator=( notifyer& notifyer ) {
	*this = notifyer.get_notifyer();
};



inline notify_getter& notify_getter::operator=( notifyer&& notifyer ) {
	*this = notifyer.get_notifyer();
};



#pragma endregion notify_getter operator=
