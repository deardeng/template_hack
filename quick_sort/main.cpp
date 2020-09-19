//
// Created by dengxin05 on 2020/8/7.
//

#include <iostream>
#include <algorithm>
#include <iterator>
#include <type_traits>

template <int... valsT>
struct iseq{};

template <int... valsT>
void print(iseq<valsT...>){
  static constexpr int vals[] = {valsT...};
  ::std::copy(::std::begin(vals), ::std::end(vals),
      ::std::ostream_iterator<int>(::std::cout, " "));
  ::std::endl(::std::cout);
}

template <typename L, typename R, int pT, typename S>
struct partition;

template <typename S>
struct quick_sort;

template <typename L, int pT, typename R>
struct join;

template <int... lT, int pT, int... rT>
struct join<iseq<lT...>, pT, iseq<rT...>> : ::std::enable_if<true, iseq<lT...,pT, rT...>>{
};

template <> struct quick_sort<iseq<>> : ::std::enable_if<true, iseq<>>{};

template <int xT> struct quick_sort<iseq<xT>> : ::std::enable_if<true, iseq<xT>>{};

template <int xT, int yT> struct quick_sort<iseq<xT, yT>>
: ::std::conditional<(xT < yT), iseq<xT, yT>,iseq<yT, xT>>
{};

template <int... lT, int... rT, int pT> struct partition<iseq<lT...>, iseq<rT...>, pT, iseq<>>
    : ::std::pair<iseq<lT...>, iseq<rT...>>{};

template <int... lT, int... rT, int pT, int xT, int... sT> struct partition<iseq<lT...>, iseq<rT...>, pT, iseq<xT, sT...>>
    : ::std::conditional<(xT < pT),
    partition<iseq<lT...,xT>, iseq<rT...>, pT, iseq<sT...>>,
    partition<iseq<lT...>, iseq<rT..., xT>, pT, iseq<sT...>>
    >::type
{};


template <int xT, int yT, int pT, int ... sT>
struct quick_sort<iseq<xT, yT, pT, sT...>> :
    join<
    typename quick_sort<typename partition<iseq<>, iseq<>, pT, iseq<xT, yT, sT...>>::first_type>::type,
    pT,
    typename quick_sort<typename partition<iseq<>, iseq<>, pT, iseq<xT, yT, sT...>>::second_type>::type
    >
{};



int main(){
  print(quick_sort<iseq<4,9,3,2,7,5,1,8,6>>::type());
}