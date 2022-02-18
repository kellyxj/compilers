#include <sched.h>
#include <string>
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

#include <L1.h>
#include <parser.h>

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

using namespace pegtl;
using namespace std;

namespace L1 {

    /*
     * Data required to parse
     */
    std::vector<Item*> parsed_items;

    /*
     * Grammar rules from now on.
     */
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

    struct str_rdi : TAOCPP_PEGTL_STRING("rdi") {};
    struct str_rsi : TAOCPP_PEGTL_STRING("rsi") {};
    struct str_rdx : TAOCPP_PEGTL_STRING("rdx") {};
    struct str_rcx : TAOCPP_PEGTL_STRING("rcx") {};
    struct str_r8 : TAOCPP_PEGTL_STRING("r8") {};
    struct str_r9 : TAOCPP_PEGTL_STRING("r9") {};
    struct str_rax : TAOCPP_PEGTL_STRING("rax") {};
    struct str_rbx : TAOCPP_PEGTL_STRING("rbx") {};
    struct str_rbp : TAOCPP_PEGTL_STRING("rbp") {};
    struct str_r10 : TAOCPP_PEGTL_STRING("r10") {};
    struct str_r11 : TAOCPP_PEGTL_STRING("r11") {};
    struct str_r12 : TAOCPP_PEGTL_STRING("r12") {};
    struct str_r13 : TAOCPP_PEGTL_STRING("r13") {};
    struct str_r14 : TAOCPP_PEGTL_STRING("r14") {};
    struct str_r15 : TAOCPP_PEGTL_STRING("r15") {};

    struct str_rsp : TAOCPP_PEGTL_STRING("rsp") {};

    struct str_mem : TAOCPP_PEGTL_STRING("mem") {};

    struct str_pluseq : TAOCPP_PEGTL_STRING("+=") {};
    struct str_minuseq : TAOCPP_PEGTL_STRING("-=") {};
    struct str_timeseq : TAOCPP_PEGTL_STRING("*=") {};
    struct str_andeq : TAOCPP_PEGTL_STRING("&=") {};

    struct str_lshift : TAOCPP_PEGTL_STRING("<<=") {};
    struct str_rshift : TAOCPP_PEGTL_STRING(">>=") {};

    struct str_less : TAOCPP_PEGTL_STRING("<") {};
    struct str_leq : TAOCPP_PEGTL_STRING("<=") {};
    struct str_eq : TAOCPP_PEGTL_STRING("=") {};

    struct str_cjump : TAOCPP_PEGTL_STRING("cjump") {};
    struct str_goto : TAOCPP_PEGTL_STRING("goto") {};

    struct str_call : TAOCPP_PEGTL_STRING("call") {};

    struct str_print : TAOCPP_PEGTL_STRING("print") {};
    struct str_input : TAOCPP_PEGTL_STRING("input") {};
    struct str_allocate : TAOCPP_PEGTL_STRING("allocate") {};
    struct str_tensor_error : TAOCPP_PEGTL_STRING("tensor-error") {};

    struct str_increment : TAOCPP_PEGTL_STRING("++") {};
    struct str_decrement : TAOCPP_PEGTL_STRING("--") {};

    struct str_lea : TAOCPP_PEGTL_STRING("@") {};

    struct label :
        pegtl::seq<
        pegtl::one<':'>,
        name
        > {};

    struct register_rdi_rule :
        str_rdi {};

    struct register_rsi_rule :
        str_rsi {};

    struct register_rdx_rule :
        str_rdx {};

    struct register_rcx_rule :
        str_rcx {};

    struct register_r8_rule :
        str_r8 {};

    struct register_r9_rule :
        str_r9 {};

    struct register_rax_rule :
        str_rax {};

    struct register_rbx_rule :
        str_rbx {};

    struct register_rbp_rule :
        str_rbp {};

    struct register_r10_rule :
        str_r10 {};

    struct register_r11_rule :
        str_r11 {};

    struct register_r12_rule :
        str_r12 {};

    struct register_r13_rule :
        str_r13 {};

    struct register_r14_rule :
        str_r14 {};

    struct register_r15_rule :
        str_r15 {};

    struct register_rsp_rule :
        str_rsp {};


    struct register_rule :
        pegtl::sor<
        register_rdi_rule,
        register_rsi_rule,
        register_rdx_rule,
        register_rcx_rule,
        register_r8_rule,
        register_r9_rule,
        register_rax_rule,
        register_rbx_rule,
        register_rbp_rule,
        register_r10_rule,
        register_r11_rule,
        register_r12_rule,
        register_r13_rule,
        register_r14_rule,
        register_r15_rule
        > {};

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
        label {};

    struct argument_number :
        number {};

    struct local_number :
        number {};

    struct number_rule :
        number {};

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

    struct x_rule :
        pegtl::sor<
        register_rule,
        register_rsp_rule
        > {};

    struct s_rule :
        pegtl::sor<
        Label_rule,
        number_rule,
        x_rule
        > {};

    struct t_rule :
        pegtl::sor<
        number_rule,
        x_rule
        > {};

    struct pluseq_rule :
        str_pluseq {};

    struct minuseq_rule :
        str_minuseq {};

    struct timeseq_rule :
        str_timeseq {};

    struct andeq_rule :
        str_andeq {};

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