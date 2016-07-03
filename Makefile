
LLVM_BIN_PATH   := /usr/local/bin
#CXX := g++
CXX := clang++
CXXFLAGS := -fno-rtti -O0 -g
PLUGIN_CXXFLAGS := -fpic

LLVM_CXXFLAGS := `$(LLVM_BIN_PATH)/llvm-config --cxxflags`
LLVM_LDFLAGS := `$(LLVM_BIN_PATH)/llvm-config --ldflags --libs --system-libs`

# Plugins shouldn't link LLVM and Clang libs statically, because they are
# already linked into the main executable (opt or clang). LLVM doesn't like its
# libs to be linked more than once because it uses globals for configuration
# and plugin registration, and these trample over each other.
LLVM_LDFLAGS_NOLIBS := `$(LLVM_BIN_PATH)/llvm-config --ldflags`
PLUGIN_LDFLAGS := -shared

CLANG_LIBS := \
    -Wl,--start-group \
    -lclangAST \
    -lclangAnalysis \
    -lclangBasic \
    -lclangDriver \
    -lclangEdit \
    -lclangFormat \
    -lclangFrontend \
    -lclangFrontendTool \
    -lclangLex \
    -lclangParse \
    -lclangSema \
    -lclangEdit \
    -lclangASTMatchers \
    -lclangRewrite \
    -lclangRewriteFrontend \
    -lclangStaticAnalyzerFrontend \
    -lclangStaticAnalyzerCheckers \
    -lclangStaticAnalyzerCore \
    -lclangSerialization \
    -lclangToolingCore \
    -lclangTooling \
    -Wl,--end-group

# Internal paths in this project: where to find sources, and where to put
# build artifacts.
SRC_DIR := src
BUILDDIR := build

.PHONY: all
all: make_builddir \
    emit_build_config \
    $(BUILDDIR)/minimal \
    $(BUILDDIR)/restart \
    $(BUILDDIR)/dynamic \

.PHONY: emit_build_config
emit_build_config: make_builddir
	@echo $(LLVM_BIN_PATH) > $(BUILDDIR)/_build_config

.PHONY: make_builddir
make_builddir:
	@test -d $(BUILDDIR) || mkdir $(BUILDDIR)

$(BUILDDIR)/minimal: $(SRC_DIR)/minimal.cpp
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) $(CLANG_INCLUDES) $^ $(CLANG_LIBS) $(LLVM_LDFLAGS) -o $@

$(BUILDDIR)/dynamic: $(SRC_DIR)/dynamic.cpp
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) $(CLANG_INCLUDES) $^ $(CLANG_LIBS) $(LLVM_LDFLAGS) -o $@

$(BUILDDIR)/restart: $(SRC_DIR)/restart.cpp
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) $(CLANG_INCLUDES) $^ $(CLANG_LIBS) $(LLVM_LDFLAGS) -o $@

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)/* 
