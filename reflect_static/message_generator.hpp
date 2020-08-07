//
// Created by dengxin05 on 2020/8/3.
//

#ifndef TEMPLATE_HACK_REFLECT_STATIC_MESSAGE_GENERATOR_H_
#define TEMPLATE_HACK_REFLECT_STATIC_MESSAGE_GENERATOR_H_

#ifndef message_name
# error Please define `message_name`
#endif

#ifndef message_body
# error Please define `message_body`
#endif

struct message_name{
#undef field_int64
#define field_int64(name, ...) ::std::int64_t name = { __VA_ARGS__ };

#undef field_double
#define field_double(name, ...) double name = { __VA_ARGS__ };


#undef field_string
#define field_string(name, ...) ::std::string name = { __VA_ARGS__ };

  message_body
};

inline ::std::ostream& operator<<(::std::ostream& os, const foo& f){
#undef field_int64
#define field_int64(name, ...) os << #name " = " << f.name << ::std::endl;

#undef field_double
#define field_double(name, ...) os << #name " = " << f.name << ::std::endl;

 #undef field_string
#define field_string(name, ...) os << #name " = " << f.name << ::std::endl;

  message_body
  return os;
}

#endif //TEMPLATE_HACK_REFLECT_STATIC_MESSAGE_GENERATOR_H_

#undef message_name
#undef message_body