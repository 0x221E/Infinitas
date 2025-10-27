#include "repl.h"
#include "parser/nodes/base_node.h"
#include "virtual-machine/virtual_machine.h"
#include <expected>

ReadEvalPrintLoop::ReadEvalPrintLoop()  : m_LastWorkingState()
{
    m_Context.m_VM.Register();
}

void ReadEvalPrintLoop::Run()
{
    PrintBanner();
    Loop();
}

void ReadEvalPrintLoop::PrintBanner()
{
    std::cout << "============================================================" << std::endl;
    std::cout << "Infinitas Programming Language Copyright 2025." << std::endl;
    std::cout << "Visit https://docs.0xinfinity.dev for documentation." << std::endl;
    std::cout << "This project is open-source and community governed." << std::endl;
    std::cout << "Licensed under Apache 2." << std::endl;
    std::cout << "============================================================" << std::endl;
}

void ReadEvalPrintLoop::Loop()
{
    while (m_Running)
    {
        std::string input = GetUserInput();

        if (!m_Running)
        {
            break;
        }

        CompileAndRun(input);
   }
}

std::string ReadEvalPrintLoop::GetUserInput()
{
    std::cout << ">>> ";
    std::string input;

    if (!std::getline(std::cin, input))
    {
        /** @todo Handle unexpected cases. */
        m_Running = false;
    }

    return input;
}

/** @todo Clean this pipeline more, specifically the interpreter pipeline. */
void ReadEvalPrintLoop::CompileAndRun(std::string sourceCode)
{
    auto tokens = m_Context.m_Lexer.FetchAllTokens(sourceCode)
    
    .and_then([this](std::vector<shared::Token> tokens) 
    {
        return m_Context.m_Parser.Parse(tokens);
    })
    .and_then([this](std::vector<parser::ASTNodePtr> nodes)
    {  
        return m_Context.m_Compiler.Compile(std::move(nodes));
    })
    .and_then([this](compiler::CompilerResult result) 
    {
        shared::ConstantPool copy(result.m_Constants);
        m_Context.m_Compiler.PassConstants(std::move(copy)); /** @todo This is a first sketch, implement a better system to handle constantpool in between REPL runs. */
        vm::VMInterpretProperties interpretProps{std::move(result.m_Instructions), result.m_Constants.MoveConstants()};
        return m_Context.m_VM.Interpret(interpretProps);
    })
    .and_then([this]() -> std::expected<void, bool>
    {
        m_LastWorkingState = m_Context.m_VM.TakeSnapshot();
        return {};
    })
    .or_else([this](bool) -> std::expected<void, bool>
    {
        m_Context.m_VM.Recover(m_LastWorkingState); 
        m_Context.m_Compiler.ResetErrors();
        return {};
    });
}
