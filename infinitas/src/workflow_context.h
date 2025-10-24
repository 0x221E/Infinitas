#pragma once

#include <virtual-machine/virtual_machine.h>
#include <compiler/compiler.h>
#include <parser/parser.h>
#include <tokenizer/tokenizer.h>

struct WorkflowContext
{
    lexer::Tokenizer m_Lexer;
    parser::Parser m_Parser;
    compiler::Compiler m_Compiler;
    vm::VirtualMachine m_VM;
};
