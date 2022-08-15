#pragma once

#include<vector>

namespace Algorithm {

	namespace Sort {

	// �Q�l�T�C�g�@�Fhttps://qiita.com/ta7uw/items/fbe35038436bbffea9d6
	// �Q�l�T�C�g�A�Fhttps://qiita.com/hiso/items/5c36f50c7de61fe870a2

		template<class Iter>
		void Sort(Iter begin, Iter end) {

		}

		template<class Iter>
		void StableSort(Iter begin, Iter end) {

		}

		// �o�u���\�[�g
		template<class Iter, class ValueType = typename Iter::value_type>
		void BoubleSort(Iter begin, Iter end) {
			for (auto iter = begin; iter != end; iter++) {

			}
		}

		// �V�F�[�J�[�\�[�g�i�o�u���\�[�g�̏�ʌ݊��j
		template<class Iter>
		void ShakerSort(Iter begin, Iter end) {
			for (auto iter = begin; iter != end; iter++) {

			}
		}

		// ����]�u�\�[�g
		template<class Iter>
		void OddEvenSort(Iter begin, Iter end) {
			for (auto iter = begin; iter != end; iter++) {

			}
		}

		// �}�[�W�\�[�g
		template<class Iter>
		void MargeSort(Iter begin, Iter end) {

		}

		// �N�C�b�N�\�[�g
		template<class Iter>
		void QuickSort(Iter begin, Iter end) {

		}

		// �C���g���\�[�g�i�N�C�b�N�\�[�g�̏�ʌ݊��j
		template<class Iter>
		void IntroSort(Iter begin, Iter end) {

		}

		// �}���\�[�g
		template<class Iter>
		std::vector<typename Iter::value_type> InsertSort(Iter begin, Iter end) {
		}

		// �I���\�[�g
		template<class Iter>
		void SelectSort(Iter begin, Iter end) {

		}

		// �V�F���\�[�g
		template<class Iter>
		void ShellSort(Iter begin, Iter end) {

		}

		// �p�P�b�g�\�[�g
		template<class Iter>
		void PacketSort(Iter begin, Iter end) {

		}

	}



	namespace Search {

		constexpr std::nullptr_t MissValue = nullptr;

		// �擪������`�T�����s���v�f�����������Ȃ炻�̗v�f�̃|�C���^��Ԃ��A������Ȃ����Search::MissValue��Ԃ�
		template<class Iter, class ValueType = typename Iter::value_type>
		ValueType* LinearSearch(Iter begin, Iter end, const ValueType& searchValue) {
			for (auto iter = begin; iter != end; iter++) {
				if (*iter == searchValue) {
					return iter;
				}
			}
			return Search::MissValue;
		}

		// ����������`�T�����s���v�f�����������Ȃ炻�̗v�f�̃|�C���^��Ԃ��A������Ȃ����Search::MissValue��Ԃ�
		template<class Iter, class ValueType = typename Iter::value_type>
		ValueType* InverseLinearSearch(Iter begin, Iter end, const ValueType& searchValue) {
			for (auto iter = --end; iter != begin - 1; iter--) {
				if (*iter == searchValue) {
					return iter;
				}
			}
			return Search::MissValue;
		}

		// �񕪒T�����s���v�f�����������Ȃ炻�̗v�f�̃|�C���^��Ԃ��A������Ȃ����Search::MissValue��Ԃ�
		// �����̊֐��̓\�[�g�ς݂̃f�[�^���g�p����K�v������
		template<class Iter, class ValueType = typename Iter::value_type>
		ValueType* BinarySearch(Iter begin, Iter end, const ValueType& searchValue) {

		}

	}

}