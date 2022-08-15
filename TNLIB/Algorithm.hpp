#pragma once

#include<vector>

namespace Algorithm {

	namespace Sort {

	// 参考サイト①：https://qiita.com/ta7uw/items/fbe35038436bbffea9d6
	// 参考サイト②：https://qiita.com/hiso/items/5c36f50c7de61fe870a2

		template<class Iter>
		void Sort(Iter begin, Iter end) {

		}

		template<class Iter>
		void StableSort(Iter begin, Iter end) {

		}

		// バブルソート
		template<class Iter, class ValueType = typename Iter::value_type>
		void BoubleSort(Iter begin, Iter end) {
			for (auto iter = begin; iter != end; iter++) {

			}
		}

		// シェーカーソート（バブルソートの上位互換）
		template<class Iter>
		void ShakerSort(Iter begin, Iter end) {
			for (auto iter = begin; iter != end; iter++) {

			}
		}

		// 奇偶転置ソート
		template<class Iter>
		void OddEvenSort(Iter begin, Iter end) {
			for (auto iter = begin; iter != end; iter++) {

			}
		}

		// マージソート
		template<class Iter>
		void MargeSort(Iter begin, Iter end) {

		}

		// クイックソート
		template<class Iter>
		void QuickSort(Iter begin, Iter end) {

		}

		// イントロソート（クイックソートの上位互換）
		template<class Iter>
		void IntroSort(Iter begin, Iter end) {

		}

		// 挿入ソート
		template<class Iter>
		std::vector<typename Iter::value_type> InsertSort(Iter begin, Iter end) {
		}

		// 選択ソート
		template<class Iter>
		void SelectSort(Iter begin, Iter end) {

		}

		// シェルソート
		template<class Iter>
		void ShellSort(Iter begin, Iter end) {

		}

		// パケットソート
		template<class Iter>
		void PacketSort(Iter begin, Iter end) {

		}

	}



	namespace Search {

		constexpr std::nullptr_t MissValue = nullptr;

		// 先頭から線形探索を行い要素が見つかったならその要素のポインタを返し、見つからなければSearch::MissValueを返す
		template<class Iter, class ValueType = typename Iter::value_type>
		ValueType* LinearSearch(Iter begin, Iter end, const ValueType& searchValue) {
			for (auto iter = begin; iter != end; iter++) {
				if (*iter == searchValue) {
					return iter;
				}
			}
			return Search::MissValue;
		}

		// 末尾から線形探索を行い要素が見つかったならその要素のポインタを返し、見つからなければSearch::MissValueを返す
		template<class Iter, class ValueType = typename Iter::value_type>
		ValueType* InverseLinearSearch(Iter begin, Iter end, const ValueType& searchValue) {
			for (auto iter = --end; iter != begin - 1; iter--) {
				if (*iter == searchValue) {
					return iter;
				}
			}
			return Search::MissValue;
		}

		// 二分探索を行い要素が見つかったならその要素のポインタを返し、見つからなければSearch::MissValueを返す
		// ※この関数はソート済みのデータを使用する必要がある
		template<class Iter, class ValueType = typename Iter::value_type>
		ValueType* BinarySearch(Iter begin, Iter end, const ValueType& searchValue) {

		}

	}

}