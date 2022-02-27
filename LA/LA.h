#pragma once

#include <vector>
#include <string>
#include <utility>
#include <set>
#include <map>

namespace LA {
    class Visitor;

    enum Operator { op_plus, op_minus, op_mult, op_and, op_lshift, op_rshift, cmp_lt, cmp_le, cmp_eq, cmp_ge, cmp_gt};
    enum DataType {type_void, int64, tensor, tuple, code};

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

    class Type : public Item {
        public:
            Type(DataType t, int64_t rank = 0);
            DataType get(void);
            std::string toString(void) override;
        private:
            DataType type;
            int64_t rank;
    };

    class Variable : public Item {
        public:
            Variable(std::string name);
            std::string get(void);
            std::string toString(void) override;
        private:
            std::string name;
    };

    class Integer : public Variable {
        public:
            Integer(std::string name) : Variable(name) {};
    };

    class Tensor : public Variable {
        public:
            Tensor(std::string name, int64_t rank);
        private:
            std::string name;
            int64_t rank;
    };

    class Tuple : public Variable {
        public:
            Tuple(std::string name) : Variable(name) {};
            std::string toString(void) override;
    };

    class Code : public Variable {
        public:
            Code(std::string name) : Variable(name) {};
            std::string toString(void) override;
    };

    class Instruction {
        public:
            virtual std::string toString(void) = 0;
            virtual void accept(Visitor* visitor) = 0;
            void setLineNumber(int64_t n);
            int64_t getLineNumber(void);
        private:
            int64_t lineNumber;
    };

    class Instruction_declare : public Instruction {
        public:
            Instruction_declare(Variable* v, Type* t);
            Variable* getVar(void);
            Type* getType(void);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            Variable* var;
            Type* type;
    };

    class Instruction_assignment : public Instruction {
        public:
            Instruction_assignment(Item* s, Variable* d);
            Item* getSrc(void);
            Item* getDst(void);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            Item* src;
            Variable* dst;
    };

    class Instruction_op : public Instruction {
        public:
            Instruction_op(Variable* d, Item* l, Operator o, Item* r);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            Variable* dst;
            Item* left;
            Operator op;
            Item* right;
    };

    class Instruction_label : public Instruction {
        public:
            Instruction_label(Label* l);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            Label* l;
    };

    class Instruction_goto : public Instruction {
        public:
            Instruction_goto(Label* l);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            Label* l;
    };

    class Instruction_cjump : public Instruction {
        public:
            Instruction_cjump(Item* t, Label* l1, Label* l2);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            Item* t;
            Label* l1;
            Label* l2;
    };

    class Instruction_return : public Instruction {
        public:
            Instruction_return(Item* v);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;

        private:
            Item* src;
    };

    class Instruction_empty_return : public Instruction {
        public:
            Instruction_empty_return(void);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
    };

    class Instruction_assignToTensor : public Instruction {
        public:
            Instruction_assignToTensor(Variable* dst, std::vector<Item*> args, Item* src);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            Variable* dst;
            std::vector<Item*> args;
            Item* src;
    };

    class Instruction_assignFromTensor : public Instruction {
        public:
            Instruction_assignFromTensor(Variable* dst, std::vector<Item*> args, Variable* src);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            Variable* dst;
            std::vector<Item*> args;
            Variable* src;
    };

    class Instruction_length : public Instruction {
        public:
            Instruction_length(Variable* dst, Variable* v, Item* dim);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            Variable* dst;
            Variable* v;
            Item* dim;
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
            Instruction_assignFromCall(Variable* dst, Item* callee, std::vector<Item*> args);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            Variable* dst;
            Item* callee;
            std::vector<Item*> args;
    };

    class Instruction_newArray : public Instruction {
        public:
            Instruction_newArray(Variable* dst, std::vector<Item*> dims);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            Variable* dst;
            std::vector<Item*> dims;
    };

    class Instruction_newTuple : public Instruction {
        public:
            Instruction_newTuple(Variable* dst, Item* size);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            Variable* dst;
            Item* size;
    };

    class Instruction_print : public Instruction {
        public:
            Instruction_print(Item* t);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            Item* t;
    };

    class Instruction_input : public Instruction {
        public:
            Instruction_input(Variable* dst);
            std::string toString(void) override;
            void accept(Visitor* visitor) override;
        private:
            Item* dst;
    };

    class Function {
        public:
            std::string name;
            Type* returnType;
            std::vector<Variable*> args;
            std::vector<Type*> types;
            std::vector<Instruction*> instructions;
            std::map<std::string, Variable*> variables;

            Variable* newVariable(std::string name, bool isCallee = false);
            Variable* getVariable(std::string name);

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
            virtual void visit(Instruction_declare* inst) = 0;
            virtual void visit(Instruction_assignment* inst) = 0;
            virtual void visit(Instruction_op* inst) = 0;
            virtual void visit(Instruction_label* inst) = 0;
            virtual void visit(Instruction_goto* inst) = 0;
            virtual void visit(Instruction_cjump* inst) = 0;
            virtual void visit(Instruction_return* inst) = 0;
            virtual void visit(Instruction_empty_return* inst) = 0;
            virtual void visit(Instruction_assignToTensor* inst) = 0;
            virtual void visit(Instruction_assignFromTensor* inst) = 0;
            virtual void visit(Instruction_length* inst) = 0;
            virtual void visit(Instruction_call* inst) = 0;
            virtual void visit(Instruction_assignFromCall* inst) = 0;
            virtual void visit(Instruction_newArray* inst) = 0;
            virtual void visit(Instruction_newTuple* inst) = 0;
            virtual void visit(Instruction_print* inst) = 0;
            virtual void visit(Instruction_input* inst) = 0;
    };
}