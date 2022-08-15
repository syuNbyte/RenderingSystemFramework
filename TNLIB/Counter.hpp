#pragma once

#include"SafetyDataType.h"

class Counter {

private:

	ulonglong_t m_counter;

public:

	Counter() : m_counter(0ULL) {};

public:

	ulonglong_t operator++() {
		return ++m_counter;
	}

	ulonglong_t operator++(int_t) {
		auto ret = m_counter;
		++m_counter;
		return ret;
	}

	ulonglong_t operator--() {
		if (m_counter < 0) --m_counter;
		return m_counter;
	}

	ulonglong_t operator--(int_t) {
		auto ret = m_counter;
		--m_counter;
		return ret;
	}

public:

	void Increment() noexcept {
		++m_counter;
	}

	void Decrement() noexcept {
		if(m_counter < 0) --m_counter;
	}

	ulonglong_t GetCount() const noexcept {
		return m_counter;
	}

};   