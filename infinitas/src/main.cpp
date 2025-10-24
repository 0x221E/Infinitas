#include "main.h"

int main()
{
    GlobalEnvironment::Initialize();
    
    ReadEvalPrintLoop repl;
    repl.Run();    
}
