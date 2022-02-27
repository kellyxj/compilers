#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <set>
#include <utility>
#include <map>

#include <LA.h>

using namespace LA;

namespace LA {
	Number::Number(int64_t n) {
        this->number = n;
	}
    std::string Number::toString(void) {
        return std::to_string(this->number);
    }
    int64_t Number::get(void) {
        return this->number;
    }

	Label::Label(std::string l) {
        this->label = l;
	}
    std::string Label::toString(void) {
        return this->label;
    }
    std::string Label::get(void) {
        return this->label;
    }

    Type::Type(DataType t, int64_t rank) {
        this->type = t;
        this->rank = rank;
    }
    DataType Type::get() {
        return this->type;
    }
    std::string Type::toString(void) {
        std::map<DataType, std::string> typeNames;
        typeNames.insert(std::make_pair(type_void, "void"));
        typeNames.insert(std::make_pair(int64, "int64"));
        typeNames.insert(std::make_pair(tensor, "tensor"));
        typeNames.insert(std::make_pair(tuple, "tuple"));
        typeNames.insert(std::make_pair(code, "code"));

        if (this->type == tensor) {
            std::string outputString = "int64";
            for (int i = 0; i < this->rank; i++) {
                outputString += "[]";
            }
            return outputString;
        }
        else {
            return typeNames[this->type];
        }
    }

	Variable::Variable(std::string name) {
        this->name = name;
	}
    std::string Variable::toString(void) {
        return this->name;
    }
    std::string Variable::get(void) {
        return this->name;
    }

    void Instruction::setLineNumber(int64_t n) {
        this->lineNumber = n;
    }
    int64_t Instruction::getLineNumber() {
        return this->lineNumber;
    }

    Instruction_declare::Instruction_declare(Variable* v, Type* t) {
        this->var = v;
        this->type = t;
    }
    Type* Instruction_declare::getType() {
        return this->type;
    }
    Variable* Instruction_declare::getVar() {
        return this->var;
    }
    std::string Instruction_declare::toString(void) {
        std::map<DataType, std::string> typeNames;
        return this->type->toString() + " " + this->var->toString();
    }
    void Instruction_declare::accept(Visitor* visitor) {
        visitor->visit(this);
    }

	Instruction_assignment::Instruction_assignment(Item* s, Variable* d) {
        this->src = s;
        this->dst = d;
	}
    Item* Instruction_assignment::getSrc() {
        return this->src;
    }
    Item* Instruction_assignment::getDst() {
        return this->dst;
    }
    std::string Instruction_assignment::toString(void) {
        return this->dst->toString() + " <- " + this->src->toString();
    }
    void Instruction_assignment::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_op::Instruction_op(Variable* d, Item* l, Operator o, Item* r) {
        this->dst = d;
        this->left = l;
        this->op = o;
        this->right = r;
    }
    std::string Instruction_op::toString(void) {
        std::map<Operator, std::string> opStrings;
        opStrings.insert(std::make_pair(op_plus, "+"));
        opStrings.insert(std::make_pair(op_minus, "-"));
        opStrings.insert(std::make_pair(op_mult, "*"));
        opStrings.insert(std::make_pair(op_and, "&"));
        opStrings.insert(std::make_pair(op_lshift, "<<"));
        opStrings.insert(std::make_pair(op_rshift, ">>"));
        opStrings.insert(std::make_pair(cmp_lt, "<"));
        opStrings.insert(std::make_pair(cmp_le, "<="));
        opStrings.insert(std::make_pair(cmp_eq, "="));
        opStrings.insert(std::make_pair(cmp_ge, ">="));
        opStrings.insert(std::make_pair(cmp_gt, ">"));

        return this->dst->toString() + " <- " + this->left->toString() + opStrings[this->op] + this->right->toString();
    }
    void Instruction_op::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_label::Instruction_label(Label* l) {
        this->l = l;
    }
    std::string Instruction_label::toString(void) {
        return this->l->toString();
    }
    void Instruction_label::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_goto::Instruction_goto(Label* l) {
        this->l = l;
    }
    std::string Instruction_goto::toString(void) {
        return "br " + this->l->toString();
    }
    void Instruction_goto::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_cjump::Instruction_cjump(Item* t, Label* l1, Label* l2) {
        this->t = t;
        this->l1 = l1;
        this->l2 = l2;
    }
    std::string Instruction_cjump::toString(void) {
        return "br " + this->t->toString() + " " + this->l1->toString() + " " + this->l2->toString();
    }
    void Instruction_cjump::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_return::Instruction_return(Item* v) {
        this->src = v;
    }
    std::string Instruction_return::toString(void) {
        return "return " + this->src->toString();
    }
    void Instruction_return::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_empty_return::Instruction_empty_return() {

    }
    std::string Instruction_empty_return::toString(void) {
        return "return";
    }
    void Instruction_empty_return::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_assignToTensor::Instruction_assignToTensor(Variable* dst, std::vector<Item*> args, Item* src) {
        this->dst = dst;
        this->args = args;
        this->src = src;
    }
    std::string Instruction_assignToTensor::toString(void) {
        std::string outputString;
        outputString += this->dst->toString();
        for (auto arg : this->args) {
            outputString += "[";
            outputString += arg->toString();
            outputString += "]";
        }
        outputString += " <- " + this->src->toString();
        return outputString;
    }
    void Instruction_assignToTensor::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_assignFromTensor::Instruction_assignFromTensor(Variable* dst, std::vector<Item*> args, Variable* src) {
        this->dst = dst;
        this->args = args;
        this->src = src;
    }
    std::string Instruction_assignFromTensor::toString(void) {
        std::string outputString;
        outputString += this->dst->toString() + " <- " + this->src->toString();
        for (auto arg : this->args) {
            outputString += "[";
            outputString += arg->toString();
            outputString += "]";
        }
        return outputString;
    }
    void Instruction_assignFromTensor::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_length::Instruction_length(Variable* dst, Variable* v, Item* dim) {
        this->dst = dst;
        this->v = v;
        this->dim = dim;
    }
    std::string Instruction_length::toString(void) {
        return this->dst->toString() + " <- length " + this->v->toString() + " " + this->dim->toString();
    }
    void Instruction_length::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_call::Instruction_call(Item* callee, std::vector<Item*> args) {
        this->callee = callee;
        this->args = args;
    }
    std::string Instruction_call::toString(void) {
        std::string outputString;
        outputString += this->callee->toString();
        outputString += "(";

        int n = this->args.size();
        for (int i = 0; i < n; i++) {
            outputString += this->args[i]->toString();
            if (i < n - 1) {
                outputString += ", ";
            }
        }

        outputString += ")";
        return outputString;
    }
    void Instruction_call::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_assignFromCall::Instruction_assignFromCall(Variable* dst, Item* callee, std::vector<Item*> args) {
        this->dst = dst;
        this->callee = callee;
        this->args = args;
    }
    std::string Instruction_assignFromCall::toString(void) {
        std::string outputString;
        outputString += this->dst->toString() + " <- ";
        outputString += this->callee->toString();
        outputString += "(";

        int n = this->args.size();
        for (int i = 0; i < n; i++) {
            outputString += this->args[i]->toString();
            if (i < n - 1) {
                outputString += ", ";
            }
        }

        outputString += ")";
        return outputString;
    }
    void Instruction_assignFromCall::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_newArray::Instruction_newArray(Variable* dst, std::vector<Item*> dims) {
        this->dst = dst;
        this->dims = dims;
    }
    std::string Instruction_newArray::toString(void) {
        std::string outputString;
        outputString += this->dst->toString() + " <- new Array(";
        int n = this->dims.size();
        for (int i = 0; i < n; i++) {
            outputString += dims[i]->toString();
            if (i < n - 1) {
                outputString += ", ";
            }
        }
        outputString += ")";
        return outputString;
    }
    void Instruction_newArray::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_newTuple::Instruction_newTuple(Variable* dst, Item* size) {
        this->dst = dst;
        this->size = size;
    }
    std::string Instruction_newTuple::toString(void) {
        return this->dst->toString() + " <- new Tuple(" + this->size->toString() + ")";
    }
    void Instruction_newTuple::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_print::Instruction_print(Item* t) {
        this->t = t;
    }
    std::string Instruction_print::toString() {
        return "print(" + this->t->toString() + ")";
    }
    void Instruction_print::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_input::Instruction_input(Variable* dst) {
        this->dst = dst;
    }
    std::string Instruction_input::toString() {
        return this->dst->toString() + " <- input()";
    }
    void Instruction_input::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    std::string Function::toString() {
        std::string output = " ";
        output += this->returnType->toString() + " ";
        output += this->name + "(";
        int n = this->args.size();
        for (int i = 0; i < n; i++) {
            output += this->types[i]->toString() + " " + this->args[i]->toString();
            if (i < n - 1) {
                output += ", ";
            }
        }
        output += ") {\n";
        for (auto inst : this->instructions) {
            output += inst->toString() + "\n";
        }
        output += "}";
        return output;
    }
    Variable* Function::newVariable(std::string name, bool isCallee) {
        if (this->variables.find(name) != this->variables.end()) {
            return this->variables[name];
        }
        else {
            Variable* v = new Variable(name);
            if (!isCallee) {
                this->variables.insert(std::make_pair(name, v));
            }
            return v;
        }
    }
    Variable* Function::getVariable(std::string name) {
        return NULL;
    }

    std::string Program::toString() {
        std::string outputString;
        for (auto f : this->functions) {
            outputString += f->toString() +"\n";
        }
        return outputString;
    }
}