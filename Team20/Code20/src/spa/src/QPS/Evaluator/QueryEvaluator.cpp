#include <memory>
#include <string>
#include <vector>

#include "Models/PQL.h"
#include "Models/SimpleProgram.h"
#include "PKB/facade/IPKBReader.h"
#include "QueryEvaluator.h"
#include "Result.h"
#include "ResultStore.h"
#include "StatementEvaluator.h"
#include "EntityEvaluator.h"

namespace QueryEvaluator {
    QueryEvaluator::QueryEvaluator(std::shared_ptr<IPKBReader> r) : resultStore(std::make_shared<ResultStore>()), reader(r) {}

    std::vector<std::string> QueryEvaluator::evaluateQuery(const PQL::Query &q) {
        initialiseDeclaration(q);

        for (const auto &clause: q.clauses) {
            auto isTrue = evaluateClause(clause);
            if (!isTrue) {
                // empty result, final result will also be empty, can just return
                return {};
            }
        }

        // need to select from the results
        std::vector<std::string> selectResult = resultStore->retrieveSelect(q.selectSynonym);
//        return resultStore.retrieveSelect(q.selectSynonym);
        return selectResult;
    }

    bool QueryEvaluator::evaluateClause(const PQL::Clause &clause) {
        // TODO: Add pattern evaluator
        switch (clause.clauseType) {
            case SimpleProgram::DesignAbstraction::FOLLOWS:
            case SimpleProgram::DesignAbstraction::FOLLOWST:
            case SimpleProgram::DesignAbstraction::PARENT:
            case SimpleProgram::DesignAbstraction::PARENTT:
                return StatementEvaluator{reader, clause, resultStore}.evaluate();
            case SimpleProgram::DesignAbstraction::USESS:
            case SimpleProgram::DesignAbstraction::MODIFIESS:
                return EntityEvaluator{reader, clause, resultStore}.evaluate();
            default:
                return false;
        }
    }

    void QueryEvaluator::initialiseDeclaration(const PQL::Query &q) {
        // Add all declarations to the
        for (const auto &syn: q.declarations) {
            addSynonymToStore(syn);
        }
    }

    void QueryEvaluator::addSynonymToStore(const PQL::Synonym &syn) {
        std::vector<std::string> strRes;
        std::vector<int> intRes;
        switch (syn.entityType) {
            case SimpleProgram::DesignEntity::PROCEDURE:
                strRes = reader->getAllProcedures();
                if (strRes.empty()) {
                    return;
                }
                resultStore->createColumn(syn, std::vector<std::string>(strRes.begin(), strRes.end()));
                break;

            case SimpleProgram::DesignEntity::STMT:
                intRes = reader->getAllStatementNum();
                if (intRes.empty()) {
                    return;
                }
                resultStore->createColumn(syn, std::vector<int>(intRes.begin(), intRes.end()));
                break;

            case SimpleProgram::DesignEntity::READ:
                intRes = reader->getAllReadStmtNum();
                if (intRes.empty()) {
                    return;
                }
                resultStore->createColumn(syn, std::vector<int>(intRes.begin(), intRes.end()));
                break;

            case SimpleProgram::DesignEntity::PRINT:
                intRes = reader->getAllPrintStmtNum();
                if (intRes.empty()) {
                    return;
                }
                resultStore->createColumn(syn, std::vector<int>(intRes.begin(), intRes.end()));
                break;

            case SimpleProgram::DesignEntity::ASSIGN:
                intRes = reader->getAllAssignStmtNum();
                if (intRes.empty()) {
                    return;
                }
                resultStore->createColumn(syn, std::vector<int>(intRes.begin(), intRes.end()));
                break;

            case SimpleProgram::DesignEntity::CALL:
                intRes = reader->getAllCallStmtNum();
                if (intRes.empty()) {
                    return;
                }
                resultStore->createColumn(syn, std::vector<int>(intRes.begin(), intRes.end()));
                break;

            case SimpleProgram::DesignEntity::WHILE:
                intRes = reader->getAllWhileStmtNum();
                if (intRes.empty()) {
                    return;
                }
                resultStore->createColumn(syn, std::vector<int>(intRes.begin(), intRes.end()));
                break;

            case SimpleProgram::DesignEntity::IF:
                intRes = reader->getAllIfStmtNum();
                if (intRes.empty()) {
                    return;
                }
                resultStore->createColumn(syn, std::vector<int>(intRes.begin(), intRes.end()));
                break;

            case SimpleProgram::DesignEntity::VARIABLE:
                strRes = reader->getAllVariables();
                if (strRes.empty()) {
                    return;
                }
                resultStore->createColumn(syn, std::vector<std::string>(strRes.begin(), strRes.end()));
                break;

            case SimpleProgram::DesignEntity::CONSTANT:
                intRes = reader->getAllConstants();
                if (intRes.empty()) {
                    return;
                }
                resultStore->createColumn(syn, std::vector<int>(intRes.begin(), intRes.end()));

            case SimpleProgram::DesignEntity::STMT_NO:
            case SimpleProgram::DesignEntity::WILDCARD:
            case SimpleProgram::DesignEntity::IDENT:
            case SimpleProgram::DesignEntity::EXPR:
            case SimpleProgram::DesignEntity::INTEGER:
                return;
        }
    }
}
