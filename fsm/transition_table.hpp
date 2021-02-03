/*
 * transition_table.hpp
 *
 *  Created on: 18 янв. 2021 г.
 *      Author: sazhi
 */

#ifndef TRANSITION_TABLE_HPP_
#define TRANSITION_TABLE_HPP_

#pragma once

#include "transition.hpp"

namespace th{

template<typename ... Types> struct Collection
 {
   static constexpr int size  =  (sizeof...(Types));
 };


//Определяем серию операторов сравнения списков типов:

template <class... Ts, class... Us>
constexpr bool operator==(Collection<Ts...>, Collection<Us...>){
	return false;
}

template <class... Ts>
constexpr bool operator==(Collection<Ts...>, Collection<Ts...>){
	return true;
}


template <class... Ts, class... Us>
constexpr bool operator!=(Collection<Ts...>, Collection<Us...>){
	return true;
}

template <class... Ts>
constexpr bool operator!=(Collection<Ts...>, Collection<Ts...>){
	return false;
}
  struct NullType {};

  /////////////////  NoDuplicates from LOKI ////////////////
  template<class X, class Y> struct Glue;
  template<class T, class... Ts>
  struct Glue<T, Collection<Ts...>> {
    using Result = Collection<T, Ts...>;};

  template<class Q, class X> struct Erase;

  template<class Q>
  struct Erase<Q, Collection<>> {
    using Result = Collection<>;};

  template<class Q, class... Tail>
  struct Erase<Q, Collection<Q, Tail...>> {
    using Result = Collection<Tail...>;};

  template<class Q, class T, class... Tail>
  struct Erase<Q, Collection<T, Tail...>> {
    using Result = typename Glue<T, typename Erase<Q, Collection<Tail...>>::Result>::Result;};

  template <class X> struct NoDuplicates;

  template <> struct NoDuplicates<Collection<>>
  {
    using Result = Collection<>;
  };

  template <class T, class... Tail>
  struct NoDuplicates< Collection<T, Tail...> >
  {
    private:
      using L1 = typename NoDuplicates<Collection<Tail...>>::Result;
      using L2 = typename Erase<T,L1>::Result;
    public:
      using Result = typename Glue<T, L2>::Result;
  };
///////////////// LOKI ////////////////
} // end of namespace th (table_helper)


/*--START STATIC TEST--*/
using tc = th::Collection<char, int, char, double>;
using tcwd = typename th::NoDuplicates<tc>::Result;

static_assert(tcwd{} == th::Collection<char, int, double>{});

/*--STOP STATIC TEST--*/


template<typename... Types>
constexpr auto get_var (th::Collection<Types...>){
	return std::variant<Types...>{};
}

struct TransitionTableBase{};

template<typename... T>
struct TransitionTable : TransitionTableBase{

	using test_t =  typename th::NoDuplicates<th::Collection<T...>>::Result;
	static_assert(std::is_same_v<test_t, th::Collection<T...>>, "Repeated transitions");

	using transition_p = type_pack<T...>;

	using state_collection =
			typename th::NoDuplicates<th::Collection<typename T::source_t... , typename T::target_t...> >::Result;

	using event_collection =
				typename th::NoDuplicates<th::Collection<typename T::event_t...> >::Result;

	using state_v = decltype(get_var(state_collection{}));
	using event_v = decltype(get_var(event_collection{}));
	using transition_v = std::variant<T...>;
};

/*--START STATIC TEST--*/

/*--STOP STATIC TEST--*/

#endif /* TRANSITION_TABLE_HPP_ */
