#-------------------------------------------------
#
# Project created by QtCreator 2016-10-03T09:33:51
#
#-------------------------------------------------

QT       -= core gui

TARGET = libaeon
TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH += ../../src \
                ../../include

SOURCES +=  ../../src/aeon/aeBindingHelper.cpp \
    ../../src/aeon/aeBuilder.cpp \
    ../../src/aeon/aeClosure.cpp \
    ../../src/aeon/aeCodeFile.cpp \
    ../../src/aeon/AEDocument.cpp \
    ../../src/aeon/aeDynamicType.cpp \
    ../../src/aeon/aeIR.cpp \
    ../../src/aeon/aeParser.cpp \
    ../../src/aeon/aeQualType.cpp \
    ../../src/aeon/aeReportManager.cpp \
    ../../src/aeon/aeSymbol.cpp \
    ../../src/aeon/aeTokenizer.cpp \
    ../../src/aeon/AST/AEBaseNode.cpp \
    ../../src/aeon/AST/aeNodeAccessOperator.cpp \
    ../../src/aeon/AST/aeNodeBinaryOperator.cpp \
    ../../src/aeon/AST/aeNodeBlock.cpp \
    ../../src/aeon/AST/aeNodeBranch.cpp \
    ../../src/aeon/AST/aeNodeEcosystem.cpp \
    ../../src/aeon/AST/aeNodeExpr.cpp \
    ../../src/aeon/AST/aeNodeFor.cpp \
    ../../src/aeon/AST/aeNodeFunction.cpp \
    ../../src/aeon/AST/aeNodeFunctionCall.cpp \
    ../../src/aeon/AST/aeNodeIdentifier.cpp \
    ../../src/aeon/AST/aeNodeLiterals.cpp \
    ../../src/aeon/AST/aeNodeModule.cpp \
    ../../src/aeon/AST/aeNodeNamespace.cpp \
    ../../src/aeon/AST/aeNodeNew.cpp \
    ../../src/aeon/AST/aeNodeReturn.cpp \
    ../../src/aeon/AST/aeNodeSubscript.cpp \
    ../../src/aeon/AST/aeNodeTernaryOperator.cpp \
    ../../src/aeon/AST/aeNodeUnaryOperator.cpp \
    ../../src/aeon/AST/aeNodeValue.cpp \
    ../../src/aeon/AST/aeNodeVarDecl.cpp \
    ../../src/aeon/AST/aeNodeWhile.cpp \
    ../../src/aeon/AST/AEStructNode.cpp \
    ../../src/aeon/Build/aeBuildMake.cpp \
    ../../src/aeon/Compiler/aeCompiler.cpp \
    ../../src/aeon/Compiler/aeCompilerConv.cpp \
    ../../src/aeon/Compiler/AECompilerData.cpp \
    ../../src/aeon/Compiler/CompileCalls.cpp \
    ../../src/aeon/Compiler/CompileExpressions.cpp \
    ../../src/aeon/Compiler/CompileFunctions.cpp \
    ../../src/aeon/Compiler/CompileStatements.cpp \
    ../../src/aeon/Parser/AEParserData.cpp \
    ../../src/aeon/Runtime/AEByteCode.cpp \
    ../../src/aeon/Runtime/AEContext.cpp \
    ../../src/aeon/Runtime/AEFunction.cpp \
    ../../src/aeon/Runtime/AEGeneric.cpp \
    ../../src/aeon/Runtime/AEModule.cpp \
    ../../src/aeon/Runtime/AEObject.cpp \
    ../../src/aeon/Runtime/AEString.cpp \
    ../../src/aeon/Runtime/AEType.cpp \
    ../../src/aeon/Runtime/AEValue.cpp \
    ../../src/aeon/Runtime/AEValueList.cpp \
    ../../src/aeon/stdlib/io/File.cpp \
    ../../src/aeon/stdlib/io/FileInfo.cpp \
    ../../src/aeon/stdlib/media/Device.cpp \
    ../../src/aeon/stdlib/net/Socket.cpp \
    ../../src/aeon/stdlib/window/Window.cpp \
    ../../src/aeon/VM/AEVm.cpp \
    ../../src/aeon/VM/AEVmStack.cpp \
    ../../src/aeon/VM/AEVmThread.cpp \
    ../../src/aeon/stdlib/ExportStd.cpp

HEADERS +=   ../../src/aeon/aeBindingHelper.h \
    ../../src/aeon/aeBuilder.h \
    ../../src/aeon/aeClosure.h \
    ../../src/aeon/aeCodeFile.h \
    ../../src/aeon/aeDynamicType.h \
    ../../src/aeon/aeIR.h \
    ../../src/aeon/aeParser.h \
    ../../src/aeon/aeQualType.h \
    ../../src/aeon/aeReportManager.h \
    ../../src/aeon/aeSymbol.h \
    ../../src/aeon/aeTokenizer.h \
    ../../src/aeon/DebugDefs.h \
    ../../src/aeon/AST/AEBaseNode.h \
    ../../src/aeon/AST/aeNodeAccessOperator.h \
    ../../src/aeon/AST/aeNodeBinaryOperator.h \
    ../../src/aeon/AST/aeNodeBlock.h \
    ../../src/aeon/AST/aeNodeBranch.h \
    ../../src/aeon/AST/aeNodeEcosystem.h \
    ../../src/aeon/AST/aeNodeExpr.h \
    ../../src/aeon/AST/aeNodeFor.h \
    ../../src/aeon/AST/aeNodeFunction.h \
    ../../src/aeon/AST/aeNodeFunctionCall.h \
    ../../src/aeon/AST/aeNodeIdentifier.h \
    ../../src/aeon/AST/aeNodeLiterals.h \
    ../../src/aeon/AST/aeNodeModule.h \
    ../../src/aeon/AST/aeNodeNamespace.h \
    ../../src/aeon/AST/aeNodeNew.h \
    ../../src/aeon/AST/aeNodeReturn.h \
    ../../src/aeon/AST/aeNodeSubscript.h \
    ../../src/aeon/AST/aeNodeTernaryOperator.h \
    ../../src/aeon/AST/aeNodeUnaryOperator.h \
    ../../src/aeon/AST/aeNodeValue.h \
    ../../src/aeon/AST/aeNodeVarDecl.h \
    ../../src/aeon/AST/aeNodeWhile.h \
    ../../src/aeon/AST/AEStructNode.h \
    ../../src/aeon/AST/Nodes.h \
    ../../src/aeon/Build/aeBuildMake.h \
    ../../src/aeon/Compiler/aeCompiler.h \
    ../../src/aeon/Compiler/aeCompilerConv.h \
    ../../src/aeon/Compiler/aeCompilerErrorIds.h \
    ../../src/aeon/Compiler/aeCompilerWarningIds.h \
    ../../src/aeon/Runtime/AEByteCode.h \
    ../../src/aeon/Runtime/AEFunction.h \
    ../../src/aeon/Runtime/AEGeneric.h \    
    ../../src/aeon/Runtime/AEObject.h \
    ../../src/aeon/Runtime/AEString.h \
    ../../src/aeon/Runtime/AEType.h \
    ../../src/aeon/Runtime/AEValue.h \
    ../../src/aeon/Runtime/AEValueList.h \
    ../../src/aeon/stdlib/io/File.h \
    ../../src/aeon/stdlib/io/FileInfo.h \
    ../../src/aeon/stdlib/media/Device.h \
    ../../src/aeon/stdlib/net/Socket.h \
    ../../src/aeon/stdlib/window/Window.h \
    ../../src/aeon/stdlib/exportDefs.h \
    ../../src/aeon/VM/AEVm.h \
    ../../src/aeon/VM/AEVmArithmetic.h \
    ../../src/aeon/VM/AEVmCalls.h \
    ../../src/aeon/VM/AEVmStack.h \
    ../../src/aeon/VM/AEVmThread.h \
    ../../src/aeon/VM/VariantOps.h \
    ../../src/aeon/VM/VmDispatch.h \
    ../../src/aeon/VM/VmInstructions.h \
    ../../include/AEON/AEContext.h \
    ../../include/AEON/AEDocument.h \
    ../../include/AEON/AEModule.h \
    ../../src/aeon/stdlib/ExportStd.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
