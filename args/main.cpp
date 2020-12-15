#include <iostream>
#include <string>
#include <tuple>

// cpp 元组与模板的一些技巧
template <int ...Ints>
void test(std::integer_sequence<int, Ints...>){
    ((std::cout << Ints << ' '),...) << std::endl;
}

template <typename ...Args>
class Test{};

// 主模板
template <typename, typename >
class Demo;

// 偏特化
template <typename ...Args1, typename  ...Args2>
class Demo<Test<Args1...>, Test<Args2...>>{
 public:
  using Type1 = std::tuple<Args1...>;
  using Type2 = std::tuple<Args2...>;
};


template <int ...Ints>
class Integers{};

template <typename ,int>
class Traits;

template <int ...Ints, int N>
class Traits<Integers<Ints...>,N>{
 public:
  using type = Integers<Ints..., N>;
};

// 主模板
template <bool, int>
class BuildSeq{
 public:
  using type = Integers<0>;
};

template <int N>
class BuildSeq<false, N> : public BuildSeq<N - 1 == 0, N - 1>{
 public:
  using Parent = BuildSeq<N - 1 == 0, N-1>;
  using type = typename Traits<typename Parent::type, N>::type;
};

template <int Num>
class MakeSeq{
 public:
  using type = typename BuildSeq<Num - 1 == 0, Num -1>::type;
};

template <int ...Ints>
void test(Integers<Ints...>){
    ((std::cout << Ints << ' '), ...) << std::endl;
}

void hello(int num1, const char* str, int val2){
    std::cout << num1 << str << val2 << std::endl;
}

template <int ...Ints, typename ...Args>
void test(Integers<Ints...>, std::tuple<Args...>& tp){
//    ((std::cout << std::get<Ints>(tp) << ' '), ...) << std::endl;
    hello(std::get<Ints>(tp)...);
}



int main(int, char**){
    std::integer_sequence<int, 1, 2, 3, 4 > numbers;
    test(numbers);

    typename Demo<Test<int, float>, Test<const char*, int>>::Type1 tp1;
    typename Demo<Test<int, float>, Test<const char*, int>>::Type2 tp2;

    std::make_integer_sequence<int, 10> values;
    test(values);

    typename MakeSeq<10>::type v;
    test(v);

    auto tp = std::make_tuple(6666, "hello", 8888);

    MakeSeq<std::tuple_size<decltype(tp)>::value>::type n;
    test(n, tp);

    std::apply(hello, std::make_tuple(111,"222",333));
}