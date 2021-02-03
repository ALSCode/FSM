/*
 * event.hpp
 *
 *  Created on: 18 янв. 2021 г.
 *      Author: sazhi
 */

#ifndef EVENT_HPP_
#define EVENT_HPP_

#pragma once

#include "defs.hpp"
#include "names.hpp"

struct EventBase{};

template <unsigned int N>
struct Event : EventBase {
	static constexpr unsigned int idx = N;
};


struct EventEqual{
	template <unsigned int L, unsigned int R>
	constexpr bool operator() (const Event<L>& lhs, const Event<R>& rhs) const{
		return (lhs.idx == rhs.idx);
	}
};

struct EventComp{
	template <unsigned int L, unsigned int R>
		constexpr bool operator() (const Event<L>& lhs, const Event<R>& rhs) const{
		return (lhs.idx > rhs.idx);
	}
};

template <unsigned int N>
struct EventHash{
	std::size_t operator()(const Event<N>& k) const
	{

		return std::hash<unsigned int>{}(k.idx);
	}
};

/*--START STATIC TEST--*/
static_assert(Event<15>::idx == 15);

Event<10> e;
static_assert(e.idx == 10);
/*--END STATIC TEST--*/

#endif /* EVENT_HPP_ */
