#include <vector>
#include <string>
#include <utility>
#include <set>

namespace LA {
    class Visitor;

    enum RegisterID { rdi, rsi, rdx, rcx, r8, r9, rax, rbx, rbp, r10, r11, r12, r13, r14, r15, rsp };
    enum Operator { op_plus, op_minus, op_mult, op_and, op_lshift, op_rshift, op_inc, op_dec };
    enum LogicOperator { cmp_lt, cmp_le, cmp_eq, cmp_ge, cmp_gt };

    class Item {
        public:
            virtual std::string toString(void) = 0;
    };

    class Number : public Item {

    };

    class Label : public Item {

    };

    class Variable : public Item {

    };

    class Tensor : public Variable {

    };

    class Tuple : public Variable {

    };

    class Code : public Variable {

    };

    class Operation : public Item {

    };

    class CompareOp : public Item {

    };

    class Instruction {
        public:
            std::set<Instruction*> predecessors;
            std::set<Instruction*> successors;

            std::set<Variable*> uses;
            std::set<Variable*> defs;

            virtual std::string toString(void) = 0;
            virtual void accept(Visitor visitor) = 0;
    };

    class Instruction_assignment : public Instruction {

    };

    class Instruction_compare : public Instruction {

    };

    class Instruction_label : public Instruction {

    };

    class Instruction_goto : public Instruction {

    };

    class Instruction_cjump : public Instruction {

    };

    class Instruction_return : public Instruction {

    };

    class Instruction_assignToTensor : public Instruction {

    };

    class Instruction_assignFromTensor : public Instruction {

    };

    class Instruction_length : public Instruction {

    };

    class Instruction_call : public Instruction {

    };
    
    class Instruction_assignFromCall : public Instruction {

    };

    class Instruction_newArray : public Instruction {

    };

    class Instruction_newTuple : public Instruction {

    };

    class Instruction_input : public Instruction {

    };

    class Instruction_print : public Instruction {

    };

    class Function {
        public:
            std::string name;
            std::vector<Variable*> args;
            std::vector<Instruction*> instructions;

            void computeCFG(void);
            void computeUseDefs(void);

            Variable* newVariable();
            Tensor* newTensor();
            Tuple* newTuple();
            Code* newCode();

            std::string toString(void);
    };
    
    class Program {
        public:
            std::string entryPointLabel;
            std::vector<Function*> functions;

            std::string toString(void);
    };

    class Visitor {
        public:
            virtual void visit(Instruction_assignment* inst) = 0;
            virtual void visit(Instruction_compare* inst) = 0;
            virtual void visit(Instruction_label* inst) = 0;
            virtual void visit(Instruction_goto* inst) = 0;
            virtual void visit(Instruction_cjump* inst) = 0;
            virtual void visit(Instruction_return* inst) = 0;
            virtual void visit(Instruction_assignToTensor* inst) = 0;
            virtual void visit(Instruction_assignFromTensor* inst) = 0;
            virtual void visit(Instruction_length* inst) = 0;
            virtual void visit(Instruction_call* inst) = 0;
            virtual void visit(Instruction_assignFromCall* inst) = 0;
            virtual void visit(Instruction_newArray* inst) = 0;
            virtual void visit(Instruction_newTuple* inst) = 0;
            virtual void visit(Instruction_input* inst) = 0;
            virtual void visit(Instruction_print* inst) = 0;
    };
}