/*
 * transition.hpp
 *
 *  Created on: 18 янв. 2021 г.
 *      Author: sazhi
 */

#ifndef TRANSITION_HPP_
#define TRANSITION_HPP_

#pragma once

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

/*--END STATIC TEST--*/


#endif /* TRANSITION_HPP_ */
