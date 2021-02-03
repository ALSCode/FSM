/*
 * state_machine.hpp
 *
 *  Created on: 18 янв. 2021 г.
 *      Author: sazhi
 */

#ifndef STATE_MACHINE_HPP_
#define STATE_MACHINE_HPP_

#pragma once

#include <unordered_map>
#include <algorithm>

#include "ring_buffer.hpp"

#include <transition_table.hpp>
#include "key.hpp"

template<typename Table>
class StateMachine{

public:

	using transition_pack = typename Table::transition_p;

	using transition_v = typename Table::transition_v;
	using state_v = typename Table::state_v;
	using event_v = typename Table::event_v;

	StateMachine()
	{
		set(transition_pack{});
	}

	template <unsigned int N>
	void push_event (const Event<N>& e){

		events.push_back(e);
	}

	void process (void){

		state_action();

		auto it = transitions.begin();

		Key k;
		k.state_idx = key.state_idx;

		for (uint32_t i = 0; i != events.size(); ++i){

			auto v = events.front();

			std::visit([&](const auto& arg){

				using event_t =  std::decay_t<decltype(arg)>;
				k.event_idx = event_t::idx;

				it = transitions.find(k);

			}, v);


			if ( it != transitions.end() ){

				events.pop_front();

				on_event_impl(k);

				return;

			} else {

				events.push_back(v);

				events.pop_front();
			}
		}
	}

	template <typename... Args>
	void state_action (Args... args){

		state_v temp_v{current_state};

		std::visit([&](const auto& arg) {

			using state_t =  std::decay_t<decltype(arg)>;
			using functor_t = typename state_t::action_t;

			if constexpr (!std::is_same_v<functor_t, void>){

				functor_t{}(args...);
			}

		}, temp_v);
	}

	template <typename Event,
				class = std::enable_if_t<std::is_base_of_v<EventBase, Event>>>
	void on_event(void){

		Key k;
		k.event_idx = Event::idx;
		k.state_idx = key.state_idx;

		on_event_impl(k);
	}

	template <typename Event,
				class = std::enable_if_t<std::is_base_of_v<EventBase, Event>>>
	void on_event(const Event& e){

		Key k;
		k.event_idx = e.idx;
		k.state_idx = key.state_idx;

		on_event_impl(k);
	}

private:

	template <class... Ts>
	void set (type_pack<Ts...>){
		(set_impl(just_type<Ts>{}), ...);
	};


	template <typename T>
	void set_impl (just_type<T> t){

		using transition = typename decltype(t)::type;

		using state_t = typename transition::source_t;
		using event_t = typename transition::event_t;
		Guard g = transition::guard;

		Key k;

		k.state_idx = state_t::idx;
		k.event_idx = event_t::idx;

//		tt.emplace(std::make_pair(k, transition{}));
		transitions.insert( {k, transition{}} );

		if (0 == key.state_idx) {

			key.state_idx = k.state_idx;
			guard = g;
			current_state = state_t{};
		}
	}

	void on_event_impl (Key& k){

		transition_v tr_var = transitions[k];

		Key &ref_k = key;
		Guard &ref_g = guard;
		state_v &ref_state = current_state;

		std::visit([&](const auto& arg) {

			using tr_t =  std::decay_t<decltype(arg)>;
			using functor_t = typename tr_t::action_t;

			if ( GuardEqual{}(ref_g, tr_t::guard) ){

				using target_t = typename tr_t::target_t;

				ref_k.state_idx = target_t::idx;
				ref_state = target_t{};

				functor_t{}();
			}
		}, tr_var);
	}

	using map_type =
				std::unordered_map
				<
					Key,
					transition_v,
					KeyHash,
					KeyEqual,
					std::allocator<std::pair<Key, transition_v>>
				>;

	using stack_type =
					Mcucpp::Containers::RingBufferPO2
				<
					EVENT_STACK_SIZE,
					event_v,
					Mcucpp::Atomic
				>;

	Key key;

	map_type transitions;
	stack_type events;

	state_v current_state;
	Guard guard = Guard::NO_GUARD;
};



#endif /* STATE_MACHINE_HPP_ */
