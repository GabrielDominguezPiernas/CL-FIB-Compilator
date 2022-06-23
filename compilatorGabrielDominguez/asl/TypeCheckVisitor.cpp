//////////////////////////////////////////////////////////////////////
//
//    TypeCheckVisitor - Walk the parser tree to do the semantic
//                       typecheck for the Asl programming language
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

#include "TypeCheckVisitor.h"
#include "antlr4-runtime.h"

#include "../common/TypesMgr.h"
#include "../common/SymTable.h"
#include "../common/TreeDecoration.h"
#include "../common/SemErrors.h"

#include <iostream>
#include <string>

// uncomment the following line to enable debugging messages with DEBUG*
// #define DEBUG_BUILD
#include "../common/debug.h"

// using namespace std;


// Constructor
TypeCheckVisitor::TypeCheckVisitor(TypesMgr       & Types,
                                   SymTable       & Symbols,
                                   TreeDecoration & Decorations,
                                   SemErrors      & Errors) :
  Types{Types},
  Symbols{Symbols},
  Decorations{Decorations},
  Errors{Errors} {
}

// Accessor/Mutator to the attribute currFunctionType
TypesMgr::TypeId TypeCheckVisitor::getCurrentFunctionTy() const {
  return currFunctionType;
}

void TypeCheckVisitor::setCurrentFunctionTy(TypesMgr::TypeId type) {
  currFunctionType = type;
}


// Methods to visit each kind of node:
//
antlrcpp::Any TypeCheckVisitor::visitProgram(AslParser::ProgramContext *ctx) {
  DEBUG_ENTER();

  SymTable::ScopeId sc = getScopeDecor(ctx);
  Symbols.pushThisScope(sc);
  for (auto ctxFunc : ctx->function()) { 
    visit(ctxFunc);
  }
  if (Symbols.noMainProperlyDeclared())
    Errors.noMainProperlyDeclared(ctx);
  Symbols.popScope();
  Errors.print();

  DEBUG_EXIT();
  return 0;
}


antlrcpp::Any TypeCheckVisitor::visitFunction(AslParser::FunctionContext *ctx) {
  DEBUG_ENTER();

  SymTable::ScopeId sc = getScopeDecor(ctx);
  Symbols.pushThisScope(sc);
  TypesMgr::TypeId returnType = Types.createVoidTy();
  if(ctx->basic_type()){
     returnType  = getTypeDecor(ctx->basic_type());
  }
  std::vector<TypesMgr::TypeId> params;

  TypesMgr::TypeId tFunc = Types.createFunctionTy(params, returnType); //tipo funcion en scope
  setCurrentFunctionTy(tFunc);

  visit(ctx->statements());
  Symbols.popScope();

  DEBUG_EXIT();
  return 0;
}

// Function Ident
// Comprobamos la función que se llama des de codigo:
// Forma: ident = function(parameters?);
// var a = func(b,c);
antlrcpp::Any TypeCheckVisitor::visitFunctionIdent(AslParser::FunctionIdentContext *ctx) {
  DEBUG_ENTER();

  // Visitamos el identificador
  visit(ctx->ident());
  TypesMgr::TypeId t1 = getTypeDecor(ctx->ident());
  // Comprobamos que no haya un error y sea una funcion
  if ((not Types.isErrorTy(t1)) and (not Types.isFunctionTy(t1))) {
    Errors.isNotCallable(ctx->ident());
    TypesMgr::TypeId t = Types.createErrorTy();
    putTypeDecor(ctx, t);
  } else if (Types.isFunctionTy(t1)){
    // Es una funcion
    // Pueden ser funciones que devuelvan algo mediante return o voids

    // Miramos sus parametros
    uint nParams = ctx->expr().size();
    for(uint i = 0; i < nParams; ++i){
      // Por cada parametro lo visitamos
      visit(ctx->expr(i));
    }
    // Pillamos el tipo de la funcion
    TypesMgr::TypeId t = Types.getFuncReturnType(t1);
    putTypeDecor(ctx, t);
    // Comprobamos que no haya error en los parametros
    if(nParams != Types.getNumOfParameters(t1)) {
      Errors.numberOfParameters(ctx->ident());
    } else {
      for(uint i = 0; i < nParams; ++i) {
        TypesMgr::TypeId tParam = getTypeDecor(ctx->expr(i));
        TypesMgr::TypeId tCheck = Types.getParameterType(t1, i);
        if(not Types.copyableTypes(tCheck,tParam) and (not Types.isErrorTy(tParam))) {
          Errors.incompatibleParameter(ctx->expr(i),i+1,ctx->ident());
        }
      }
      if((not Types.isErrorTy(t1)) and Types.isVoidFunction(t1)) {
        Errors.isNotFunction(ctx->ident());
        TypesMgr::TypeId t = Types.createErrorTy();
        putTypeDecor(ctx, t);
      }
    }
  }

  putIsLValueDecor(ctx, false);
  DEBUG_EXIT();
  return 0;
}

// antlrcpp::Any TypeCheckVisitor::visitDeclarations(AslParser::DeclarationsContext *ctx) {
//   DEBUG_ENTER();
//   antlrcpp::Any r = visitChildren(ctx);
//   DEBUG_EXIT();
//   return r;
// }

// antlrcpp::Any TypeCheckVisitor::visitVariable_decl(AslParser::Variable_declContext *ctx) {
//   DEBUG_ENTER();
//   antlrcpp::Any r = visitChildren(ctx);
//   DEBUG_EXIT();
//   return r;
// }

// antlrcpp::Any TypeCheckVisitor::visitType(AslParser::TypeContext *ctx) {
//   DEBUG_ENTER();
//   antlrcpp::Any r = visitChildren(ctx);
//   DEBUG_EXIT();
//   return r;
// }

antlrcpp::Any TypeCheckVisitor::visitStatements(AslParser::StatementsContext *ctx) {
  DEBUG_ENTER();

  visitChildren(ctx);

  DEBUG_EXIT();
  return 0;
}

// Assign statement
// Forma: id = expr;
antlrcpp::Any TypeCheckVisitor::visitAssignStmt(AslParser::AssignStmtContext *ctx) {
  DEBUG_ENTER();

  // Visitamos las dos expresiones, la de la izquierda (identificador o variable)
  visit(ctx->left_expr());
  // La de la derecha, la expresion a asignar
  visit(ctx->expr());
  TypesMgr::TypeId t1 = getTypeDecor(ctx->left_expr());
  TypesMgr::TypeId t2 = getTypeDecor(ctx->expr());
  // Comprobamos que son asignables una a la otra
  if ((not Types.isErrorTy(t1)) and (not Types.isErrorTy(t2)) and
      (not Types.copyableTypes(t1, t2)))
    // Error, los dos tipos de variable no son asignables una a la otra
    Errors.incompatibleAssignment(ctx->ASSIGN());
  // Compruebo que la expresion de la izquierda sea referenciable
  // Ej que el array 'a' no este haciendo a = 6, si no a[2] = 6
  if ((not Types.isErrorTy(t1)) and (not getIsLValueDecor(ctx->left_expr())))
    // Error, el identificador no es referenciable
    Errors.nonReferenceableLeftExpr(ctx->left_expr());
  
  DEBUG_EXIT();
  return 0;
}

/*
antlrcpp::Any TypeCheckVisitor::visitMax(AslParser::MaxContext *ctx) {
  DEBUG_ENTER();
  TypesMgr::TypeId telem  = Types.createErrorTy();

  uint nParams = (ctx->expr()).size();

  if(nParams < 2){
    //Errors.numberOfMaxArguments(ctx);
  }

  // si esta mezclao devuelvo float

  bool checked = false;
  bool itWasFloat = false;
  for (size_t i = 0; i < nParams; ++i) {
    visit(ctx->expr(i));
    TypesMgr::TypeId t2 = getTypeDecor(ctx->expr(i));
    TypesMgr::TypeId t = getTypeDecor(ctx->expr(0));
    
    if((not Types.isErrorTy(t2)) and (not Types.isErrorTy(t)) and (not checked)){
      if((Types.isCharacterTy(t) and (Types.isNumericTy(t2))) or
        (Types.isCharacterTy(t2) and (Types.isNumericTy(t)))){
          //Errors.incompatibleMaxArguments(ctx);
          checked = true;
      }
    }
    if(not checked){
      if(Types.isFloatTy(t2)){
        itWasFloat = true;
        telem = t2;
      } else if(Types.isIntegerTy(t2)){
        if(not itWasFloat) telem = t2;
      } else if(Types.isCharacterTy(t2)){
        telem = t2;
      }
    } else telem = Types.createErrorTy();
  }

  putTypeDecor(ctx, telem);
  DEBUG_EXIT();
  return 0;
}
*/

/*
antlrcpp::Any TypeCheckVisitor::visitMaxDot(AslParser::MaxDotContext *ctx) {
  DEBUG_ENTER();
  visit(ctx->ident());
  TypesMgr::TypeId t1 = getTypeDecor(ctx->ident());


  TypesMgr::TypeId array1T; 
  if(Types.isArrayTy(t1)){ array1T = Types.getArrayElemType(t1); }
  else array1T = Types.createErrorTy();
  


  if ((not Types.isErrorTy(t1)) and (not Types.isArrayTy(t1)) or 
    ((not Types.isErrorTy(array1T)) and (Types.isBooleanTy(array1T)))
    ){
      Errors.incompatibleOperator(ctx->op);
  }

  putTypeDecor(ctx, array1T);
  putIsLValueDecor(ctx, false);

  DEBUG_EXIT();
  return 0;
}
*/
/*
antlrcpp::Any TypeCheckVisitor::visitProdEscalar(AslParser::ProdEscalarContext *ctx) {
  DEBUG_ENTER();
  visit(ctx->ident(0));
  TypesMgr::TypeId t1 = getTypeDecor(ctx->ident(0));
  visit(ctx->ident(1));
  TypesMgr::TypeId t2 = getTypeDecor(ctx->ident(1));
  
  TypesMgr::TypeId array1T; 
  if(Types.isArrayTy(t1)){ array1T = Types.getArrayElemType(t1); }
  else array1T = Types.createErrorTy();
  
  TypesMgr::TypeId array2T; 
  if(Types.isArrayTy(t2)){ array2T = Types.getArrayElemType(t2); }
  else array2T = Types.createErrorTy();

  if ((not Types.isErrorTy(t1)) and (not Types.isArrayTy(t1)) or 
    ((not Types.isErrorTy(t2)) and (not Types.isArrayTy(t2))) or
    (not(Types.getArraySize(t1) == Types.getArraySize(t2))) or
    (not Types.isIntegerTy(array1T) and not Types.isFloatTy(array1T)) or
    (not Types.isIntegerTy(array2T) and not Types.isFloatTy(array2T)) or
    (not (array1T == array2T))
    ){
      Errors.incompatibleOperator(ctx->op);
  }  

  putTypeDecor(ctx, array1T);
  putIsLValueDecor(ctx, false);

  DEBUG_EXIT();
  return 0;
}
*/

antlrcpp::Any TypeCheckVisitor::visitForStmt(AslParser::ForStmtContext *ctx) {
  DEBUG_ENTER();
  visit(ctx->ident());
  uint nParams = (ctx->expr()).size();
  bool checked = false;
  for (size_t i = 0; i < nParams; ++i) {
    visit(ctx->expr(i));
    TypesMgr::TypeId t = getTypeDecor(ctx->expr(i));
    if((not Types.isErrorTy(t)) and (not checked) and not (Types.isIntegerTy(t))){
      //Errors.forRequireIntegerExpr(ctx->expr(i));
      checked = true;
    }
  }
  visit(ctx->statements());

  if(nParams > 3){
    //Errors.numberOfRangeExpressions(ctx);
  }

  TypesMgr::TypeId t = getTypeDecor(ctx->ident());
  if(not Types.isIntegerTy(t) and (not Types.isErrorTy(t))){
    //Errors.forRequireIntegerVar(ctx->ident());
  }
  DEBUG_EXIT();
  return 0;
}

/*
antlrcpp::Any TypeCheckVisitor::visitPackStmt(AslParser::PackStmtContext *ctx) {
  DEBUG_ENTER();
  uint nParams = (ctx->expr()).size();
  for (size_t i = 0; i < nParams; ++i) {
    visit(ctx->expr(i));
  }
  visit(ctx->ident());
  TypesMgr::TypeId t = getTypeDecor(ctx->ident());
  if(Types.isArrayTy(t)){
    TypesMgr::TypeId tArr = Types.getArrayElemType(t);
    
    if(nParams != Types.getArraySize(t)){
      Errors.packUnpackNumberOfItemsMismatch(ctx);
    }

    for (size_t i = 0; i < nParams; ++i) {
      TypesMgr::TypeId t2 = getTypeDecor(ctx->expr(i));
      if((not Types.isErrorTy(tArr)) and (not Types.isErrorTy(t2)) and
        (not Types.copyableTypes(tArr, t2))){
        Errors.packUnpackIncompatibleTypes(ctx, i+1);
      }
    }
  } else if(not Types.isErrorTy(t)){
    Errors.packUnpackWithNonArray(ctx);
  }
  DEBUG_EXIT();
  return 0;
}
*/

/*
antlrcpp::Any TypeCheckVisitor::visitUnpackStmt(AslParser::UnpackStmtContext *ctx) {
  DEBUG_ENTER();
  visit(ctx->ident());
  uint nParams = (ctx->expr()).size();
  for (size_t i = 0; i < nParams; ++i) {
    visit(ctx->expr(i));
  }
  TypesMgr::TypeId t = getTypeDecor(ctx->ident());
  if(Types.isArrayTy(t)){
    TypesMgr::TypeId tArr = Types.getArrayElemType(t);

    if(nParams != Types.getArraySize(t)){
      Errors.packUnpackNumberOfItemsMismatch(ctx);
    }

    for (size_t i = 0; i < nParams; ++i) {
      TypesMgr::TypeId t2 = getTypeDecor(ctx->expr(i));
      if((not Types.isErrorTy(tArr)) and (not Types.isErrorTy(t2)) and
        (not Types.copyableTypes(t2,tArr))){
        Errors.packUnpackIncompatibleTypes(ctx, i+1);
      }
    }
  } else if(not Types.isErrorTy(t)){
    Errors.packUnpackWithNonArray(ctx);
  }
  DEBUG_EXIT();
  return 0;
}
*/
/*
antlrcpp::Any TypeCheckVisitor::visitMapStmt(AslParser::MapStmtContext *ctx) {
  DEBUG_ENTER();
  visit(ctx->ident(0));
  TypesMgr::TypeId t1 = getTypeDecor(ctx->ident(0));
  visit(ctx->ident(1));
  TypesMgr::TypeId t2 = getTypeDecor(ctx->ident(1));
  visit(ctx->ident(2));
  TypesMgr::TypeId t3 = getTypeDecor(ctx->ident(2));
    TypesMgr::TypeId array1T; 
  if(Types.isArrayTy(t1)){ array1T = Types.getArrayElemType(t1); }
  else array1T = Types.createErrorTy();
  
  TypesMgr::TypeId array2T; 
  if(Types.isArrayTy(t2)){ array2T = Types.getArrayElemType(t2); }
  else array2T = Types.createErrorTy();

  std::vector<TypesMgr::TypeId> fParams;
  TypesMgr::TypeId funcReturnT; 

  if(Types.isFunctionTy(t3)){ 
    fParams = Types.getFuncParamsTypes(t3); funcReturnT = Types.getFuncReturnType(t3);
  } else funcReturnT = Types.createErrorTy();


  if ((not Types.isErrorTy(t1)) and (not Types.isArrayTy(t1)) or 
    ((not Types.isErrorTy(t2)) and (not Types.isArrayTy(t2))) or
    (Types.getArraySize(t1) != Types.getArraySize(t2)) or
    ((not Types.isErrorTy(t3)) and (not Types.isFunctionTy(t3)))or
    (fParams.size() > 1)  or
    (not Types.copyableTypes(fParams[0],array1T)) 


    ){
      Errors.incompatibleMapOperands(ctx);
  }  


  DEBUG_EXIT();
  return 0;
}
*/
/*
antlrcpp::Any TypeCheckVisitor::visitFilter(AslParser::FilterContext *ctx) {
  DEBUG_ENTER();
  visit(ctx->ident(0));
  TypesMgr::TypeId t1 = getTypeDecor(ctx->ident(0));
  visit(ctx->ident(1));
  TypesMgr::TypeId t2 = getTypeDecor(ctx->ident(1));
  visit(ctx->ident(2));
  TypesMgr::TypeId t3 = getTypeDecor(ctx->ident(2));


  TypesMgr::TypeId array1T; 
  if(Types.isArrayTy(t1)){ array1T = Types.getArrayElemType(t1); }
  else array1T = Types.createErrorTy();
  
  TypesMgr::TypeId array2T; 
  if(Types.isArrayTy(t2)){ array2T = Types.getArrayElemType(t2); }
  else array2T = Types.createErrorTy();

  std::vector<TypesMgr::TypeId> fParams;
  if(Types.isFunctionTy(t3)){ fParams = Types.getFuncParamsTypes(t3); } 


  if ((not Types.isErrorTy(t1)) and (not Types.isArrayTy(t1)) or 
    ((not Types.isErrorTy(t2)) and (not Types.isArrayTy(t2))) or
    (Types.getArraySize(t1) != Types.getArraySize(t2)) or
    ((not Types.isErrorTy(t3)) and (not Types.isFunctionTy(t3))) or
    ((not Types.isErrorTy(array2T)) and (not Types.isBooleanTy(array2T))) or 
    (fParams.size() > 1) or
    ((not Types.isErrorTy(array1T)) and (not Types.equalTypes(fParams[0], array1T)))
    ){
      Errors.incompatibleOperator(ctx->op);
  }

  TypesMgr::TypeId telem  = Types.createIntegerTy();

  putTypeDecor(ctx, telem);
  putIsLValueDecor(ctx, false);

  DEBUG_EXIT();
  return 0;
}
*/

/*
antlrcpp::Any TypeCheckVisitor::visitTuple(AslParser::TupleContext *ctx) {
  DEBUG_ENTER();
  visit(ctx->ident());
  visit(ctx->expr());
  TypesMgr::TypeId tident = getTypeDecor(ctx->ident());
  TypesMgr::TypeId tindex = getTypeDecor(ctx->expr());
  TypesMgr::TypeId telem  = Types.createErrorTy();

  // Si hay error en el identificador
  if(not Types.isErrorTy(tident)){
    // Si no de tipo tupla
    if (not Types.isTupleTy(tident)){
      //Errors.nonTupleInTupleAccess(ctx->ident());
    } else {
      // Si no hay error en el identificador
      std::size_t tuple_size = Types.getTupleSize(tident);
      // Cojo el indice de la expresion
      int index = std::stoi(ctx->expr()->getText());
      // Compruebo que el indice es entero y esta dentro de los limites de la tupla
      if ((not Types.isErrorTy(tindex) and not Types.isIntegerTy(tindex)) or 
          (index < 0 or index >= int(tuple_size))){
        //Errors.nonExistentFieldInTuple(ctx);
      } else{
        telem = Types.getTupleFieldType(tident, index);
      }
    }
  }

  putTypeDecor(ctx, telem);
  putIsLValueDecor(ctx, true);

  DEBUG_EXIT();
  return 0;
}
*/
// If statament
// Forma: if expr then statements endif / if expr then statements else statements endif 
antlrcpp::Any TypeCheckVisitor::visitIfStmt(AslParser::IfStmtContext *ctx) {
  DEBUG_ENTER();
  // Visitamos la expresion
  visit(ctx->expr());
  TypesMgr::TypeId t1 = getTypeDecor(ctx->expr());
  // La expresion tiene que tener forma booleana
  if ((not Types.isErrorTy(t1)) and (not Types.isBooleanTy(t1)))
    // Error, hay una expresion pero no es de forma booleana
    Errors.booleanRequired(ctx);
  // Visitamos los statements del if
  visit(ctx->statements(0));
  // Comprobamos si hay else
  if(ctx->ELSE()) {
    // Visitamos los statements del else
    visit(ctx->statements(1));
  }
  DEBUG_EXIT();
  return 0;
}

// While statement
// Forma: while expr do statements endwhile
antlrcpp::Any TypeCheckVisitor::visitWhileStmt(AslParser::WhileStmtContext *ctx) {
  DEBUG_ENTER();
  // Visitamos la expr
  visit(ctx->expr());
  TypesMgr::TypeId t1 = getTypeDecor(ctx->expr());
  // La expresion tiene que tener forma booleana
  if ((not Types.isErrorTy(t1)) and (not Types.isBooleanTy(t1)))
    // Error, hay una expresion pero no es de forma booleana
    Errors.booleanRequired(ctx);
  // Visitamos los statements
  visit(ctx->statements());
  DEBUG_EXIT();
  return 0;
}

// Return statement
// Forma: return; / return expr;
antlrcpp::Any TypeCheckVisitor::visitReturnStmt(AslParser::ReturnStmtContext *ctx){
  DEBUG_ENTER();
  // Miramos el tipo de la funcion -> Puede ser void o no
  TypesMgr::TypeId f = getCurrentFunctionTy();
  TypesMgr::TypeId ftype   = Types.getFuncReturnType(f);
  // Si tiene expr es que tiene que ser una función que devuelva algo
  // SI devuelve algo
  if(ctx->expr()){
      // Visitamos la expr
      visit(ctx->expr());
      // Miramos el tipo de la expresion
      TypesMgr::TypeId valType = getTypeDecor(ctx->expr());
      // Miramos si la funcion es void, no puede devolver nada si es void
      if(not Types.isErrorTy(valType) and Types.isVoidFunction(f)) {
        // Error, devuelve algo en una función que es void
	      Errors.incompatibleReturn(ctx->RETURN());
      } 
      // Miramos que el tipo de la expresion corresponda al tipo de la funcion
      else if (not Types.isErrorTy(valType) and (not Types.equalTypes(ftype, valType)) and 
              not (Types.isIntegerTy(valType) and Types.isFloatTy(ftype))){
        Errors.incompatibleReturn(ctx->RETURN());
      } 
  } 
  // NO devuelve nada
  // Si no tiene expresion tiene que ser un void
  // Comprobamos que el tipo de la funcion sea void
  else if (not Types.isVoidFunction(f)){
      // Error, no devuelve nada en una función que no es void
      Errors.incompatibleReturn(ctx->RETURN());
  }

  //putTypeDecor(ctx, f);
  DEBUG_EXIT();
  return 0;
}

antlrcpp::Any TypeCheckVisitor::visitProcCall(AslParser::ProcCallContext *ctx) {
  DEBUG_ENTER();
  visit(ctx->ident());
  for (size_t i = 0; i < (ctx->expr()).size(); ++i) {
      visit(ctx->expr(i));
  }
  TypesMgr::TypeId t1 = getTypeDecor(ctx->ident());
  if (not Types.isFunctionTy(t1) and not Types.isErrorTy(t1)) {
    Errors.isNotCallable(ctx->ident());
  }else if(Types.isFunctionTy(t1)){
      std::size_t nparams   = ctx->expr().size();
      std::size_t fncparams = Types.getNumOfParameters(t1);
      if (nparams != fncparams){
            Errors.numberOfParameters(ctx->ident());
      } else{
          std::vector<TypesMgr::TypeId> lParamsTy = Types.getFuncParamsTypes(t1);
	  for(std::size_t i=0;i<nparams;++i){
	      TypesMgr::TypeId param_type = getTypeDecor(ctx->expr(i));
	      if(not Types.copyableTypes(lParamsTy[i], param_type)){	      
		      Errors.incompatibleParameter(ctx->expr(i), i+1, ctx);
	      }
	    }
    }
  }
  DEBUG_EXIT();
  return 0;
}

antlrcpp::Any TypeCheckVisitor::visitReadStmt(AslParser::ReadStmtContext *ctx) {
  DEBUG_ENTER();
  visit(ctx->left_expr());
  TypesMgr::TypeId t1 = getTypeDecor(ctx->left_expr());
  if ((not Types.isErrorTy(t1)) and (not Types.isPrimitiveTy(t1)) and
      (not Types.isFunctionTy(t1)))
    Errors.readWriteRequireBasic(ctx);
  if ((not Types.isErrorTy(t1)) and (not getIsLValueDecor(ctx->left_expr())))
    Errors.nonReferenceableExpression(ctx);
  DEBUG_EXIT();
  return 0;
}

antlrcpp::Any TypeCheckVisitor::visitWriteExpr(AslParser::WriteExprContext *ctx) {
  DEBUG_ENTER();
  visit(ctx->expr());
  TypesMgr::TypeId t1 = getTypeDecor(ctx->expr());
  if ((not Types.isErrorTy(t1)) and (not Types.isPrimitiveTy(t1)))
    Errors.readWriteRequireBasic(ctx);
  DEBUG_EXIT();
  return 0;
}

// antlrcpp::Any TypeCheckVisitor::visitWriteString(AslParser::WriteStringContext *ctx) {
//   DEBUG_ENTER();
//   antlrcpp::Any r = visitChildren(ctx);
//   DEBUG_EXIT();
//   return r;
// }

antlrcpp::Any TypeCheckVisitor::visitLeft_expr(AslParser::Left_exprContext *ctx) {
  DEBUG_ENTER();
  visit(ctx->ident());
  TypesMgr::TypeId t1 = getTypeDecor(ctx->ident());

  // Si es una posicion del array
  if(ctx->LSQ()){
    visit(ctx->expr());
    if ((not Types.isErrorTy(t1)) and (not Types.isArrayTy(t1))) {
      Errors.nonArrayInArrayAccess(ctx->ident());
      t1 = Types.createErrorTy();
    } else if(Types.isArrayTy(t1)){
      t1 = Types.getArrayElemType(t1);
    }

    TypesMgr::TypeId t2 = getTypeDecor(ctx->expr());
    if ((not Types.isErrorTy(t2)) and (not Types.isIntegerTy(t2))){
      Errors.nonIntegerIndexInArrayAccess(ctx->expr());
    }
  } else if(ctx->KEYL()){
    // Si es una posicion de la tupla
    visit(ctx->expr());
    //TypesMgr::TypeId tindex = getTypeDecor(ctx->expr());
    //bool tuple_ok = not Types.isErrorTy(t1);
    // Si hay un error en el tipo
    if (not Types.isErrorTy(t1)){
      // Si no es tupla
      if(not Types.isTupleTy(t1)){	      
          //Errors.nonTupleInTupleAccess(ctx->ident());
          t1 = Types.createErrorTy();
          //tuple_ok = false;
      } else{
        /*
	      int index = std::stoi(ctx->expr()->getText());
	      std::size_t tuple_size = Types.getTupleSize(t1);
	      if ((not Types.isErrorTy(tindex) and not Types.isIntegerTy(tindex))
		        or (index < 0 or index >= int(tuple_size))){
          Errors.nonExistentFieldInTuple(ctx);
          tuple_ok = false;
          t1 = Types.createErrorTy();
        } else{
          if(tuple_ok){
            t1 = Types.getTupleFieldType(t1, index);
          }
        
        }*/   
	    }
    }
    //t1 = Types.getTupleFieldType(t1);
  }
  putTypeDecor(ctx, t1);
  bool b = getIsLValueDecor(ctx->ident());
  putIsLValueDecor(ctx, b);
  DEBUG_EXIT();
  return 0;
}

antlrcpp::Any TypeCheckVisitor::visitArray(AslParser::ArrayContext *ctx) {
  DEBUG_ENTER();
  visit(ctx->expr());
  visit(ctx->ident());
  TypesMgr::TypeId tident = getTypeDecor(ctx->ident());
  TypesMgr::TypeId texpr = getTypeDecor(ctx->expr());
  TypesMgr::TypeId telem  = Types.createErrorTy();

  // Comprobamos los errores:
  // Del ID hemos de comprobar que sea un array
  if((not Types.isErrorTy(tident)) and (not Types.isArrayTy(tident))){
    Errors.nonArrayInArrayAccess(ctx->ident());
  }
  // Del expr hemos de comprobar que el indice sea un entero
  if((not Types.isErrorTy(texpr)) and (not Types.isIntegerTy(texpr))){
    Errors.nonIntegerIndexInArrayAccess(ctx->expr());
  }
  
  if(Types.isArrayTy(tident)){
    telem = Types.getArrayElemType(tident);
    putTypeDecor(ctx, telem);
    
  }
  // Esto hacerlo general
  putTypeDecor(ctx, telem);
  putIsLValueDecor(ctx, true); 

  DEBUG_EXIT();
  return 0;
}

antlrcpp::Any TypeCheckVisitor::visitParenthesis(AslParser::ParenthesisContext *ctx) {
  DEBUG_ENTER();
  visit(ctx->expr());
  TypesMgr::TypeId t1 = getTypeDecor(ctx->expr());   
  putTypeDecor(ctx, t1);
  DEBUG_EXIT();
  return 0;
}

antlrcpp::Any TypeCheckVisitor::visitUnary(AslParser::UnaryContext *ctx) {
  DEBUG_ENTER();
  visit(ctx->expr());
  TypesMgr::TypeId t1 = getTypeDecor(ctx->expr());
  // El not solo puede ser de booleanos
  if(ctx->NOT()){
    if((not Types.isErrorTy(t1)) and (not Types.isBooleanTy(t1))){
      Errors.incompatibleOperator(ctx->op);
      t1 = Types.createBooleanTy();
      //t1 = Types.createErrorTy();
    }
  } else {
    // El + - solo puede ser a numerics?
  }
  putTypeDecor(ctx, t1);
  DEBUG_EXIT();
  return 0;
}

antlrcpp::Any TypeCheckVisitor::visitArithmetic(AslParser::ArithmeticContext *ctx) {
  DEBUG_ENTER();
  visit(ctx->expr(0));
  TypesMgr::TypeId t1 = getTypeDecor(ctx->expr(0));
  visit(ctx->expr(1));
  TypesMgr::TypeId t2 = getTypeDecor(ctx->expr(1));

  // Compruebo que no haya error y las dos expresiones sean compatibles
  if (ctx->MOD() and (((not Types.isErrorTy(t1)) and (not Types.isIntegerTy(t1))) or
      ((not Types.isErrorTy(t2)) and (not Types.isIntegerTy(t2))))){
    Errors.incompatibleOperator(ctx->op);
  }
  else if (((not Types.isErrorTy(t1)) and (not Types.isNumericTy(t1))) or
      ((not Types.isErrorTy(t2)) and (not Types.isNumericTy(t2)))){
    Errors.incompatibleOperator(ctx->op);
  }
  TypesMgr::TypeId t;
  // Heredaremos el type
  if(Types.isFloatTy(t1) or Types.isFloatTy(t2)) {
    t = Types.createFloatTy();
  } /*else if(Types.isCharacterTy(t1)){
    t = Types.createCharacterTy();
  } else if (Types.isBooleanTy(t1)){
    t = Types.createBooleanTy();
  }*/ else {
    t = Types.createIntegerTy();
  }
  putTypeDecor(ctx, t);
  putIsLValueDecor(ctx, false);
  DEBUG_EXIT();
  return 0;
}

/*
antlrcpp::Any TypeCheckVisitor::visitSum(AslParser::SumContext *ctx) {
  DEBUG_ENTER();

  uint nParams = (ctx->expr()).size();
  bool isFloat = false;
  bool isError = false;
  for(uint i = 0; i < nParams; ++i){
    // Por cada parametro lo visitamos
    visit(ctx->expr(i));
    TypesMgr::TypeId t1 = getTypeDecor(ctx->expr(i));

    // Compruebo que no haya error y las dos expresiones sean compatibles
    if (((not Types.isErrorTy(t1)) and (not Types.isNumericTy(t1))) and (not isError)){
      Errors.incompatibleOperator(ctx->op);
      isError = true;
    }
    if(Types.isFloatTy(t1)) isFloat = true;
  }

  TypesMgr::TypeId t;
  if(isFloat)  t = Types.createFloatTy();
  else t = Types.createIntegerTy();
  putTypeDecor(ctx, t);
  putIsLValueDecor(ctx, false);
  
  DEBUG_EXIT();
  return 0;
}
*/
/*
antlrcpp::Any TypeCheckVisitor::visitPotencia(AslParser::PotenciaContext *ctx) {
  DEBUG_ENTER();

  visit(ctx->expr(0));
  TypesMgr::TypeId t1 = getTypeDecor(ctx->expr(0));
  visit(ctx->expr(1));
  TypesMgr::TypeId t2 = getTypeDecor(ctx->expr(1));

  if((((not Types.isErrorTy(t1)) and ((not Types.isIntegerTy(t1)) and (not Types.isFloatTy(t1)))) or
      ((not Types.isErrorTy(t2)) and (not Types.isIntegerTy(t2))))){
      Errors.incompatibleOperator(ctx->op);
  }
  TypesMgr::TypeId t = Types.createFloatTy();

  putTypeDecor(ctx, t);
  putIsLValueDecor(ctx, false);
  DEBUG_EXIT();
  return 0;
}
*/

antlrcpp::Any TypeCheckVisitor::visitBoolean(AslParser::BooleanContext *ctx) {
  DEBUG_ENTER();
  visit(ctx->expr(0));
  TypesMgr::TypeId t1 = getTypeDecor(ctx->expr(0));
  visit(ctx->expr(1));
  TypesMgr::TypeId t2 = getTypeDecor(ctx->expr(1));
  if (((not Types.isErrorTy(t1)) and (not Types.isBooleanTy(t1))) or
      ((not Types.isErrorTy(t2)) and (not Types.isBooleanTy(t2)))){
    Errors.incompatibleOperator(ctx->op);
  }
  TypesMgr::TypeId t = Types.createBooleanTy();
  putTypeDecor(ctx, t);
  putIsLValueDecor(ctx, false);
  DEBUG_EXIT();
  return 0;
}

antlrcpp::Any TypeCheckVisitor::visitRelational(AslParser::RelationalContext *ctx) {
  DEBUG_ENTER();
  visit(ctx->expr(0));
  TypesMgr::TypeId t1 = getTypeDecor(ctx->expr(0));
  visit(ctx->expr(1));
  TypesMgr::TypeId t2 = getTypeDecor(ctx->expr(1));
  std::string oper = ctx->op->getText();
  if ((not Types.isErrorTy(t1)) and (not Types.isErrorTy(t2)) and
      (not Types.comparableTypes(t1, t2, oper)))
    Errors.incompatibleOperator(ctx->op);
  TypesMgr::TypeId t = Types.createBooleanTy();
  putTypeDecor(ctx, t);
  putIsLValueDecor(ctx, false);
  DEBUG_EXIT();
  return 0;
}

antlrcpp::Any TypeCheckVisitor::visitValue(AslParser::ValueContext *ctx) {
  DEBUG_ENTER();
  TypesMgr::TypeId t;
  if(ctx->values()->INTVAL()){
    t = Types.createIntegerTy();
  } else if(ctx->values()->FLOATVAL()){
    t = Types.createFloatTy();
  } else if(ctx->values()->BOOLVAL()){
    t = Types.createBooleanTy();
  } else if(ctx->values()->CHARVAL()){
    t = Types.createCharacterTy();
  }
  putTypeDecor(ctx, t);
  putIsLValueDecor(ctx, false);
  DEBUG_EXIT();
  return 0;
}

antlrcpp::Any TypeCheckVisitor::visitExprIdent(AslParser::ExprIdentContext *ctx) {
  DEBUG_ENTER();
  visit(ctx->ident());
  TypesMgr::TypeId t1 = getTypeDecor(ctx->ident());
  putTypeDecor(ctx, t1);
  bool b = getIsLValueDecor(ctx->ident());
  putIsLValueDecor(ctx, b);
  DEBUG_EXIT();
  return 0;
}

antlrcpp::Any TypeCheckVisitor::visitIdent(AslParser::IdentContext *ctx) {
  DEBUG_ENTER();
  std::string ident = ctx->getText();
  if (Symbols.findInStack(ident) == -1) {
    Errors.undeclaredIdent(ctx->ID());
    TypesMgr::TypeId te = Types.createErrorTy();
    putTypeDecor(ctx, te);
    putIsLValueDecor(ctx, true);
  }
  else {
    TypesMgr::TypeId t1 = Symbols.getType(ident);
    putTypeDecor(ctx, t1);
    if (Symbols.isFunctionClass(ident))
      putIsLValueDecor(ctx, false);
    else
      putIsLValueDecor(ctx, true);
  }
  DEBUG_EXIT();
  return 0;
}


// Getters for the necessary tree node atributes:
//   Scope, Type ans IsLValue
SymTable::ScopeId TypeCheckVisitor::getScopeDecor(antlr4::ParserRuleContext *ctx) {
  return Decorations.getScope(ctx);
}
TypesMgr::TypeId TypeCheckVisitor::getTypeDecor(antlr4::ParserRuleContext *ctx) {
  return Decorations.getType(ctx);
}
bool TypeCheckVisitor::getIsLValueDecor(antlr4::ParserRuleContext *ctx) {
  return Decorations.getIsLValue(ctx);
}

// Setters for the necessary tree node attributes:
//   Scope, Type ans IsLValue
void TypeCheckVisitor::putScopeDecor(antlr4::ParserRuleContext *ctx, SymTable::ScopeId s) {
  Decorations.putScope(ctx, s);
}
void TypeCheckVisitor::putTypeDecor(antlr4::ParserRuleContext *ctx, TypesMgr::TypeId t) {
  Decorations.putType(ctx, t);
}
void TypeCheckVisitor::putIsLValueDecor(antlr4::ParserRuleContext *ctx, bool b) {
  Decorations.putIsLValue(ctx, b);
}
