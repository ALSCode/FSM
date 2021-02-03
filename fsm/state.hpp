/*
 * state.hpp
 *
 *  Created on: 18 янв. 2021 г.
 *      Author: sazhi
 */

#ifndef STATE_HPP_
#define STATE_HPP_

#pragma once

#include "function_view.hpp"
#include "typelist.hpp"

#include "names.hpp"

struct StateBase{};

template <unsigned int N, typename Action = void>
struct State : StateBase{

	static constexpr unsigned int idx = N;

	using action_t = Action;
};

/*--START STATIC TEST--*/
/*--END STATIC TEST--*/

#endif /* STATE_HPP_ */
