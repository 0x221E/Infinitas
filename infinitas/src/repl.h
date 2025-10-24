#pragma once

#include "workflow_context.h"
#include <virtual-machine/virtual_machine.h>
#include <compiler/compiler.h>
#include <parser/parser.h>
#include <tokenizer/tokenizer.h>

class ReadEvalPrintLoop
{
public:
    /** Constructor/Deconstructor */
    ReadEvalPrintLoop();

public:
    /** Public Functions */
    void Run();

private:
    /** Private Functions and Internal Implementations */
    void PrintBanner();
    void Loop();
    std::string GetUserInput();
    void CompileAndRun(std::string sourceCode);
    void PostInterpretation();

private:
    /** Temporary/State Variables */
    bool m_Running = true;

private:
    /** Workflow */
    WorkflowContext m_Context;
}; 
