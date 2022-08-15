#pragma once






#include<thread>
#include<future>
#include<chrono>
#include<type_traits>
#include<iostream>

#include"ThreadSafeFunction.hpp"





template<class Func>
class async_function_interface;


template<class Result, class ...FuncArgs>
class async_function_interface<Result(FuncArgs...)> {

public:


	using threadsafe_func_type = threadsafe_function<Result( FuncArgs... )>;	// ���g�ۗ̕L����X���b�h�Z�[�t�֐��N���X�̃f�[�^�^
	using func_type = std::function<Result( FuncArgs... )>;							// ���g�ۗ̕L����X���b�h�Z�[�t�֐��N���X�̊֐��I�u�W�F�N�g�^
	using result_type = Result;																// ���g�ۗ̕L����X���b�h�Z�[�t�֐��N���X�̊֐��I�u�W�F�N�g�̖߂�l�^
	using args_type = std::tuple<FuncArgs...>;											// ���g�ۗ̕L����X���b�h�Z�[�t�֐��N���X�̊֐��I�u�W�F�N�g�̈����^���܂Ƃ߂�tuple�^

public:


	// �֐������s����@�����̃����o�֐���ʂ��Ď��s����֐��̓X���b�h�Z�[�t�ł͂Ȃ�
	virtual result_type operator()( FuncArgs... args ) = 0;

	// �֐����w�肵�����[���`�ŊJ�n����
	virtual void operator()( std::launch launch, FuncArgs... args ) = 0;

	// �X���b�h�Z�[�t�ɂ��邩�ǂ�����I�����A�֐����w�肵�����[���`�ŊJ�n����
	virtual void operator()( threadsafe_flag isThreadsafe, std::launch launch, FuncArgs... args ) = 0;

public:


	// �g�p����֐��I�u�W�F�N�g��ݒ肷��
	virtual void set_func( func_type func ) = 0;


	// ���g�ۗ̕L����֐��I�u�W�F�N�g��n���@���������Ă��Ȃ��Ȃ�nullptr��Ԃ�
	virtual func_type own_func() = 0;


	// �֐������s����@�����̃����o�֐���ʂ��Ď��s����֐��̓X���b�h�Z�[�t�ł͂Ȃ�
	virtual result_type func_exe( FuncArgs... args ) = 0;


	// �X���b�h�Z�[�t�Ɋ֐������s����
	virtual result_type threadsafe_func_exe( FuncArgs... args ) = 0;

public:


	// �֐���񓯊����s����
	virtual void async( FuncArgs... args ) = 0;


	// �֐����X���b�h�Z�[�t�ɔ񓯊����s����
	virtual void deferred( FuncArgs... args ) = 0;

};



// ���̃C���^�[�t�F�C�X�N���X��async_function�N���X�Ōp�����Ă���ƃG���[���o��
// async_function_deque�N���X�̏ꍇ�̓G���[���o�Ȃ�