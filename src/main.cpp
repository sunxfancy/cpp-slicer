// Declares clang::SyntaxOnlyAction.
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
// Declares llvm::cl::extrahelp.
#include "llvm/Support/CommandLine.h"
#include "pdgBuilderAction.h"

#include <iostream>
#include <sstream>

using namespace clang::tooling;
using namespace llvm;

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory Slicer("slicer options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);


static cl::opt<std::string> FuncName("function-name", cl::desc(R"(The name of the function to slice.)"),
                                     cl::init("main"), cl::cat(Slicer));
static cl::opt<int>         LineNo("line", cl::desc(R"(The line number of the statement to slice.)"), cl::init(0),
                                   cl::cat(Slicer));

static cl::opt<int> ColNo("column", cl::desc(R"(The column number of the statement to slice.)"), cl::init(0),
                          cl::cat(Slicer));

static cl::opt<bool> DumpDot("dump-dot", cl::desc(R"(Specifies whether tool should dump dot.)"), cl::init(false),
                             cl::cat(Slicer));

static cl::opt<bool> Server("server", cl::desc(R"(Specifies whether tool should run as a server.)"), cl::init(false),
                            cl::cat(Slicer));

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...\n");

void run_server(ClangTool& Tool) {
    while (true) {
        std::string buffer;
        std::getline(std::cin, buffer);
        if (buffer == "exit") {
            break;
        }

        std::stringstream X(buffer);
        std::string       command;
        X >> command;

        if (command == "slice") {
            std::string funcName;
            int         lineNo, colNo;
            X >> funcName >> lineNo >> colNo;
            
            std::cout << "Slicing " << funcName << " at " << lineNo << ":" << colNo << std::endl;
            const auto Factory = std::make_unique<clang::slicer::PDGBuilderActionFactory>(FuncName, LineNo, ColNo, DumpDot);
            Tool.run(Factory.get());
        } else {
            std::cout << "Unknown command: " << command << std::endl;
        }
    }
}


int main(int argc, const char **argv) {
    std::cout << "Clang Tool Starting..." << std::endl;
    auto      OptionsParser = CommonOptionsParser::create(argc, argv, Slicer);
    ClangTool Tool(OptionsParser->getCompilations(), OptionsParser->getSourcePathList());
    Tool.setPrintErrorMessage(true);
    int res = Tool.run(newFrontendActionFactory<clang::SyntaxOnlyAction>().get());
    if (!res) std::cout << "Syntax OK" << std::endl;

    if (Server) {
        run_server(Tool);
    } else {
        const auto Factory = std::make_unique<clang::slicer::PDGBuilderActionFactory>(FuncName, LineNo, ColNo, DumpDot);
        return Tool.run(Factory.get());
    }
}