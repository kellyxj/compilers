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
#include <map>

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

    struct aop_rule :
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

    struct op_rule :
        pegtl::sor<
        aop_rule,
        cmp_rule
        > {};

    struct callee_name :
        name {};

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

    struct type_rule :
        pegtl::sor<
            pegtl::seq<
                str_int64,
                seps,
                pegtl::plus<
                    pegtl::seq<
                    pegtl::one<'['>,
                    seps,
                    pegtl::one<']'>
                >
            >
        >,
        str_int64,
        str_tuple,
        str_code
        > {};

    struct args_rule :
        pegtl::seq<
            t_rule,
            seps,
            pegtl::opt<
                pegtl::plus<
                    seps,
                    pegtl::one<','>,
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

    struct arguments_rule :
        pegtl::seq<
            argument_rule,
            seps,
            pegtl::opt<
                pegtl::plus<
                    seps,
                    pegtl::one<','>,
                    seps,
                    argument_rule,
                    seps
                >
            >
        > {};

    struct T_rule :
        pegtl::sor<
            type_rule,
            str_void
        > {};

    struct Instruction_declare_rule :
        pegtl::seq<
            type_rule,
            seps,
            variable_rule
        > {};

    struct Instruction_assignment_rule :
        pegtl::seq<
            variable_rule,
            seps,
            str_arrow,
            seps,
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
    struct Instruction_op_rule :
        pegtl::seq<
            variable_rule,
            seps,
            str_arrow,
            seps,
            t_rule,
            seps,
            op_rule,
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
                seps,
                pegtl::one<'['>,
                seps,
                t_rule,
                seps,
                pegtl::one<']'>,
                seps
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
            seps,
            variable_rule,
            seps,
            pegtl::plus<
                seps,
                pegtl::one<'['>,
                seps,
                t_rule,
                seps,
                pegtl::one<']'>,
                seps
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
            callee_name,
            seps,
            pegtl::one<'('>,
            seps,
            pegtl::opt<
            args_rule
            >,
            seps,
            pegtl::one<')'>,
            seps
        > {};
    struct Instruction_assignFromCall_rule:
        pegtl::seq<
            variable_rule,
            seps,
            str_arrow,
            seps,
            callee_name,
            seps,
            pegtl::one<'('>,
            seps,
            pegtl::opt<
            args_rule
            >,
            seps,
            pegtl::one<')'>,
            seps
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
            seps,
            pegtl::one<'('>,
            seps,
            args_rule,
            seps,
            pegtl::one<')'>,
            seps
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
            seps,
            pegtl::one<'('>,
            seps,
            t_rule,
            seps,
            pegtl::one<')'>,
            seps
        > {};
    struct Instruction_input_rule:
        pegtl::seq<
            variable_rule,
            seps,
            str_arrow,
            seps,
            str_input,
            seps,
            pegtl::one<'('>,
            seps,
            pegtl::one<')'>,
            seps
        > {};
    struct Instruction_print_rule:
        pegtl::seq<
            str_print,
            pegtl::one<'('>,
            seps,
            t_rule,
            seps,
            pegtl::one<')'>
        > {};

    struct Instruction_rule :
        pegtl::sor<
            pegtl::seq<pegtl::at<Instruction_declare_rule>, Instruction_declare_rule>,
            pegtl::seq<pegtl::at<Instruction_op_rule>, Instruction_op_rule>,
            pegtl::seq<pegtl::at<Instruction_label_rule>, Instruction_label_rule>,
            pegtl::seq<pegtl::at<Instruction_goto_rule>, Instruction_goto_rule>,
            pegtl::seq<pegtl::at<Instruction_cjump_rule>, Instruction_cjump_rule>,
            pegtl::seq<pegtl::at<Instruction_assignToTensor_rule>, Instruction_assignToTensor_rule>,
            pegtl::seq<pegtl::at<Instruction_assignFromTensor_rule>, Instruction_assignFromTensor_rule>,
            pegtl::seq<pegtl::at<Instruction_length_rule>, Instruction_length_rule>,
            pegtl::seq<pegtl::at<Instruction_input_rule>, Instruction_input_rule>,
            pegtl::seq<pegtl::at<Instruction_print_rule>, Instruction_print_rule>,
            pegtl::seq<pegtl::at<Instruction_call_rule>, Instruction_call_rule>,
            pegtl::seq<pegtl::at<Instruction_assignFromCall_rule>, Instruction_assignFromCall_rule>,
            pegtl::seq<pegtl::at<Instruction_newArray_rule>, Instruction_newArray_rule>,
            pegtl::seq<pegtl::at<Instruction_newTuple_rule>, Instruction_newTuple_rule>,
            pegtl::seq<pegtl::at<Instruction_assignment_rule>, Instruction_assignment_rule>,
            pegtl::seq<pegtl::at<Instruction_return_rule>, Instruction_return_rule>,
            pegtl::seq<pegtl::at<Instruction_empty_return_rule>, Instruction_empty_return_rule>
        > {};

    struct Instructions_rule :
        pegtl::plus<
            pegtl::seq<
            seps,
            Instruction_rule,
            seps
            >
        > {};

    struct Function_rule :
        pegtl::seq <
            seps,
            T_rule,
            seps,
            function_name,
            seps,
            pegtl::one<'('>,
            seps,
            pegtl::opt<
                arguments_rule
            >,
            seps,
            pegtl::one<')'>,
            seps,   
            pegtl::one<'{'>,
            pegtl::opt<
                Instructions_rule
            >,
            seps,
            pegtl::one<'}'>,
            seps
        > {};

    struct Functions_rule :
        pegtl::plus<
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
            pegtl::one<'('>,
            seps,
            pegtl::one<')'>,
            seps,  
            pegtl::one<'{'>,
            pegtl::opt<
                Instructions_rule
            >,
            seps,
            pegtl::one<'}'>
        > {};

    struct entry_point_rule :
        pegtl::seq<
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

            Type* t = dynamic_cast<Type*>(parsed_items.back());
            parsed_items.pop_back();
            newF->returnType = t;

            p.functions.push_back(newF);
        }
    };

    template<> struct action <main_rule> {
        template<typename Input>
        static void apply(const Input& in, Program& p) {
            auto newF = new Function();
            newF->name = "main";
            newF->returnType = type_void;
            p.functions.push_back(newF);
        }
    };

    template<> struct action <argument_rule> {
        template<typename Input>
        static void apply(const Input& in, Program& p) {
            auto currentF = p.functions.back();
            Variable* var = dynamic_cast<Variable*>(parsed_items.back());
            parsed_items.pop_back();

            Type* type = dynamic_cast<Type*>(parsed_items.back());
            parsed_items.pop_back();

            currentF->args.push_back(var);
            currentF->types.push_back(type);
        }
    };

    template<> struct action <T_rule> {
        template<typename Input>
        static void apply(const Input& in, Program& p) {
            if (in.string() == "void") {
                Type* t = new Type(type_void);
                parsed_items.push_back(t);
            }
        }
    };

    template<> struct action <Label_rule> {
        template<typename Input>
        static void apply(const Input& in, Program& p) {
            auto currentF = p.functions.back();
            Label* l = currentF->newLabel(in.string());

            parsed_items.push_back(l);
        }
    };

    template<> struct action < variable_rule > {
        template< typename Input >
        static void apply(const Input& in, Program& p) {
            auto currentF = p.functions.back();
            Variable* i = currentF->newVariable(in.string());

            parsed_items.push_back(i);
        }
    };

    template<> struct action <type_rule> {
        template <typename Input>
        static void apply(const Input& in, Program& p) {
            std::map<std::string, DataType> allTypes;
            allTypes.insert(std::make_pair("int64", int64));
            allTypes.insert(std::make_pair("tuple", tuple));
            allTypes.insert(std::make_pair("code", code));
            if (allTypes.find(in.string()) != allTypes.end()) {
                Type* t = new Type(allTypes[in.string()]);
                parsed_items.push_back(t);
            }
            else {
                int rank = 0;
                int n = in.string().size();
                for (int i = 0; i < n; i++) {
                    if (in.string().at(i) == '[') {
                        rank++;
                    }
                }
                Type* t = new Type(tensor, rank);
                parsed_items.push_back(t);
            }
        }
    };

    template<> struct action <number_rule> {
        template <typename Input>
        static void apply(const Input& in, Program& p) {
            Number* n = new Number(std::stoll(in.string()));
            parsed_items.push_back(n);
        }
    };

    template<> struct action <callee_name> {
        template <typename Input>
        static void apply(const Input& in, Program& p) {
            auto currentF = p.functions.back();
            Variable* v = currentF->newVariable(in.string(), true);

            parsed_items.push_back(v);
        }
    };

    template<> struct action<Instruction_declare_rule> {
        template <typename Input>
        static void apply(const Input& in, Program& p) {
            auto currentF = p.functions.back();
            Variable* v = dynamic_cast<Variable*>(parsed_items.back());
            parsed_items.pop_back();

            Type* t = dynamic_cast<Type*>(parsed_items.back());
            parsed_items.pop_back();

            Instruction_declare* i = new Instruction_declare(v, t);
            i->setLineNumber(in.position().line);
            currentF->instructions.push_back(i);
        }
    };

    template<> struct action<Instruction_assignment_rule> {
        template <typename Input>
        static void apply(const Input& in, Program& p) {
            auto currentF = p.functions.back();

            Item* s = parsed_items.back();
            parsed_items.pop_back();

            Variable* d = dynamic_cast<Variable*>(parsed_items.back());
            parsed_items.pop_back();

            auto i = new Instruction_assignment(s, d);
            i->setLineNumber(in.position().line);
            currentF->instructions.push_back(i);
        }
    };
    template<> struct action<Instruction_op_rule> {
        template <typename Input>
        static void apply(const Input& in, Program& p) {
            auto currentF = p.functions.back();

            Item* right = parsed_items.back();
            parsed_items.pop_back();

            Item* left = parsed_items.back();
            parsed_items.pop_back();

            Variable* dest = dynamic_cast<Variable*>(parsed_items.back());
            parsed_items.pop_back();

            std::map<std::string, Operator> ops;
            ops.insert(std::make_pair("<", cmp_lt));
            ops.insert(std::make_pair(">", cmp_gt));
            ops.insert(std::make_pair("=", cmp_eq));
            ops.insert(std::make_pair("<=", cmp_le));
            ops.insert(std::make_pair(">=", cmp_ge));
            ops.insert(std::make_pair("+", op_plus));
            ops.insert(std::make_pair("-", op_minus));
            ops.insert(std::make_pair("*", op_mult));
            ops.insert(std::make_pair("&", op_and));
            ops.insert(std::make_pair("<<", op_lshift));
            ops.insert(std::make_pair(">>", op_rshift));

            std::vector<std::string> opStrings{"-", "<", ">", "=", "<=", ">=", "+", "*", "&", "<<", ">>"};

            Operator o;
            for (auto op : opStrings) {
                if (in.string().find(op) != std::string::npos) {
                    o = ops[op];
                }
            }

            auto i = new Instruction_op(dest, left, o, right);
            i->setLineNumber(in.position().line);

            currentF->instructions.push_back(i);
        }
    };
    template<> struct action<Instruction_label_rule> {
        template <typename Input>
        static void apply(const Input& in, Program& p) {
            auto currentF = p.functions.back();
            Label* l = currentF->newLabel(in.string());

            auto i = new Instruction_label(l);
            i->setLineNumber(in.position().line);
            currentF->instructions.push_back(i);
        }
    };
    template<> struct action<Instruction_goto_rule> {
        template <typename Input>
        static void apply(const Input& in, Program& p) {
            auto currentF = p.functions.back();
            Label* l = dynamic_cast<Label*>(parsed_items.back());
            parsed_items.pop_back();

            auto i = new Instruction_goto(l);
            i->setLineNumber(in.position().line);

            currentF->instructions.push_back(i);
        }
    };
    template<> struct action<Instruction_cjump_rule> {
        template <typename Input>
        static void apply(const Input& in, Program& p) {
            auto currentF = p.functions.back();
            Label* l2 = dynamic_cast<Label*>(parsed_items.back());
            parsed_items.pop_back();

            Label* l1 = dynamic_cast<Label*>(parsed_items.back());
            parsed_items.pop_back();

            Item* t = parsed_items.back();
            parsed_items.pop_back();

            auto i = new Instruction_cjump(t, l1, l2);
            i->setLineNumber(in.position().line);

            currentF->instructions.push_back(i);
        }
    };
    template<> struct action<Instruction_return_rule> {
        template <typename Input>
        static void apply(const Input& in, Program& p) {
            auto currentF = p.functions.back();

            Item* i = parsed_items.back();
            parsed_items.pop_back();

            auto inst = new Instruction_return(i);
            inst->setLineNumber(in.position().line);
            currentF->instructions.push_back(inst);
        }
    };
    template<> struct action<Instruction_empty_return_rule> {
        template <typename Input>
        static void apply(const Input& in, Program& p) {
            auto currentF = p.functions.back();

            auto i = new Instruction_empty_return();
            i->setLineNumber(in.position().line);
            currentF->instructions.push_back(i);
        }
    };
    template<> struct action<Instruction_assignToTensor_rule> {
        template <typename Input>
        static void apply(const Input& in, Program& p) {
            auto currentF = p.functions.back();

            Item* src = parsed_items.back();
            parsed_items.pop_back();

            std::vector<Item*> indices;
            while (parsed_items.size() > 1) {
                indices.push_back(parsed_items.back());
                parsed_items.pop_back();
            }
            std::reverse(indices.begin(), indices.end());

            Variable* dst = dynamic_cast<Variable*>(parsed_items.back());
            parsed_items.pop_back();

            auto i = new Instruction_assignToTensor(dst, indices, src);
            i->setLineNumber(in.position().line);

            currentF->instructions.push_back(i);
        }
    };
    template<> struct action<Instruction_assignFromTensor_rule> {
        template <typename Input>
        static void apply(const Input& in, Program& p) {
            auto currentF = p.functions.back();
            std::vector<Item*> indices;
            while (parsed_items.size() > 2) {
                indices.push_back(parsed_items.back());
                parsed_items.pop_back();
            }
            std::reverse(indices.begin(), indices.end());

            Variable* src = dynamic_cast<Variable*>(parsed_items.back());
            parsed_items.pop_back();

            Variable* dst = dynamic_cast<Variable*>(parsed_items.back());
            parsed_items.pop_back();

            auto i = new Instruction_assignFromTensor(dst, indices, src);
            i->setLineNumber(in.position().line);

            currentF->instructions.push_back(i);
        }
    };
    template<> struct action<Instruction_length_rule> {
        template <typename Input>
        static void apply(const Input& in, Program& p) {
            auto currentF = p.functions.back();

            Item* t = parsed_items.back();
            parsed_items.pop_back();

            Variable* v = dynamic_cast<Variable*>(parsed_items.back());
            parsed_items.pop_back();

            Variable* dst = dynamic_cast<Variable*>(parsed_items.back());
            parsed_items.pop_back();

            auto i = new Instruction_length(dst, v, t);
            i->setLineNumber(in.position().line);

            currentF->instructions.push_back(i);
        }
    };
    template<> struct action<Instruction_call_rule> {
        template <typename Input>
        static void apply(const Input& in, Program& p) {
            auto currentF = p.functions.back();

            std::vector<Item*> args;

            while (parsed_items.size() > 1) {
                args.push_back(parsed_items.back());
                parsed_items.pop_back();
            }
            std::reverse(args.begin(), args.end());

            Item* callee = parsed_items.back();
            parsed_items.pop_back();

            auto i = new Instruction_call(callee, args);
            i->setLineNumber(in.position().line);

            currentF->instructions.push_back(i);
        }
    };
    template<> struct action<Instruction_assignFromCall_rule> {
        template <typename Input>
        static void apply(const Input& in, Program& p) {
            auto currentF = p.functions.back();

            std::vector<Item*> args;

            while (parsed_items.size() > 2) {
                args.push_back(parsed_items.back());
                parsed_items.pop_back();
            }
            std::reverse(args.begin(), args.end());

            Item* callee = parsed_items.back();
            parsed_items.pop_back();

            Variable* dest = dynamic_cast<Variable*>(parsed_items.back());
            parsed_items.pop_back();

            auto i = new Instruction_assignFromCall(dest, callee, args);
            i->setLineNumber(in.position().line);

            currentF->instructions.push_back(i);
        }
    };
    template<> struct action<Instruction_newArray_rule> {
        template <typename Input>
        static void apply(const Input& in, Program& p) {
            auto currentF = p.functions.back();
            std::vector<Item*> dims;
            while (parsed_items.size() > 1) {
                dims.push_back(parsed_items.back());
                parsed_items.pop_back();
            }
            std::reverse(dims.begin(), dims.end());

            Variable* dst = dynamic_cast<Variable*>(parsed_items.back());
            parsed_items.pop_back();

            auto i = new Instruction_newArray(dst, dims);
            i->setLineNumber(in.position().line);

            currentF->instructions.push_back(i);
        }
    };
    template<> struct action<Instruction_newTuple_rule> {
        template <typename Input>
        static void apply(const Input& in, Program& p) {
            auto currentF = p.functions.back();
            Item* t = parsed_items.back();
            parsed_items.pop_back();

            Variable* dst = dynamic_cast<Variable*>(parsed_items.back());
            parsed_items.pop_back();

            auto i = new Instruction_newTuple(dst, t);
            i->setLineNumber(in.position().line);

            currentF->instructions.push_back(i);
        }
    };

    template<> struct action<Instruction_print_rule> {
        template <typename Input>
        static void apply(const Input& in, Program& p) {
            auto currentF = p.functions.back();

            auto t = parsed_items.back();
            parsed_items.pop_back();

            auto i = new Instruction_print(t);
            i->setLineNumber(in.position().line);
            currentF->instructions.push_back(i);
        }
    };

    template<> struct action<Instruction_input_rule> {
        template <typename Input>
        static void apply(const Input& in, Program& p) {
            auto currentF = p.functions.back();

            auto dst = dynamic_cast<Variable*>(parsed_items.back());
            parsed_items.pop_back();

            auto i = new Instruction_input(dst);
            i->setLineNumber(in.position().line);
            currentF->instructions.push_back(i);
        }
    };

    Program parse_file(char* fileName) {
        pegtl::analyze<grammar>();

        file_input<> fileInput(fileName);
        Program p;
        p.entryPoint = "main";
        parse<grammar, action>(fileInput, p);
        return p;
    }
}