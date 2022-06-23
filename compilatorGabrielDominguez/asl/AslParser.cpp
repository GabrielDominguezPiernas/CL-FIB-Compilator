
// Generated from Asl.g4 by ANTLR 4.7.2


#include "AslVisitor.h"

#include "AslParser.h"


using namespace antlrcpp;
using namespace antlr4;

AslParser::AslParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

AslParser::~AslParser() {
  delete _interpreter;
}

std::string AslParser::getGrammarFileName() const {
  return "Asl.g4";
}

const std::vector<std::string>& AslParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& AslParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- ProgramContext ------------------------------------------------------------------

AslParser::ProgramContext::ProgramContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AslParser::ProgramContext::EOF() {
  return getToken(AslParser::EOF, 0);
}

std::vector<AslParser::FunctionContext *> AslParser::ProgramContext::function() {
  return getRuleContexts<AslParser::FunctionContext>();
}

AslParser::FunctionContext* AslParser::ProgramContext::function(size_t i) {
  return getRuleContext<AslParser::FunctionContext>(i);
}


size_t AslParser::ProgramContext::getRuleIndex() const {
  return AslParser::RuleProgram;
}

antlrcpp::Any AslParser::ProgramContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitProgram(this);
  else
    return visitor->visitChildren(this);
}

AslParser::ProgramContext* AslParser::program() {
  ProgramContext *_localctx = _tracker.createInstance<ProgramContext>(_ctx, getState());
  enterRule(_localctx, 0, AslParser::RuleProgram);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(33); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(32);
      function();
      setState(35); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == AslParser::FUNC);
    setState(37);
    match(AslParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionContext ------------------------------------------------------------------

AslParser::FunctionContext::FunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AslParser::FunctionContext::FUNC() {
  return getToken(AslParser::FUNC, 0);
}

tree::TerminalNode* AslParser::FunctionContext::ID() {
  return getToken(AslParser::ID, 0);
}

tree::TerminalNode* AslParser::FunctionContext::LP() {
  return getToken(AslParser::LP, 0);
}

tree::TerminalNode* AslParser::FunctionContext::RP() {
  return getToken(AslParser::RP, 0);
}

AslParser::DeclarationsContext* AslParser::FunctionContext::declarations() {
  return getRuleContext<AslParser::DeclarationsContext>(0);
}

AslParser::StatementsContext* AslParser::FunctionContext::statements() {
  return getRuleContext<AslParser::StatementsContext>(0);
}

tree::TerminalNode* AslParser::FunctionContext::ENDFUNC() {
  return getToken(AslParser::ENDFUNC, 0);
}

AslParser::ParametersContext* AslParser::FunctionContext::parameters() {
  return getRuleContext<AslParser::ParametersContext>(0);
}

tree::TerminalNode* AslParser::FunctionContext::COLON() {
  return getToken(AslParser::COLON, 0);
}

AslParser::Basic_typeContext* AslParser::FunctionContext::basic_type() {
  return getRuleContext<AslParser::Basic_typeContext>(0);
}


size_t AslParser::FunctionContext::getRuleIndex() const {
  return AslParser::RuleFunction;
}

antlrcpp::Any AslParser::FunctionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitFunction(this);
  else
    return visitor->visitChildren(this);
}

AslParser::FunctionContext* AslParser::function() {
  FunctionContext *_localctx = _tracker.createInstance<FunctionContext>(_ctx, getState());
  enterRule(_localctx, 2, AslParser::RuleFunction);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(39);
    match(AslParser::FUNC);
    setState(40);
    match(AslParser::ID);
    setState(41);
    match(AslParser::LP);
    setState(43);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == AslParser::ID) {
      setState(42);
      parameters();
    }
    setState(45);
    match(AslParser::RP);
    setState(48);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == AslParser::COLON) {
      setState(46);
      match(AslParser::COLON);
      setState(47);
      basic_type();
    }
    setState(50);
    declarations();
    setState(51);
    statements();
    setState(52);
    match(AslParser::ENDFUNC);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParametersContext ------------------------------------------------------------------

AslParser::ParametersContext::ParametersContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<AslParser::ParameterContext *> AslParser::ParametersContext::parameter() {
  return getRuleContexts<AslParser::ParameterContext>();
}

AslParser::ParameterContext* AslParser::ParametersContext::parameter(size_t i) {
  return getRuleContext<AslParser::ParameterContext>(i);
}

std::vector<tree::TerminalNode *> AslParser::ParametersContext::COMMA() {
  return getTokens(AslParser::COMMA);
}

tree::TerminalNode* AslParser::ParametersContext::COMMA(size_t i) {
  return getToken(AslParser::COMMA, i);
}


size_t AslParser::ParametersContext::getRuleIndex() const {
  return AslParser::RuleParameters;
}

antlrcpp::Any AslParser::ParametersContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitParameters(this);
  else
    return visitor->visitChildren(this);
}

AslParser::ParametersContext* AslParser::parameters() {
  ParametersContext *_localctx = _tracker.createInstance<ParametersContext>(_ctx, getState());
  enterRule(_localctx, 4, AslParser::RuleParameters);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(54);
    parameter();
    setState(59);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == AslParser::COMMA) {
      setState(55);
      match(AslParser::COMMA);
      setState(56);
      parameter();
      setState(61);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParameterContext ------------------------------------------------------------------

AslParser::ParameterContext::ParameterContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AslParser::ParameterContext::ID() {
  return getToken(AslParser::ID, 0);
}

tree::TerminalNode* AslParser::ParameterContext::COLON() {
  return getToken(AslParser::COLON, 0);
}

AslParser::TypeContext* AslParser::ParameterContext::type() {
  return getRuleContext<AslParser::TypeContext>(0);
}


size_t AslParser::ParameterContext::getRuleIndex() const {
  return AslParser::RuleParameter;
}

antlrcpp::Any AslParser::ParameterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitParameter(this);
  else
    return visitor->visitChildren(this);
}

AslParser::ParameterContext* AslParser::parameter() {
  ParameterContext *_localctx = _tracker.createInstance<ParameterContext>(_ctx, getState());
  enterRule(_localctx, 6, AslParser::RuleParameter);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(62);
    match(AslParser::ID);
    setState(63);
    match(AslParser::COLON);
    setState(64);
    type();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclarationsContext ------------------------------------------------------------------

AslParser::DeclarationsContext::DeclarationsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<AslParser::Variable_declContext *> AslParser::DeclarationsContext::variable_decl() {
  return getRuleContexts<AslParser::Variable_declContext>();
}

AslParser::Variable_declContext* AslParser::DeclarationsContext::variable_decl(size_t i) {
  return getRuleContext<AslParser::Variable_declContext>(i);
}


size_t AslParser::DeclarationsContext::getRuleIndex() const {
  return AslParser::RuleDeclarations;
}

antlrcpp::Any AslParser::DeclarationsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitDeclarations(this);
  else
    return visitor->visitChildren(this);
}

AslParser::DeclarationsContext* AslParser::declarations() {
  DeclarationsContext *_localctx = _tracker.createInstance<DeclarationsContext>(_ctx, getState());
  enterRule(_localctx, 8, AslParser::RuleDeclarations);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(69);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == AslParser::VAR) {
      setState(66);
      variable_decl();
      setState(71);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Variable_declContext ------------------------------------------------------------------

AslParser::Variable_declContext::Variable_declContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AslParser::Variable_declContext::VAR() {
  return getToken(AslParser::VAR, 0);
}

std::vector<tree::TerminalNode *> AslParser::Variable_declContext::ID() {
  return getTokens(AslParser::ID);
}

tree::TerminalNode* AslParser::Variable_declContext::ID(size_t i) {
  return getToken(AslParser::ID, i);
}

tree::TerminalNode* AslParser::Variable_declContext::COLON() {
  return getToken(AslParser::COLON, 0);
}

AslParser::TypeContext* AslParser::Variable_declContext::type() {
  return getRuleContext<AslParser::TypeContext>(0);
}

std::vector<tree::TerminalNode *> AslParser::Variable_declContext::COMMA() {
  return getTokens(AslParser::COMMA);
}

tree::TerminalNode* AslParser::Variable_declContext::COMMA(size_t i) {
  return getToken(AslParser::COMMA, i);
}


size_t AslParser::Variable_declContext::getRuleIndex() const {
  return AslParser::RuleVariable_decl;
}

antlrcpp::Any AslParser::Variable_declContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitVariable_decl(this);
  else
    return visitor->visitChildren(this);
}

AslParser::Variable_declContext* AslParser::variable_decl() {
  Variable_declContext *_localctx = _tracker.createInstance<Variable_declContext>(_ctx, getState());
  enterRule(_localctx, 10, AslParser::RuleVariable_decl);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(72);
    match(AslParser::VAR);
    setState(73);
    match(AslParser::ID);
    setState(78);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == AslParser::COMMA) {
      setState(74);
      match(AslParser::COMMA);
      setState(75);
      match(AslParser::ID);
      setState(80);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(81);
    match(AslParser::COLON);
    setState(82);
    type();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeContext ------------------------------------------------------------------

AslParser::TypeContext::TypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

AslParser::Basic_typeContext* AslParser::TypeContext::basic_type() {
  return getRuleContext<AslParser::Basic_typeContext>(0);
}

AslParser::Array_typeContext* AslParser::TypeContext::array_type() {
  return getRuleContext<AslParser::Array_typeContext>(0);
}

AslParser::Tuple_typeContext* AslParser::TypeContext::tuple_type() {
  return getRuleContext<AslParser::Tuple_typeContext>(0);
}


size_t AslParser::TypeContext::getRuleIndex() const {
  return AslParser::RuleType;
}

antlrcpp::Any AslParser::TypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitType(this);
  else
    return visitor->visitChildren(this);
}

AslParser::TypeContext* AslParser::type() {
  TypeContext *_localctx = _tracker.createInstance<TypeContext>(_ctx, getState());
  enterRule(_localctx, 12, AslParser::RuleType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(87);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case AslParser::INT:
      case AslParser::FLOAT:
      case AslParser::BOOL:
      case AslParser::CHAR: {
        enterOuterAlt(_localctx, 1);
        setState(84);
        basic_type();
        break;
      }

      case AslParser::ARRAY: {
        enterOuterAlt(_localctx, 2);
        setState(85);
        array_type();
        break;
      }

      case AslParser::KEYL: {
        enterOuterAlt(_localctx, 3);
        setState(86);
        tuple_type();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Basic_typeContext ------------------------------------------------------------------

AslParser::Basic_typeContext::Basic_typeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AslParser::Basic_typeContext::INT() {
  return getToken(AslParser::INT, 0);
}

tree::TerminalNode* AslParser::Basic_typeContext::FLOAT() {
  return getToken(AslParser::FLOAT, 0);
}

tree::TerminalNode* AslParser::Basic_typeContext::BOOL() {
  return getToken(AslParser::BOOL, 0);
}

tree::TerminalNode* AslParser::Basic_typeContext::CHAR() {
  return getToken(AslParser::CHAR, 0);
}


size_t AslParser::Basic_typeContext::getRuleIndex() const {
  return AslParser::RuleBasic_type;
}

antlrcpp::Any AslParser::Basic_typeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitBasic_type(this);
  else
    return visitor->visitChildren(this);
}

AslParser::Basic_typeContext* AslParser::basic_type() {
  Basic_typeContext *_localctx = _tracker.createInstance<Basic_typeContext>(_ctx, getState());
  enterRule(_localctx, 14, AslParser::RuleBasic_type);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(89);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << AslParser::INT)
      | (1ULL << AslParser::FLOAT)
      | (1ULL << AslParser::BOOL)
      | (1ULL << AslParser::CHAR))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Array_typeContext ------------------------------------------------------------------

AslParser::Array_typeContext::Array_typeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AslParser::Array_typeContext::ARRAY() {
  return getToken(AslParser::ARRAY, 0);
}

tree::TerminalNode* AslParser::Array_typeContext::LSQ() {
  return getToken(AslParser::LSQ, 0);
}

tree::TerminalNode* AslParser::Array_typeContext::INTVAL() {
  return getToken(AslParser::INTVAL, 0);
}

tree::TerminalNode* AslParser::Array_typeContext::RSQ() {
  return getToken(AslParser::RSQ, 0);
}

tree::TerminalNode* AslParser::Array_typeContext::OF() {
  return getToken(AslParser::OF, 0);
}

AslParser::Basic_typeContext* AslParser::Array_typeContext::basic_type() {
  return getRuleContext<AslParser::Basic_typeContext>(0);
}


size_t AslParser::Array_typeContext::getRuleIndex() const {
  return AslParser::RuleArray_type;
}

antlrcpp::Any AslParser::Array_typeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitArray_type(this);
  else
    return visitor->visitChildren(this);
}

AslParser::Array_typeContext* AslParser::array_type() {
  Array_typeContext *_localctx = _tracker.createInstance<Array_typeContext>(_ctx, getState());
  enterRule(_localctx, 16, AslParser::RuleArray_type);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(91);
    match(AslParser::ARRAY);
    setState(92);
    match(AslParser::LSQ);
    setState(93);
    match(AslParser::INTVAL);
    setState(94);
    match(AslParser::RSQ);
    setState(95);
    match(AslParser::OF);
    setState(96);
    basic_type();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Tuple_typeContext ------------------------------------------------------------------

AslParser::Tuple_typeContext::Tuple_typeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AslParser::Tuple_typeContext::KEYL() {
  return getToken(AslParser::KEYL, 0);
}

std::vector<AslParser::Basic_typeContext *> AslParser::Tuple_typeContext::basic_type() {
  return getRuleContexts<AslParser::Basic_typeContext>();
}

AslParser::Basic_typeContext* AslParser::Tuple_typeContext::basic_type(size_t i) {
  return getRuleContext<AslParser::Basic_typeContext>(i);
}

tree::TerminalNode* AslParser::Tuple_typeContext::KEYR() {
  return getToken(AslParser::KEYR, 0);
}

std::vector<tree::TerminalNode *> AslParser::Tuple_typeContext::COMMA() {
  return getTokens(AslParser::COMMA);
}

tree::TerminalNode* AslParser::Tuple_typeContext::COMMA(size_t i) {
  return getToken(AslParser::COMMA, i);
}


size_t AslParser::Tuple_typeContext::getRuleIndex() const {
  return AslParser::RuleTuple_type;
}

antlrcpp::Any AslParser::Tuple_typeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitTuple_type(this);
  else
    return visitor->visitChildren(this);
}

AslParser::Tuple_typeContext* AslParser::tuple_type() {
  Tuple_typeContext *_localctx = _tracker.createInstance<Tuple_typeContext>(_ctx, getState());
  enterRule(_localctx, 18, AslParser::RuleTuple_type);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(98);
    match(AslParser::KEYL);
    setState(99);
    basic_type();
    setState(104);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == AslParser::COMMA) {
      setState(100);
      match(AslParser::COMMA);
      setState(101);
      basic_type();
      setState(106);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(107);
    match(AslParser::KEYR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementsContext ------------------------------------------------------------------

AslParser::StatementsContext::StatementsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<AslParser::StatementContext *> AslParser::StatementsContext::statement() {
  return getRuleContexts<AslParser::StatementContext>();
}

AslParser::StatementContext* AslParser::StatementsContext::statement(size_t i) {
  return getRuleContext<AslParser::StatementContext>(i);
}


size_t AslParser::StatementsContext::getRuleIndex() const {
  return AslParser::RuleStatements;
}

antlrcpp::Any AslParser::StatementsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitStatements(this);
  else
    return visitor->visitChildren(this);
}

AslParser::StatementsContext* AslParser::statements() {
  StatementsContext *_localctx = _tracker.createInstance<StatementsContext>(_ctx, getState());
  enterRule(_localctx, 20, AslParser::RuleStatements);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(112);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 42) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 42)) & ((1ULL << (AslParser::IF - 42))
      | (1ULL << (AslParser::WHILE - 42))
      | (1ULL << (AslParser::READ - 42))
      | (1ULL << (AslParser::WRITE - 42))
      | (1ULL << (AslParser::RETURN - 42))
      | (1ULL << (AslParser::FOR - 42))
      | (1ULL << (AslParser::ID - 42)))) != 0)) {
      setState(109);
      statement();
      setState(114);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

AslParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t AslParser::StatementContext::getRuleIndex() const {
  return AslParser::RuleStatement;
}

void AslParser::StatementContext::copyFrom(StatementContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ForStmtContext ------------------------------------------------------------------

tree::TerminalNode* AslParser::ForStmtContext::FOR() {
  return getToken(AslParser::FOR, 0);
}

AslParser::IdentContext* AslParser::ForStmtContext::ident() {
  return getRuleContext<AslParser::IdentContext>(0);
}

tree::TerminalNode* AslParser::ForStmtContext::IN() {
  return getToken(AslParser::IN, 0);
}

tree::TerminalNode* AslParser::ForStmtContext::RANGE() {
  return getToken(AslParser::RANGE, 0);
}

tree::TerminalNode* AslParser::ForStmtContext::LP() {
  return getToken(AslParser::LP, 0);
}

tree::TerminalNode* AslParser::ForStmtContext::RP() {
  return getToken(AslParser::RP, 0);
}

tree::TerminalNode* AslParser::ForStmtContext::DO() {
  return getToken(AslParser::DO, 0);
}

AslParser::StatementsContext* AslParser::ForStmtContext::statements() {
  return getRuleContext<AslParser::StatementsContext>(0);
}

tree::TerminalNode* AslParser::ForStmtContext::ENDFOR() {
  return getToken(AslParser::ENDFOR, 0);
}

std::vector<AslParser::ExprContext *> AslParser::ForStmtContext::expr() {
  return getRuleContexts<AslParser::ExprContext>();
}

AslParser::ExprContext* AslParser::ForStmtContext::expr(size_t i) {
  return getRuleContext<AslParser::ExprContext>(i);
}

std::vector<tree::TerminalNode *> AslParser::ForStmtContext::COMMA() {
  return getTokens(AslParser::COMMA);
}

tree::TerminalNode* AslParser::ForStmtContext::COMMA(size_t i) {
  return getToken(AslParser::COMMA, i);
}

AslParser::ForStmtContext::ForStmtContext(StatementContext *ctx) { copyFrom(ctx); }

antlrcpp::Any AslParser::ForStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitForStmt(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ProcCallContext ------------------------------------------------------------------

AslParser::IdentContext* AslParser::ProcCallContext::ident() {
  return getRuleContext<AslParser::IdentContext>(0);
}

tree::TerminalNode* AslParser::ProcCallContext::LP() {
  return getToken(AslParser::LP, 0);
}

tree::TerminalNode* AslParser::ProcCallContext::RP() {
  return getToken(AslParser::RP, 0);
}

tree::TerminalNode* AslParser::ProcCallContext::SCOLON() {
  return getToken(AslParser::SCOLON, 0);
}

std::vector<AslParser::ExprContext *> AslParser::ProcCallContext::expr() {
  return getRuleContexts<AslParser::ExprContext>();
}

AslParser::ExprContext* AslParser::ProcCallContext::expr(size_t i) {
  return getRuleContext<AslParser::ExprContext>(i);
}

std::vector<tree::TerminalNode *> AslParser::ProcCallContext::COMMA() {
  return getTokens(AslParser::COMMA);
}

tree::TerminalNode* AslParser::ProcCallContext::COMMA(size_t i) {
  return getToken(AslParser::COMMA, i);
}

AslParser::ProcCallContext::ProcCallContext(StatementContext *ctx) { copyFrom(ctx); }

antlrcpp::Any AslParser::ProcCallContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitProcCall(this);
  else
    return visitor->visitChildren(this);
}
//----------------- WriteExprContext ------------------------------------------------------------------

tree::TerminalNode* AslParser::WriteExprContext::WRITE() {
  return getToken(AslParser::WRITE, 0);
}

AslParser::ExprContext* AslParser::WriteExprContext::expr() {
  return getRuleContext<AslParser::ExprContext>(0);
}

tree::TerminalNode* AslParser::WriteExprContext::SCOLON() {
  return getToken(AslParser::SCOLON, 0);
}

AslParser::WriteExprContext::WriteExprContext(StatementContext *ctx) { copyFrom(ctx); }

antlrcpp::Any AslParser::WriteExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitWriteExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- WhileStmtContext ------------------------------------------------------------------

tree::TerminalNode* AslParser::WhileStmtContext::WHILE() {
  return getToken(AslParser::WHILE, 0);
}

AslParser::ExprContext* AslParser::WhileStmtContext::expr() {
  return getRuleContext<AslParser::ExprContext>(0);
}

tree::TerminalNode* AslParser::WhileStmtContext::DO() {
  return getToken(AslParser::DO, 0);
}

AslParser::StatementsContext* AslParser::WhileStmtContext::statements() {
  return getRuleContext<AslParser::StatementsContext>(0);
}

tree::TerminalNode* AslParser::WhileStmtContext::ENDWHILE() {
  return getToken(AslParser::ENDWHILE, 0);
}

AslParser::WhileStmtContext::WhileStmtContext(StatementContext *ctx) { copyFrom(ctx); }

antlrcpp::Any AslParser::WhileStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitWhileStmt(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IfStmtContext ------------------------------------------------------------------

tree::TerminalNode* AslParser::IfStmtContext::IF() {
  return getToken(AslParser::IF, 0);
}

AslParser::ExprContext* AslParser::IfStmtContext::expr() {
  return getRuleContext<AslParser::ExprContext>(0);
}

tree::TerminalNode* AslParser::IfStmtContext::THEN() {
  return getToken(AslParser::THEN, 0);
}

std::vector<AslParser::StatementsContext *> AslParser::IfStmtContext::statements() {
  return getRuleContexts<AslParser::StatementsContext>();
}

AslParser::StatementsContext* AslParser::IfStmtContext::statements(size_t i) {
  return getRuleContext<AslParser::StatementsContext>(i);
}

tree::TerminalNode* AslParser::IfStmtContext::ENDIF() {
  return getToken(AslParser::ENDIF, 0);
}

tree::TerminalNode* AslParser::IfStmtContext::ELSE() {
  return getToken(AslParser::ELSE, 0);
}

AslParser::IfStmtContext::IfStmtContext(StatementContext *ctx) { copyFrom(ctx); }

antlrcpp::Any AslParser::IfStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitIfStmt(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ReadStmtContext ------------------------------------------------------------------

tree::TerminalNode* AslParser::ReadStmtContext::READ() {
  return getToken(AslParser::READ, 0);
}

AslParser::Left_exprContext* AslParser::ReadStmtContext::left_expr() {
  return getRuleContext<AslParser::Left_exprContext>(0);
}

tree::TerminalNode* AslParser::ReadStmtContext::SCOLON() {
  return getToken(AslParser::SCOLON, 0);
}

AslParser::ReadStmtContext::ReadStmtContext(StatementContext *ctx) { copyFrom(ctx); }

antlrcpp::Any AslParser::ReadStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitReadStmt(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AssignStmtContext ------------------------------------------------------------------

AslParser::Left_exprContext* AslParser::AssignStmtContext::left_expr() {
  return getRuleContext<AslParser::Left_exprContext>(0);
}

tree::TerminalNode* AslParser::AssignStmtContext::ASSIGN() {
  return getToken(AslParser::ASSIGN, 0);
}

AslParser::ExprContext* AslParser::AssignStmtContext::expr() {
  return getRuleContext<AslParser::ExprContext>(0);
}

tree::TerminalNode* AslParser::AssignStmtContext::SCOLON() {
  return getToken(AslParser::SCOLON, 0);
}

AslParser::AssignStmtContext::AssignStmtContext(StatementContext *ctx) { copyFrom(ctx); }

antlrcpp::Any AslParser::AssignStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitAssignStmt(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ReturnStmtContext ------------------------------------------------------------------

tree::TerminalNode* AslParser::ReturnStmtContext::RETURN() {
  return getToken(AslParser::RETURN, 0);
}

tree::TerminalNode* AslParser::ReturnStmtContext::SCOLON() {
  return getToken(AslParser::SCOLON, 0);
}

AslParser::ExprContext* AslParser::ReturnStmtContext::expr() {
  return getRuleContext<AslParser::ExprContext>(0);
}

AslParser::ReturnStmtContext::ReturnStmtContext(StatementContext *ctx) { copyFrom(ctx); }

antlrcpp::Any AslParser::ReturnStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitReturnStmt(this);
  else
    return visitor->visitChildren(this);
}
//----------------- WriteStringContext ------------------------------------------------------------------

tree::TerminalNode* AslParser::WriteStringContext::WRITE() {
  return getToken(AslParser::WRITE, 0);
}

tree::TerminalNode* AslParser::WriteStringContext::STRING() {
  return getToken(AslParser::STRING, 0);
}

tree::TerminalNode* AslParser::WriteStringContext::SCOLON() {
  return getToken(AslParser::SCOLON, 0);
}

AslParser::WriteStringContext::WriteStringContext(StatementContext *ctx) { copyFrom(ctx); }

antlrcpp::Any AslParser::WriteStringContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitWriteString(this);
  else
    return visitor->visitChildren(this);
}
AslParser::StatementContext* AslParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 22, AslParser::RuleStatement);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(185);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx)) {
    case 1: {
      _localctx = dynamic_cast<StatementContext *>(_tracker.createInstance<AslParser::AssignStmtContext>(_localctx));
      enterOuterAlt(_localctx, 1);
      setState(115);
      left_expr();
      setState(116);
      match(AslParser::ASSIGN);
      setState(117);
      expr(0);
      setState(118);
      match(AslParser::SCOLON);
      break;
    }

    case 2: {
      _localctx = dynamic_cast<StatementContext *>(_tracker.createInstance<AslParser::IfStmtContext>(_localctx));
      enterOuterAlt(_localctx, 2);
      setState(120);
      match(AslParser::IF);
      setState(121);
      expr(0);
      setState(122);
      match(AslParser::THEN);
      setState(123);
      statements();
      setState(126);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == AslParser::ELSE) {
        setState(124);
        match(AslParser::ELSE);
        setState(125);
        statements();
      }
      setState(128);
      match(AslParser::ENDIF);
      break;
    }

    case 3: {
      _localctx = dynamic_cast<StatementContext *>(_tracker.createInstance<AslParser::WhileStmtContext>(_localctx));
      enterOuterAlt(_localctx, 3);
      setState(130);
      match(AslParser::WHILE);
      setState(131);
      expr(0);
      setState(132);
      match(AslParser::DO);
      setState(133);
      statements();
      setState(134);
      match(AslParser::ENDWHILE);
      break;
    }

    case 4: {
      _localctx = dynamic_cast<StatementContext *>(_tracker.createInstance<AslParser::ForStmtContext>(_localctx));
      enterOuterAlt(_localctx, 4);
      setState(136);
      match(AslParser::FOR);
      setState(137);
      ident();
      setState(138);
      match(AslParser::IN);
      setState(139);
      match(AslParser::RANGE);
      setState(140);
      match(AslParser::LP);

      setState(141);
      expr(0);
      setState(146);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == AslParser::COMMA) {
        setState(142);
        match(AslParser::COMMA);
        setState(143);
        expr(0);
        setState(148);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(149);
      match(AslParser::RP);
      setState(150);
      match(AslParser::DO);
      setState(151);
      statements();
      setState(152);
      match(AslParser::ENDFOR);
      break;
    }

    case 5: {
      _localctx = dynamic_cast<StatementContext *>(_tracker.createInstance<AslParser::ProcCallContext>(_localctx));
      enterOuterAlt(_localctx, 5);
      setState(154);
      ident();
      setState(155);
      match(AslParser::LP);
      setState(164);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << AslParser::INTVAL)
        | (1ULL << AslParser::FLOATVAL)
        | (1ULL << AslParser::BOOLVAL)
        | (1ULL << AslParser::CHARVAL)
        | (1ULL << AslParser::LP)
        | (1ULL << AslParser::PLUS)
        | (1ULL << AslParser::SUB))) != 0) || _la == AslParser::NOT

      || _la == AslParser::ID) {
        setState(156);
        expr(0);
        setState(161);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == AslParser::COMMA) {
          setState(157);
          match(AslParser::COMMA);
          setState(158);
          expr(0);
          setState(163);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(166);
      match(AslParser::RP);
      setState(167);
      match(AslParser::SCOLON);
      break;
    }

    case 6: {
      _localctx = dynamic_cast<StatementContext *>(_tracker.createInstance<AslParser::ReadStmtContext>(_localctx));
      enterOuterAlt(_localctx, 6);
      setState(169);
      match(AslParser::READ);
      setState(170);
      left_expr();
      setState(171);
      match(AslParser::SCOLON);
      break;
    }

    case 7: {
      _localctx = dynamic_cast<StatementContext *>(_tracker.createInstance<AslParser::WriteExprContext>(_localctx));
      enterOuterAlt(_localctx, 7);
      setState(173);
      match(AslParser::WRITE);
      setState(174);
      expr(0);
      setState(175);
      match(AslParser::SCOLON);
      break;
    }

    case 8: {
      _localctx = dynamic_cast<StatementContext *>(_tracker.createInstance<AslParser::WriteStringContext>(_localctx));
      enterOuterAlt(_localctx, 8);
      setState(177);
      match(AslParser::WRITE);
      setState(178);
      match(AslParser::STRING);
      setState(179);
      match(AslParser::SCOLON);
      break;
    }

    case 9: {
      _localctx = dynamic_cast<StatementContext *>(_tracker.createInstance<AslParser::ReturnStmtContext>(_localctx));
      enterOuterAlt(_localctx, 9);
      setState(180);
      match(AslParser::RETURN);
      setState(182);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << AslParser::INTVAL)
        | (1ULL << AslParser::FLOATVAL)
        | (1ULL << AslParser::BOOLVAL)
        | (1ULL << AslParser::CHARVAL)
        | (1ULL << AslParser::LP)
        | (1ULL << AslParser::PLUS)
        | (1ULL << AslParser::SUB))) != 0) || _la == AslParser::NOT

      || _la == AslParser::ID) {
        setState(181);
        expr(0);
      }
      setState(184);
      match(AslParser::SCOLON);
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Left_exprContext ------------------------------------------------------------------

AslParser::Left_exprContext::Left_exprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

AslParser::IdentContext* AslParser::Left_exprContext::ident() {
  return getRuleContext<AslParser::IdentContext>(0);
}

tree::TerminalNode* AslParser::Left_exprContext::LSQ() {
  return getToken(AslParser::LSQ, 0);
}

AslParser::ExprContext* AslParser::Left_exprContext::expr() {
  return getRuleContext<AslParser::ExprContext>(0);
}

tree::TerminalNode* AslParser::Left_exprContext::RSQ() {
  return getToken(AslParser::RSQ, 0);
}

tree::TerminalNode* AslParser::Left_exprContext::KEYL() {
  return getToken(AslParser::KEYL, 0);
}

tree::TerminalNode* AslParser::Left_exprContext::KEYR() {
  return getToken(AslParser::KEYR, 0);
}


size_t AslParser::Left_exprContext::getRuleIndex() const {
  return AslParser::RuleLeft_expr;
}

antlrcpp::Any AslParser::Left_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitLeft_expr(this);
  else
    return visitor->visitChildren(this);
}

AslParser::Left_exprContext* AslParser::left_expr() {
  Left_exprContext *_localctx = _tracker.createInstance<Left_exprContext>(_ctx, getState());
  enterRule(_localctx, 24, AslParser::RuleLeft_expr);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(198);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 15, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(187);
      ident();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(188);
      ident();
      setState(189);
      match(AslParser::LSQ);
      setState(190);
      expr(0);
      setState(191);
      match(AslParser::RSQ);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(193);
      ident();
      setState(194);
      match(AslParser::KEYL);
      setState(195);
      expr(0);
      setState(196);
      match(AslParser::KEYR);
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

AslParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t AslParser::ExprContext::getRuleIndex() const {
  return AslParser::RuleExpr;
}

void AslParser::ExprContext::copyFrom(ExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- TupleContext ------------------------------------------------------------------

AslParser::IdentContext* AslParser::TupleContext::ident() {
  return getRuleContext<AslParser::IdentContext>(0);
}

tree::TerminalNode* AslParser::TupleContext::KEYL() {
  return getToken(AslParser::KEYL, 0);
}

AslParser::ExprContext* AslParser::TupleContext::expr() {
  return getRuleContext<AslParser::ExprContext>(0);
}

tree::TerminalNode* AslParser::TupleContext::KEYR() {
  return getToken(AslParser::KEYR, 0);
}

AslParser::TupleContext::TupleContext(ExprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any AslParser::TupleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitTuple(this);
  else
    return visitor->visitChildren(this);
}
//----------------- FunctionIdentContext ------------------------------------------------------------------

AslParser::IdentContext* AslParser::FunctionIdentContext::ident() {
  return getRuleContext<AslParser::IdentContext>(0);
}

tree::TerminalNode* AslParser::FunctionIdentContext::LP() {
  return getToken(AslParser::LP, 0);
}

tree::TerminalNode* AslParser::FunctionIdentContext::RP() {
  return getToken(AslParser::RP, 0);
}

std::vector<AslParser::ExprContext *> AslParser::FunctionIdentContext::expr() {
  return getRuleContexts<AslParser::ExprContext>();
}

AslParser::ExprContext* AslParser::FunctionIdentContext::expr(size_t i) {
  return getRuleContext<AslParser::ExprContext>(i);
}

std::vector<tree::TerminalNode *> AslParser::FunctionIdentContext::COMMA() {
  return getTokens(AslParser::COMMA);
}

tree::TerminalNode* AslParser::FunctionIdentContext::COMMA(size_t i) {
  return getToken(AslParser::COMMA, i);
}

AslParser::FunctionIdentContext::FunctionIdentContext(ExprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any AslParser::FunctionIdentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitFunctionIdent(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BooleanContext ------------------------------------------------------------------

std::vector<AslParser::ExprContext *> AslParser::BooleanContext::expr() {
  return getRuleContexts<AslParser::ExprContext>();
}

AslParser::ExprContext* AslParser::BooleanContext::expr(size_t i) {
  return getRuleContext<AslParser::ExprContext>(i);
}

tree::TerminalNode* AslParser::BooleanContext::AND() {
  return getToken(AslParser::AND, 0);
}

tree::TerminalNode* AslParser::BooleanContext::OR() {
  return getToken(AslParser::OR, 0);
}

AslParser::BooleanContext::BooleanContext(ExprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any AslParser::BooleanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitBoolean(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ArrayContext ------------------------------------------------------------------

AslParser::IdentContext* AslParser::ArrayContext::ident() {
  return getRuleContext<AslParser::IdentContext>(0);
}

tree::TerminalNode* AslParser::ArrayContext::LSQ() {
  return getToken(AslParser::LSQ, 0);
}

AslParser::ExprContext* AslParser::ArrayContext::expr() {
  return getRuleContext<AslParser::ExprContext>(0);
}

tree::TerminalNode* AslParser::ArrayContext::RSQ() {
  return getToken(AslParser::RSQ, 0);
}

AslParser::ArrayContext::ArrayContext(ExprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any AslParser::ArrayContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitArray(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprIdentContext ------------------------------------------------------------------

AslParser::IdentContext* AslParser::ExprIdentContext::ident() {
  return getRuleContext<AslParser::IdentContext>(0);
}

AslParser::ExprIdentContext::ExprIdentContext(ExprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any AslParser::ExprIdentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitExprIdent(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ArithmeticContext ------------------------------------------------------------------

std::vector<AslParser::ExprContext *> AslParser::ArithmeticContext::expr() {
  return getRuleContexts<AslParser::ExprContext>();
}

AslParser::ExprContext* AslParser::ArithmeticContext::expr(size_t i) {
  return getRuleContext<AslParser::ExprContext>(i);
}

tree::TerminalNode* AslParser::ArithmeticContext::MUL() {
  return getToken(AslParser::MUL, 0);
}

tree::TerminalNode* AslParser::ArithmeticContext::DIV() {
  return getToken(AslParser::DIV, 0);
}

tree::TerminalNode* AslParser::ArithmeticContext::MOD() {
  return getToken(AslParser::MOD, 0);
}

tree::TerminalNode* AslParser::ArithmeticContext::PLUS() {
  return getToken(AslParser::PLUS, 0);
}

tree::TerminalNode* AslParser::ArithmeticContext::SUB() {
  return getToken(AslParser::SUB, 0);
}

AslParser::ArithmeticContext::ArithmeticContext(ExprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any AslParser::ArithmeticContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitArithmetic(this);
  else
    return visitor->visitChildren(this);
}
//----------------- RelationalContext ------------------------------------------------------------------

std::vector<AslParser::ExprContext *> AslParser::RelationalContext::expr() {
  return getRuleContexts<AslParser::ExprContext>();
}

AslParser::ExprContext* AslParser::RelationalContext::expr(size_t i) {
  return getRuleContext<AslParser::ExprContext>(i);
}

tree::TerminalNode* AslParser::RelationalContext::EQUAL() {
  return getToken(AslParser::EQUAL, 0);
}

tree::TerminalNode* AslParser::RelationalContext::NEG() {
  return getToken(AslParser::NEG, 0);
}

tree::TerminalNode* AslParser::RelationalContext::GT() {
  return getToken(AslParser::GT, 0);
}

tree::TerminalNode* AslParser::RelationalContext::GE() {
  return getToken(AslParser::GE, 0);
}

tree::TerminalNode* AslParser::RelationalContext::LE() {
  return getToken(AslParser::LE, 0);
}

tree::TerminalNode* AslParser::RelationalContext::LT() {
  return getToken(AslParser::LT, 0);
}

AslParser::RelationalContext::RelationalContext(ExprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any AslParser::RelationalContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitRelational(this);
  else
    return visitor->visitChildren(this);
}
//----------------- UnaryContext ------------------------------------------------------------------

AslParser::ExprContext* AslParser::UnaryContext::expr() {
  return getRuleContext<AslParser::ExprContext>(0);
}

tree::TerminalNode* AslParser::UnaryContext::PLUS() {
  return getToken(AslParser::PLUS, 0);
}

tree::TerminalNode* AslParser::UnaryContext::SUB() {
  return getToken(AslParser::SUB, 0);
}

tree::TerminalNode* AslParser::UnaryContext::NOT() {
  return getToken(AslParser::NOT, 0);
}

AslParser::UnaryContext::UnaryContext(ExprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any AslParser::UnaryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitUnary(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ParenthesisContext ------------------------------------------------------------------

tree::TerminalNode* AslParser::ParenthesisContext::LP() {
  return getToken(AslParser::LP, 0);
}

AslParser::ExprContext* AslParser::ParenthesisContext::expr() {
  return getRuleContext<AslParser::ExprContext>(0);
}

tree::TerminalNode* AslParser::ParenthesisContext::RP() {
  return getToken(AslParser::RP, 0);
}

AslParser::ParenthesisContext::ParenthesisContext(ExprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any AslParser::ParenthesisContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitParenthesis(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ValueContext ------------------------------------------------------------------

AslParser::ValuesContext* AslParser::ValueContext::values() {
  return getRuleContext<AslParser::ValuesContext>(0);
}

AslParser::ValueContext::ValueContext(ExprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any AslParser::ValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitValue(this);
  else
    return visitor->visitChildren(this);
}

AslParser::ExprContext* AslParser::expr() {
   return expr(0);
}

AslParser::ExprContext* AslParser::expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  AslParser::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
  AslParser::ExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 26;
  enterRecursionRule(_localctx, 26, AslParser::RuleExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(233);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 18, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<ParenthesisContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(201);
      match(AslParser::LP);
      setState(202);
      expr(0);
      setState(203);
      match(AslParser::RP);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<TupleContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(205);
      ident();
      setState(206);
      match(AslParser::KEYL);
      setState(207);
      expr(0);
      setState(208);
      match(AslParser::KEYR);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<ArrayContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(210);
      ident();
      setState(211);
      match(AslParser::LSQ);
      setState(212);
      expr(0);
      setState(213);
      match(AslParser::RSQ);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<FunctionIdentContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(215);
      ident();
      setState(216);
      match(AslParser::LP);
      setState(225);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << AslParser::INTVAL)
        | (1ULL << AslParser::FLOATVAL)
        | (1ULL << AslParser::BOOLVAL)
        | (1ULL << AslParser::CHARVAL)
        | (1ULL << AslParser::LP)
        | (1ULL << AslParser::PLUS)
        | (1ULL << AslParser::SUB))) != 0) || _la == AslParser::NOT

      || _la == AslParser::ID) {
        setState(217);
        expr(0);
        setState(222);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == AslParser::COMMA) {
          setState(218);
          match(AslParser::COMMA);
          setState(219);
          expr(0);
          setState(224);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(227);
      match(AslParser::RP);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<UnaryContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(229);
      dynamic_cast<UnaryContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(((((_la - 31) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 31)) & ((1ULL << (AslParser::PLUS - 31))
        | (1ULL << (AslParser::SUB - 31))
        | (1ULL << (AslParser::NOT - 31)))) != 0))) {
        dynamic_cast<UnaryContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(230);
      expr(8);
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<ValueContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(231);
      values();
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<ExprIdentContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(232);
      ident();
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(252);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 20, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(250);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<ArithmeticContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(235);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(236);
          dynamic_cast<ArithmeticContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << AslParser::MUL)
            | (1ULL << AslParser::DIV)
            | (1ULL << AslParser::MOD))) != 0))) {
            dynamic_cast<ArithmeticContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(237);
          expr(8);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<ArithmeticContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(238);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(239);
          dynamic_cast<ArithmeticContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == AslParser::PLUS

          || _la == AslParser::SUB)) {
            dynamic_cast<ArithmeticContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(240);
          expr(7);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<RelationalContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(241);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(242);
          dynamic_cast<RelationalContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << AslParser::EQUAL)
            | (1ULL << AslParser::NEG)
            | (1ULL << AslParser::GT)
            | (1ULL << AslParser::GE)
            | (1ULL << AslParser::LT)
            | (1ULL << AslParser::LE))) != 0))) {
            dynamic_cast<RelationalContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(243);
          expr(6);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<BooleanContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(244);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(245);
          dynamic_cast<BooleanContext *>(_localctx)->op = match(AslParser::AND);
          setState(246);
          expr(5);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<BooleanContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(247);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(248);
          dynamic_cast<BooleanContext *>(_localctx)->op = match(AslParser::OR);
          setState(249);
          expr(4);
          break;
        }

        } 
      }
      setState(254);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 20, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- ValuesContext ------------------------------------------------------------------

AslParser::ValuesContext::ValuesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AslParser::ValuesContext::INTVAL() {
  return getToken(AslParser::INTVAL, 0);
}

tree::TerminalNode* AslParser::ValuesContext::FLOATVAL() {
  return getToken(AslParser::FLOATVAL, 0);
}

tree::TerminalNode* AslParser::ValuesContext::BOOLVAL() {
  return getToken(AslParser::BOOLVAL, 0);
}

tree::TerminalNode* AslParser::ValuesContext::CHARVAL() {
  return getToken(AslParser::CHARVAL, 0);
}


size_t AslParser::ValuesContext::getRuleIndex() const {
  return AslParser::RuleValues;
}

antlrcpp::Any AslParser::ValuesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitValues(this);
  else
    return visitor->visitChildren(this);
}

AslParser::ValuesContext* AslParser::values() {
  ValuesContext *_localctx = _tracker.createInstance<ValuesContext>(_ctx, getState());
  enterRule(_localctx, 28, AslParser::RuleValues);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(255);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << AslParser::INTVAL)
      | (1ULL << AslParser::FLOATVAL)
      | (1ULL << AslParser::BOOLVAL)
      | (1ULL << AslParser::CHARVAL))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IdentContext ------------------------------------------------------------------

AslParser::IdentContext::IdentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* AslParser::IdentContext::ID() {
  return getToken(AslParser::ID, 0);
}


size_t AslParser::IdentContext::getRuleIndex() const {
  return AslParser::RuleIdent;
}

antlrcpp::Any AslParser::IdentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<AslVisitor*>(visitor))
    return parserVisitor->visitIdent(this);
  else
    return visitor->visitChildren(this);
}

AslParser::IdentContext* AslParser::ident() {
  IdentContext *_localctx = _tracker.createInstance<IdentContext>(_ctx, getState());
  enterRule(_localctx, 30, AslParser::RuleIdent);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(257);
    match(AslParser::ID);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool AslParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 13: return exprSempred(dynamic_cast<ExprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool AslParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 7);
    case 1: return precpred(_ctx, 6);
    case 2: return precpred(_ctx, 5);
    case 3: return precpred(_ctx, 4);
    case 4: return precpred(_ctx, 3);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> AslParser::_decisionToDFA;
atn::PredictionContextCache AslParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN AslParser::_atn;
std::vector<uint16_t> AslParser::_serializedATN;

std::vector<std::string> AslParser::_ruleNames = {
  "program", "function", "parameters", "parameter", "declarations", "variable_decl", 
  "type", "basic_type", "array_type", "tuple_type", "statements", "statement", 
  "left_expr", "expr", "values", "ident"
};

std::vector<std::string> AslParser::_literalNames = {
  "", "", "", "", "", "'**'", "'map'", "'using'", "'sum'", "'filter'", "'.max'", 
  "'='", "'=='", "'!='", "'>'", "'>='", "'<'", "'\"'", "'''", "'<='", "'('", 
  "')'", "'['", "']'", "'{'", "'}'", "':'", "';'", "'.'", "','", "'#'", 
  "'+'", "'-'", "'*'", "'/'", "'%'", "'var'", "'int'", "'float'", "'bool'", 
  "'char'", "'array'", "'if'", "'then'", "'else'", "'endif'", "'while'", 
  "'do'", "'endwhile'", "'func'", "'endfunc'", "'read'", "'write'", "'return'", 
  "'of'", "'for'", "'in'", "'range'", "'endfor'", "'pack'", "'unpack'", 
  "'into'", "'true'", "'false'", "'and'", "'or'", "'not'"
};

std::vector<std::string> AslParser::_symbolicNames = {
  "", "INTVAL", "FLOATVAL", "BOOLVAL", "CHARVAL", "POT", "MAP", "USING", 
  "SUM", "FILTER", "DOTMAX", "ASSIGN", "EQUAL", "NEG", "GT", "GE", "LT", 
  "AP", "AT", "LE", "LP", "RP", "LSQ", "RSQ", "KEYL", "KEYR", "COLON", "SCOLON", 
  "DOT", "COMMA", "HASH", "PLUS", "SUB", "MUL", "DIV", "MOD", "VAR", "INT", 
  "FLOAT", "BOOL", "CHAR", "ARRAY", "IF", "THEN", "ELSE", "ENDIF", "WHILE", 
  "DO", "ENDWHILE", "FUNC", "ENDFUNC", "READ", "WRITE", "RETURN", "OF", 
  "FOR", "IN", "RANGE", "ENDFOR", "PACK", "UNPACK", "INTO", "TRUE", "FALSE", 
  "AND", "OR", "NOT", "ID", "STRING", "COMMENT", "WS"
};

dfa::Vocabulary AslParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> AslParser::_tokenNames;

AslParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x48, 0x106, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
    0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 
    0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 0xb, 
    0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 0xe, 
    0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 0x11, 0x3, 
    0x2, 0x6, 0x2, 0x24, 0xa, 0x2, 0xd, 0x2, 0xe, 0x2, 0x25, 0x3, 0x2, 0x3, 
    0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x5, 0x3, 0x2e, 0xa, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x5, 0x3, 0x33, 0xa, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x7, 0x4, 0x3c, 
    0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0x3f, 0xb, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 
    0x5, 0x3, 0x5, 0x3, 0x6, 0x7, 0x6, 0x46, 0xa, 0x6, 0xc, 0x6, 0xe, 0x6, 
    0x49, 0xb, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x7, 0x7, 0x4f, 
    0xa, 0x7, 0xc, 0x7, 0xe, 0x7, 0x52, 0xb, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 
    0x7, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x5, 0x8, 0x5a, 0xa, 0x8, 0x3, 0x9, 
    0x3, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 
    0x3, 0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x7, 0xb, 0x69, 0xa, 
    0xb, 0xc, 0xb, 0xe, 0xb, 0x6c, 0xb, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xc, 
    0x7, 0xc, 0x71, 0xa, 0xc, 0xc, 0xc, 0xe, 0xc, 0x74, 0xb, 0xc, 0x3, 0xd, 
    0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 
    0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x5, 0xd, 0x81, 0xa, 0xd, 0x3, 0xd, 0x3, 
    0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 
    0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 
    0xd, 0x7, 0xd, 0x93, 0xa, 0xd, 0xc, 0xd, 0xe, 0xd, 0x96, 0xb, 0xd, 0x3, 
    0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 
    0xd, 0x3, 0xd, 0x3, 0xd, 0x7, 0xd, 0xa2, 0xa, 0xd, 0xc, 0xd, 0xe, 0xd, 
    0xa5, 0xb, 0xd, 0x5, 0xd, 0xa7, 0xa, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 
    0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 
    0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x5, 0xd, 
    0xb9, 0xa, 0xd, 0x3, 0xd, 0x5, 0xd, 0xbc, 0xa, 0xd, 0x3, 0xe, 0x3, 0xe, 
    0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 
    0x3, 0xe, 0x3, 0xe, 0x5, 0xe, 0xc9, 0xa, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 
    0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 
    0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 
    0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x7, 0xf, 0xdf, 0xa, 0xf, 0xc, 0xf, 
    0xe, 0xf, 0xe2, 0xb, 0xf, 0x5, 0xf, 0xe4, 0xa, 0xf, 0x3, 0xf, 0x3, 0xf, 
    0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x5, 0xf, 0xec, 0xa, 0xf, 0x3, 
    0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 
    0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 
    0xf, 0x7, 0xf, 0xfd, 0xa, 0xf, 0xc, 0xf, 0xe, 0xf, 0x100, 0xb, 0xf, 
    0x3, 0x10, 0x3, 0x10, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x2, 0x3, 0x1c, 
    0x12, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 
    0x1a, 0x1c, 0x1e, 0x20, 0x2, 0x8, 0x3, 0x2, 0x27, 0x2a, 0x4, 0x2, 0x21, 
    0x22, 0x44, 0x44, 0x3, 0x2, 0x23, 0x25, 0x3, 0x2, 0x21, 0x22, 0x4, 0x2, 
    0xe, 0x12, 0x15, 0x15, 0x3, 0x2, 0x3, 0x6, 0x2, 0x11b, 0x2, 0x23, 0x3, 
    0x2, 0x2, 0x2, 0x4, 0x29, 0x3, 0x2, 0x2, 0x2, 0x6, 0x38, 0x3, 0x2, 0x2, 
    0x2, 0x8, 0x40, 0x3, 0x2, 0x2, 0x2, 0xa, 0x47, 0x3, 0x2, 0x2, 0x2, 0xc, 
    0x4a, 0x3, 0x2, 0x2, 0x2, 0xe, 0x59, 0x3, 0x2, 0x2, 0x2, 0x10, 0x5b, 
    0x3, 0x2, 0x2, 0x2, 0x12, 0x5d, 0x3, 0x2, 0x2, 0x2, 0x14, 0x64, 0x3, 
    0x2, 0x2, 0x2, 0x16, 0x72, 0x3, 0x2, 0x2, 0x2, 0x18, 0xbb, 0x3, 0x2, 
    0x2, 0x2, 0x1a, 0xc8, 0x3, 0x2, 0x2, 0x2, 0x1c, 0xeb, 0x3, 0x2, 0x2, 
    0x2, 0x1e, 0x101, 0x3, 0x2, 0x2, 0x2, 0x20, 0x103, 0x3, 0x2, 0x2, 0x2, 
    0x22, 0x24, 0x5, 0x4, 0x3, 0x2, 0x23, 0x22, 0x3, 0x2, 0x2, 0x2, 0x24, 
    0x25, 0x3, 0x2, 0x2, 0x2, 0x25, 0x23, 0x3, 0x2, 0x2, 0x2, 0x25, 0x26, 
    0x3, 0x2, 0x2, 0x2, 0x26, 0x27, 0x3, 0x2, 0x2, 0x2, 0x27, 0x28, 0x7, 
    0x2, 0x2, 0x3, 0x28, 0x3, 0x3, 0x2, 0x2, 0x2, 0x29, 0x2a, 0x7, 0x33, 
    0x2, 0x2, 0x2a, 0x2b, 0x7, 0x45, 0x2, 0x2, 0x2b, 0x2d, 0x7, 0x16, 0x2, 
    0x2, 0x2c, 0x2e, 0x5, 0x6, 0x4, 0x2, 0x2d, 0x2c, 0x3, 0x2, 0x2, 0x2, 
    0x2d, 0x2e, 0x3, 0x2, 0x2, 0x2, 0x2e, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x2f, 
    0x32, 0x7, 0x17, 0x2, 0x2, 0x30, 0x31, 0x7, 0x1c, 0x2, 0x2, 0x31, 0x33, 
    0x5, 0x10, 0x9, 0x2, 0x32, 0x30, 0x3, 0x2, 0x2, 0x2, 0x32, 0x33, 0x3, 
    0x2, 0x2, 0x2, 0x33, 0x34, 0x3, 0x2, 0x2, 0x2, 0x34, 0x35, 0x5, 0xa, 
    0x6, 0x2, 0x35, 0x36, 0x5, 0x16, 0xc, 0x2, 0x36, 0x37, 0x7, 0x34, 0x2, 
    0x2, 0x37, 0x5, 0x3, 0x2, 0x2, 0x2, 0x38, 0x3d, 0x5, 0x8, 0x5, 0x2, 
    0x39, 0x3a, 0x7, 0x1f, 0x2, 0x2, 0x3a, 0x3c, 0x5, 0x8, 0x5, 0x2, 0x3b, 
    0x39, 0x3, 0x2, 0x2, 0x2, 0x3c, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x3d, 0x3b, 
    0x3, 0x2, 0x2, 0x2, 0x3d, 0x3e, 0x3, 0x2, 0x2, 0x2, 0x3e, 0x7, 0x3, 
    0x2, 0x2, 0x2, 0x3f, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x40, 0x41, 0x7, 0x45, 
    0x2, 0x2, 0x41, 0x42, 0x7, 0x1c, 0x2, 0x2, 0x42, 0x43, 0x5, 0xe, 0x8, 
    0x2, 0x43, 0x9, 0x3, 0x2, 0x2, 0x2, 0x44, 0x46, 0x5, 0xc, 0x7, 0x2, 
    0x45, 0x44, 0x3, 0x2, 0x2, 0x2, 0x46, 0x49, 0x3, 0x2, 0x2, 0x2, 0x47, 
    0x45, 0x3, 0x2, 0x2, 0x2, 0x47, 0x48, 0x3, 0x2, 0x2, 0x2, 0x48, 0xb, 
    0x3, 0x2, 0x2, 0x2, 0x49, 0x47, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x4b, 0x7, 
    0x26, 0x2, 0x2, 0x4b, 0x50, 0x7, 0x45, 0x2, 0x2, 0x4c, 0x4d, 0x7, 0x1f, 
    0x2, 0x2, 0x4d, 0x4f, 0x7, 0x45, 0x2, 0x2, 0x4e, 0x4c, 0x3, 0x2, 0x2, 
    0x2, 0x4f, 0x52, 0x3, 0x2, 0x2, 0x2, 0x50, 0x4e, 0x3, 0x2, 0x2, 0x2, 
    0x50, 0x51, 0x3, 0x2, 0x2, 0x2, 0x51, 0x53, 0x3, 0x2, 0x2, 0x2, 0x52, 
    0x50, 0x3, 0x2, 0x2, 0x2, 0x53, 0x54, 0x7, 0x1c, 0x2, 0x2, 0x54, 0x55, 
    0x5, 0xe, 0x8, 0x2, 0x55, 0xd, 0x3, 0x2, 0x2, 0x2, 0x56, 0x5a, 0x5, 
    0x10, 0x9, 0x2, 0x57, 0x5a, 0x5, 0x12, 0xa, 0x2, 0x58, 0x5a, 0x5, 0x14, 
    0xb, 0x2, 0x59, 0x56, 0x3, 0x2, 0x2, 0x2, 0x59, 0x57, 0x3, 0x2, 0x2, 
    0x2, 0x59, 0x58, 0x3, 0x2, 0x2, 0x2, 0x5a, 0xf, 0x3, 0x2, 0x2, 0x2, 
    0x5b, 0x5c, 0x9, 0x2, 0x2, 0x2, 0x5c, 0x11, 0x3, 0x2, 0x2, 0x2, 0x5d, 
    0x5e, 0x7, 0x2b, 0x2, 0x2, 0x5e, 0x5f, 0x7, 0x18, 0x2, 0x2, 0x5f, 0x60, 
    0x7, 0x3, 0x2, 0x2, 0x60, 0x61, 0x7, 0x19, 0x2, 0x2, 0x61, 0x62, 0x7, 
    0x38, 0x2, 0x2, 0x62, 0x63, 0x5, 0x10, 0x9, 0x2, 0x63, 0x13, 0x3, 0x2, 
    0x2, 0x2, 0x64, 0x65, 0x7, 0x1a, 0x2, 0x2, 0x65, 0x6a, 0x5, 0x10, 0x9, 
    0x2, 0x66, 0x67, 0x7, 0x1f, 0x2, 0x2, 0x67, 0x69, 0x5, 0x10, 0x9, 0x2, 
    0x68, 0x66, 0x3, 0x2, 0x2, 0x2, 0x69, 0x6c, 0x3, 0x2, 0x2, 0x2, 0x6a, 
    0x68, 0x3, 0x2, 0x2, 0x2, 0x6a, 0x6b, 0x3, 0x2, 0x2, 0x2, 0x6b, 0x6d, 
    0x3, 0x2, 0x2, 0x2, 0x6c, 0x6a, 0x3, 0x2, 0x2, 0x2, 0x6d, 0x6e, 0x7, 
    0x1b, 0x2, 0x2, 0x6e, 0x15, 0x3, 0x2, 0x2, 0x2, 0x6f, 0x71, 0x5, 0x18, 
    0xd, 0x2, 0x70, 0x6f, 0x3, 0x2, 0x2, 0x2, 0x71, 0x74, 0x3, 0x2, 0x2, 
    0x2, 0x72, 0x70, 0x3, 0x2, 0x2, 0x2, 0x72, 0x73, 0x3, 0x2, 0x2, 0x2, 
    0x73, 0x17, 0x3, 0x2, 0x2, 0x2, 0x74, 0x72, 0x3, 0x2, 0x2, 0x2, 0x75, 
    0x76, 0x5, 0x1a, 0xe, 0x2, 0x76, 0x77, 0x7, 0xd, 0x2, 0x2, 0x77, 0x78, 
    0x5, 0x1c, 0xf, 0x2, 0x78, 0x79, 0x7, 0x1d, 0x2, 0x2, 0x79, 0xbc, 0x3, 
    0x2, 0x2, 0x2, 0x7a, 0x7b, 0x7, 0x2c, 0x2, 0x2, 0x7b, 0x7c, 0x5, 0x1c, 
    0xf, 0x2, 0x7c, 0x7d, 0x7, 0x2d, 0x2, 0x2, 0x7d, 0x80, 0x5, 0x16, 0xc, 
    0x2, 0x7e, 0x7f, 0x7, 0x2e, 0x2, 0x2, 0x7f, 0x81, 0x5, 0x16, 0xc, 0x2, 
    0x80, 0x7e, 0x3, 0x2, 0x2, 0x2, 0x80, 0x81, 0x3, 0x2, 0x2, 0x2, 0x81, 
    0x82, 0x3, 0x2, 0x2, 0x2, 0x82, 0x83, 0x7, 0x2f, 0x2, 0x2, 0x83, 0xbc, 
    0x3, 0x2, 0x2, 0x2, 0x84, 0x85, 0x7, 0x30, 0x2, 0x2, 0x85, 0x86, 0x5, 
    0x1c, 0xf, 0x2, 0x86, 0x87, 0x7, 0x31, 0x2, 0x2, 0x87, 0x88, 0x5, 0x16, 
    0xc, 0x2, 0x88, 0x89, 0x7, 0x32, 0x2, 0x2, 0x89, 0xbc, 0x3, 0x2, 0x2, 
    0x2, 0x8a, 0x8b, 0x7, 0x39, 0x2, 0x2, 0x8b, 0x8c, 0x5, 0x20, 0x11, 0x2, 
    0x8c, 0x8d, 0x7, 0x3a, 0x2, 0x2, 0x8d, 0x8e, 0x7, 0x3b, 0x2, 0x2, 0x8e, 
    0x8f, 0x7, 0x16, 0x2, 0x2, 0x8f, 0x94, 0x5, 0x1c, 0xf, 0x2, 0x90, 0x91, 
    0x7, 0x1f, 0x2, 0x2, 0x91, 0x93, 0x5, 0x1c, 0xf, 0x2, 0x92, 0x90, 0x3, 
    0x2, 0x2, 0x2, 0x93, 0x96, 0x3, 0x2, 0x2, 0x2, 0x94, 0x92, 0x3, 0x2, 
    0x2, 0x2, 0x94, 0x95, 0x3, 0x2, 0x2, 0x2, 0x95, 0x97, 0x3, 0x2, 0x2, 
    0x2, 0x96, 0x94, 0x3, 0x2, 0x2, 0x2, 0x97, 0x98, 0x7, 0x17, 0x2, 0x2, 
    0x98, 0x99, 0x7, 0x31, 0x2, 0x2, 0x99, 0x9a, 0x5, 0x16, 0xc, 0x2, 0x9a, 
    0x9b, 0x7, 0x3c, 0x2, 0x2, 0x9b, 0xbc, 0x3, 0x2, 0x2, 0x2, 0x9c, 0x9d, 
    0x5, 0x20, 0x11, 0x2, 0x9d, 0xa6, 0x7, 0x16, 0x2, 0x2, 0x9e, 0xa3, 0x5, 
    0x1c, 0xf, 0x2, 0x9f, 0xa0, 0x7, 0x1f, 0x2, 0x2, 0xa0, 0xa2, 0x5, 0x1c, 
    0xf, 0x2, 0xa1, 0x9f, 0x3, 0x2, 0x2, 0x2, 0xa2, 0xa5, 0x3, 0x2, 0x2, 
    0x2, 0xa3, 0xa1, 0x3, 0x2, 0x2, 0x2, 0xa3, 0xa4, 0x3, 0x2, 0x2, 0x2, 
    0xa4, 0xa7, 0x3, 0x2, 0x2, 0x2, 0xa5, 0xa3, 0x3, 0x2, 0x2, 0x2, 0xa6, 
    0x9e, 0x3, 0x2, 0x2, 0x2, 0xa6, 0xa7, 0x3, 0x2, 0x2, 0x2, 0xa7, 0xa8, 
    0x3, 0x2, 0x2, 0x2, 0xa8, 0xa9, 0x7, 0x17, 0x2, 0x2, 0xa9, 0xaa, 0x7, 
    0x1d, 0x2, 0x2, 0xaa, 0xbc, 0x3, 0x2, 0x2, 0x2, 0xab, 0xac, 0x7, 0x35, 
    0x2, 0x2, 0xac, 0xad, 0x5, 0x1a, 0xe, 0x2, 0xad, 0xae, 0x7, 0x1d, 0x2, 
    0x2, 0xae, 0xbc, 0x3, 0x2, 0x2, 0x2, 0xaf, 0xb0, 0x7, 0x36, 0x2, 0x2, 
    0xb0, 0xb1, 0x5, 0x1c, 0xf, 0x2, 0xb1, 0xb2, 0x7, 0x1d, 0x2, 0x2, 0xb2, 
    0xbc, 0x3, 0x2, 0x2, 0x2, 0xb3, 0xb4, 0x7, 0x36, 0x2, 0x2, 0xb4, 0xb5, 
    0x7, 0x46, 0x2, 0x2, 0xb5, 0xbc, 0x7, 0x1d, 0x2, 0x2, 0xb6, 0xb8, 0x7, 
    0x37, 0x2, 0x2, 0xb7, 0xb9, 0x5, 0x1c, 0xf, 0x2, 0xb8, 0xb7, 0x3, 0x2, 
    0x2, 0x2, 0xb8, 0xb9, 0x3, 0x2, 0x2, 0x2, 0xb9, 0xba, 0x3, 0x2, 0x2, 
    0x2, 0xba, 0xbc, 0x7, 0x1d, 0x2, 0x2, 0xbb, 0x75, 0x3, 0x2, 0x2, 0x2, 
    0xbb, 0x7a, 0x3, 0x2, 0x2, 0x2, 0xbb, 0x84, 0x3, 0x2, 0x2, 0x2, 0xbb, 
    0x8a, 0x3, 0x2, 0x2, 0x2, 0xbb, 0x9c, 0x3, 0x2, 0x2, 0x2, 0xbb, 0xab, 
    0x3, 0x2, 0x2, 0x2, 0xbb, 0xaf, 0x3, 0x2, 0x2, 0x2, 0xbb, 0xb3, 0x3, 
    0x2, 0x2, 0x2, 0xbb, 0xb6, 0x3, 0x2, 0x2, 0x2, 0xbc, 0x19, 0x3, 0x2, 
    0x2, 0x2, 0xbd, 0xc9, 0x5, 0x20, 0x11, 0x2, 0xbe, 0xbf, 0x5, 0x20, 0x11, 
    0x2, 0xbf, 0xc0, 0x7, 0x18, 0x2, 0x2, 0xc0, 0xc1, 0x5, 0x1c, 0xf, 0x2, 
    0xc1, 0xc2, 0x7, 0x19, 0x2, 0x2, 0xc2, 0xc9, 0x3, 0x2, 0x2, 0x2, 0xc3, 
    0xc4, 0x5, 0x20, 0x11, 0x2, 0xc4, 0xc5, 0x7, 0x1a, 0x2, 0x2, 0xc5, 0xc6, 
    0x5, 0x1c, 0xf, 0x2, 0xc6, 0xc7, 0x7, 0x1b, 0x2, 0x2, 0xc7, 0xc9, 0x3, 
    0x2, 0x2, 0x2, 0xc8, 0xbd, 0x3, 0x2, 0x2, 0x2, 0xc8, 0xbe, 0x3, 0x2, 
    0x2, 0x2, 0xc8, 0xc3, 0x3, 0x2, 0x2, 0x2, 0xc9, 0x1b, 0x3, 0x2, 0x2, 
    0x2, 0xca, 0xcb, 0x8, 0xf, 0x1, 0x2, 0xcb, 0xcc, 0x7, 0x16, 0x2, 0x2, 
    0xcc, 0xcd, 0x5, 0x1c, 0xf, 0x2, 0xcd, 0xce, 0x7, 0x17, 0x2, 0x2, 0xce, 
    0xec, 0x3, 0x2, 0x2, 0x2, 0xcf, 0xd0, 0x5, 0x20, 0x11, 0x2, 0xd0, 0xd1, 
    0x7, 0x1a, 0x2, 0x2, 0xd1, 0xd2, 0x5, 0x1c, 0xf, 0x2, 0xd2, 0xd3, 0x7, 
    0x1b, 0x2, 0x2, 0xd3, 0xec, 0x3, 0x2, 0x2, 0x2, 0xd4, 0xd5, 0x5, 0x20, 
    0x11, 0x2, 0xd5, 0xd6, 0x7, 0x18, 0x2, 0x2, 0xd6, 0xd7, 0x5, 0x1c, 0xf, 
    0x2, 0xd7, 0xd8, 0x7, 0x19, 0x2, 0x2, 0xd8, 0xec, 0x3, 0x2, 0x2, 0x2, 
    0xd9, 0xda, 0x5, 0x20, 0x11, 0x2, 0xda, 0xe3, 0x7, 0x16, 0x2, 0x2, 0xdb, 
    0xe0, 0x5, 0x1c, 0xf, 0x2, 0xdc, 0xdd, 0x7, 0x1f, 0x2, 0x2, 0xdd, 0xdf, 
    0x5, 0x1c, 0xf, 0x2, 0xde, 0xdc, 0x3, 0x2, 0x2, 0x2, 0xdf, 0xe2, 0x3, 
    0x2, 0x2, 0x2, 0xe0, 0xde, 0x3, 0x2, 0x2, 0x2, 0xe0, 0xe1, 0x3, 0x2, 
    0x2, 0x2, 0xe1, 0xe4, 0x3, 0x2, 0x2, 0x2, 0xe2, 0xe0, 0x3, 0x2, 0x2, 
    0x2, 0xe3, 0xdb, 0x3, 0x2, 0x2, 0x2, 0xe3, 0xe4, 0x3, 0x2, 0x2, 0x2, 
    0xe4, 0xe5, 0x3, 0x2, 0x2, 0x2, 0xe5, 0xe6, 0x7, 0x17, 0x2, 0x2, 0xe6, 
    0xec, 0x3, 0x2, 0x2, 0x2, 0xe7, 0xe8, 0x9, 0x3, 0x2, 0x2, 0xe8, 0xec, 
    0x5, 0x1c, 0xf, 0xa, 0xe9, 0xec, 0x5, 0x1e, 0x10, 0x2, 0xea, 0xec, 0x5, 
    0x20, 0x11, 0x2, 0xeb, 0xca, 0x3, 0x2, 0x2, 0x2, 0xeb, 0xcf, 0x3, 0x2, 
    0x2, 0x2, 0xeb, 0xd4, 0x3, 0x2, 0x2, 0x2, 0xeb, 0xd9, 0x3, 0x2, 0x2, 
    0x2, 0xeb, 0xe7, 0x3, 0x2, 0x2, 0x2, 0xeb, 0xe9, 0x3, 0x2, 0x2, 0x2, 
    0xeb, 0xea, 0x3, 0x2, 0x2, 0x2, 0xec, 0xfe, 0x3, 0x2, 0x2, 0x2, 0xed, 
    0xee, 0xc, 0x9, 0x2, 0x2, 0xee, 0xef, 0x9, 0x4, 0x2, 0x2, 0xef, 0xfd, 
    0x5, 0x1c, 0xf, 0xa, 0xf0, 0xf1, 0xc, 0x8, 0x2, 0x2, 0xf1, 0xf2, 0x9, 
    0x5, 0x2, 0x2, 0xf2, 0xfd, 0x5, 0x1c, 0xf, 0x9, 0xf3, 0xf4, 0xc, 0x7, 
    0x2, 0x2, 0xf4, 0xf5, 0x9, 0x6, 0x2, 0x2, 0xf5, 0xfd, 0x5, 0x1c, 0xf, 
    0x8, 0xf6, 0xf7, 0xc, 0x6, 0x2, 0x2, 0xf7, 0xf8, 0x7, 0x42, 0x2, 0x2, 
    0xf8, 0xfd, 0x5, 0x1c, 0xf, 0x7, 0xf9, 0xfa, 0xc, 0x5, 0x2, 0x2, 0xfa, 
    0xfb, 0x7, 0x43, 0x2, 0x2, 0xfb, 0xfd, 0x5, 0x1c, 0xf, 0x6, 0xfc, 0xed, 
    0x3, 0x2, 0x2, 0x2, 0xfc, 0xf0, 0x3, 0x2, 0x2, 0x2, 0xfc, 0xf3, 0x3, 
    0x2, 0x2, 0x2, 0xfc, 0xf6, 0x3, 0x2, 0x2, 0x2, 0xfc, 0xf9, 0x3, 0x2, 
    0x2, 0x2, 0xfd, 0x100, 0x3, 0x2, 0x2, 0x2, 0xfe, 0xfc, 0x3, 0x2, 0x2, 
    0x2, 0xfe, 0xff, 0x3, 0x2, 0x2, 0x2, 0xff, 0x1d, 0x3, 0x2, 0x2, 0x2, 
    0x100, 0xfe, 0x3, 0x2, 0x2, 0x2, 0x101, 0x102, 0x9, 0x7, 0x2, 0x2, 0x102, 
    0x1f, 0x3, 0x2, 0x2, 0x2, 0x103, 0x104, 0x7, 0x45, 0x2, 0x2, 0x104, 
    0x21, 0x3, 0x2, 0x2, 0x2, 0x17, 0x25, 0x2d, 0x32, 0x3d, 0x47, 0x50, 
    0x59, 0x6a, 0x72, 0x80, 0x94, 0xa3, 0xa6, 0xb8, 0xbb, 0xc8, 0xe0, 0xe3, 
    0xeb, 0xfc, 0xfe, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

AslParser::Initializer AslParser::_init;
