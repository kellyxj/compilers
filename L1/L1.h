#pragma once

#include <vector>
#include <string>

namespace L1 {
    class Visitor;

    enum Register_ID { rdi, rsi, rdx, rcx, r8, r9, rax, rbx, rbp, r10, r11, r12, r13, r14, r15, rsp };
    enum Operator { op_plus, op_minus, op_mult, op_and, op_lshift, op_rshift };
    enum LogicOperator { cmp_lt, cmp_le, cmp_eq, cmp_ge, cmp_gt };
}