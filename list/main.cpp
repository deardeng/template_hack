#include <iostream>
#include <boost/type_index.hpp>

#define CAP(_name_, ...) _name_<__VA_ARGS__>::R
#define TCAP(_name_, ...) typename _name_<__VA_ARGS__>::R

template <typename _Head, typename _Tail> struct Cons {};

template <class _L1, class _L2> struct list_concat;

// list_concat
template <class _L2> struct list_concat<void, _L2> { typedef _L2 R; };

template <typename _H, class _T, class _L2>
struct list_concat<Cons<_H, _T>, _L2>
{ typedef Cons<_H, TCAP(list_concat, _T, _L2)> R; };

template <class _L> struct list_size;


template <typename _H, class _T> struct list_size<Cons<_H, _T> >
{ static const int R = 1 + CAP(list_size, _T); };

template <> struct list_size<void> { static const int R = 0; };

template <class _L> struct list_head;

// list_head
template <typename _H, class _T> struct list_head<Cons<_H, _T> >
{ typedef _H R; };

template <> struct list_head<void> { typedef void R; };

template <class _L> struct list_tail;
// list_tail
template <typename _H, class _T> struct list_tail<Cons<_H, _T> >
{ typedef _T R; };

template <> struct list_tail<void> { typedef void R; };

template <typename _Type, typename _Then, typename _Else> struct if_void;

// if_void
template <typename _Then, typename _Else> struct if_void<void, _Then, _Else>
{ typedef _Then R; };

template <typename _Type, typename _Then, typename _Else> struct if_void
{ typedef _Else R; };


template <typename _Any> struct c_identity { typedef _Any R; };

template <class _L> struct list_last;

template <typename _H, class _T> struct list_last<Cons<_H, _T> >
{ typedef TCAP(if_void, _T, c_identity<_H>, list_last<_T>)::R R; };

template <> struct list_last<void> { typedef void R; };

template <int _POS, class _L> struct list_at;

template <int _POS, typename _H, class _T>
struct list_at <_POS, Cons<_H, _T> >
{ typedef TCAP(list_at, _POS-1, _T) R; };

template <int _POS> struct list_at<_POS, void>
{ typedef void R; };

template <typename _H, class _T>
struct list_at <0, Cons<_H, _T> >
{ typedef _H R; };

template <int _N> struct Int { static const int R = _N; };

template <typename _T1, typename _T2> struct c_same;

template <typename _T> struct c_same <_T, _T>
{ static const bool R = true; };

template <typename _T1, typename _T2> struct c_same
{ static const bool R = false; };

template <bool _Condition, typename _Then, typename _Else> struct if_;

// if_
template <typename _Then, typename _Else> struct if_<false, _Then, _Else>
{ typedef _Else R; };

template <bool _Cond, typename _Then, typename _Else> struct if_
{ typedef _Then R; };

template <typename _Item, class _L> struct list_seek_first;

template <typename _Item, typename _H, class _T>
class list_seek_first<_Item, Cons<_H, _T> > {
  // if _idx is -1, returns -1; otherwise returns 1+_idx
  struct case_false
  {
    enum {
      R_ = CAP(list_seek_first, _Item, _T),
      R = (R_ < 0) ? -1 : (1 + R_)
    };
  };

 public:
  const static int R = CAP(if_, CAP(c_same, _Item, _H), Int<0>, case_false)::R;
};

template <typename _Item> struct list_seek_first<_Item, void>
{ enum { R = -1 }; };


int main(){
    list_concat<void, char>::R c;
    c = 'a';
    std::cout << "trans type char => " <<  c << std::endl;
    CAP(list_concat, Cons<double, void>, char);
    std::cout << "trans type CAP(list_concat, Cons<char, Cons<int, Cons<double, void>>>, float) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_concat, Cons<char, Cons<int, Cons<double, void>>>, float)
        >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_size,  Cons<char, Cons<int, Cons<double, void>>>) => " <<  CAP(list_size,  Cons<char, Cons<int, Cons<double, void>>>) << std::endl;

    std::cout << "trans type CAP(list_head, Cons<char, Cons<int, Cons<double, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_head, Cons<char, Cons<int, Cons<double, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_tail, Cons<char, Cons<int, Cons<double, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_tail, Cons<char, Cons<int, Cons<double, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_last, Cons<char, Cons<int, Cons<double, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_last, Cons<char, Cons<int, Cons<double, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_last, Cons<float, void>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_last,  Cons<float, void>)
    >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_at, 0, Cons<char, Cons<int, Cons<double, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_at, 0, Cons<char, Cons<int, Cons<double, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_at, 1, Cons<char, Cons<int, Cons<double, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_at, 1, Cons<char, Cons<int, Cons<double, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_at, 2, Cons<char, Cons<int, Cons<double, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_at, 2, Cons<char, Cons<int, Cons<double, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_at, 999, Cons<char, Cons<int, Cons<double, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_at, 999, Cons<char, Cons<int, Cons<double, void>>>)
    >().pretty_name() << std::endl;

//    std::cout << list_seek_first<double ,Cons<char, Cons<int, Cons<double, void>>>>::R << std::endl;
    std::cout << "CAP(list_seek_first, double ,Cons<char, Cons<int, Cons<double, void>>>) => " << CAP(list_seek_first, double ,Cons<char, Cons<int, Cons<double, void>>>) << std::endl;
    std::cout << "CAP(list_seek_first, float ,Cons<char, Cons<int, Cons<double, void>>>) => " << CAP(list_seek_first, float ,Cons<char, Cons<int, Cons<double, void>>>) << std::endl;

}