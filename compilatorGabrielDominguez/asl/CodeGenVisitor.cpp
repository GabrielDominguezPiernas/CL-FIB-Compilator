//////////////////////////////////////////////////////////////////////
//
//    CodeGenVisitor - Walk the parser tree to do
//                     the generation of code
//
//    Copyright (C) 2017-2022  Universitat Politecnica de Catalunya
//
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU General Public License
//    as published by the Free Software Foundation; either version 3
//    of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public
//    License along with this library; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//    contact: José Miguel Rivero (rivero@cs.upc.edu)
//             Computer Science Department
//             Universitat Politecnica de Catalunya
//             despatx Omega.110 - Campus Nord UPC
//             08034 Barcelona.  SPAIN
//
//////////////////////////////////////////////////////////////////////

#include "CodeGenVisitor.h"
#include "antlr4-runtime.h"

#include "../common/TypesMgr.h"
#include "../common/SymTable.h"
#include "../common/TreeDecoration.h"
#include "../common/code.h"

#include <string>
#include <cstddef>    // std::size_t

// uncomment the following line to enable debugging messages with DEBUG*
// #define DEBUG_BUILD
#include "../common/debug.h"

// using namespace std;


// Constructor
CodeGenVisitor::CodeGenVisitor(TypesMgr       & Types,
                               SymTable       & Symbols,
                               TreeDecoration & Decorations) :
  Types{Types},
  Symbols{Symbols},
  Decorations{Decorations} {
}


// Methods to visit each kind of node:
//
antlrcpp::Any CodeGenVisitor::visitProgram(AslParser::ProgramContext *ctx) {
  DEBUG_ENTER();
  code my_code;
  SymTable::ScopeId sc = getScopeDecor(ctx);
  Symbols.pushThisScope(sc);
  for (auto ctxFunc : ctx->function()) { 
    subroutine subr = visit(ctxFunc);
    my_code.add_subroutine(subr);
  }
  Symbols.popScope();
  DEBUG_EXIT();
  return my_code;
}

// function : FUNC ID '(' parameters? ')' (':' basic_type)? declarations statements ENDFUNC
antlrcpp::Any CodeGenVisitor::visitFunction(AslParser::FunctionContext *ctx) {
  DEBUG_ENTER();

  SymTable::ScopeId sc = getScopeDecor(ctx);
  Symbols.pushThisScope(sc);
  subroutine subr(ctx->ID()->getText());
  codeCounters.reset();


  // If function has type we must add parameter to store result function
  if(ctx->basic_type()){ subr.add_param("_result"); }

  // If has parameters
  if(ctx->parameters()){
    std::vector<var> && lparams = visit(ctx->parameters());
    for (auto & oneparam : lparams) {
      subr.add_param(oneparam.name);
    }
  }

  std::vector<var> && lvars = visit(ctx->declarations());
  for (auto & onevar : lvars) {
    subr.add_var(onevar);
  }

  instructionList && code = visit(ctx->statements());
  if(!ctx->basic_type()) { code = code || instruction::RETURN(); }

  subr.set_instructions(code);
  Symbols.popScope();
  DEBUG_EXIT();
  return subr;
}

antlrcpp::Any CodeGenVisitor::visitDeclarations(AslParser::DeclarationsContext *ctx) {
  DEBUG_ENTER();

  std::vector<var> lvars;
  for (auto & varDeclCtx : ctx->variable_decl()) {
    std::vector<var> multiplesvar = visit(varDeclCtx);
    for(auto onevar : multiplesvar){
      lvars.push_back(onevar);
    }
  }

  DEBUG_EXIT();
  return lvars;
}

antlrcpp::Any CodeGenVisitor::visitParameters(AslParser::ParametersContext *ctx) {
  DEBUG_ENTER();

  std::vector<var> lparams;
  for (auto & paramCtx : ctx->parameter()) {
    var oneparam = visit(paramCtx);
    lparams.push_back(oneparam);
  }

  DEBUG_EXIT();
  return lparams;
}


antlrcpp::Any CodeGenVisitor::visitVariable_decl(AslParser::Variable_declContext *ctx) {
  DEBUG_ENTER();

  TypesMgr::TypeId   t1 = getTypeDecor(ctx->type());
  std::size_t      size = Types.getSizeOfType(t1);
  std::vector<var> lvars;
  for(unsigned int i = 0; i < ctx->ID().size(); i++){
    lvars.push_back(var{ctx->ID(i)->getText(), size});
  }

  DEBUG_EXIT();
  return lvars;
}

antlrcpp::Any CodeGenVisitor::visitParameter(AslParser::ParameterContext *ctx) {
  DEBUG_ENTER();

  TypesMgr::TypeId   t1 = getTypeDecor(ctx->type());
  std::size_t      size = Types.getSizeOfType(t1);
  
  DEBUG_EXIT();
  return var{ctx->ID()->getText(), size};
}

antlrcpp::Any CodeGenVisitor::visitStatements(AslParser::StatementsContext *ctx) {
  DEBUG_ENTER();

  instructionList code;
  for (auto stCtx : ctx->statement()) {
    instructionList && codeS = visit(stCtx);
    code = code || codeS;
  }

  DEBUG_EXIT();
  return code;
}

// functionIdent ident LP (expr (COMMA expr)*)? RP
antlrcpp::Any CodeGenVisitor::visitFunctionIdent(AslParser::FunctionIdentContext *ctx) {
  DEBUG_ENTER();

  instructionList code; // code we had generated
  instructionList codepop; // code to pop from stack
  instructionList codepush; // code to push to stack
  TypesMgr::TypeId tFunc = getTypeDecor(ctx->ident());
  //TypesMgr::TypeId t = Types.getFuncReturnType(tFunc);
  std::string name = ctx->ident()->getText();
  std::string tempReturn = "%"+codeCounters.newTEMP();
  uint nParams = ctx->expr().size();
  // we must iterate over all the expresions the func has
  for(uint i = 0; i < nParams; ++i) {
    // visit and add the code
    CodeAttribs     && codParams = visit(ctx->expr(i));
    std::string          addr = codParams.addr;
    instructionList &    codeParam = codParams.code;
    // concat the code
    code = code || codeParam;
    // now we must push the params to the stack 
    // we must check the type of params
    TypesMgr::TypeId tParam = getTypeDecor(ctx->expr(i));
    TypesMgr::TypeId tCheck = Types.getParameterType(tFunc, i);

    if (Types.isArrayTy(tCheck)) {
      std::string temp = "%"+codeCounters.newTEMP();
      code = code || instruction::ALOAD(temp, addr);
      codepush = codepush || instruction::PUSH(temp);
    } else if(Types.isIntegerTy(tParam) and Types.isFloatTy(tCheck)) {
      std::string temp = "%"+codeCounters.newTEMP();
      code = code || instruction::FLOAT(temp, addr);
      codepush = codepush || instruction::PUSH(temp);
    } else {
      codepush = codepush || instruction::PUSH(addr);
    }
    
    codepop = codepop || instruction::POP();
  }

  code = code || instruction::PUSH() || codepush || instruction::CALL(name) || codepop || instruction::POP(tempReturn);
  CodeAttribs codAts(tempReturn, "", code);

  DEBUG_EXIT();
  return codAts;
  
}

antlrcpp::Any CodeGenVisitor::visitUnary(AslParser::UnaryContext *ctx) {
    DEBUG_ENTER();

    CodeAttribs     && codAt = visit(ctx->expr());
    std::string         addr = codAt.addr;
    instructionList &   code = codAt.code;
    TypesMgr::TypeId  t = getTypeDecor(ctx);
    std::string temp = "%"+codeCounters.newTEMP();
    
    if(ctx->SUB()) {
      if(Types.isIntegerTy(t)) code = code || instruction::NEG(temp, addr);
      else if(Types.isFloatTy(t)) code = code || instruction::FNEG(temp, addr);
    } else if(ctx->NOT()) {
      if(Types.isBooleanTy(t)) code = code || instruction::NOT(temp, addr);
    } else if(ctx->PLUS()){
      DEBUG_EXIT();
      return codAt;
    }

    CodeAttribs codAts(temp, "", code);
    DEBUG_EXIT();
    return codAts;
}

// CHANGE 
antlrcpp::Any CodeGenVisitor::visitAssignStmt(AslParser::AssignStmtContext *ctx) {
  DEBUG_ENTER();

  instructionList code;
  CodeAttribs     && codAtsE1 = visit(ctx->left_expr());
  std::string           addr1 = codAtsE1.addr;
  std::string           offs1 = codAtsE1.offs;
  instructionList &     code1 = codAtsE1.code;
  //TypesMgr::TypeId tid1 = getTypeDecor(ctx->left_expr());
  CodeAttribs     && codAtsE2 = visit(ctx->expr());
  std::string           addr2 = codAtsE2.addr;
  std::string           offs2 = codAtsE2.offs;
  instructionList &     code2 = codAtsE2.code;
  //TypesMgr::TypeId tid2 = getTypeDecor(ctx->expr());

  TypesMgr::TypeId tid1ident = getTypeDecor(ctx->left_expr()->ident());
  TypesMgr::TypeId tid2ident = getTypeDecor(ctx->expr());

  // Diferents assigments possibilties:
  /* - Array to array
     - Array offset to variable 
     - Variable to array offset
     - Variable to variable
  */
  if(Types.isArrayTy(tid1ident) && Types.isArrayTy(tid2ident)){
    // Assign between arrays
    std::string temp1 = "%"+codeCounters.newTEMP();
    std::string temp2 = "%"+codeCounters.newTEMP();
    if(offs2 != "" || offs1 != ""){
      // Assigment between two positions of array
      // We must check 4 cases:
      /*
        - array1 to array2 no reference 
        - array1 to array2 no reference array2
        - array1 to array2 no reference array1
        - array1 to array2 no reference both
      */
      if(not Symbols.isParameterClass(addr1) && not Symbols.isParameterClass(addr2)){
        code = code1 || code2 || instruction::LOADX(temp1, addr2, offs2) || instruction::XLOAD(addr1, offs1, temp1);
      } else if(not Symbols.isParameterClass(addr1) && not Symbols.isParameterClass(addr2)){
        std::string tmp = "%"+codeCounters.newTEMP();
        code = code1 || code2 || instruction::LOAD(tmp, addr2) || instruction::LOADX(temp1, tmp, offs2) || instruction::XLOAD(addr1, offs1, temp1);
      } else if(not Symbols.isParameterClass(addr1) && not Symbols.isParameterClass(addr2)){
        std::string tmp = "%"+codeCounters.newTEMP();
        code = code1 || code2 || instruction::LOAD(tmp, addr1) || instruction::LOADX(temp1, addr2, offs2) || instruction::XLOAD(tmp, offs1, temp1);
      } else {
        std::string tmp1 = "%"+codeCounters.newTEMP();
        std::string tmp2 = "%"+codeCounters.newTEMP();
        code = code1 || code2 || instruction::LOAD(tmp1, addr1) || instruction::LOAD(tmp2, addr2) || instruction::LOADX(temp1, tmp2, offs2) || instruction::XLOAD(tmp1, offs1, temp1);
      }
    } else {
      // Assigment between two arrays
      // assign element by element
      // We must check 4 cases:
      /*
        - array1 to array2 no reference 
        - array1 to array2 no reference array2
        - array1 to array2 no reference array1
        - array1 to array2 no reference both
      */
      if(not Symbols.isParameterClass(addr1) && not Symbols.isParameterClass(addr2)){
        int maxSize = int(Types.getArraySize(tid2ident));
        if(maxSize>0) {
          std::string temp = "%"+codeCounters.newTEMP();
          std::string tempCounter = "%"+codeCounters.newTEMP();
          std::string tempAddOne = "%"+codeCounters.newTEMP();
          std::string tempMaxSize = "%"+codeCounters.newTEMP();
          std::string label = codeCounters.newLabelWHILE();
          std::string labelWhile = "whileCopyArray"+label;
          std::string sIndex = std::to_string(0);
          std::string one = std::to_string(1);
          std::string maximumSize = std::to_string(maxSize);
          code = code || instruction::ILOAD(tempCounter, sIndex) || instruction::ILOAD(tempAddOne, one) ||
          instruction::ILOAD(tempMaxSize, maximumSize) || instruction::LABEL(labelWhile) ||
          instruction::LOADX(temp,addr2,tempCounter) || instruction::XLOAD(addr1,tempCounter,temp) ||
          instruction::ADD(tempCounter,tempCounter,tempAddOne) || instruction::LE(temp,tempMaxSize, tempCounter) ||
          instruction::FJUMP(temp,labelWhile);
        }
      } else if(not Symbols.isParameterClass(addr1) && Symbols.isParameterClass(addr2)) {
        int maxSize = int(Types.getArraySize(tid2ident));
        if(maxSize>0) {
          std::string tmp = "%"+codeCounters.newTEMP();
          // Add temp variable
          code = code  || instruction::LOAD(tmp, addr2);
          std::string temp = "%"+codeCounters.newTEMP();
          std::string tempCounter = "%"+codeCounters.newTEMP();
          std::string tempAddOne = "%"+codeCounters.newTEMP();
          std::string tempMaxSize = "%"+codeCounters.newTEMP();
          std::string label = codeCounters.newLabelWHILE();
          std::string labelWhile = "whileCopyArray"+label;
          std::string sIndex = std::to_string(0);
          std::string one = std::to_string(1);
          std::string maximumSize = std::to_string(maxSize);
          code = code || instruction::ILOAD(tempCounter, sIndex) || instruction::ILOAD(tempAddOne, one) ||
          instruction::ILOAD(tempMaxSize, maximumSize) || instruction::LABEL(labelWhile) ||
          instruction::LOADX(temp,tmp,tempCounter) || instruction::XLOAD(addr1,tempCounter,temp) ||
          instruction::ADD(tempCounter,tempCounter,tempAddOne) || instruction::LE(temp,tempMaxSize, tempCounter) ||
          instruction::FJUMP(temp,labelWhile);
        }
      } else if(Symbols.isParameterClass(addr1) && not Symbols.isParameterClass(addr2)) {
        int maxSize = int(Types.getArraySize(tid2ident));
        if(maxSize>0) {
          std::string tmp = "%"+codeCounters.newTEMP();
          // Add temp variable
          code = code  || instruction::LOAD(tmp, addr1);
          std::string temp = "%"+codeCounters.newTEMP();
          std::string tempCounter = "%"+codeCounters.newTEMP();
          std::string tempAddOne = "%"+codeCounters.newTEMP();
          std::string tempMaxSize = "%"+codeCounters.newTEMP();
          std::string label = codeCounters.newLabelWHILE();
          std::string labelWhile = "whileCopyArray"+label;
          std::string sIndex = std::to_string(0);
          std::string one = std::to_string(1);
          std::string maximumSize = std::to_string(maxSize);
          code = code || instruction::ILOAD(tempCounter, sIndex) || instruction::ILOAD(tempAddOne, one) ||
          instruction::ILOAD(tempMaxSize, maximumSize) || instruction::LABEL(labelWhile) ||
          instruction::LOADX(temp,addr2,tempCounter) || instruction::XLOAD(tmp,tempCounter,temp) ||
          instruction::ADD(tempCounter,tempCounter,tempAddOne) || instruction::LE(temp,tempMaxSize, tempCounter) ||
          instruction::FJUMP(temp,labelWhile);
        }
      } else {
        int maxSize = int(Types.getArraySize(tid2ident));
        if(maxSize>0) {
          std::string tmp1 = "%"+codeCounters.newTEMP();
          std::string tmp2 = "%"+codeCounters.newTEMP();
          // Add temp variable
          code = code  || instruction::LOAD(tmp1, addr1);
          code = code  || instruction::LOAD(tmp2, addr2);
          std::string temp = "%"+codeCounters.newTEMP();
          std::string tempCounter = "%"+codeCounters.newTEMP();
          std::string tempAddOne = "%"+codeCounters.newTEMP();
          std::string tempMaxSize = "%"+codeCounters.newTEMP();
          std::string label = codeCounters.newLabelWHILE();
          std::string labelWhile = "whileCopyArray"+label;
          std::string sIndex = std::to_string(0);
          std::string one = std::to_string(1);
          std::string maximumSize = std::to_string(maxSize);
          code = code || instruction::ILOAD(tempCounter, sIndex) || instruction::ILOAD(tempAddOne, one) ||
          instruction::ILOAD(tempMaxSize, maximumSize) || instruction::LABEL(labelWhile) ||
          instruction::LOADX(temp,tmp2,tempCounter) || instruction::XLOAD(tmp1,tempCounter,temp) ||
          instruction::ADD(tempCounter,tempCounter,tempAddOne) || instruction::LE(temp,tempMaxSize, tempCounter) ||
          instruction::FJUMP(temp,labelWhile);
        }
      }
    }

  } else if(Types.isArrayTy(tid1ident) && not Types.isArrayTy(tid2ident)){
    TypesMgr::TypeId tid1Funcident = Types.getArrayElemType(tid1ident);
    // XLOAD
    if(not Symbols.isParameterClass(ctx->left_expr()->ident()->getText())){
      if(Types.isFloatTy(tid1Funcident) && Types.isIntegerTy(tid2ident)){
        std::string tempCast = "%"+codeCounters.newTEMP();  
        code = code1 || code2 || instruction::FLOAT(tempCast, addr2) || instruction::XLOAD(addr1, offs1, tempCast);
      } else code = code1 || code2 || instruction::XLOAD(addr1, offs1, addr2);
    } else {
      if(Types.isFloatTy(tid1Funcident) && Types.isIntegerTy(tid2ident)){
        std::string tempCast = "%"+codeCounters.newTEMP();  
        std::string tmp = "%"+codeCounters.newTEMP();
        code = code1 || code2 || instruction::FLOAT(tempCast, addr2);
        code = code  || instruction::LOAD(tmp, addr1);
        code = code  || instruction::XLOAD(tmp, offs1, tempCast);
      } else {
        std::string tmp = "%"+codeCounters.newTEMP();
        code = code1 || code2;
        code = code  || instruction::LOAD(tmp, addr1);
        code = code  || instruction::XLOAD(tmp, offs1, addr2);
      }
    }
  } else if(not Types.isArrayTy(tid1ident) && Types.isArrayTy(tid2ident)){
    TypesMgr::TypeId tid2Funcident = Types.getArrayElemType(tid2ident);
    // LOADX
    if(not Symbols.isParameterClass(ctx->left_expr()->ident()->getText())){
      if(Types.isFloatTy(tid1ident) && Types.isIntegerTy(tid2Funcident)){
        std::string tempCast = "%"+codeCounters.newTEMP(); 
        std::string tempRes = "%"+codeCounters.newTEMP(); 
        code = code1 || code2 || instruction::LOADX(tempRes, addr2, offs2);
        code = code || instruction::FLOAT(tempCast, tempRes) || instruction::FLOAD(addr1, tempCast);
      } else {
        code = code1 || code2 || instruction::LOADX(addr1, addr2, offs2);
      }
    } else {
      if(Types.isFloatTy(tid1ident) && Types.isIntegerTy(tid2Funcident)){
        std::string tempCast = "%"+codeCounters.newTEMP(); 
        std::string tempRes = "%"+codeCounters.newTEMP(); 
        std::string tmp = "%"+codeCounters.newTEMP();
        code = code1 || code2;
        code = code || instruction::LOAD(tmp, addr2);
        code = code || instruction::LOADX(tempRes, tmp, offs2);
        code = code || instruction::FLOAT(tempCast, tempRes) || instruction::FLOAD(addr1, tempCast);

      } else {  
        std::string tmp = "%"+codeCounters.newTEMP();
        code = code1 || code2;
        code = code  || instruction::LOAD(tmp, addr2);
        code = code  || instruction::LOADX(addr1, tmp, offs2);
      }
    }

  }else {
    // Left expr and expr both are not arrays
    // To avoid nan number
    if(Types.isFloatTy(tid1ident) && Types.isIntegerTy(tid2ident)){
      std::string temp = "%"+codeCounters.newTEMP();
      code = code1 || code2 || instruction::FLOAT(temp, addr2) || instruction::FLOAD(addr1, temp);
    } 
    else code = code1 || code2 || instruction::LOAD(addr1, addr2);
  }

  DEBUG_EXIT();
  return code;
}

antlrcpp::Any CodeGenVisitor::visitIfStmt(AslParser::IfStmtContext *ctx) {
  DEBUG_ENTER();

  instructionList code;
  CodeAttribs     && codAtsE = visit(ctx->expr());
  std::string          addr1 = codAtsE.addr;
  instructionList &    code1 = codAtsE.code;
  instructionList &&   code2 = visit(ctx->statements(0));
  std::string label = codeCounters.newLabelIF();
  std::string labelEndIf = "endif"+label;
  if(ctx->ELSE()){
    std::string labelElse = "else"+label;
    instructionList &&   code3 = visit(ctx->statements(1));
    code = code1 || instruction::FJUMP(addr1, labelElse) ||
          code2 || instruction::UJUMP(labelEndIf) || 
          instruction::LABEL(labelElse) || code3 || 
          instruction::LABEL(labelEndIf);
  } else{
    code = code1 || instruction::FJUMP(addr1, labelEndIf) ||
      code2 || instruction::LABEL(labelEndIf);

  }

  DEBUG_EXIT();
  return code;
}
/*
antlrcpp::Any CodeGenVisitor::visitMapStmt(AslParser::MapStmtContext *ctx) {
  DEBUG_ENTER();

  instructionList code;
  CodeAttribs     && codAtsE1 = visit(ctx->ident(0));
  std::string           addr1 = codAtsE1.addr;
  std::string           offs1 = codAtsE1.offs;
  instructionList &     code1 = codAtsE1.code;
  CodeAttribs     && codAtsE2 = visit(ctx->ident(1));
  std::string           addr2 = codAtsE2.addr;
  std::string           offs2 = codAtsE2.offs;
  instructionList &     code2 = codAtsE2.code;
  CodeAttribs     && codAtsE3 = visit(ctx->ident(2));
  std::string           addr3 = codAtsE3.addr;
  std::string           offs3 = codAtsE3.offs;
  instructionList &     code3 = codAtsE3.code;

  code = code || code1 || code2 || code3;

  TypesMgr::TypeId tid1ident = getTypeDecor(ctx->ident(0));
  TypesMgr::TypeId tid2ident = getTypeDecor(ctx->ident(1));

  TypesMgr::TypeId tid1Funcident = Types.getArrayElemType(tid1ident);
  TypesMgr::TypeId tid2Funcident = Types.getArrayElemType(tid2ident);


  int nParams = int(Types.getArraySize(tid1ident));
  std::string funcName = ctx->ident(2)->getText();

  std::string temp = "%"+codeCounters.newTEMP();
  std::string tempCounter = "%"+codeCounters.newTEMP();
  std::string tempAddOne = "%"+codeCounters.newTEMP();
  std::string tempMaxSize = "%"+codeCounters.newTEMP();
  std::string tempReturn = "%"+codeCounters.newTEMP();
  std::string tempCast = "%"+codeCounters.newTEMP();
  std::string label = codeCounters.newLabelWHILE();
  std::string labelWhile = "whileMapArray"+label;
  std::string sIndex = std::to_string(0);
  std::string one = std::to_string(1);
  std::string maximumSize = std::to_string(nParams);

  if(Types.isIntegerTy(tid1Funcident) && Types.isFloatTy(tid2Funcident)){
    code = code || 
    instruction::ILOAD(tempCounter, sIndex) || instruction::ILOAD(tempAddOne, one) ||
    instruction::ILOAD(tempMaxSize, maximumSize) || instruction::LABEL(labelWhile) ||
    instruction::LOADX(temp, addr1, tempCounter) || // en tmp tengo la pos a[i] a ser transformada por la funcion
    instruction::PUSH() || instruction::PUSH(temp) || instruction::CALL(funcName) || instruction::POP() || instruction::POP(tempReturn) ||
    instruction::XLOAD(addr2,tempCounter,tempReturn) ||
    instruction::ADD(tempCounter,tempCounter,tempAddOne) || instruction::LE(temp,tempMaxSize, tempCounter) ||
    instruction::FJUMP(temp,labelWhile);
  }  else {
    code = code || 
    instruction::ILOAD(tempCounter, sIndex) || instruction::ILOAD(tempAddOne, one) ||
    instruction::ILOAD(tempMaxSize, maximumSize) || instruction::LABEL(labelWhile) ||
    instruction::LOADX(temp,addr1,tempCounter) || // en tmp tengo la pos a[i] a ser transformada por la funcion
    instruction::PUSH() || instruction::PUSH(temp) || instruction::CALL(funcName) || instruction::POP() || instruction::POP(tempReturn) ||
    instruction::XLOAD(addr2,tempCounter,tempReturn) ||
    instruction::ADD(tempCounter,tempCounter,tempAddOne) || instruction::LE(temp,tempMaxSize, tempCounter) ||
    instruction::FJUMP(temp,labelWhile);
  }

  DEBUG_EXIT();
  return code;
}
*/

/*
antlrcpp::Any CodeGenVisitor::visitMaxDot(AslParser::MaxDotContext *ctx) {
  DEBUG_ENTER();

  instructionList code;
  CodeAttribs     && codAtsE1 = visit(ctx->ident());
  std::string           addr1 = codAtsE1.addr;
  std::string           offs1 = codAtsE1.offs;
  instructionList &     code1 = codAtsE1.code;

  code = code || code1;

  TypesMgr::TypeId tid1ident = getTypeDecor(ctx->ident());
  TypesMgr::TypeId tid1Funcident = Types.getArrayElemType(tid1ident);


  int nParams = int(Types.getArraySize(tid1ident));

  std::string temp = "%"+codeCounters.newTEMP();
  std::string tempCounter = "%"+codeCounters.newTEMP();
  std::string tempAddOne = "%"+codeCounters.newTEMP();
  std::string tempMaxSize = "%"+codeCounters.newTEMP();
  std::string tempArray = "%"+codeCounters.newTEMP();
  std::string tempResult = "%"+codeCounters.newTEMP();
  std::string tempLT = "%"+codeCounters.newTEMP();
  std::string label = codeCounters.newLabelWHILE();
  std::string labelWhile = "whileMaxArray"+label;
  std::string labelNoUpdate = "exitUpdateMax"+label;
  std::string sIndex = std::to_string(0);
  std::string one = std::to_string(1);
  std::string maximumSize = std::to_string(nParams);
  

  if(Symbols.isParameterClass(addr1)){
    std::string tmpPar = "%"+codeCounters.newTEMP();
    if(Types.isFloatTy(tid1Funcident)){
      code = code || 
      instruction::ILOAD(tempCounter, sIndex) || instruction::ILOAD(tempAddOne, one) ||
      instruction::ILOAD(tempMaxSize, maximumSize) || instruction::LOAD(tmpPar, addr1) || instruction::LOADX(tempResult, tmpPar, tempCounter) || 
      instruction::LABEL(labelWhile) || instruction::LOADX(temp,tmpPar,tempCounter) || // en tmp tengo la pos a[i] a ser transformada por la funcion
      instruction::FLT(tempLT, tempResult, temp) || instruction::FJUMP(tempLT, labelNoUpdate) || 
      instruction::FLOAD(tempResult, temp) || instruction::LABEL(labelNoUpdate) ||
      instruction::ADD(tempCounter,tempCounter,tempAddOne) || instruction::LE(temp,tempMaxSize, tempCounter) ||
      instruction::FJUMP(temp,labelWhile);
    }  else {
      code = code || 
      instruction::ILOAD(tempCounter, sIndex) || instruction::ILOAD(tempAddOne, one) ||
      instruction::ILOAD(tempMaxSize, maximumSize) || instruction::LOAD(tmpPar, addr1) || instruction::LOADX(tempResult, tmpPar, tempCounter) || 
      instruction::LABEL(labelWhile) || instruction::LOADX(temp,tmpPar,tempCounter) || // en tmp tengo la pos a[i] a ser transformada por la funcion
      instruction::LT(tempLT, tempResult, temp) || instruction::FJUMP(tempLT, labelNoUpdate) || 
      instruction::LOAD(tempResult, temp) || instruction::LABEL(labelNoUpdate) ||
      instruction::ADD(tempCounter,tempCounter,tempAddOne) || instruction::LE(temp,tempMaxSize, tempCounter) ||
      instruction::FJUMP(temp,labelWhile);
    }

  } else {
    if(Types.isFloatTy(tid1Funcident)){
      code = code || 
      instruction::ILOAD(tempCounter, sIndex) || instruction::ILOAD(tempAddOne, one) ||
      instruction::ILOAD(tempMaxSize, maximumSize) || instruction::LOADX(tempResult, addr1, tempCounter) || 
      instruction::LABEL(labelWhile) || instruction::LOADX(temp,addr1,tempCounter) || // en tmp tengo la pos a[i] a ser transformada por la funcion
      instruction::FLT(tempLT, tempResult, temp)  || instruction::FJUMP(tempLT, labelNoUpdate) || 
      instruction::FLOAD(tempResult, temp) || instruction::LABEL(labelNoUpdate) ||
      instruction::ADD(tempCounter,tempCounter,tempAddOne) || instruction::LE(temp,tempMaxSize, tempCounter) ||
      instruction::FJUMP(temp,labelWhile);
    }  else {
      code = code || 
      instruction::ILOAD(tempCounter, sIndex) || instruction::ILOAD(tempAddOne, one) ||
      instruction::ILOAD(tempMaxSize, maximumSize) || instruction::LOADX(tempResult, addr1, tempCounter) || 
      instruction::LABEL(labelWhile) || instruction::LOADX(temp,addr1,tempCounter) || // en tmp tengo la pos a[i] a ser transformada por la funcion
      instruction::LT(tempLT, tempResult, temp)  || instruction::FJUMP(tempLT, labelNoUpdate) || 
      instruction::LOAD(tempResult, temp) || instruction::LABEL(labelNoUpdate) ||
      instruction::ADD(tempCounter,tempCounter,tempAddOne) || instruction::LE(temp,tempMaxSize, tempCounter) ||
      instruction::FJUMP(temp,labelWhile);
    }
  }


  CodeAttribs codAts(tempResult, "", code);
  DEBUG_EXIT();
  return codAts;
}
*/
/*
antlrcpp::Any CodeGenVisitor::visitProdEscalar(AslParser::ProdEscalarContext *ctx) {
  DEBUG_ENTER();

  CodeAttribs     && codAt1 = visit(ctx->ident(0));
  std::string         addr1 = codAt1.addr;
  instructionList &   code1 = codAt1.code;
  CodeAttribs     && codAt2 = visit(ctx->ident(1));
  std::string         addr2 = codAt2.addr;
  instructionList &   code2 = codAt2.code;
  instructionList &&   code = code1 || code2;

  TypesMgr::TypeId t1 = getTypeDecor(ctx->ident(0));
  TypesMgr::TypeId tid1Funcident = Types.getArrayElemType(t1);

  int nParams = int(Types.getArraySize(t1));

  std::string temp = "%"+codeCounters.newTEMP();
  std::string totalTemp = "%"+codeCounters.newTEMP();
  std::string tempArray1 = "%"+codeCounters.newTEMP();
  std::string tempArray2 = "%"+codeCounters.newTEMP();
  std::string tempArrayValue1 = "%"+codeCounters.newTEMP();
  std::string tempArrayValue2 = "%"+codeCounters.newTEMP();
  std::string tempMul = "%"+codeCounters.newTEMP();
  std::string tempCounter = "%"+codeCounters.newTEMP();
  std::string tempAddOne = "%"+codeCounters.newTEMP();
  std::string tempMaxSize = "%"+codeCounters.newTEMP();
  std::string label = codeCounters.newLabelWHILE();
  std::string labelWhile = "whileProdEscalar"+label;
  std::string labelExitWhile = "exitWhileProdEscalar"+label;
  std::string zeroValue = std::to_string(0);
  std::string zeroValueF = std::to_string(0.0);
  std::string oneValue = std::to_string(1);
  std::string oneValueF = std::to_string(1.0);
  std::string maximumSize = std::to_string(nParams);

  if(Symbols.isParameterClass(addr1) and Symbols.isParameterClass(addr2)){
    if(Types.isFloatTy(tid1Funcident)){
      code = code || instruction::ILOAD(tempCounter, zeroValue) || instruction::ILOAD(tempAddOne, oneValue) || instruction::FLOAD(totalTemp, zeroValueF) ||
      instruction::ILOAD(tempMaxSize, maximumSize) || instruction::LOAD(tempArray1, addr1) || instruction::LOAD(tempArray2, addr2) ||
      instruction::LABEL(labelWhile) ||
      instruction::LOADX(tempArrayValue1, tempArray1, tempCounter) || instruction::LOADX(tempArrayValue2, tempArray2, tempCounter) ||
      instruction::FMUL(tempMul, tempArrayValue1, tempArrayValue2) ||
      instruction::FADD(totalTemp, totalTemp, tempMul) ||
      instruction::ADD(tempCounter,tempCounter,tempAddOne) || instruction::LE(temp,tempMaxSize, tempCounter) ||
      instruction::FJUMP(temp,labelWhile) || instruction::LABEL(labelExitWhile);
    } else {
      code = code || instruction::ILOAD(tempCounter, zeroValue) || instruction::ILOAD(tempAddOne, oneValue) || instruction::LOAD(totalTemp, zeroValue) ||
      instruction::ILOAD(tempMaxSize, maximumSize) || instruction::LOAD(tempArray1, addr1) || instruction::LOAD(tempArray2, addr2) ||
      instruction::LABEL(labelWhile) ||
      instruction::LOADX(tempArrayValue1, tempArray1, tempCounter) || instruction::LOADX(tempArrayValue2, tempArray2, tempCounter) ||
      instruction::MUL(tempMul, tempArrayValue1, tempArrayValue2) ||
      instruction::ADD(totalTemp, totalTemp, tempMul) ||
      instruction::ADD(tempCounter,tempCounter,tempAddOne) || instruction::LE(temp,tempMaxSize, tempCounter) ||
      instruction::FJUMP(temp,labelWhile) || instruction::LABEL(labelExitWhile);
    }
  } else if(Symbols.isParameterClass(addr1) and not Symbols.isParameterClass(addr2)){
    if(Types.isFloatTy(tid1Funcident)){
      code = code || instruction::ILOAD(tempCounter, zeroValue) || instruction::ILOAD(tempAddOne, oneValue) || instruction::FLOAD(totalTemp, zeroValueF) ||
      instruction::ILOAD(tempMaxSize, maximumSize) || instruction::LOAD(tempArray1, addr1) || instruction::LOAD(tempArray2, addr2) ||
      instruction::LABEL(labelWhile) ||
      instruction::LOADX(tempArrayValue1, tempArray1, tempCounter) || instruction::LOADX(tempArrayValue2, addr2, tempCounter) ||
      instruction::FMUL(tempMul, tempArrayValue1, tempArrayValue2) ||
      instruction::FADD(totalTemp, totalTemp, tempMul) ||
      instruction::ADD(tempCounter,tempCounter,tempAddOne) || instruction::LE(temp,tempMaxSize, tempCounter) ||
      instruction::FJUMP(temp,labelWhile) || instruction::LABEL(labelExitWhile);
    } else {
      code = code || instruction::ILOAD(tempCounter, zeroValue) || instruction::ILOAD(tempAddOne, oneValue) || instruction::LOAD(totalTemp, zeroValue) ||
      instruction::ILOAD(tempMaxSize, maximumSize) || instruction::LOAD(tempArray1, addr1) || instruction::LOAD(tempArray2, addr2) ||
      instruction::LABEL(labelWhile) ||
      instruction::LOADX(tempArrayValue1, tempArray1, tempCounter) || instruction::LOADX(tempArrayValue2, addr2, tempCounter) ||
      instruction::MUL(tempMul, tempArrayValue1, tempArrayValue2) ||
      instruction::ADD(totalTemp, totalTemp, tempMul) ||
      instruction::ADD(tempCounter,tempCounter,tempAddOne) || instruction::LE(temp,tempMaxSize, tempCounter) ||
      instruction::FJUMP(temp,labelWhile) || instruction::LABEL(labelExitWhile);
    }
  } else if(not Symbols.isParameterClass(addr1) and Symbols.isParameterClass(addr2)){
    if(Types.isFloatTy(tid1Funcident)){
      code = code || instruction::ILOAD(tempCounter, zeroValue) || instruction::ILOAD(tempAddOne, oneValue) || instruction::FLOAD(totalTemp, zeroValueF) ||
      instruction::ILOAD(tempMaxSize, maximumSize) || instruction::LOAD(tempArray1, addr1) || instruction::LOAD(tempArray2, addr2) ||
      instruction::LABEL(labelWhile) ||
      instruction::LOADX(tempArrayValue1, addr1, tempCounter) || instruction::LOADX(tempArrayValue2, tempArray2, tempCounter) ||
      instruction::FMUL(tempMul, tempArrayValue1, tempArrayValue2) ||
      instruction::FADD(totalTemp, totalTemp, tempMul) ||
      instruction::ADD(tempCounter,tempCounter,tempAddOne) || instruction::LE(temp,tempMaxSize, tempCounter) ||
      instruction::FJUMP(temp,labelWhile) || instruction::LABEL(labelExitWhile);
    } else {
      code = code || instruction::ILOAD(tempCounter, zeroValue) || instruction::ILOAD(tempAddOne, oneValue) || instruction::LOAD(totalTemp, zeroValue) ||
      instruction::ILOAD(tempMaxSize, maximumSize) || instruction::LOAD(tempArray1, addr1) || instruction::LOAD(tempArray2, addr2) ||
      instruction::LABEL(labelWhile) ||
      instruction::LOADX(tempArrayValue1, addr1, tempCounter) || instruction::LOADX(tempArrayValue2, tempArray2, tempCounter) ||
      instruction::MUL(tempMul, tempArrayValue1, tempArrayValue2) ||
      instruction::ADD(totalTemp, totalTemp, tempMul) ||
      instruction::ADD(tempCounter,tempCounter,tempAddOne) || instruction::LE(temp,tempMaxSize, tempCounter) ||
      instruction::FJUMP(temp,labelWhile) || instruction::LABEL(labelExitWhile);
    }
  } else {
    if(Types.isFloatTy(tid1Funcident)){
      code = code || instruction::ILOAD(tempCounter, zeroValue) || instruction::ILOAD(tempAddOne, oneValue) || instruction::FLOAD(totalTemp, zeroValueF) ||
      instruction::ILOAD(tempMaxSize, maximumSize) || instruction::LOAD(tempArray1, addr1) || instruction::LOAD(tempArray2, addr2) ||
      instruction::LABEL(labelWhile) ||
      instruction::LOADX(tempArrayValue1, addr1, tempCounter) || instruction::LOADX(tempArrayValue2, addr2, tempCounter) ||
      instruction::FMUL(tempMul, tempArrayValue1, tempArrayValue2) ||
      instruction::FADD(totalTemp, totalTemp, tempMul) ||
      instruction::ADD(tempCounter,tempCounter,tempAddOne) || instruction::LE(temp,tempMaxSize, tempCounter) ||
      instruction::FJUMP(temp,labelWhile) || instruction::LABEL(labelExitWhile);
    } else {
      code = code || instruction::ILOAD(tempCounter, zeroValue) || instruction::ILOAD(tempAddOne, oneValue) || instruction::LOAD(totalTemp, zeroValue) ||
      instruction::ILOAD(tempMaxSize, maximumSize) || instruction::LOAD(tempArray1, addr1) || instruction::LOAD(tempArray2, addr2) ||
      instruction::LABEL(labelWhile) ||
      instruction::LOADX(tempArrayValue1, addr1, tempCounter) || instruction::LOADX(tempArrayValue2, addr2, tempCounter) ||
      instruction::MUL(tempMul, tempArrayValue1, tempArrayValue2) ||
      instruction::ADD(totalTemp, totalTemp, tempMul) ||
      instruction::ADD(tempCounter,tempCounter,tempAddOne) || instruction::LE(temp,tempMaxSize, tempCounter) ||
      instruction::FJUMP(temp,labelWhile) || instruction::LABEL(labelExitWhile);
    }
  }

  CodeAttribs codAts(totalTemp, "", code);
  DEBUG_EXIT();
  return codAts;
}
*/
/*
antlrcpp::Any CodeGenVisitor::visitFilter(AslParser::FilterContext *ctx) {
  DEBUG_ENTER();

  instructionList code;
  CodeAttribs     && codAtsE1 = visit(ctx->ident(0));
  std::string           addr1 = codAtsE1.addr;
  std::string           offs1 = codAtsE1.offs;
  instructionList &     code1 = codAtsE1.code;
  CodeAttribs     && codAtsE2 = visit(ctx->ident(1));
  std::string           addr2 = codAtsE2.addr;
  std::string           offs2 = codAtsE2.offs;
  instructionList &     code2 = codAtsE2.code;
  CodeAttribs     && codAtsE3 = visit(ctx->ident(2));
  std::string           addr3 = codAtsE3.addr;
  std::string           offs3 = codAtsE3.offs;
  instructionList &     code3 = codAtsE3.code;

  code = code || code1 || code2 || code3;

  TypesMgr::TypeId tid1ident = getTypeDecor(ctx->ident(0));
  TypesMgr::TypeId tid2ident = getTypeDecor(ctx->ident(1));

  TypesMgr::TypeId tid1Funcident = Types.getArrayElemType(tid1ident);
  TypesMgr::TypeId tid2Funcident = Types.getArrayElemType(tid2ident);


  int nParams = int(Types.getArraySize(tid1ident));
  std::string funcName = ctx->ident(2)->getText();

  std::string temp = "%"+codeCounters.newTEMP();
  std::string tempCounter = "%"+codeCounters.newTEMP();
  std::string tempAddOne = "%"+codeCounters.newTEMP();
  std::string tempMaxSize = "%"+codeCounters.newTEMP();
  std::string tempReturn = "%"+codeCounters.newTEMP();
  std::string tempResult = "%"+codeCounters.newTEMP();
  std::string label = codeCounters.newLabelWHILE();
  std::string labelWhile = "whileFilterArray"+label;
  std::string sIndex = std::to_string(0);
  std::string one = std::to_string(1);
  std::string maximumSize = std::to_string(nParams);

  code = code || 
  instruction::ILOAD(tempCounter, sIndex) || instruction::ILOAD(tempAddOne, one) || instruction::ILOAD(tempResult, sIndex) || 
  instruction::ILOAD(tempMaxSize, maximumSize) || instruction::LABEL(labelWhile) ||
  instruction::LOADX(temp, addr1, tempCounter) || // en tmp tengo la pos a[i] a ser transformada por la funcion
  instruction::PUSH() || instruction::PUSH(temp) || instruction::CALL(funcName) || instruction::POP() || instruction::POP(tempReturn) ||
  instruction::XLOAD(addr2,tempCounter,tempReturn) || instruction::ADD(tempResult,tempResult,tempReturn) ||
  instruction::ADD(tempCounter,tempCounter,tempAddOne) || instruction::LE(temp,tempMaxSize, tempCounter) ||
  instruction::FJUMP(temp,labelWhile);

  CodeAttribs codAts(tempResult, "", code);
  DEBUG_EXIT();
  return codAts;
}
*/

antlrcpp::Any CodeGenVisitor::visitWhileStmt(AslParser::WhileStmtContext *ctx) {
  DEBUG_ENTER();

  instructionList code;
  CodeAttribs     && codAtsE = visit(ctx->expr());
  std::string          addr1 = codAtsE.addr;
  instructionList &    code1 = codAtsE.code;
  instructionList &&   code2 = visit(ctx->statements());
  std::string label = codeCounters.newLabelWHILE();
  std::string labelCondWhile = "condwhile"+label;
  std::string labelEndWhile = "endwhile"+label;
  code = instruction::LABEL(labelCondWhile) || code1 || instruction::FJUMP(addr1, labelEndWhile) ||
         code2 || instruction::UJUMP(labelCondWhile) || instruction::LABEL(labelEndWhile);
  
  DEBUG_EXIT();
  return code;
}

antlrcpp::Any CodeGenVisitor::visitProcCall(AslParser::ProcCallContext *ctx) {
  DEBUG_ENTER();

  instructionList code;
  instructionList codepop;
  instructionList codepush;
  std::string name = ctx->ident()->getText();
  TypesMgr::TypeId tFunc = getTypeDecor(ctx->ident());
  uint nParams = ctx->expr().size();
  for(uint i = 0; i < nParams; ++i) {
    CodeAttribs     && codParams = visit(ctx->expr(i));
    std::string          addr = codParams.addr;
    instructionList &    codeParam = codParams.code;
    code = code || codeParam;
    TypesMgr::TypeId tParam = getTypeDecor(ctx->expr(i));
    TypesMgr::TypeId tCheck = Types.getParameterType(tFunc, i);
    if (Types.isArrayTy(tCheck)) {
      std::string temp = "%"+codeCounters.newTEMP();
      if(Symbols.isParameterClass(addr)){
        std::string temp2 = "%"+codeCounters.newTEMP();
        code = code || instruction::LOAD(temp2, addr) || instruction::LOAD(temp, temp2);
      } else {
        code = code || instruction::ALOAD(temp, addr);
      }
      codepush = codepush || instruction::PUSH(temp);
    }
    else if(Types.isIntegerTy(tParam) and Types.isFloatTy(tCheck)) {
      std::string temp = "%"+codeCounters.newTEMP();
      code = code || instruction::FLOAT(temp, addr);
      codepush = codepush || instruction::PUSH(temp);
    }
    else {
      codepush = codepush || instruction::PUSH(addr);
    }
    codepop = codepop || instruction::POP();
  }
  if(not Types.isVoidFunction(tFunc)) {
    code = code || instruction::PUSH();
    codepop = codepop || instruction::POP();
  }
  code = code || codepush || instruction::CALL(name) || codepop;
  
  DEBUG_EXIT();
  return code;
}

antlrcpp::Any CodeGenVisitor::visitReturnStmt(AslParser::ReturnStmtContext *ctx) {
  DEBUG_ENTER();

  instructionList code;
  // Si devuelve algo la funcion
  if(ctx->expr()) {
    CodeAttribs     && codAts = visit(ctx->expr());
    std::string           addr = codAts.addr;
    instructionList &     codeExpr = codAts.code;

    code = codeExpr || instruction::LOAD("_result",addr);
  
  }

  code = code || instruction::RETURN();
  DEBUG_EXIT();
  return code;
}

antlrcpp::Any CodeGenVisitor::visitReadStmt(AslParser::ReadStmtContext *ctx) {
  DEBUG_ENTER();

  CodeAttribs     && codAtsE = visit(ctx->left_expr());
  std::string          addr1 = codAtsE.addr;
  std::string          offs1 = codAtsE.offs;
  instructionList &    code1 = codAtsE.code;
  instructionList &     code = code1;
  std::string temp = "%"+codeCounters.newTEMP();
  // We can read this type of variables
  /*
    - Integer
    - Float
    - Boolean
    - Char
  */
  // But we can read this types to array
  // We get the type of the param
  TypesMgr::TypeId tid1 = getTypeDecor(ctx->left_expr()->ident());
  if(Types.isArrayTy(tid1)){
    // We must get the type of variable
    TypesMgr::TypeId tarray = getTypeDecor(ctx->left_expr());
    if(Types.isFloatTy(tarray)){
      if(not Symbols.isParameterClass(addr1)){
        code = code1 || instruction::READF(temp) || instruction::XLOAD(addr1, offs1, temp);
      } else {
        std::string temp2 = "%"+codeCounters.newTEMP();
        code = code1 || instruction::LOAD(temp2, addr1) || instruction::READF(temp) || instruction::XLOAD(temp2, offs1, temp);
      }
    } else if(Types.isCharacterTy(tarray)){
      if(not Symbols.isParameterClass(addr1)){
        code = code1 || instruction::READC(temp) || instruction::XLOAD(addr1, offs1, temp);
      } else {
        std::string temp2 = "%"+codeCounters.newTEMP();
        code = code1 || instruction::LOAD(temp2, addr1) || instruction::READC(temp) || instruction::XLOAD(temp2, offs1, temp);
      }
    } else {
      if(not Symbols.isParameterClass(addr1)){
        code = code1 || instruction::READI(temp) || instruction::XLOAD(addr1, offs1, temp);
      } else {
        std::string temp2 = "%"+codeCounters.newTEMP();
        code = code1 || instruction::LOAD(temp2, addr1) || instruction::READI(temp) || instruction::XLOAD(temp2, offs1, temp);
      }
    }
  } else {
    if(Types.isFloatTy(tid1)){
      code = code1 || instruction::READF(addr1);
    } else if(Types.isCharacterTy(tid1)){
      code = code1 || instruction::READC(addr1);
    } else {
      code = code1 || instruction::READI(addr1);
    }
  }

  DEBUG_EXIT();
  return code;
}

antlrcpp::Any CodeGenVisitor::visitWriteExpr(AslParser::WriteExprContext *ctx) {
  DEBUG_ENTER();

  CodeAttribs     && codAt1 = visit(ctx->expr());
  std::string         addr1 = codAt1.addr;
  instructionList &   code1 = codAt1.code;
  instructionList &    code = code1;
  TypesMgr::TypeId tid1 = getTypeDecor(ctx->expr());
  if(Types.isFloatTy(tid1)){
    code = code1 || instruction::WRITEF(addr1);
  } else if(Types.isCharacterTy(tid1)){
    code = code1 || instruction::WRITEC(addr1);
  } else {
    code = code1 || instruction::WRITEI(addr1);
  } 
  
  DEBUG_EXIT();
  return code;
}

antlrcpp::Any CodeGenVisitor::visitWriteString(AslParser::WriteStringContext *ctx) {
  DEBUG_ENTER();

  instructionList code;
  std::string s = ctx->STRING()->getText();
  code = code || instruction::WRITES(s);

  DEBUG_EXIT();
  return code;
}

antlrcpp::Any CodeGenVisitor::visitLeft_expr(AslParser::Left_exprContext *ctx) {
  DEBUG_ENTER();

  CodeAttribs && codAts = visit(ctx->ident());
  std::string         addr1 = codAts.addr;
  instructionList &   code1 = codAts.code;
  std::string offs = "";

  instructionList &   code = code1;
  std::string temp = "%"+codeCounters.newTEMP();
  // We must check if it's an array type
  if(ctx->expr()){
    CodeAttribs     && codAts2 = visit(ctx->expr());
    std::string         addr2 = codAts2.addr;
    instructionList &   code2 = codAts2.code;
    offs = addr2;
    code = code || code2 || instruction::LOAD(temp, addr1);
  }
  
  CodeAttribs codRes(addr1, offs, code);
  DEBUG_EXIT();
  return codRes;
}


antlrcpp::Any CodeGenVisitor::visitParenthesis(AslParser::ParenthesisContext *ctx) {
    DEBUG_ENTER();
    CodeAttribs && codAts = visit(ctx->expr());
    DEBUG_EXIT();
    return codAts;
}

// expr op expr
antlrcpp::Any CodeGenVisitor::visitArithmetic(AslParser::ArithmeticContext *ctx) {
  DEBUG_ENTER();

  CodeAttribs     && codAt1 = visit(ctx->expr(0));
  std::string         addr1 = codAt1.addr;
  instructionList &   code1 = codAt1.code;
  CodeAttribs     && codAt2 = visit(ctx->expr(1));
  std::string         addr2 = codAt2.addr;
  instructionList &   code2 = codAt2.code;
  instructionList &&   code = code1 || code2;

  TypesMgr::TypeId t1 = getTypeDecor(ctx->expr(0));
  TypesMgr::TypeId t2 = getTypeDecor(ctx->expr(1));
  TypesMgr::TypeId  t = getTypeDecor(ctx);
  // Hay que tener en cuenta si es float o integer para sumarlos
  std::string temp = "%"+codeCounters.newTEMP();
  std::string tempCastFloat = "%"+codeCounters.newTEMP();

  // MOD:
  // x%y => x / y = z -> x%y = x-(y*z)
  // Careful : MOD is only between integers
  
  // Both are integers
  if(Types.isIntegerTy(t1) && Types.isIntegerTy(t2)){
    if(ctx->MUL()){
      code = code || instruction::MUL(temp, addr1, addr2); 
    } else if(ctx->DIV()){
      // We must check if div of two integers is float number. i.e.: 1/3 = 0.333...
      if(Types.isFloatTy(t)){
        std::string tempCastFloat2 = "%"+codeCounters.newTEMP();
        code = code || instruction::FLOAT(tempCastFloat, addr1) || instruction::FLOAT(tempCastFloat2, addr2) || instruction::FDIV(temp,tempCastFloat, tempCastFloat2); 
      } else code = code || instruction::DIV(temp, addr1, addr2); 
    } else if(ctx->MOD()){
      code = code || instruction::DIV(temp, addr1, addr2) ||
        instruction::MUL(temp, addr2, temp) || instruction::SUB(temp, addr1, temp);
    } else if(ctx->PLUS()){
      code = code || instruction::ADD(temp, addr1, addr2); 
    } else if(ctx->SUB()){
      code = code || instruction::SUB(temp, addr1, addr2); 
    }
  }
  // First is float and second one is integer 
  else if(Types.isFloatTy(t1) && Types.isIntegerTy(t2)){
    if(ctx->MUL()){
      code = code || instruction::FLOAT(tempCastFloat, addr2) || instruction::FMUL(temp, addr1, tempCastFloat); 
    } else if(ctx->DIV()){
      code = code || instruction::FLOAT(tempCastFloat, addr2) || instruction::FDIV(temp, addr1, tempCastFloat); 
    } else if(ctx->MOD()){
      code = code || instruction::FLOAT(tempCastFloat, addr2) || instruction::FDIV(temp, addr1, addr2) ||
          instruction::FMUL(temp, addr2, temp) || instruction::FSUB(temp, addr1, temp);
    } else if(ctx->PLUS()){
      code = code || instruction::FLOAT(tempCastFloat, addr2) || instruction::FADD(temp, addr1, tempCastFloat); 
    } else if(ctx->SUB()){
      code = code || instruction::FLOAT(tempCastFloat, addr2) || instruction::FSUB(temp, addr1, tempCastFloat); 
    }
  }
  // First is integer and second one is float 
  else if(Types.isIntegerTy(t1) && Types.isFloatTy(t2)){
    if(ctx->MUL()){
      code = code || instruction::FLOAT(tempCastFloat, addr1) || instruction::FMUL(temp, tempCastFloat, addr2); 
    } else if(ctx->DIV()){
      code = code || instruction::FLOAT(tempCastFloat, addr1) || instruction::FDIV(temp, tempCastFloat, addr2); 
    } else if(ctx->MOD()){
      code = code || instruction::FLOAT(tempCastFloat, addr2) || instruction::FDIV(temp, addr1, addr2) ||
          instruction::FMUL(temp, addr2, temp) || instruction::FSUB(temp, addr1, temp);
    } else if(ctx->PLUS()){
      code = code || instruction::FLOAT(tempCastFloat, addr1) || instruction::FADD(temp, tempCastFloat, addr2); 
    } else if(ctx->SUB()){
      code = code || instruction::FLOAT(tempCastFloat, addr1) || instruction::FSUB(temp, tempCastFloat, addr2); 
    }
  }
  // Both are float 
  else if(Types.isFloatTy(t2) && Types.isFloatTy(t1)){
    if(ctx->MUL()){
      code = code || instruction::FMUL(temp, addr1, addr2); 
    } else if(ctx->DIV()){
      code = code || instruction::FDIV(temp, addr1, addr2); 
    } else if(ctx->MOD()){
      code = code || instruction::FLOAT(tempCastFloat, addr2) || instruction::FDIV(temp, addr1, addr2) ||
          instruction::FMUL(temp, addr2, temp) || instruction::FSUB(temp, addr1, temp);
    } else if(ctx->PLUS()){
      code = code || instruction::FADD(temp, addr1, addr2); 
    } else if(ctx->SUB()){
      code = code || instruction::FSUB(temp, addr1, addr2); 
    }
  }

  CodeAttribs codAts(temp, "", code);
  DEBUG_EXIT();
  return codAts;
}
/*
antlrcpp::Any CodeGenVisitor::visitSum(AslParser::SumContext *ctx) {
  DEBUG_ENTER();

  std::string zeroValue = std::to_string(0);
  std::string oneValue = std::to_string(1);
  std::string oneValueF = std::to_string(1.0);
  
  std::string totalTemp = "%"+codeCounters.newTEMP();
  
  instructionList code = instruction::LOAD(totalTemp, zeroValue);
  
  bool thereIsFloatInVars = false;
  bool lastWasFloat = false;
  for(unsigned int i = 0; i < ctx->expr().size(); i++){
    CodeAttribs     && codVar = visit(ctx->expr(i));
    std::string         addrVar = codVar.addr;
    instructionList &   codeVar = codVar.code;
    code = code || codeVar;

    TypesMgr::TypeId t1 = getTypeDecor(ctx->expr(i));
    if(Types.isFloatTy(t1)) thereIsFloatInVars = true;

    if(thereIsFloatInVars && Types.isFloatTy(t1) && lastWasFloat){
      // Si es float y no es el primer float -> totalTemp ya esta casteado
      code = code || instruction::FADD(totalTemp, totalTemp, addrVar);
    } else if(thereIsFloatInVars && Types.isFloatTy(t1) && not lastWasFloat){
      // Si es float y es el primer float -> totalTemp no esta casteado
      std::string tempCast = "%"+codeCounters.newTEMP();
      code = code || instruction::FLOAT(tempCast, totalTemp) || instruction::FADD(totalTemp, tempCast, addrVar);
    } else if(thereIsFloatInVars && Types.isIntegerTy(t1)){
      std::string tempCast = "%"+codeCounters.newTEMP();
      code = code || instruction::FLOAT(tempCast, addrVar) || instruction::FADD(totalTemp, totalTemp, tempCast);
    } else {
      code = code || instruction::ADD(totalTemp, totalTemp, addrVar);
    }

    if(thereIsFloatInVars) lastWasFloat = true;
  }

  CodeAttribs codAts(totalTemp, "", code);
  DEBUG_EXIT();
  return codAts;
}
*/
/*
antlrcpp::Any CodeGenVisitor::visitPotencia(AslParser::PotenciaContext *ctx) {
  DEBUG_ENTER();

  CodeAttribs     && codAt1 = visit(ctx->expr(0));
  std::string         addr1 = codAt1.addr;
  instructionList &   code1 = codAt1.code;
  CodeAttribs     && codAt2 = visit(ctx->expr(1));
  std::string         addr2 = codAt2.addr;
  instructionList &   code2 = codAt2.code;
  instructionList &&   code = code1 || code2;

  TypesMgr::TypeId t1 = getTypeDecor(ctx->expr(0));

  std::string temp = "%"+codeCounters.newTEMP();
  std::string totalTemp = "%"+codeCounters.newTEMP();
  std::string tempCounter = "%"+codeCounters.newTEMP();
  std::string tempAddOne = "%"+codeCounters.newTEMP();
  std::string tempMaxSize = "%"+codeCounters.newTEMP();
  std::string tempIsZeroExponent = "%"+codeCounters.newTEMP();
  std::string label = codeCounters.newLabelWHILE();
  std::string labelWhile = "whilePotencia"+label;
  std::string labelExitWhile = "exitWhilePotencia"+label;
  std::string zeroValue = std::to_string(0);
  std::string oneValue = std::to_string(1);
  std::string oneValueF = std::to_string(1.0);

  if(Types.isFloatTy(t1)){
    code = code || instruction::ILOAD(tempCounter, zeroValue) || instruction::ILOAD(tempAddOne, oneValue) || instruction::FLOAD(totalTemp, oneValueF) ||
    instruction::ILOAD(tempMaxSize, addr2) || 
    instruction::LT(tempIsZeroExponent, tempCounter, addr2) || instruction::FJUMP(tempIsZeroExponent, labelExitWhile) ||
    instruction::LABEL(labelWhile) ||
    instruction::FMUL(totalTemp, totalTemp, addr1) ||
    instruction::ADD(tempCounter,tempCounter,tempAddOne) || instruction::LE(temp,tempMaxSize, tempCounter) ||
    instruction::FJUMP(temp,labelWhile) || instruction::LABEL(labelExitWhile);
  } else {
    std::string tempCast = "%"+codeCounters.newTEMP();
    code = code || instruction::ILOAD(tempCounter, zeroValue) || instruction::ILOAD(tempAddOne, oneValue) || instruction::FLOAD(totalTemp, oneValueF) ||
    instruction::ILOAD(tempMaxSize, addr2) || 
    instruction::LT(tempIsZeroExponent, tempCounter, addr2) || instruction::FJUMP(tempIsZeroExponent, labelExitWhile) ||
    instruction::LABEL(labelWhile) ||
    instruction::FLOAT(tempCast, addr1) || instruction::FMUL(totalTemp, totalTemp, tempCast) ||
    instruction::ADD(tempCounter,tempCounter,tempAddOne) || instruction::LE(temp,tempMaxSize, tempCounter) ||
    instruction::FJUMP(temp,labelWhile) || instruction::LABEL(labelExitWhile);

  }

  CodeAttribs codAts(totalTemp, "", code);
  DEBUG_EXIT();
  return codAts;
}
*/

antlrcpp::Any CodeGenVisitor::visitBoolean(AslParser::BooleanContext *ctx) {
  DEBUG_ENTER();

  CodeAttribs     && codAt1 = visit(ctx->expr(0));
  std::string         addr1 = codAt1.addr;
  instructionList &   code1 = codAt1.code;

  CodeAttribs     && codAt2 = visit(ctx->expr(1));
  std::string         addr2 = codAt2.addr;
  instructionList &   code2 = codAt2.code;
  instructionList &&   code = code1 || code2;

  // Hay que tener en cuenta si es float o integer para sumarlos
  std::string temp = "%"+codeCounters.newTEMP();
  if (ctx->AND()){
    code = code || instruction::AND(temp, addr1, addr2);
  } else if(ctx->OR()){
    code = code || instruction::OR(temp, addr1, addr2);
  }

  CodeAttribs codAts(temp, "", code);
  DEBUG_EXIT();
  return codAts;
}

antlrcpp::Any CodeGenVisitor::visitRelational(AslParser::RelationalContext *ctx) {
  DEBUG_ENTER();

  CodeAttribs     && codAt1 = visit(ctx->expr(0));
  std::string         addr1 = codAt1.addr;
  instructionList &   code1 = codAt1.code;
  CodeAttribs     && codAt2 = visit(ctx->expr(1));
  std::string         addr2 = codAt2.addr;
  instructionList &   code2 = codAt2.code;
  instructionList &&   code = code1 || code2;
  TypesMgr::TypeId t1 = getTypeDecor(ctx->expr(0));
  TypesMgr::TypeId t2 = getTypeDecor(ctx->expr(1));
  // TypesMgr::TypeId  t = getTypeDecor(ctx);
  std::string temp = "%"+codeCounters.newTEMP();
  std::string tempCastFloat = "%"+codeCounters.newTEMP();

  // First is float and second one is integer 
  if(Types.isFloatTy(t1) && Types.isIntegerTy(t2)){
    if(ctx->EQUAL()){
      code = code || instruction::FLOAT(tempCastFloat, addr2) || instruction::FEQ(temp, addr1, tempCastFloat); 
    } else if(ctx->NEG()){
      code = code || instruction::FLOAT(tempCastFloat, addr2) || instruction::FEQ(temp, addr1, tempCastFloat) || instruction::NOT(temp,temp); 
    } else if(ctx->GT()){
      code = code || instruction::FLOAT(tempCastFloat, addr2) || instruction::FLT(temp, tempCastFloat, addr1); 
    } else if(ctx->GE()){
      code = code || instruction::FLOAT(tempCastFloat, addr2) || instruction::FLE(temp, tempCastFloat, addr1); 
    } else if(ctx->LE()){
      code = code || instruction::FLOAT(tempCastFloat, addr2) || instruction::FLE(temp, addr1, tempCastFloat); 
    } else if(ctx->LT()){
      code = code || instruction::FLOAT(tempCastFloat, addr2) || instruction::FLT(temp, addr1, tempCastFloat); 
    }
  }
  // First is integer and second one is float 
  else if(Types.isFloatTy(t2) && Types.isIntegerTy(t1)){
    if(ctx->EQUAL()){
      code = code || instruction::FLOAT(tempCastFloat, addr1) || instruction::FEQ(temp, tempCastFloat, addr2); 
    } else if(ctx->NEG()){
      code = code || instruction::FLOAT(tempCastFloat, addr1) || instruction::FEQ(temp, tempCastFloat, addr2) || instruction::NOT(temp,temp); 
    } else if(ctx->GT()){
      code = code || instruction::FLOAT(tempCastFloat, addr1) || instruction::FLT(temp, addr2, tempCastFloat); 
    } else if(ctx->GE()){
      code = code || instruction::FLOAT(tempCastFloat, addr1) || instruction::FLE(temp, addr2, tempCastFloat); 
    } else if(ctx->LE()){
      code = code || instruction::FLOAT(tempCastFloat, addr1) || instruction::FLE(temp, tempCastFloat, addr2); 
    } else if(ctx->LT()){
      code = code || instruction::FLOAT(tempCastFloat, addr1) || instruction::FLT(temp, tempCastFloat, addr2); 
    }
  }
  // Both are float 
  else if(Types.isFloatTy(t2) && Types.isFloatTy(t1)){
    if(ctx->EQUAL()){
      code = code || instruction::FEQ(temp, addr1, addr2); 
    } else if(ctx->NEG()){
      code = code || instruction::FEQ(temp, addr1, addr2) || instruction::NOT(temp,temp); 
    } else if(ctx->GT()){
      code = code || instruction::FLT(temp, addr2, addr1); 
    } else if(ctx->GE()){
      code = code || instruction::FLE(temp, addr2, addr1); 
    } else if(ctx->LE()){
      code = code || instruction::FLE(temp, addr1, addr2); 
    } else if(ctx->LT()){
      //if(ctx->expr(0)->SUB()) 
      code = code || instruction::FLT(temp, addr1, addr2); 
    }
  }
  // Else -> both are integer, integer and boolean, chars
  else{
    if(ctx->EQUAL()){
      code = code || instruction::EQ(temp, addr1, addr2); 
    } else if(ctx->NEG()){
      code = code || instruction::EQ(temp, addr1, addr2) || instruction::NOT(temp,temp); 
    } else if(ctx->GT()){
      code = code || instruction::LT(temp, addr2, addr1); 
    } else if(ctx->GE()){
      code = code || instruction::LE(temp, addr2, addr1); 
    } else if(ctx->LE()){
      code = code || instruction::LE(temp, addr1, addr2); 
    } else if(ctx->LT()){
      code = code || instruction::LT(temp, addr1, addr2); 
    }
  }

  CodeAttribs codAts(temp, "", code);
  DEBUG_EXIT();
  return codAts;
}

antlrcpp::Any CodeGenVisitor::visitValue(AslParser::ValueContext *ctx) {
  DEBUG_ENTER();

  instructionList code;
  std::string temp = "%"+codeCounters.newTEMP();
  if(ctx->getText() == "false"){
    code = instruction::ILOAD(temp, "0");
  } else if(ctx->getText() == "true"){
    code = instruction::ILOAD(temp, "1");
  } else {
    code = instruction::ILOAD(temp, ctx->getText());
  }

  CodeAttribs codAts(temp, "", code);
  DEBUG_EXIT();
  return codAts;
}

antlrcpp::Any CodeGenVisitor::visitArray(AslParser::ArrayContext *ctx) {
  DEBUG_ENTER();

  CodeAttribs     && codAt1 = visit(ctx->ident());
  std::string         addr1 = codAt1.addr;
  instructionList &   code1 = codAt1.code;
  CodeAttribs     && codAt2 = visit(ctx->expr());
  std::string         addr2 = codAt2.addr;
  instructionList &   code2 = codAt2.code;
  instructionList &&   code = code1 || code2;
  std::string temp = "%"+codeCounters.newTEMP();
  std::string temp2 = "%"+codeCounters.newTEMP();

  code = code1 || code2;
  if(not Symbols.isParameterClass(ctx->ident()->getText())){
    code = code || instruction::LOADX(temp, addr1, addr2);
  } else {
    code = code  || instruction::LOAD(temp2, addr1);
    code = code  || instruction::LOADX(temp, temp2, addr2);
  }
  
  CodeAttribs codAts(temp, addr2, code);
  DEBUG_EXIT();
  return codAts;
}

antlrcpp::Any CodeGenVisitor::visitExprIdent(AslParser::ExprIdentContext *ctx) {
  DEBUG_ENTER();

  CodeAttribs && codAts = visit(ctx->ident());

  DEBUG_EXIT();
  return codAts;
}

antlrcpp::Any CodeGenVisitor::visitIdent(AslParser::IdentContext *ctx) {
  DEBUG_ENTER();

  CodeAttribs codAts(ctx->ID()->getText(), "", instructionList());

  DEBUG_EXIT();
  return codAts;
}

// Getters for the necessary tree node atributes:
//   Scope and Type
SymTable::ScopeId CodeGenVisitor::getScopeDecor(antlr4::ParserRuleContext *ctx) const {
  return Decorations.getScope(ctx);
}
TypesMgr::TypeId CodeGenVisitor::getTypeDecor(antlr4::ParserRuleContext *ctx) const {
  return Decorations.getType(ctx);
}

// Constructors of the class CodeAttribs:
//
CodeGenVisitor::CodeAttribs::CodeAttribs(const std::string & addr,
                                         const std::string & offs,
                                         instructionList & code) :
  addr{addr}, offs{offs}, code{code} {
}

CodeGenVisitor::CodeAttribs::CodeAttribs(const std::string & addr,
                                         const std::string & offs,
                                         instructionList && code) :
  addr{addr}, offs{offs}, code{code} {
}
