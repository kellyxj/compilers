#pragma once

#include <vector>
#include <string>
#include <utility>

namespace L1 {
    class Visitor;

    enum RegisterID { rdi, rsi, rdx, rcx, r8, r9, rax, rbx, rbp, r10, r11, r12, r13, r14, r15, rsp };
    enum Operator { op_plus, op_minus, op_mult, op_and, op_lshift, op_rshift , op_inc, op_dec};
    enum LogicOperator { cmp_lt, cmp_le, cmp_eq, cmp_ge, cmp_gt };

    class Item {
        virtual std::string toString(void) = 0;
    };

    class Number : public Item {
        public: 
            Number(int64_t n);
            int64_t get(void);

            std::string toString(void) override;
            bool operator == (const Number& other) const;

        private:
            int64_t number;
    };

    class Label : public Item {
        public:
            Label(std::string s);
            std::string get(void);

            std::string toString(void) override;
            bool operator == (const Number& other) const;

        private:
            std::string labelName;
    };

    class Register : public Item {
        public:
            Register(RegisterID id);
            RegisterID get(void);
        
            std::string toString(void) override;
            bool operator == (const Register& other) const;

        private:
            RegisterID registerID;
    };

    class Memory : public Item {
        public:
            Memory(Register* s, Number* o);
            std::pair<RegisterID id, int64_t> get(void);

            std::string toString(void) override;
            bool operator == (const Memory& other) const;

        private:
            Register *startAddress;
            Number *offset;
        };

    class Operation : public Item {
        public:
            Operation(Operator op);
            Operator get(void);

            std::string toString(void) override;
            bool operator == (const Operation& other) const;

        private:
            Operator op;
    };

    class CompareOp : public Item {
        public:
            CompareOp(LogicOperator cmp);
            LogicOperator get(void);

            std::string toString(void) override;
            bool operator == (const CompareOp& other) const;

        private:
            LogicOperator cmp;
    };

    class Instruction {
        public:
            virtual std::string toString(void) = 0;
            virtual void accept(Visitor visitor) = 0;
    };

    class Instruction_assignment : public Instruction {

    };
    class Instruction_aop : public Instruction {

    };
    class Instruction_memop : public Instruction {

    };
    class Instruction_lea : public Instruction {

    };
    class Instruction_compare : public Instruction {

    };
    class Instruction_label : public Instruction {

    };
    class Instruction_goto : public Instruction {

    };
    class Instruction_cjump : public Instruction {

    };
    class Instruction_ret : public Instruction {

    };
    class Instruction_print : public Instruction {

    };
    class Instruction_allocate : public Instruction {

    };
    class Instruction_input : public Instruction {

    };
    class Instruction_tensor_error : public Instruction {

    };
    class Instruction_call : public Instruction {

    };

    class Function {
        public:
            std::string name;
            int64_t arguments;
            int64_t locals;
            std::vector<Instruction*> instructions;

            Register* newRegister(RegisterID id);

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
            virtual void visit(Instruction_aop* inst) = 0;
            virtual void visit(Instruction_memop* inst) = 0;
            virtual void visit(Instruction_lea* inst) = 0;
            virtual void visit(Instruction_compare* inst) = 0;
            virtual void visit(Instruction_label* inst) = 0;
            virtual void visit(Instruction_goto* inst) = 0;
            virtual void visit(Instruction_cjump* inst) = 0;
            virtual void visit(Instruction_ret* inst) = 0;
            virtual void visit(Instruction_print* inst) = 0;
            virtual void visit(Instruction_allocate* inst) = 0;
            virtual void visit(Instruction_input* inst) = 0;
            virtual void visit(Instruction_tensor_error* inst) = 0;
            virtual void visit(Instruction_call* inst) = 0;
        };
}