#include <vector>
#include <utility>
#include <algorithm>
#include <set>
#include <iterator>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <stdint.h>
#include <assert.h>

#include <tao/pegtl.hpp>
#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/contrib/raw_string.hpp>

#include <LA.h>
#include <parser.h>

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

using namespace pegtl;
using namespace std;

namespace LA {
	std::vector<Item*> parsed_items;

    struct name :
        pegtl::seq<
            pegtl::plus<
                pegtl::sor<
                    pegtl::alpha,
                    pegtl::one< '_' >
                    >
                >,
            pegtl::star<
                pegtl::sor<
                    pegtl::alpha,
                    pegtl::one< '_' >,
                pegtl::digit
                >
            >
        > {};

    /*
     * Keywords.
     */
    struct str_return : TAOCPP_PEGTL_STRING("return") {};
    struct str_arrow : TAOCPP_PEGTL_STRING("<-") {};

    struct str_pluseq : TAOCPP_PEGTL_STRING("+") {};
    struct str_minuseq : TAOCPP_PEGTL_STRING("-") {};
    struct str_timeseq : TAOCPP_PEGTL_STRING("*") {};
    struct str_andeq : TAOCPP_PEGTL_STRING("&") {};

    struct str_lshift : TAOCPP_PEGTL_STRING("<<") {};
    struct str_rshift : TAOCPP_PEGTL_STRING(">>") {};

    struct str_less : TAOCPP_PEGTL_STRING("<") {};
    struct str_leq : TAOCPP_PEGTL_STRING("<=") {};
    struct str_eq : TAOCPP_PEGTL_STRING("=") {};
    ruct str_less : TAOCPP_PEGTL_STRING(">") {};
    struct str_leq : TAOCPP_PEGTL_STRING(">=") {};

    struct str_cjump : TAOCPP_PEGTL_STRING("br") {};

    struct str_print : TAOCPP_PEGTL_STRING("print") {};
    struct str_input : TAOCPP_PEGTL_STRING("input") {};

    struct str_length : TAOCPP_PEGTL_STRING("length") {};
    struct str_new : TAOCPP_PEGTL_STRING("new") {};
    struct str_new_array : TAOCPP_PEGTL_STRING("Array") {};
    struct str_new_tuple : TAOCPP_PEGTL_STRING("Tuple") {};

    struct str_int64 : TAOCPP_PEGTL_STRING("int64") {};
    struct str_tuple : TAOCPP_PEGTL_STRING("tuple") {};
    struct str_code : TAOCPP_PEGTL_STRING("code") {};
    struct str_void : TAOCPP_PEGTL_STRING("void") {};
}