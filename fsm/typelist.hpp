/*
 * typelist.hpp
 *
 *  Created on: 26 авг. 2020 г.
 *      Author: sazhi
 */

#ifndef CPP_WRAPPER_INC_TYPELIST_HPP_
#define CPP_WRAPPER_INC_TYPELIST_HPP_

#include <type_traits>
#include <utility> //for std::index_sequence
#include <cstdint>

template<class A, class B>
struct is_same : std::false_type{};

template<class T>
struct is_same<T, T> : std::true_type{};

static_assert(is_same<int, int>::value);


//Создаем базовый примитив, "typeholder", для хранения одного типа.
template <class T>
struct just_type{
	using type = T;
};


//С помощью этого примитива можно доставать, преобразовывать в условное "значение"
//	и затем оперировать им в последующем коде:

constexpr auto type = just_type<int>{};
//using type_t = just_type<int>::type;
//just_type<int> type{};
using type_t = typename decltype(type)::type;
static_assert(is_same<type_t, int>::value);



//Создаем метафункцию, т.н. "индикатор", что это список типов:

template <class... Ts>
struct type_pack{

};


//Создаем алиас для пустого списка:
using empty_pack = type_pack<>;



//Функция, возвращающая размер списка:
template <class... Ts>
constexpr std::size_t size(type_pack<Ts...>){
	return sizeof...(Ts);
}

//Функция, проверяющая что размер списка равен нулю:
template <class... Ts>
constexpr bool empty(type_pack<Ts...> tp){
	return size(tp) == 0;
}

//Функция, возвращающая "голову" списка:
template <class T, class... Ts>
constexpr just_type<T> head(type_pack<T, Ts...>){
	return {};
}

//Функция, возвращающая "хвост"списка:
template <class T, class... Ts>
constexpr type_pack<Ts...> tail(type_pack<T, Ts...>){
	return {};
}

//Определяем серию операторов сравнения типов:

template <class T, class U>
constexpr bool operator==(just_type<T>, just_type<U>){
	return false;
}

template <class T>
constexpr bool operator==(just_type<T>, just_type<T>){
	return true;
}


//using ex_type = type_pack<char, int, double>;
//static_assert(head(ex_type{}) == just_type<char>{});

template <class T, class U>
constexpr bool operator!=(just_type<T>, just_type<U>){
	return true;
}

template <class T>
constexpr bool operator!=(just_type<T>, just_type<T>){
	return false;
}

template <class T, class U>
constexpr auto operator+(just_type<T>, just_type<U>){
	return type_pack<T, U>{};
}


//Определяем серию операторов сравнения списков типов:

template <class... Ts, class... Us>
constexpr bool operator==(type_pack<Ts...>, type_pack<Us...>){
	return false;
}

template <class... Ts>
constexpr bool operator==(type_pack<Ts...>, type_pack<Ts...>){
	return true;
}


template <class... Ts, class... Us>
constexpr bool operator!=(type_pack<Ts...>, type_pack<Us...>){
	return true;
}

template <class... Ts>
constexpr bool operator!=(type_pack<Ts...>, type_pack<Ts...>){
	return false;
}

//Оператор конкатенации (склейки двух списков типов):
template <class... Ts, class U>
constexpr auto operator+(type_pack<Ts...>, just_type<U>){
	return type_pack<Ts..., U>{};
}

template <class... Ts, class... Us>
constexpr auto operator+(type_pack<Ts...>, type_pack<Us...>){
	return type_pack<Ts..., Us...>{};
}

//static_assert(type_pack<char, int>{} + type_pack<char*, int*>{} == type_pack<char, int, char*, int*>{});

//Функция push_front:

//type_based approach
template <class T, class... Ts>
constexpr type_pack<T, Ts...> push_front(type_pack<Ts...>){
	return {};
}

static_assert(push_front<char>(type_pack<int, double>{})
						== type_pack<char, int, double>{} );


//value_based approach
template <class... Ts, class T>
constexpr type_pack<T, Ts...> push_front(type_pack<Ts...>, just_type<T>){
	return {};
}

type_pack<int, double> a;
type_pack<char, int, double> c;
just_type<char> b;

static_assert(push_front(a, b) == c );


//Функция pop_front:
//type_based approach
template <class T, class... Ts>
constexpr type_pack<Ts...> pop_front(type_pack<T, Ts...>){
	return {};
}
//static_assert(pop_front(type_pack<int, double, char>{}) == type_pack<double, char>{});



//Функция push_back:
//type_based approach
template <class T, class... Ts>
constexpr type_pack<Ts..., T> push_back(type_pack<Ts...>){
	return {};
}

//value_based approach
template <class T, class... Ts>
constexpr type_pack<Ts..., T> push_back(type_pack<Ts...>, just_type<T>){
	return {};
}
//static_assert(push_back<int>(type_pack<double, char>{}) == type_pack<double, char, int>{});
//static_assert(push_back(type_pack<double, char>{}, just_type<int>{}) == type_pack<double, char, int>{});


//Функция contains. Проверяет, есть ли искомый тип в списке:
//Best version

template <class T, class... Ts>
constexpr bool contains(type_pack<Ts...>, just_type<T>){
	 // c++17 fold expressions.
	return 	(... || std::is_same_v<T, Ts>);
}

////type_based approach
//template <class T, class... Ts>
//constexpr bool contains(type_pack<Ts...>){
//
//	 bool bs[] = {std::is_same_v<T, Ts>...}; //brace-init-list кастится к массиву его значений
//	 bool res = false;
//
//	 for(bool b : bs){
//		 res |= b;
//	 }
//
//	 return res;
//}
//
////value_based approach
//template <class T, class... Ts>
//constexpr bool contains(type_pack<Ts...>, just_type<T>){
//
//	 bool bs[] = {std::is_same_v<T, Ts>...}; //brace-init-list кастится к массиву его значений
//	 bool res = false;
//
//	 for(bool b : bs){
//		 res |= b;
//	 }
//
//	 return res;
//}
//static_assert(contains<int>(type_pack<int, double, char>{}));
static_assert(contains(type_pack<int, double, char>{}, just_type<char>{}));


//Функция find. Если искомый тип находится в данном списке типов,
//то функция возвращает ее индекс. Если нет, то возвращает размер списка.
//Поведение, аналогичное итераторам.

//type_based approach
template <class T, class... Ts>
constexpr std::size_t find(type_pack<Ts...> tp){

	using fsize = std::size_t;
	bool bs[] = {std::is_same_v<T, Ts>...};
	fsize s = size(tp);

	for (fsize i = 0; i != s; ++i){
		if (bs[i]) return i;
	}

	return s;
}

//value_based approach
template <class T, class... Ts>
constexpr std::size_t find(type_pack<Ts...> tp, [[maybe_unused]] just_type<T> t){

	using fsize = std::size_t;
	bool bs[] = {std::is_same_v<T, Ts>...};
	fsize s = size(tp);

	for (fsize i = 0; i != s; ++i){
		if (bs[i]) return i;
	}

	return s;
}

//static_assert(find<int>(type_pack<char, int, double>{}) == 1);
//static_assert(find(type_pack<char, int, double>{}, just_type<int>{}) == 1);


//Функция find_if. Поиск по функции(лямбде) - предикату. Если тип
//в списке типов найден, возвращает его индекс, если нет - размер списка.

//Определяем вспомогательную метафункцию, позволяющую частично применять предикат
template <template <class...> class F, class... Ts>
struct part_caller{
	template <class... Us>
	using type = typename F<Ts..., Us...>::type;
};

//type_based approach
template<template <class...> class F, class... Ts>
constexpr std::size_t find_if(type_pack<Ts...> tp){

	using fsize = std::size_t;
	bool bs[] = {F<Ts>::value...};
	fsize s = size(tp);

	for (fsize i = 0; i != s; ++i){
		if (bs[i]) return i;
	}

	return s;
}

//static_assert(find_if<std::is_pointer>(type_pack<double, int, char>{}) == 3);
//static_assert(find_if<part_caller<std::is_same, double>::type>(type_pack<char, double, int>{}) == 1);

//value_based approach

// Определяем метафункцию-обертку, возвращающую значение
template <template <class...> class F>
struct value_fn{
	template<class... Ts>
	constexpr auto operator()(just_type<Ts>...){
		return F<Ts...>::value;
	}
};

template<template <class...> class F>
constexpr value_fn<F> value_fn_v;


// Определяем метафункцию-обертку, возвращающую тип
template <template <class...> class F>
struct type_fn{
	template <class... Ts>
	constexpr auto operator()(just_type<Ts>...){
		return just_type<typename F<Ts...>::type>{};
	}
};

template<template <class...> class F>
constexpr type_fn<F> type_fn_v;

// Определяем саму value_based функцию find_if:

template <class F, class... Ts>
constexpr std::size_t find_if(F f, type_pack<Ts...> tp){

	using fsize = std::size_t;
	bool bs[] = {f(just_type<Ts>{})...};

	fsize s = size(tp);

	for (fsize i = 0; i != s; ++i){
		if (bs[i]) return i;
	}

	return s;
}

//static_assert(find_if(value_fn_v<std::is_pointer>, type_pack<char*, double, int>{}) == 0);


//Функция all_of:
template <class F, class... Ts>
constexpr bool all_of(F f, type_pack<Ts...>){

	/*bool bs[] = {f(just_type<Ts>{})...}; //brace-init-list кастится к массиву его значений
	bool res = true;

	for(bool b : bs){
		res &= b;
	}

	return res;*/ //realization without c++17 fold expressions

	return (... & f(just_type<Ts>{}));
}

//static_assert(all_of(value_fn_v<std::is_pointer>, type_pack<char*, double*, int*>{}) );


//Функция any_of:
template <class F, class... Ts>
constexpr bool any_of(F f, type_pack<Ts...>){

	/*bool bs[] = {f(just_type<Ts>{})...}; //brace-init-list кастится к массиву его значений
	bool res = false;

	for(bool b : bs){
		res |= b;
	}

	return res;*/ //realization without c++17 fold expressions

	return (... || f(just_type<Ts>{}));
}

//static_assert(any_of(value_fn_v<std::is_pointer>, type_pack<char*, double, int>{}) );


//Функция non_of:
template <class F, class... Ts>
constexpr bool non_of(F f, type_pack<Ts...> tp){

	return !any_of(f, tp);
}

//static_assert(non_of(value_fn_v<std::is_pointer>, type_pack<char, double, int>{}) );


//Функция transform
//type_based approach
template <template <class...> class F, class... Ts>
constexpr auto transform (type_pack<Ts...>){

	return type_pack<typename F<Ts>::type...>{};
}


//value_based approach

template <class F, class... Ts>
constexpr auto transform (F f, type_pack<Ts...>){

	return (... + f(just_type<Ts>{}));
}


//static_assert(transform<std::add_pointer>(type_pack<int, char, double>{})
//											== type_pack<int*, char*, double*>{});


static_assert(transform(type_fn_v<std::add_pointer>, type_pack<int, char, double>{}) ==
		type_pack<int*, char*, double*>{});


//Функция get. Выводит тип из списка типов по индексу. Если индекс за границами списка
//ошибка компиляции (sfinae-friendly)

// 1 этап. Создаем вспомогательную структуру  - "indexed_type":
template <std::size_t I, class T>
struct indexed_type{
	static constexpr std::size_t value = I;
	using type = T;
};

// 2 этап. Создаем метафункцию, создающую наследника всех проиндексированных типов:
template <class Is, class... Ts>
struct indexed_types;

template <std::size_t... Is, class... Ts>
struct indexed_types<std::index_sequence<Is...>, Ts...>{
	struct type : indexed_type<Is, Ts>... {};
};

// 3 этап. Создаем вспомогательный алиас, принимающий типы, и создающий наследника
//проиндексированных типов

template <class... Ts>
using indexed_types_for =
		typename indexed_types<std::index_sequence_for<Ts...>, Ts...>::type;

//4 этап. Создаем вспомогательную метафункцию, достающую из проиндексированного типа просто тип:

template<std::size_t I, class T>
constexpr just_type<T> get_indexed_type(indexed_type<I, T>){
	return {};
}

//Итоговая функция get:
template <std::size_t I, class... Ts>
constexpr auto get(type_pack<Ts...>){
	return get_indexed_type<I>(indexed_types_for<Ts...>{});
}

//static_assert(get<0>(type_pack<int, char, double>{}) == just_type<int>{});


//Функция reverse:
template <std::size_t... is, class TP>
constexpr auto reverse_impl(std::index_sequence<is...>, TP tp){

	return type_pack<typename decltype(get<size(tp) - is - 1>(tp))::type...>{};
}

template <class... Ts>
constexpr auto reverse (type_pack<Ts...> tp){

	return reverse_impl(std::index_sequence_for<Ts...>{}, tp);
}

//static_assert(reverse(type_pack<char, int, double>{}) == type_pack<double, int, char>{});


//Функция generate. Генерирует список типов заданного размера и типа.
template <class... Ts>
constexpr type_pack<Ts...> generate_helper(Ts*...){
	return {};
}

template <class T, std::size_t... Is>
constexpr auto generate_impl(std::index_sequence<Is...>){

	return generate_helper(((void)Is, (T*)0)...);
}

template <std::size_t I, class T>
constexpr auto generate(){
	return generate_impl<T>(std::make_index_sequence<I>{});
}


//static_assert(generate<3, int>() == type_pack<int, int, int>{});

//Функция filter. Принимает список типов и предикат и оставляет в нем только
//типы, удовлетворяющие предикату.
// "Bad"
/*
template <template <class...> class F, class... Us>
constexpr auto filter_impl(empty_pack, type_pack<Us...> res){
	return res;
}

template <template <class...> class F, class T, class... Ts, class... Us>
constexpr auto filter_impl(type_pack<T, Ts...>, type_pack<Us...>){

	if constexpr(F<T>::value){
			return filter_impl<F>(type_pack<Ts...>{}, type_pack<Us..., T>{});
	} else {
		return filter_impl<F>(type_pack<Ts...>{}, type_pack<Us...>{});
	}
}

template <template <class...> class F, class... Ts>
constexpr auto filter (type_pack<Ts...> tp){
	return filter_impl<F>(tp, {});
}
*/
//static_assert(filter<std::is_pointer>(type_pack<char, int*, float, double*>{}) ==
//		type_pack<int*, double*>{});

// "Best"

template<template <class...> class F, class T>
constexpr auto filter_one(){

	if constexpr (F<T>::value){
		return type_pack<T>{};
	} else {
		return empty_pack{};
	}
}

template<template <class...> class F, class... Ts>
constexpr auto filter(type_pack<Ts...>){
	return (empty_pack{} + ... + filter_one<F, Ts>());
}

static_assert(filter<std::is_pointer>(type_pack<char*, int*, double>{}) ==
									  type_pack<char*, int*>{});




using pack =  type_pack<type_pack<char, int>, type_pack<char*, int*>>;
using pack1 = typename decltype(get<0>(pack{}))::type;

static_assert( pack1{} == type_pack<char, int>{});

#endif /* CPP_WRAPPER_INC_TYPELIST_HPP_ */
