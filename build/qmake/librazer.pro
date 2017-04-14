#-------------------------------------------------
#
# Project created by QtCreator 2016-10-03T09:33:51
#
#-------------------------------------------------

QT       -= core gui

TARGET = librazer
TEMPLATE = lib
CONFIG += staticlib
DESTDIR = $$PWD/../../lib

DEFINES += LOGGING_ENABLED

INCLUDEPATH += ../../src \
                ../../include

HEADERS += \
    ../../include/Rzr/RzDocument.h \
    ../../include/Rzr/RzEngine.h \
    ../../include/Rzr/RzModule.h \
    ../../include/Rzr/RzValue.h \
    ../../include/RzrAPI/RzBuilder.h \
    ../../src/RazerCompiler/aeCompilerConv.h \
    ../../src/RazerCompiler/aeCompilerErrorIds.h \
    ../../src/RazerCompiler/aeCompilerWarningIds.h \
    ../../src/RazerCompiler/CompileResult.h \
    ../../src/RazerCore/container/Array.h \
    ../../src/RazerCore/io/File.h \
    ../../src/RazerCore/io/FileInfo.h \
    ../../src/RazerCore/media/Device.h \
    ../../src/RazerCore/net/Socket.h \
    ../../src/RazerCore/window/Window.h \
    ../../src/RazerCore/ExportStd.h \
    ../../src/RazerParser/AST/AEBaseNode.h \
    ../../src/RazerParser/AST/aeNodeAccessOperator.h \
    ../../src/RazerParser/AST/aeNodeBinaryOperator.h \
    ../../src/RazerParser/AST/aeNodeBlock.h \
    ../../src/RazerParser/AST/aeNodeBranch.h \
    ../../src/RazerParser/AST/aeNodeEcosystem.h \
    ../../src/RazerParser/AST/aeNodeExpr.h \
    ../../src/RazerParser/AST/aeNodeFor.h \
    ../../src/RazerParser/AST/aeNodeFunction.h \
    ../../src/RazerParser/AST/aeNodeFunctionCall.h \
    ../../src/RazerParser/AST/aeNodeIdentifier.h \
    ../../src/RazerParser/AST/aeNodeLiterals.h \
    ../../src/RazerParser/AST/aeNodeNamespace.h \
    ../../src/RazerParser/AST/aeNodeNew.h \
    ../../src/RazerParser/AST/aeNodeReturn.h \
    ../../src/RazerParser/AST/aeNodeSubscript.h \
    ../../src/RazerParser/AST/aeNodeTernaryOperator.h \
    ../../src/RazerParser/AST/aeNodeUnaryOperator.h \
    ../../src/RazerParser/AST/aeNodeValue.h \
    ../../src/RazerParser/AST/aeNodeVarDecl.h \
    ../../src/RazerParser/AST/aeNodeWhile.h \
    ../../src/RazerParser/AST/AEStructNode.h \
    ../../src/RazerParser/AST/Nodes.h \
    ../../src/RazerParser/AST/RzSourceUnit.h \
    ../../src/RazerParser/Parser/RzParser.h \
    ../../src/RazerParser/TypeDesc.h \
    ../../src/RazerRuntime/aeBindingHelper.h \
    ../../src/RazerRuntime/aeClosure.h \
    ../../src/RazerRuntime/AEFunction.h \
    ../../src/RazerRuntime/AEGeneric.h \
    ../../src/RazerRuntime/AEObject.h \
    ../../src/RazerRuntime/aeQualType.h \
    ../../src/RazerRuntime/AEString.h \
    ../../src/RazerRuntime/aeSymbol.h \
    ../../src/RazerRuntime/AEValueList.h \
    ../../src/RazerRuntime/RzType.h \
    ../../src/RazerVM/Executor/ExecDispatch.h \
    ../../src/RazerVM/Executor/VmInstructions.h \
    ../../src/RazerVM/InstructionSet.h \
    ../../src/RazerVM/StackFrame.h \
    ../../src/RazerVM/StackValue.h \
    ../../src/RazerVM/ThreadContext.h \
    ../../src/RazerVM/ThreadHandler.h \
    ../../src/RazerVM/VirtualMachine.h \
    ../../src/DebugDefs.h \
    ../../src/Logger.h \
    ../../src/RazerCore/window/SDLWindowImpl.h \
    ../../src/RazerVM/Debugger/Debugger.h \
    ../../src/RazerVM/Debugger/IDebugger.h \
    ../../src/RazerVM/InstructionIterator.h \
    ../../src/STS/CPP/Compiler/CppBuilder.h \
    ../../src/Base/FileUtils.h \
    ../../src/RazerCompiler/Errors.h \
    ../../src/RazerCompiler/SymbolTypename.h \
    ../../src/RazerCompiler/RzCompiler.h \
    ../../src/RazerVM/Executor/vm_dynamic.h \
    ../../src/RazerVM/Executor/vm_calls.h \
    ../../src/RazerCore/container/String.h \
    ../../src/RazerVM/ExecDispatch.h \
    ../../src/RazerCore/time/Timer.h \
    ../../src/Base/StringUtils.h \
    ../../src/RazerParser/Parser/Token.h \
    ../../src/RazerParser/Parser/TokenParser.h \
    ../../src/RazerCompiler/BuildReport.h

SOURCES += \
    ../../include/RzrAPI/RzBuilder.cpp \
    ../../src/RazerCompiler/aeCompilerConv.cpp \
    ../../src/RazerCompiler/AECompilerData.cpp \
    ../../src/RazerCompiler/CompileAllocation.cpp \
    ../../src/RazerCompiler/CompileAssignment.cpp \
    ../../src/RazerCompiler/CompileCalls.cpp \
    ../../src/RazerCompiler/CompileExpressions.cpp \
    ../../src/RazerCompiler/CompileFunctions.cpp \
    ../../src/RazerCompiler/CompileResult.cpp \
    ../../src/RazerCompiler/RzCompilerCollect.cpp \
    ../../src/RazerCore/container/Array.cpp \
    ../../src/RazerCore/io/File.cpp \
    ../../src/RazerCore/io/FileInfo.cpp \
    ../../src/RazerCore/media/Device.cpp \
    ../../src/RazerCore/net/Socket.cpp \
    ../../src/RazerCore/window/Window.cpp \
    ../../src/RazerCore/ExportStd.cpp \
    ../../src/RazerParser/AST/AEBaseNode.cpp \
    ../../src/RazerParser/AST/aeNodeAccessOperator.cpp \
    ../../src/RazerParser/AST/aeNodeBinaryOperator.cpp \
    ../../src/RazerParser/AST/aeNodeBlock.cpp \
    ../../src/RazerParser/AST/aeNodeBranch.cpp \
    ../../src/RazerParser/AST/aeNodeEcosystem.cpp \
    ../../src/RazerParser/AST/aeNodeExpr.cpp \
    ../../src/RazerParser/AST/aeNodeFor.cpp \
    ../../src/RazerParser/AST/aeNodeFunction.cpp \
    ../../src/RazerParser/AST/aeNodeFunctionCall.cpp \
    ../../src/RazerParser/AST/aeNodeIdentifier.cpp \
    ../../src/RazerParser/AST/aeNodeLiterals.cpp \
    ../../src/RazerParser/AST/aeNodeNamespace.cpp \
    ../../src/RazerParser/AST/aeNodeNew.cpp \
    ../../src/RazerParser/AST/aeNodeReturn.cpp \
    ../../src/RazerParser/AST/aeNodeSubscript.cpp \
    ../../src/RazerParser/AST/aeNodeTernaryOperator.cpp \
    ../../src/RazerParser/AST/aeNodeUnaryOperator.cpp \
    ../../src/RazerParser/AST/aeNodeValue.cpp \
    ../../src/RazerParser/AST/aeNodeVarDecl.cpp \
    ../../src/RazerParser/AST/aeNodeWhile.cpp \
    ../../src/RazerParser/AST/AEStructNode.cpp \
    ../../src/RazerParser/AST/RzSourceUnit.cpp \
    ../../src/RazerParser/Parser/RzDocument.cpp \
    ../../src/RazerParser/Parser/RzParser.cpp \
    ../../src/RazerParser/Parser/RzParserData.cpp \
    ../../src/RazerParser/TypeDesc.cpp \
    ../../src/RazerRuntime/aeBindingHelper.cpp \
    ../../src/RazerRuntime/aeClosure.cpp \
    ../../src/RazerRuntime/AEFunction.cpp \
    ../../src/RazerRuntime/AEGeneric.cpp \
    ../../src/RazerRuntime/AEObject.cpp \
    ../../src/RazerRuntime/aeQualType.cpp \
    ../../src/RazerRuntime/AEString.cpp \
    ../../src/RazerRuntime/aeSymbol.cpp \
    ../../src/RazerRuntime/AEValueList.cpp \
    ../../src/RazerRuntime/RzEngine.cpp \
    ../../src/RazerRuntime/RzModule.cpp \
    ../../src/RazerRuntime/RzType.cpp \
    ../../src/RazerRuntime/RzValue.cpp \
    ../../src/RazerVM/InstructionSet.cpp \
    ../../src/RazerVM/StackFrame.cpp \
    ../../src/RazerVM/StackValue.cpp \
    ../../src/RazerVM/ThreadContext.cpp \
    ../../src/RazerVM/ThreadHandler.cpp \
    ../../src/RazerVM/VirtualMachine.cpp \
    ../../src/Logger.cpp \
    ../../src/RazerCore/window/SDLWindowImpl.cpp \
    ../../src/RazerVM/Debugger/Debugger.cpp \
    ../../src/RazerVM/Debugger/IDebugger.cpp \
    ../../src/RazerVM/InstructionIterator.cpp \
    ../../src/STS/CPP/Compiler/CppBuilder.cpp \
    ../../src/Base/FileUtils.cpp \
    ../../src/RazerCompiler/Errors.cpp \
    ../../src/RazerCompiler/SymbolTypename.cpp \
    ../../src/RazerCompiler/RzCompiler.cpp \
    ../../src/RazerCore/container/String.cpp \
    ../../src/RazerVM/execute/vm_arithmetic.cpp \
    ../../src/RazerVM/execute/vm_calls.cpp \
    ../../src/RazerVM/execute/vm_debug.cpp \
    ../../src/RazerVM/execute/vm_dispatch.cpp \
    ../../src/RazerVM/execute/vm_dynamic.cpp \
    ../../src/RazerVM/execute/vm_general.cpp \
    ../../src/RazerVM/execute/vm_operators.cpp \
    ../../src/RazerVM/execute/vm_threading.cpp \
    ../../src/RazerCore/time/Timer.cpp \
    ../../src/Base/StringUtils.cpp \
    ../../src/RazerParser/Parser/Token.cpp \
    ../../src/RazerParser/Parser/TokenParser.cpp \
    ../../src/RazerCompiler/BuildReport.cpp
