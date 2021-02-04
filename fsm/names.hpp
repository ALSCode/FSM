/*
 * test.hpp
 *
 *  Created on: 22 янв. 2021 г.
 *      Author: sazhi
 */

#ifndef NAMES_HPP_
#define NAMES_HPP_

//#include <optional>

#define STATE(str)	str = State<name(#str)>
#define STATE_A(str, act)	str = State<name(#str), act>
#define EVENT(str)	str = Event<name(#str)>

//class ConstStr
//{
//    const char* p;
//    unsigned int cs = 0;
//    std::size_t sz = 0;
//
//public:
//    constexpr ConstStr(const char* a) : p(a)
//    {
//
//    	for (; p[sz] != '\0'; sz++) {
//
//    		char data = p[sz];
//
//    		for (std::uint_fast32_t j = sizeof (char) * 8; j > 0; j--) {
//
//    			cs = ((cs ^ data) & 1) ? (cs >> 1) ^ 0x8C : (cs >> 1);
//    			data >>= 1;
//    		}
//    	}
//    }

//    constexpr std::optional<char> operator[](std::size_t n) const
//    {
//        return n < sz ? std::optional{p[n]} : std::nullopt;
//    }

//    constexpr unsigned int operator() (void){ return cs;}
//
//    	  unsigned int  res = 0;
//
//    	  for (std::uint_fast32_t i = 0; p[i] != '\0'; i++) {
//
//    		char data = p[i];
//
//    	    for (std::uint_fast32_t j = sizeof (char) * 8; j > 0; j--) {
//
//    	      res = ((res ^ data) & 1) ? (res >> 1) ^ 0x8C : (res >> 1);
//    	      data >>= 1;
//    	    }
//    	  }
//    	  return res;
//    };
//
//    constexpr std::size_t size() const { return sz; }
//};

constexpr unsigned int name (const char* n){

	  unsigned int  res = 0;

	  for (std::uint_fast32_t i = 0; n[i] != '\0'; i++) {

		char data = n[i];

	    for (std::uint_fast32_t j = sizeof (char) * 8; j > 0; j--) {

	      res = ((res ^ data) & 1) ? (res >> 1) ^ 0x8C : (res >> 1);
	      data >>= 1;
	    }
	  }
	  return res;
};

//template<unsigned int T>
//struct Test{
//	static constexpr unsigned int idx = T;
//};
//
//Test<ConstStr("hello")()> test;
//
//Test<ConstStr("hello")()> test1;
//
//static_assert(test.idx == test1.idx);

//static_assert(t::name == "hello");

#endif /* NAMES_HPP_ */
