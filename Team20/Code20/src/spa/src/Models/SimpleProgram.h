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
        INTEGER
    };

    enum class DesignAbstraction {
        FOLLOWS,
        FOLLOWST,
        PARENT,
        PARENTT,
        USES,
        MODIFIES
    };
}