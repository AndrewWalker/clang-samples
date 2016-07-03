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
using namespace clang::ast_matchers::internal;
using namespace clang::driver;
using namespace clang::tooling;

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

  auto m1 = DynTypedMatcher(Matcher<clang::BinaryOperator>(hasOperatorName("==")));
  auto m2 = DynTypedMatcher(Matcher<clang::UnaryOperator>(hasOperatorName("==")));

  Tool.run(newFrontendActionFactory(&finder).get());
  return 0;
}
