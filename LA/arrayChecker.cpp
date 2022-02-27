#include <string>
#include <iostream>
#include <fstream>

#include <arrayChecker.h>

using namespace std;

namespace LA {
	arrayChecker::arrayChecker(Function* f) {
		this->f = f;
        this->newF = new Function();
        this->newF->name = this->f->name;
        this->newF->returnType = this->f->returnType;
        this->newF->variables = this->f->variables;
        this->newF->labels = this->f->labels;

        this->lineNumber = this->newF->makeVariable();
        this->checker = this->newF->makeVariable();
        this->lengthHolder = this->newF->makeVariable();
	}

	Function* arrayChecker::validate() {
        auto type = new Type(int64);
        auto declareLineNumber = new Instruction_declare(this->lineNumber, type);
        this->newF->instructions.push_back(declareLineNumber);

        auto declareChecker = new Instruction_declare(this->checker, type);
        this->newF->instructions.push_back(declareChecker);

        auto declareLengthHolder = new Instruction_declare(this->lengthHolder, type);
        this->newF->instructions.push_back(declareLengthHolder);

        for (auto inst : this->f->instructions) {
            inst->accept(this);
        }
        return this->newF;
	}

    void arrayChecker::visit(Instruction_declare* inst) {
        this->newF->instructions.push_back(inst);

        auto type = inst->getType()->get();
        if (type == tensor || type == tuple) {
            auto src = new Number(0);
            auto dst = inst->getVar();
            auto newInst = new Instruction_assignment(src, dst);
            this->newF->instructions.push_back(newInst);
        }
    }
    void arrayChecker::visit(Instruction_assignment* inst) {
        this->newF->instructions.push_back(inst);
    }
    void arrayChecker::visit(Instruction_op* inst) {
        this->newF->instructions.push_back(inst);
    }
    void arrayChecker::visit(Instruction_label* inst) {
        this->newF->instructions.push_back(inst);
    }
    void arrayChecker::visit(Instruction_goto* inst) {
        this->newF->instructions.push_back(inst);
    }
    void arrayChecker::visit(Instruction_cjump* inst) {
        this->newF->instructions.push_back(inst);
    }
    void arrayChecker::visit(Instruction_return* inst) {
        this->newF->instructions.push_back(inst);
    }
    void arrayChecker::visit(Instruction_empty_return* inst) {
        this->newF->instructions.push_back(inst);
    }
    void arrayChecker::visit(Instruction_assignToTensor* inst) {
        //check allocation
        Number* lineNumber = new Number(inst->getLineNumber());
        auto getLineNumber = new Instruction_assignment(lineNumber, this->lineNumber);
        this->newF->instructions.push_back(getLineNumber);

        Number* zero = new Number(0);

        Variable* tensorName = inst->getDst();

        auto checkAlloc = new Instruction_op(this->checker, tensorName, cmp_eq, zero);
        this->newF->instructions.push_back(checkAlloc);

        Label* errorLabel = this->newF->makeLabel();
        Label* continueLabel = this->newF->makeLabel();

        auto conditional = new Instruction_cjump(this->checker, errorLabel, continueLabel);
        this->newF->instructions.push_back(conditional);

        auto errorBranch = new Instruction_label(errorLabel);
        this->newF->instructions.push_back(errorBranch);

        auto throwError = new Instruction_tensor_error(this->lineNumber);
        this->newF->instructions.push_back(throwError);

        auto continueBranch = new Instruction_label(continueLabel);
        this->newF->instructions.push_back(continueBranch);

        //check tensor access
        for (int i = 0; i < inst->getArgs().size(); i++) {
            Number* dim = new Number(i);

            auto loadLength = new Instruction_length(this->lengthHolder, tensorName, dim);
            this->newF->instructions.push_back(loadLength);

            auto compareLength = new Instruction_op(this->checker, this->lengthHolder, cmp_lt, inst->getArgs()[i]);
            this->newF->instructions.push_back(compareLength);

            Label* badAccessLabel = this->newF->makeLabel();
            Label* goodAccessLabel = this->newF->makeLabel();

            auto conditional2 = new Instruction_cjump(this->checker, badAccessLabel, goodAccessLabel);
            this->newF->instructions.push_back(conditional2);

            auto badAccessBranch = new Instruction_label(badAccessLabel);
            this->newF->instructions.push_back(badAccessBranch);

            if (inst->getArgs().size() == 1) {
                auto error = new Instruction_tensor_error3(this->lineNumber, this->lengthHolder, inst->getArgs()[0]);
                this->newF->instructions.push_back(error);
            }
            else {
                auto error = new Instruction_tensor_error4(this->lineNumber, dim, this->lengthHolder, inst->getArgs()[i]);
                this->newF->instructions.push_back(error);
            }

            auto goodAccessBranch = new Instruction_label(goodAccessLabel);
            this->newF->instructions.push_back(goodAccessBranch);
        }
        
        this->newF->instructions.push_back(inst);
    }
    void arrayChecker::visit(Instruction_assignFromTensor* inst) {
        //check allocation
        Number* lineNumber = new Number(inst->getLineNumber());
        auto getLineNumber = new Instruction_assignment(lineNumber, this->lineNumber);
        this->newF->instructions.push_back(getLineNumber);

        Number* zero = new Number(0);

        Variable* tensorName = inst->getSrc();

        auto checkAlloc = new Instruction_op(this->checker, tensorName, cmp_eq, zero);
        this->newF->instructions.push_back(checkAlloc);

        Label* errorLabel = this->newF->makeLabel();
        Label* continueLabel = this->newF->makeLabel();

        auto conditional = new Instruction_cjump(this->checker, errorLabel, continueLabel);
        this->newF->instructions.push_back(conditional);

        auto errorBranch = new Instruction_label(errorLabel);
        this->newF->instructions.push_back(errorBranch);

        auto throwError = new Instruction_tensor_error(this->lineNumber);
        this->newF->instructions.push_back(throwError);

        auto continueBranch = new Instruction_label(continueLabel);
        this->newF->instructions.push_back(continueBranch);

        //check tensor access
        for (int i = 0; i < inst->getArgs().size(); i++) {
            Number* dim = new Number(i);

            auto loadLength = new Instruction_length(this->lengthHolder, tensorName, dim);
            this->newF->instructions.push_back(loadLength);

            auto compareLength = new Instruction_op(this->checker, this->lengthHolder, cmp_lt, inst->getArgs()[i]);
            this->newF->instructions.push_back(compareLength);

            Label* badAccessLabel = this->newF->makeLabel();
            Label* goodAccessLabel = this->newF->makeLabel();

            auto conditional2 = new Instruction_cjump(this->checker, badAccessLabel, goodAccessLabel);
            this->newF->instructions.push_back(conditional2);

            auto badAccessBranch = new Instruction_label(badAccessLabel);
            this->newF->instructions.push_back(badAccessBranch);

            if (inst->getArgs().size() == 1) {
                auto error = new Instruction_tensor_error3(this->lineNumber, this->lengthHolder, inst->getArgs()[0]);
                this->newF->instructions.push_back(error);
            }
            else {
                auto error = new Instruction_tensor_error4(this->lineNumber, dim, this->lengthHolder, inst->getArgs()[i]);
                this->newF->instructions.push_back(error);
            }

            auto goodAccessBranch = new Instruction_label(goodAccessLabel);
            this->newF->instructions.push_back(goodAccessBranch);
        }

        this->newF->instructions.push_back(inst);
    }
    void arrayChecker::visit(Instruction_length* inst) {
        this->newF->instructions.push_back(inst);
    }
    void arrayChecker::visit(Instruction_call* inst) {
        this->newF->instructions.push_back(inst);
    }
    void arrayChecker::visit(Instruction_assignFromCall* inst) {
        this->newF->instructions.push_back(inst);
    }
    void arrayChecker::visit(Instruction_newArray* inst) {
        this->newF->instructions.push_back(inst);
    }
    void arrayChecker::visit(Instruction_newTuple* inst) {
        this->newF->instructions.push_back(inst);
    }
    void arrayChecker::visit(Instruction_print* inst) {
        this->newF->instructions.push_back(inst);
    }
    void arrayChecker::visit(Instruction_input* inst) {
        this->newF->instructions.push_back(inst);
    }
}