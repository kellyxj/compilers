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
        return std::to_string(this->number)
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

	Variable::Variable(std::string name) {
        this->name = name;
	}
    std::string Variable::toString(void) {
        return this->name;
    }
    Variable::get(void) {
        return this->name;
    }

    Instruction_declare::Instruction_declare(Variable* v, DataType t) {
        this->var = v;
        this->type = t;
    }
    DataType Instruction_declare::getType() {
        return this->type;
    }
    Variable* Instruction_declare::getVar() {
        return this->var;
    }
    std::string Instruction_declare::toString(void) {
        std::map<DataType, std::string> typeNames;
        typeNames.insert(std::make_pair(int64, "int64"));
        typeNames.insert(std::make_pair(tensor, "tensor"));
        typeNames.insert(std::make_pair(tuple, "tuple"));
        typeNames.insert(std::make_pair(code, "code"));
        return this->var->toString() + " " + typeNames[this->type];
    }

	Instruction_assignment::Instruction_assignment(Item* s, Item* d) {
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

    Instruction_compare::Instruction_compare(Variable* d, Item* l, LogicOperator c, Item* r) {
        this->dst = d;
        this->left = l;
        this->cmp = c;
        this->right = r;
    }
    std::string Instruction_compare::toString(void) {
        std::map<LogicOperator, std::string> cmpStrings;
        cmpStrings.insert(std::make_pair(cmp_lt, "<"));
        cmpStrings.insert(std::make_pair(cmp_le, "<="));
        cmpStrings.insert(std::make_pair(cmp_eq, "="));
        cmpStrings.insert(std::make_pair(cmp_ge, ">="));
        cmpStrings.insert(std::make_pair(cmp_gt, ">"));

        return this->dst->toString() + " <- " + this->left->toString() + cmpStrings[this->cmp] + this->right->toString();
    }
    void Instruction_compare::accept(Visitor* visitor) {
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
        return "goto " + this->l->toString();
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

    Instruction_return::Instruction_return() {

    }
    Instruction_return::Instruction_return(Item* v) {
        this->src = v;
    }
    std::string Instruction_return::toString(void) {
        if (this->src == NULL) {
            return "return";
        }
        else {
            return "return " + this->src->toString();
        }
    }
    void Instruction_return::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_assignToTensor::Instruction_assignToTensor(Item* dst, std::vector<Item*> args, Item* src) {
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

    Instruction_assignFromTensor::Instruction_assignFromTensor(Item* dst, std::vector<Item*> args, Item* src) {
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

    Instruction_length::Instruction_length(Item* dst, Variable* v, int64_t dim) {
        this->dst = dst;
        this->v = v;
        this->dim = dim;
    }
    std::string Instruction_length::toString(void) {
        return this->dst->toString() + " <- length " + this->v->toString() + " " + std::to_string(this->dim);
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

    std::string Function::toString() {
        std::map<DataType, std::string> typeNames;
        typeNames.insert(std::make_pair(type_void, "void"));
        typeNames.insert(std::make_pair(int64, "int64"));
        typeNames.insert(std::make_pair(tensor, "tensor"));
        typeNames.insert(std::make_pair(tuple, "tuple"));
        typeNames.insert(std::make_pair(code, "code"));
        std::string output = " ";
        output += typeNames[this->returnType]+" ";
        output += this->name + "(";
        int n = this->args.size();
        for (int i = 0; i < n; i++) {
            outputString += this->args[i]->toString();
            if (i < n - 1) {
                outputString += ", ";
            }
        }
        output += ") {";
        for (auto inst : this->instructions) {
            output += inst->toString() + "\n";
        }
        output += "}";
        return output;
    }
    Variable* Function::newVariable(DataType t, std::string name, int64_t rank = 0) {
        return NULL;
    }
    Variable* Function::getVariable(std::string name) {
        return NULL;
    }
}