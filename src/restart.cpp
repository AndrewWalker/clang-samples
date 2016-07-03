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
  Handler() 
  {}

  void run(const MatchFinder::MatchResult &Result) override {
    if(const CXXRecordDecl* klass = Result.Nodes.getNodeAs<CXXRecordDecl>("klass"))
    {
      for(auto it : klass->attrs())
      {
        Attr& attr = (*it);
        auto annotatedAttr = dyn_cast<AnnotateAttr>(&attr);
        if((annotatedAttr == nullptr) || (std::string(annotatedAttr->getAnnotation()) != "meh"))
        {
            return;
        }
      }
    }
    if(const CXXMethodDecl* mthd = Result.Nodes.getNodeAs<CXXMethodDecl>("mthd"))
    {
      std::cout << mthd->getNameInfo().getAsString() << std::endl;
    }
  }
};

int main(int argc, const char **argv) {
  CommonOptionsParser op(argc, argv, ToolingSampleCategory);
  RefactoringTool Tool(op.getCompilations(), op.getSourcePathList());
  Handler handler;
  MatchFinder finder;
  finder.addMatcher(
          cxxRecordDecl(
              hasAttr(attr::Annotate),
              forEach(
                  cxxMethodDecl(
                      isPublic(),
                      unless(cxxConstructorDecl()),
                      unless(cxxDestructorDecl())).bind("mthd")
              )).bind("klass"), &handler);
 
  Tool.run(newFrontendActionFactory(&finder).get());
  return 0;
}
