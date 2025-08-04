#pragma once

// common args
#include <string>
using std::string;

#define DECLARE_bool(name) extern bool FLAGS_##name;
#define DECLARE_int32(name) extern int FLAGS_##name;
#define DECLARE_string(name) extern string FLAGS_##name;
#define DECLARE_double(name) extern double FLAGS_##name;

#define DEFINE_VARIABLE(type, name, value, help)                               \
    static const type FLAGS_nono##name = value;                                \
    type FLAGS_##name = FLAGS_nono##name;                                      \
    static type FLAGS_no##name = FLAGS_nono##name;

#define DEFINE_bool(name, val, txt) DEFINE_VARIABLE(bool, name, val, txt)
#define DEFINE_int32(name, val, txt) DEFINE_VARIABLE(int, name, val, txt)
#define DEFINE_string(name, val, txt) DEFINE_VARIABLE(string, name, val, txt)
#define DEFINE_double(name, val, txt) DEFINE_VARIABLE(double, name, val, txt)
