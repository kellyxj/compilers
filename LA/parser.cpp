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

    struct str_br : TAOCPP_PEGTL_STRING("br") {};

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

    struct str_main : TAOCPP_PEGTL_STRING("main") {};

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

    struct t_rule :
        pegtl::sor<
            variable_rule,
            number_rule
        > {};

    struct s_rule :
        pegtl::sor<
            t_rule,
            Label_rule
        > {};

    struct args_rule :
        pegtl::sor<
            t_rule,
            pegtl::seq<
                t_rule,
                seps,
                pegtl::plus<
                    pegtl::one<",">,
                    seps,
                    t_rule
                >
            >
        > {};

    struct argument_rule :
        pegtl::seq<
        seps,
        type_rule,
        seps,
        variable_rule,
        seps
        > {};

    struct type_rule :
        pegtl::sor<
            pegtl::seq<
                str_int64,
                pegtl::plus<
                    pegtl::seq<
                        pegtl::one<"[">,
                        seps,
                        pegtl::one<"]"
                    >
                >
            >,
            str_int64,
            str_tuple,
            str_code
        > {};

    struct T_rule :
        pegtl::sor<
            type_rule,
            str_void
        > {};

    struct Instruction_assignment_rule :
        pegtl::seq<
            variable_rule,
            seps,
            str_arrow,
            s_rule
        > {};
    struct Instruction_compare_rule :
        pegtl::seq<
            variable_rule,
            seps,
            str_arrow,
            seps,
            t_rule,
            seps,
            cmp_rule,
            seps,
            t_rule
        > {};
    struct Instruction_label_rule :
        Label_rule {};
    struct Instruction_goto_rule :
        pegtl::seq<
            str_br,
            seps,
            Label_rule
        > {};
    struct Instruction_cjump_rule :
        pegtl::seq<
            str_br,
            seps,
            t_rule,
            seps,
            Label_rule,
            seps,
            Label_rule
        > {};
    struct Instruction_empty_return_rule :
        str_return {};
    struct Instruction_return_rule :
        pegtl::seq<
            seps,
            str_return,
            seps,
            t_rule,
            seps
        > {};
    struct Instruction_assignToTensor_rule :
        pegtl::seq<
            variable_rule,
            pegtl::plus<
                pegtl::one<"[">,
                t_rule,
                pegtl::one<"]"
            >,
            seps,
            str_arrow,
            seps,
            s_rule
        > {};
    struct Instruction_assignFromTensor_rule :
        pegtl::seq<
            variable_rule,
            seps,
            str_arrow,
            variable_rule,
            pegtl::plus<
                pegtl::one<"[">,
                t_rule,
                pegtl::one<"]"
            >
        > {};
    struct Instruction_length_rule:
        pegtl::seq<
            variable_rule,
            seps,
            str_arrow,
            seps,
            str_length,
            seps,
            variable_rule,
            seps,
            t_rule
        > {};
    struct Instruction_call_rule:
        pegtl::seq<

        > {};
    struct Instruction_assignFromCall_rule:
        pegtl::seq<

        > {};
    struct Instruction_newArray_rule:
        pegtl::seq<
            variable_rule,
            seps,
            str_arrow,
            seps,
            str_new,
            seps,
            str_new_array,
            pegtl::one<"(">,
            args_rule,
            pegtl::one<")">
        > {};
    struct Instruction_newTuple_rule:
        pegtl::seq<
            variable_rule,
            seps,   
            str_arrow,
            seps,
            str_new,
            seps,
            str_new_tuple,
            pegtl::one<"(">,
            t_rule,
            pegtl::one<")">
        > {};
    struct Instruction_input_rule:
        pegtl::seq<
            variable_rule,
            seps,
            str_arrow,
            seps,
            str_input,
            pegtl::one<"(">,
            pegtl::one<")">
        > {};
    struct Instruction_print_rule:
        pegtl::seq<
            str_print,
            pegtl::one<"(">,
            variable_rule,
            pegtl::one<")">
        > {};

    struct Instruction_rule :
        pegtl::sor<
            pegt::seq<pegtl::at<Instruction_assignment_rule>, Instruction_assignment_rule>,
            pegt::seq<pegtl::at<Instruction_compare_rule>, Instruction_compare_rule>,
            pegt::seq<pegtl::at<Instruction_label_rule>, Instruction_label_rule>,
            pegt::seq<pegtl::at<Instruction_goto_rule>, Instruction_goto_rule>,
            pegt::seq<pegtl::at<Instruction_cjump_rule>, Instruction_cjump_rule>,
            pegt::seq<pegtl::at<Instruction_empty_return_rule>, Instruction_empty_return_rule>,
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

    struct Instructions_rule:
        pegtl::star<
            pegtl::seq<
                seps,
                Instruction_rule,
                seps
            >
        >

    struct Function_rule :
        pegtl::seq <
            seps,
            T_rule,
            seps,
            function_name,
            seps,
            pegtl::one<"(">,
            seps,
            pegtl::star<
                argument_rule
            >
            seps,
            pegtl::one<")">,
            seps,   
            pegtl::one<"{">
            seps,
            Instructions_rule
            seps,
            pegtl::one<"}">
            seps
        > {};

    struct Functions_rule :
        pegtl::star<
        seps,
        Function_rule,
        seps
        > {};

    struct main_rule :
        pegtl::seq<
            seps,
            str_void,
            seps,
            str_main,
            seps,
            pegtl::one<"(">,
            seps,
            pegtl::one<")">,
            seps,  
            pegtl::one<"{">,
            
            pegtl::one<"}">
        > {};

    struct entry_point_rule :
        pegtl::seq<
            seps,
            main_rule,
            seps,
            Functions_rule,
            seps
        > {};

   struct grammar :
        pegtl::must<
            entry_point_rule
        > {};

    template<typename Rule>
    struct action : pegtl::nothing<Rule> {};

    template<> struct action <function_name> {
        template<typename Input>
        static void apply(const Input& in, Program& p) {
            auto newF = new Function();
            newF->name = in.string();
            p.functions.push_back(newF);
        }
    };

    template<> struct action <argument_rule> {
        template<typename Input>
        static void apply(const Input& in, Program& p) {
            auto currentF = p.functions.back();
            Item* var = parsed_items.back();
            parsed_items.pop_back();

            Item* type = parsed_items.back();
            parsed_items.pop_back();
        }
    };

    template<> struct action <Label_rule> {
        template<typename Input>
        static void apply(const Input& in, Program& p) {
            Label* l = new Label(in.string());
            
            parsed_items.push_back(l);
        }
    };

    template<> struct action < variable_rule > {
        template< typename Input >
        static void apply(const Input& in, Program& p) {
            Variable* i = new Variable(in.string().substr(1));

            parsed_items.push_back(i);
        }
    };

    template<> struct action <number_rule> {
        template <typename Input>
        static void apply(const Input& in, Program& p) {
            Number* n = new Number(std::stoll(in.string()));

            parsed_items.push_back(i);
        }
    };

    Program parse_file(char* fileName) {
        pegtl::analyze<grammar>();

        file_input<> fileInput(fileName);
        Program p;
        parse<grammar, action>(fileInput, p);
        return p;
    }
}