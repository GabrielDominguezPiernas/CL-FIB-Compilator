
// Generated from Asl.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"




class  AslLexer : public antlr4::Lexer {
public:
  enum {
    INTVAL = 1, FLOATVAL = 2, BOOLVAL = 3, CHARVAL = 4, POT = 5, MAP = 6, 
    USING = 7, SUM = 8, FILTER = 9, DOTMAX = 10, ASSIGN = 11, EQUAL = 12, 
    NEG = 13, GT = 14, GE = 15, LT = 16, AP = 17, AT = 18, LE = 19, LP = 20, 
    RP = 21, LSQ = 22, RSQ = 23, KEYL = 24, KEYR = 25, COLON = 26, SCOLON = 27, 
    DOT = 28, COMMA = 29, HASH = 30, PLUS = 31, SUB = 32, MUL = 33, DIV = 34, 
    MOD = 35, VAR = 36, INT = 37, FLOAT = 38, BOOL = 39, CHAR = 40, ARRAY = 41, 
    IF = 42, THEN = 43, ELSE = 44, ENDIF = 45, WHILE = 46, DO = 47, ENDWHILE = 48, 
    FUNC = 49, ENDFUNC = 50, READ = 51, WRITE = 52, RETURN = 53, OF = 54, 
    FOR = 55, IN = 56, RANGE = 57, ENDFOR = 58, PACK = 59, UNPACK = 60, 
    INTO = 61, TRUE = 62, FALSE = 63, AND = 64, OR = 65, NOT = 66, ID = 67, 
    STRING = 68, COMMENT = 69, WS = 70
  };

  AslLexer(antlr4::CharStream *input);
  ~AslLexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

