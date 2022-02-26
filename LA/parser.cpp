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

    struct str_plus : TAOCPP_PEGTL_STRING("+") {};
    struct str_minus : TAOCPP_PEGTL_STRING("-") {};
    struct str_times : TAOCPP_PEGTL_STRING("*") {};
    struct str_and : TAOCPP_PEGTL_STRING("&") {};

    struct str_lshift : TAOCPP_PEGTL_STRING("<<") {};
    struct str_rshift : TAOCPP_PEGTL_STRING(">>") {};

    struct str_less : TAOCPP_PEGTL_STRING("<") {};
    struct str_leq : TAOCPP_PEGTL_STRING("<=") {};
    struct str_eq : TAOCPP_PEGTL_STRING("=") {};
    struct str_greater : TAOCPP_PEGTL_STRING(">") {};
    struct str_geq : TAOCPP_PEGTL_STRING(">=") {};

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

    struct label :
        pegtl::seq<
            pegtl::one<':'>,
            name
        > {};

    struct variable_rule :
        name{};

    struct number :
        pegtl::seq<
            pegtl::opt<
                pegtl::sor<
                    pegtl::one< '-' >,
                    pegtl::one< '+' >
                >
            >,
            pegtl::plus<
                pegtl::digit
            >
        > {};

    struct function_name :
        name {};

    struct comment :
        pegtl::disable<
        TAOCPP_PEGTL_STRING("//"),
        pegtl::until< pegtl::eolf >
        > {};

    struct seps :
        pegtl::star<
        pegtl::sor<
        pegtl::ascii::space,
        comment
        >
        > {};

    struct Label_rule :
        label {};

    struct plus_rule :
        str_plus {};

    struct minus_rule :
        str_minus {};

    struct times_rule :
        str_times {};

    struct and_rule :
        str_and {};

    struct lshift_rule :
        str_lshift {};

    struct rshift_rule :
        str_rshift {};

    struct cmp_less_rule :
        str_less {};

    struct cmp_leq_rule :
        str_leq {};

    struct cmp_eq_rule :
        str_eq {};

    struct cmp_greater_rule :
        str_greater {};

    struct cmp_geq_rule :
        str_geq {};

    struct op_rule :
        pegtl::sor<
        plus_rule,
        minus_rule,
        times_rule,
        and_rule,
        lshift_rule,
        rshift_rule
        > {};

    struct cmp_rule :
        pegtl::sor<
        cmp_leq_rule,
        cmp_less_rule,
        cmp_eq_rule,
        cmp_greater_rule,
        cmp_geq_rule
        > {};

    struct Instruction_assignment_rule:
    struct Instruction_compare_rule:
    struct Instruction_label_rule:
    struct Instruction_goto_rule:
    struct Instruction_cjump_rule:
    struct Instruction_return_rule:
    struct Instruction_assignToTensor_rule:
    struct Instruction_assignFromTensor_rule:
    struct Instruction_length_rule:
    struct Instruction_call_rule:
    struct Instruction_assignFromCall_rule:
    struct Instruction_newArray_rule:
    struct Instruction_newTuple_rule:
    struct Instruction_input_rule:
    struct Instruction_print_rule:

    struct Instruction_rule :
        pegtl::sor<
            pegt::seq<pegtl::at<Instruction_assignment_rule>, Instruction_assignment_rule>,
            pegt::seq<pegtl::at<Instruction_compare_rule>, Instruction_compare_rule>,
            pegt::seq<pegtl::at<Instruction_label_rule>, Instruction_label_rule>,
            pegt::seq<pegtl::at<Instruction_goto_rule>, Instruction_goto_rule>,
            pegt::seq<pegtl::at<Instruction_cjump_rule>, Instruction_cjump_rule>,
            pegt::seq<pegtl::at<Instruction_return_rule>, Instruction_return_rule>,
            pegt::seq<pegtl::at<Instruction_assignToTensor_rule>, Instruction_assignToTensor_rule>,
            pegt::seq<pegtl::at<Instruction_assignFromTensor_rule>, Instruction_assignFromTensor_rule>,
            pegt::seq<pegtl::at<Instruction_length_rule>, Instruction_length_rule>,
            pegt::seq<pegtl::at<Instruction_call_rule>, Instruction_call_rule>,
            pegt::seq<pegtl::at<Instruction_assignFromCall_rule>, Instruction_assignFromCall_rule>,
            pegt::seq<pegtl::at<Instruction_newArray_rule>, Instruction_newArray_rule>,
            pegt::seq<pegtl::at<Instruction_newTuple_rule>, Instruction_newTuple_rule>,
            pegt::seq<pegtl::at<Instruction_input_rule>, Instruction_input_rule>,
            pegt::seq<pegtl::at<Instruction_print_rule>, Instruction_print_rule>,
        > {};

    struct Function_rule:

    struct Functions_rule:

    struct entry_point_rule:

   struct grammar :
        pegtl::must<
            entry_point_rule
        > {};

    template<typename Rule>
    struct action : pegtl::nothing<Rule> {};
}