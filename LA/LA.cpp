#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <set>

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

    //Tensor

    //Tuple

    //Code

	Operation::Operation(Operator op) {
        this->op = op;
	}
    std::string Operation::toString(void) {

    }
    Operation::get(void) {
        return this->op;
    }

	CompareOp::CompareOp(LogicOperator cmp) {
        this->cmp = cmp;
	}
    std::string CompareOp::toString(void) {

    }
    CompareOp::get(void) {
        return this->cmp;
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

    }
    void Instruction_assignment::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_compare::Instruction_compare() {

    }
    std::string Instruction_compare::toString(void) {

    }
    void Instruction_compare::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_label::Instruction_label() {

    }
    std::string Instruction_label::toString(void) {

    }
    void Instruction_label::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_goto::Instruction_goto() {

    }
    std::string Instruction_goto::toString(void) {

    }
    void Instruction_goto::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_cjump::Instruction_cjump() {

    }
    std::string Instruction_cjump::toString(void) {

    }
    void Instruction_cjump::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_return::Instruction_return() {

    }
    std::string Instruction_return::toString(void) {

    }
    void Instruction_return::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_assignToTensor::Instruction_assignToTensor() {

    }
    std::string Instruction_assignToTensor::toString(void) {

    }
    void Instruction_assignToTensor::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_assignFromTensor::Instruction_assignFromTensor() {

    }
    std::string Instruction_assignFromTensor::toString(void) {

    }
    void Instruction_assignFromTensor::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_length::Instruction_length() {

    }
    std::string Instruction_length::toString(void) {

    }
    void Instruction_length::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_call::Instruction_call() {

    }
    std::string Instruction_call::toString(void) {

    }
    void Instruction_call::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_assignFromCall::Instruction_assignFromCall() {

    }
    std::string Instruction_assignFromCall::toString(void) {

    }
    void Instruction_assignFromCall::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_newArray::Instruction_newArray() {

    }
    std::string Instruction_newArray::toString(void) {

    }
    void Instruction_newArray::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_newTuple::Instruction_newTuple() {

    }
    std::string Instruction_newTuple::toString(void) {

    }
    void Instruction_newTuple::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_input::Instruction_input() {

    }
    std::string Instruction_input::toString(void) {

    }
    void Instruction_input::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    Instruction_print::Instruction_print() {

    }
    std::string Instruction_print::toString(void) {

    }
    void Instruction_print::accept(Visitor* visitor) {
        visitor->visit(this);
    }

    std::string Function::toString() {

    }
    void Function::computeCFG() {

    }
    void Function::computeUseDefs() {

    }
    Variable* Function::newVariable() {

    }
    Tensor* Function::newTensor() {

    }
    Tuple* Function::newTuple() {

    }
    Code* Function::newCode() {

    }


    std::string Program::toString() {

    }
}