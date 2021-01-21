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

template <typename _Item, class _L> struct list_find_first;

// list_find_first
template <typename _Item, typename _H, class _T>
struct list_find_first<_Item, Cons<_H, _T> > {
  typedef TCAP(if_, CAP(c_same, _Item, _H),
               c_identity<_H>,
               list_find_first<_Item, _T>)::R R;
};

template <typename _Item>
struct list_find_first<_Item, void> { typedef void R; };

template <template <typename> class _F, class _L>
struct list_seek_first_true;

// implement list_seek_first_true
template <template <typename> class _F, typename _H, class _T>
class list_seek_first_true<_F, Cons<_H, _T> > {
  // if _idx is -1, returns -1; otherwise returns 1+_idx
  struct case_false
  { enum { R_ = CAP(list_seek_first_true, _F, _T),
      R = (R_ < 0) ? -1 : (1 + R_) }; };

 public:
  const static int R = CAP(if_, CAP(_F, _H), Int<0>, case_false)::R;
};

template <template <typename> class _F> struct list_seek_first_true<_F, void>
{ enum { R = -1 }; };

template <class _T, class _H, class _C,
    int _M, bool _R>
class UniqueCheckIndex{};

template <typename _Index> struct is_true {
  static const bool R = false;
};

template <class _T, class _H, class _C,
    int _M, bool _R>
struct is_true<
    UniqueCheckIndex<_T, _H, _C,
                     _M, _R> > {
static const bool R = true;
};

template <template <typename> class _F, class _L>
struct list_find_first_true;

// list_find_first_true
template <template <typename> class _F, typename _H, class _T>
struct list_find_first_true<_F, Cons<_H, _T> > {
  typedef TCAP(if_, CAP(_F, _H),
               c_identity<_H>,
               list_find_first_true<_F, _T>)::R R;
};

template <template <typename> class _F>
struct list_find_first_true<_F, void> { typedef void R; };

template <class _L> struct list_dup;

// list_dup
template <typename _H, class _T>
struct list_dup<Cons<_H, _T> >
{ enum { R = CAP(if_, (CAP(list_seek_first, _H, _T) >= 0),
                 Int<true>,
                 list_dup<_T>)::R }; };

template <> struct list_dup<void> { enum { R = false }; };

template <typename _Item, class _L> struct list_erase_first;

// Implement list_erase_first
template <typename _Item, typename _H, class _T>
class list_erase_first<_Item, Cons<_H, _T> > {
  struct go_on {
    typedef Cons<_H, TCAP(list_erase_first, _Item, _T)> R;
  };

 public:
  typedef TCAP(if_, CAP(c_same, _Item, _H), c_identity<_T>, go_on)::R R;
};

template <typename _Item> struct list_erase_first<_Item, void>
{ typedef void R; };

template <typename _C1, typename _C2> struct Pair {
  typedef _C1 First;
  typedef _C2 Second;
};

template <template <typename> class _F, class _L>
struct list_erase_first_true;

// list_erase_first_true
template <template <typename> class _F, typename _H, class _T>
class list_erase_first_true<_F, Cons<_H, _T> > {
  struct case_false {
    typedef TCAP(list_erase_first_true, _F, _T) P;
    typedef Pair<typename P::First, Cons<_H, typename P::Second> > R;
  };

 public:
  typedef TCAP(if_, CAP(_F, _H),
               c_identity<Pair<_H, _T> >,
               case_false)::R R;
};

template <template <typename> class _F>
struct list_erase_first_true<_F, void>
{ typedef Pair<void, void> R; };

template <template <typename> class _F, class _L> struct list_filter;

// list_filter
template <template <typename> class _F, typename _H, class _T>
class list_filter<_F, Cons<_H, _T> > {
  typedef TCAP(list_filter, _F, _T) T2;
 public:
  typedef TCAP(if_, CAP(_F, _H), Cons<_H, T2>, T2) R;
};

template <template <typename> class _F>
struct list_filter<_F, void> { typedef void R; };

template <template <typename> class _F, class _L> struct list_unfilter;

// list_unfilter
template <template <typename> class _F, typename _H, class _T>
class list_unfilter<_F, Cons<_H, _T> > {
  typedef TCAP(list_unfilter, _F, _T) T2;
 public:
  typedef TCAP(if_, CAP(_F, _H), T2, Cons<_H, T2>) R;
};

template <template <typename> class _F>
struct list_unfilter<_F, void> { typedef void R; };

template <typename _Any> struct c_void_void { typedef void R; };

template <template <typename> class _F, class _L> struct list_map;

// list_map
template <template <typename> class _F, typename _H, class _T>
struct list_map<_F, Cons<_H, _T> >
{ typedef Cons<TCAP(_F, _H), TCAP(list_map, _F, _T)> R; };

template <template <typename> class _F>
struct list_map<_F, void> { typedef void R; };

template <typename _Any> struct c_list { typedef Cons<_Any, void> R; };

template <template <typename> class _F, class _L> struct list_map_cat;

// list_map_cat
template <template <typename> class _F, typename _H, class _T>
struct list_map_cat<_F, Cons<_H, _T> >
{ typedef TCAP(list_concat, TCAP(_F, _H), TCAP(list_map_cat, _F, _T)) R; };

template <template <typename> class _F>
struct list_map_cat<_F, void> { typedef void R; };

template <typename _Any> struct c_void;
// c_void
template <> struct c_void<void> { enum { R = true }; };
template <typename _Any> struct c_void { enum { R = false }; };

template <template <typename> class _F, class _L> struct list_filter_map;

// list_filter_map
template <template <typename> class _F, typename _H, class _T>
class list_filter_map<_F, Cons<_H, _T> > {
  typedef TCAP(list_filter_map, _F, _T) T2;
  typedef TCAP(_F, _H) H2;
 public:
  typedef TCAP(if_, !CAP(c_void, H2), Cons<H2, T2>, T2) R;
};

template <template <typename> class _F>
struct list_filter_map<_F, void> { typedef void R; };

template <template <typename> class _F, class _L> struct list_partition;

// list_partition
template <template <typename> class _F, typename _H, class _T>
class list_partition<_F, Cons<_H, _T> > {
  typedef TCAP(list_partition, _F, _T) P;
 public:
  typedef
  TCAP(if_, CAP(_F, _H)
  , Pair<Cons<_H, typename P::First>, typename P::Second>
  , Pair<typename P::First, Cons<_H, typename P::Second> >) R;

};

template <template <typename> class _F>
struct list_partition<_F, void> { typedef Pair<void, void> R; };

template <typename _Any> struct c_not_void;

// c_not_void
template <> struct c_not_void<void> { enum { R = false }; };
template <typename _Any> struct c_not_void { enum { R = true }; };

template <typename _A0,        typename _A1  = void, typename _A2  = void,
    typename _A3  = void, typename _A4  = void, typename _A5  = void,
    typename _A6  = void, typename _A7  = void, typename _A8  = void,
    typename _A9  = void, typename _A10 = void, typename _A11 = void,
    typename _A12 = void, typename _A13 = void, typename _A14 = void,
    typename _A15 = void, typename _A16 = void, typename _A17 = void>
struct make_non_void_list;

// Implement make_non_void_list
template <typename _A0,  typename _A1,  typename _A2,
    typename _A3,  typename _A4,  typename _A5,
    typename _A6,  typename _A7,  typename _A8,
    typename _A9,  typename _A10, typename _A11,
    typename _A12, typename _A13, typename _A14,
    typename _A15, typename _A16, typename _A17>
struct make_non_void_list {
  typedef TCAP(list_filter, c_not_void,
               Cons<_A0, Cons<_A1, Cons<_A2, Cons<_A3, Cons<_A4,
                                                            Cons<_A5, Cons<_A6, Cons<_A7, Cons<_A8, Cons<_A9,
                                                                                                         Cons<_A10, Cons<_A11, Cons<_A12, Cons<_A13, Cons<_A14,
                                                                                                                                                          Cons<_A15, Cons<_A16, Cons<_A17,
                   void> > > > > > > > > > > > > > > > > >) R;
};

template <template <typename, typename> class _F, typename _Init, class _L>
struct list_foldl;

// list_foldl
template <template <typename, typename> class _F,
    typename _Init, typename _H, class _T>
struct list_foldl<_F, _Init, Cons<_H, _T> >
{ typedef TCAP(list_foldl, _F, TCAP(_F, _Init, _H), _T) R; };

template <template <typename, typename> class _F, typename _Init>
struct list_foldl<_F, _Init, void> { typedef _Init R; };


template <typename _H, typename _L> struct gen_foo{
  typedef Cons<_H, _L> R;
};


int main(){
    list_concat<void, char>::R c;
    c = 'a';
    std::cout << "trans type char => " <<  c << std::endl;
    CAP(list_concat, Cons<double, void>, char);
    std::cout << "trans type CAP(list_concat, Cons<char, Cons<int, Cons<double, void>>>, Cons<float, void>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_concat, Cons<char, Cons<int, Cons<double, void>>>, Cons<float, void>)
        >().pretty_name() << std::endl;

    std::cout << "trans type  CAP(list_concat, void, void) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_concat, void, void)
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

    std::cout << "trans type CAP(list_find_first, int, Cons<char, Cons<int, Cons<double, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_find_first, int, Cons<char, Cons<int, Cons<double, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_find_first, float, Cons<char, Cons<int, Cons<double, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_find_first, float, Cons<char, Cons<int, Cons<double, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "CAP(list_seek_first_true, is_true, Cons<char, Cons<int, Cons<double, void>>>) => "
              << CAP(list_seek_first_true, is_true, Cons<char, Cons<int, Cons<double, void>>>) << std::endl;

    std::cout << "CAP(list_seek_first_true, is_true, Cons<UniqueCheckIndex<char, double, float, 3, false>, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<double, void>>>) => "
              << CAP(list_seek_first_true, is_true, Cons<UniqueCheckIndex<char, double, float, 3, false>, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<double, void>>>) << std::endl;

    std::cout << "CAP(list_seek_first_true, is_true, Cons<char, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<double, void>>>) => "
              << CAP(list_seek_first_true, is_true, Cons<char, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<double, void>>>) << std::endl;

    std::cout << "trans type CAP(list_find_first_true, is_true, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<int, Cons<double, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_find_first_true, is_true, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<int, Cons<double, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_find_first_true, is_true, Cons<int, Cons<UniqueCheckIndex<char, double, float, 3, false>, Cons<double, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_find_first_true, is_true, Cons<int, Cons<UniqueCheckIndex<char, double, float, 3, false>, Cons<double, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "CAP(list_dup, Cons<char, Cons<int, Cons<double, void>>>) => " << CAP(list_dup, Cons<char, Cons<int, Cons<double, void>>>) << std::endl;
    std::cout << "CAP(list_dup, Cons<double, Cons<int, Cons<double, void>>>) => " << CAP(list_dup, Cons<double, Cons<int, Cons<double, void>>>) << std::endl;

    std::cout << "trans type CAP(list_erase_first, int, Cons<int, Cons<UniqueCheckIndex<char, double, float, 3, false>, Cons<double, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_erase_first, int, Cons<int, Cons<UniqueCheckIndex<char, double, float, 3, false>, Cons<double, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_erase_first, float, Cons<int, Cons<UniqueCheckIndex<char, double, float, 3, false>, Cons<double, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_erase_first, float, Cons<int, Cons<UniqueCheckIndex<char, double, float, 3, false>, Cons<double, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_erase_first_true, is_true, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<int, Cons<double, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_erase_first_true, is_true, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<int, Cons<double, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_erase_first_true, is_true, Cons<float, Cons<int, Cons<double, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_erase_first_true, is_true, Cons<float, Cons<int, Cons<double, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_erase_first_true, is_true, Cons<int, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<double, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_erase_first_true, is_true, Cons<int, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<double, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_filter, is_true, Cons<int, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<double, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_filter, is_true, Cons<int, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<double, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_filter, is_true, Cons<int, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<UniqueCheckIndex<char, double, float, 3, false>, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_filter, is_true, Cons<int, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<UniqueCheckIndex<char, double, float, 3, false>, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_unfilter, is_true, Cons<int, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<double, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_unfilter, is_true, Cons<int, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<double, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_unfilter, is_true, Cons<int, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<UniqueCheckIndex<char, double, float, 3, false>, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_unfilter, is_true, Cons<int, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<UniqueCheckIndex<char, double, float, 3, false>, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_map, c_identity, Cons<int, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<UniqueCheckIndex<char, double, float, 3, false>, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_map, c_identity, Cons<int, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<UniqueCheckIndex<char, double, float, 3, false>, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_map, c_void, Cons<int, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<UniqueCheckIndex<char, double, float, 3, false>, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_map, c_void_void, Cons<int, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<UniqueCheckIndex<char, double, float, 3, false>, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_map_cat, c_list, Cons<int, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<UniqueCheckIndex<char, double, float, 3, false>, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_map_cat, c_list, Cons<int, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<UniqueCheckIndex<char, double, float, 3, false>, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_map_cat, c_void, Cons<int, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<UniqueCheckIndex<char, double, float, 3, false>, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_map_cat, c_void_void, Cons<int, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<UniqueCheckIndex<char, double, float, 3, false>, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_filter_map, c_identity, Cons<int, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<UniqueCheckIndex<char, double, float, 3, false>, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_filter_map, c_identity, Cons<int, Cons<UniqueCheckIndex<char, double, float, 5, true>, Cons<UniqueCheckIndex<char, double, float, 3, false>, void>>>)
    >().pretty_name() << std::endl;


    std::cout << "trans type CAP(list_filter_map, c_identity, Cons<void, Cons<void, Cons<UniqueCheckIndex<char, double, float, 3, false>, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_filter_map, c_identity, Cons<void, Cons<void, Cons<UniqueCheckIndex<char, double, float, 3, false>, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_partition, is_true, Cons<int, Cons<char, Cons<UniqueCheckIndex<char, double, float, 3, false>, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_partition, is_true, Cons<int, Cons<char, Cons<UniqueCheckIndex<char, double, float, 3, false>, void>>>)
    >().pretty_name() << std::endl;

    std::cout << "CAP(make_non_void_list, int, char) => " << boost::typeindex::type_id_with_cvr<
        CAP(make_non_void_list, int, char)
    >().pretty_name() << std::endl;

    std::cout << "CAP(make_non_void_list, int, char, void, float) => " << boost::typeindex::type_id_with_cvr<
        CAP(make_non_void_list, int, char, void, float)
    >().pretty_name() << std::endl;

    std::cout << "trans type CAP(list_foldl, gen_foo, int, Cons<float, Cons<char, Cons<double, void>>>) => " << boost::typeindex::type_id_with_cvr<
        CAP(list_foldl, gen_foo, int, Cons<float, Cons<char, Cons<double, void>>>)
    >().pretty_name() << std::endl;
}