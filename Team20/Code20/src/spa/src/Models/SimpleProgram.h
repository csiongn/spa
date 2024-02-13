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
        CONSTANT
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