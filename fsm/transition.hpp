/*
 * transition.hpp
 *
 *  Created on: 18 янв. 2021 г.
 *      Author: sazhi
 */

#ifndef TRANSITION_HPP_
#define TRANSITION_HPP_

#pragma once
#include <variant>

#include "guard.hpp"
#include "state.hpp"
#include "event.hpp"

struct TrBase{};

template <typename Source,
		  typename Event,
		  typename Target,
		  typename Action,
		  Guard G,
		  class = std::enable_if_t<std::is_base_of_v<StateBase, Source> &&
		  	  	  	  	  	  	   std::is_base_of_v<EventBase, Event> &&
								   std::is_base_of_v<StateBase, Target>>
		  >
struct Tr : TrBase {

	using source_t = Source;
	using event_t  = Event;
	using target_t = Target;
	using action_t = Action;

	static constexpr Guard guard = G;
};


/*--START STATIC TEST--*/

using functor_t =  function_view<void(void)>;

using tr = Tr<State<1, functor_t>, Event<2>, State<3, functor_t>, functor_t, Guard::NO_GUARD>;

tr t;
//
//using state_t = typename decltype(t)::source_t;
//static_assert(std::is_same_v<state_t, State<1>>);
//static_assert(tr::guard == Guard::NO_GUARD);
static_assert(std::is_same_v<functor_t, typename decltype(t)::action_t>);

/*--END STATIC TEST--*/


#endif /* TRANSITION_HPP_ */
