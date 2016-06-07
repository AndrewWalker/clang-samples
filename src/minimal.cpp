#include <string>
#include <iostream>
#include "clang/AST/AST.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Refactoring.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::driver;
using namespace clang::tooling;
namespace clami = clang::ast_matchers::internal;

static llvm::cl::OptionCategory ToolingSampleCategory("Matcher Sample");

class Handler : public MatchFinder::MatchCallback {
public:
  Handler() {}

  void run(const MatchFinder::MatchResult &Result) override {
    std::cout << "handler" << std::endl;
  }
};

int main(int argc, const char **argv) {
  CommonOptionsParser op(argc, argv, ToolingSampleCategory);
  RefactoringTool Tool(op.getCompilations(), op.getSourcePathList());
  Handler handler;
  MatchFinder finder;
  //Finder.addMatcher(cxxRecordDecl().bind("klass"), &handler);
 
  clami::DynTypedMatcher m = cxxRecordDecl();
  clami::DynTypedMatcher m2 = m.tryBind("foo").getValue(); 
  finder.addDynamicMatcher(m2, &handler);
  
  Tool.run(newFrontendActionFactory(&finder).get());
  return 0;
}
