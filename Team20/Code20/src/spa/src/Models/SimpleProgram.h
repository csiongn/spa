#pragma once

namespace SimpleProgram {
enum class DesignEntity {
  PROCEDURE,
  STMT,
  READ,
  PRINT,
  ASSIGN,
  CALL,
  WHILE,
  IF,
  VARIABLE,
  CONSTANT,
  STMT_NO,
  WILDCARD,
  IDENT,
  EXPR,
  PARTIAL_EXPR,
  INTEGER,
  BOOLEAN
};

enum class DesignAbstraction {
  FOLLOWS,
  FOLLOWST,
  PARENT,
  PARENTT,
  USESS,
  USESP,
  MODIFIESS,
  MODIFIESP,
  PATTERN_ASSIGN,
  PATTERN_IF,
  PATTERN_WHILE,
  CALLS,
  CALLST,
  NEXT,
  NEXTT,
  AFFECTS,
  WITH
};

enum class AttributeRef {
  NAME, // for proc/call/var/read/print
  INTEGER, // for const/stmt/read/print/call/while/if/assign
  NO_REF // for synonym not using any of the above
};
}