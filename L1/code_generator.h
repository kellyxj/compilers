#pragma once

#include <L1.h>

namespace L1 {

    class codeGenerator : public Visitor {
    public:
        codeGenerator(Function* f, std::ofstream& outputFile);
        void visit(Instruction_assignment* inst) override;
        void visit(Instruction_aop* inst) override;
        void visit(Instruction_lea* inst) override;
        void visit(Instruction_label* inst) override;
        void visit(Instruction_goto* inst) override;
        void visit(Instruction_cjump* inst) override;
        void visit(Instruction_ret* inst) override;
        void visit(Instruction_print* inst) override;
        void visit(Instruction_allocate* inst) override;
        void visit(Instruction_input* inst) override;
        void visit(Instruction_tensor_error* inst) override;
        void visit(Instruction_call* inst) override;

    private:
        Function* f;
        std::ofstream& outputFile;
    };

    void generate_code(Program p);

}
