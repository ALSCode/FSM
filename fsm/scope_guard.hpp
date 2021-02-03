/*
 * scope_guard.hpp
 *
 *  Created on: 27 янв. 2021 г.
 *      Author: sazhi
 */

#ifndef SCOPE_GUARD_HPP_
#define SCOPE_GUARD_HPP_

#pragma once

#include <utility>

#ifndef CONCAT
#define CONACT2(s1, s2) s1##s2
#define CONCAT(s1, s2) CONCAT2(s1, s2)
#endif

#define SCOPE_VARIABLE(str) CONCAT(str, __COUNTER__)

#define SCOPE_EXIT							\
	auto SCOPE_VARIABLE(scope_exit_state)	\
	 = ScopeGuardOnExit() + [&]()


enum class ScopeGuardOnExit{};

template <typename F>
class ScopeGuard{

public:

	explicit ScopeGuard (const F& f) : f_(f){}
	explicit ScopeGuard (const F&& f) : f_(std::move(f)){}

	~ScopeGuard(){ f_(); }

private:
	F f_;
};

template<typename F>
ScopeGuard<F> operator+ (ScopeGuardOnExit, F&& f) {
	return ScopeGuard<F>(std::forward<F>(f));
}


#endif /* SCOPE_GUARD_HPP_ */
