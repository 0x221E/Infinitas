#pragma once

#include "workflow_context.h"
#include <virtual-machine/virtual_machine.h>
#include <compiler/compiler.h>
#include <parser/parser.h>
#include <tokenizer/tokenizer.h>

class ReadEvalPrintLoop
{
public:
    ReadEvalPrintLoop();

public:
    void Run();

private:
    void PrintBanner();
    void Loop();
    std::string GetUserInput();
    void CompileAndRun(std::string sourceCode);
    void PostInterpretation();

private:
    bool m_Running = true;

private:
    WorkflowContext m_Context;

    std::unique_ptr<vm::VMSnapshot> m_LastWorkingState;
}; 
