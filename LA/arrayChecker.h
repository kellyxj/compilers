#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include <LA.h>

namespace LA {
	class arrayChecker : public Visitor {
		public:
			arrayChecker(Function* f);
            Function* validate(void);
            void visit(Instruction_declare* inst) override;
            void visit(Instruction_assignment* inst) override;
            void visit(Instruction_op* inst) override;
            void visit(Instruction_label* inst) override;
            void visit(Instruction_goto* inst) override;
            void visit(Instruction_cjump* inst) override;
            void visit(Instruction_return* inst) override;
            void visit(Instruction_empty_return* inst) override;
            void visit(Instruction_assignToTensor* inst) override;
            void visit(Instruction_assignFromTensor* inst) override;
            void visit(Instruction_length* inst) override;
            void visit(Instruction_call* inst) override;
            void visit(Instruction_assignFromCall* inst) override;
            void visit(Instruction_newArray* inst) override;
            void visit(Instruction_newTuple* inst) override;
            void visit(Instruction_print* inst) override;
            void visit(Instruction_input* inst) override;
		private:
            Function* f;
            Function* newF;
            Variable* lineNumber;
            Variable* checker;
            Variable* lengthHolder;
	};
}