cmake_minimum_required(VERSION 3.22)
project(cpp-slicer)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fuse-ld=lld")

include_directories(${LLVM_INCLUDE_DIRS})
if(LLVM_BUILD_MAIN_SRC_DIR)
  include_directories(${LLVM_BUILD_MAIN_SRC_DIR}/tools/clang/include)
  include_directories(${LLVM_BUILD_BINARY_DIR}/tools/clang/include)
endif()

link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_definitions(
-D__STDC_LIMIT_MACROS
-D__STDC_CONSTANT_MACROS
)

add_executable(${PROJECT_NAME} src/main.cpp src/pdg.cpp src/pdgBuilder.cpp src/pdgBuilderAction.cpp)

# llvm_map_components_to_libnames(llvm_libs ${COMPONENT_LIST})
# target_link_libraries(${PROJECT_NAME} ${llvm_libs})
target_link_libraries(${PROJECT_NAME}
    clangAST
    clangASTMatchers
    clangBasic
    clangDriver
    clangFrontend
    clangRewriteFrontend
    clangSerialization
    clangStaticAnalyzerFrontend
    clangTooling
    clangToolingSyntax
    clangFrontendTool
    clangEdit
    clangAPINotes
    clangLex
    clangSema
    clangSupport
    clangParse
    clangAnalysis
    LLVMX86AsmParser # MC, MCParser, Support, X86Desc, X86Info
    LLVMX86Desc # MC, Support, X86AsmPrinter, X86Info
    LLVMX86Disassembler # MC, Support, X86Utils
    LLVMX86Info # MC, Support, Target
    LLVMX86CodeGen # Core, Support
    LLVMX86TargetMCA # Core, Support
    LLVMipo
    LLVMScalarOpts
    LLVMInstCombine
    LLVMTransformUtils
    LLVMAnalysis
    LLVMTarget
    LLVMOption # Support
    LLVMMCParser # MC, Support
    LLVMMC # Object, Support
    LLVMObject # BitReader, Core, Support
    LLVMBitReader # Core, Support
    LLVMCore # Support
    LLVMSupport
    LLVMWindowsDriver
)

# message(STATUS "Linked librarys = ${llvm_libs}")