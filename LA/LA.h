#pragma once

#include <vector>
#include <string>
#include <utility>
#include <set>

namespace LA {
    class Visitor;

    enum Operator { op_plus, op_minus, op_mult, op_and, op_lshift, op_rshift, op_inc, op_dec };
    enum LogicOperator { cmp_lt, cmp_le, cmp_eq, cmp_ge, cmp_gt };
    enum DataTypes {int64, tensor, tuple, code};

    class Item {
        public:
            virtual std::string toString(void) = 0;
    };

    class Number : public Item {
        public:
            Number(int64_t n);
            int64_t get(void);
            std::string toString(void) override;
        private:
            int64_t number;
    };

    class Label : public Item {
        public:
            Label(std::string l);
            std::string get(void);
            std::string toString(void) override;
        private:
            std::string label;
    };

    class Variable : public Item {
        public:
            Variable(std::string name);
            std::string get(void);
            std::string toString(void) override;
        private:
            std::string name;
    };

    class Tensor : public Variable {
        public:
            Tensor(std::string name);
            std::string get(void);
            std::string toString(void) override;
        private:
            std::string name;
    };

    class Tuple : public Variable {
        public:
            Tuple(std::string name);
            std::string get(void);
            std::string toString(void) override;
        private:
            std::string name;
    };

    class Code : public Variable {
        public:
            Code(std::string name);
            std::string get(void);
            std::string toString(void) override;
        private:
            std::string name;
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
        public:
            Instruction_assignment(Item* s, Item* d);
            Item* getSrc(void);
            Item* getDst(void);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            Item* src;
            Item* dst;
    };

    class Instruction_compare : public Instruction {
        public:
            Instruction_compare(Item* d, Item* l, LogicOperator c, Item* r);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            Item* dst;
            Item* left;
            LogicOperator cmp;
            Item* right;
    };

    class Instruction_label : public Instruction {
        public:
            Instruction_label(Label* l);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            Label* l
    };

    class Instruction_goto : public Instruction {
        public:
            Instruction_goto(Label* l);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            Label* l
    };

    class Instruction_cjump : public Instruction {
        public:
            Instruction_cjump(Item* l, LogicOperator c, Item* r, Label* label);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            Item* left;
            Item* right;
            LogicOperator cmp;
            Label* label;
    };

    class Instruction_return : public Instruction {
        public:
            Instruction_return();
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
    };

    class Instruction_assignToTensor : public Instruction {
        public:
            Instruction_assignToTensor(Item* dst, std::vector<Item*> args, Item* src);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            Item* dst;
            std::vector<Item*> args;
            Item* src;
    };

    class Instruction_assignFromTensor : public Instruction {
        public:
            Instruction_assignFromTensor(Item* dst, std::vector<Item*> args, Item* src);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            Item* dst;
            std::vector<Item*> args;
            Item* src;
    };

    class Instruction_length : public Instruction {
        public:
            Instruction_length(Item* dst, Variable* v, int64_t dim);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            Item* dst;
            Variable* v;
            int64_t dim;
    };

    class Instruction_call : public Instruction {
        public:
            Instruction_call(Item* callee, std::vector<Item*> args);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            Item* callee;
            std::vector<Item*> args;
    };
    
    class Instruction_assignFromCall : public Instruction {
        public:
            Instruction_assignFromCall();
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
    };

    class Instruction_newArray : public Instruction {
        public:
            Instruction_newArray(std::vector<int> dims);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            std::vector<int> dims
    };

    class Instruction_newTuple : public Instruction {
        public:
            Instruction_newTuple(int64_t size);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            int64_t size;
    };

    class Instruction_input : public Instruction {
        public:
            Instruction_input();
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
    };

    class Instruction_print : public Instruction {
        public:
            Instruction_print();
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
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