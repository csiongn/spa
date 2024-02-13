#include <memory>
#include <string>
#include <vector>

#include "Models/PQL.h"
#include "Models/SimpleProgram.h"
#include "PKB/facade/IPKBReader.h"
#include "QueryEvaluator.h"
#include "Result.h"
#include "ResultStore.h"

namespace QueryEvaluator {
    QueryEvaluator::QueryEvaluator(std::shared_ptr<IPKBReader> r) : resultStore(), reader(r) {}

    std::vector<std::string> QueryEvaluator::evaluateQuery(const PQL::Query &q) {
        initialiseDeclaration(q);

        for (const auto &clause : q.clauses) {
            auto result = evaluateClause(clause);
            if (result.isEmpty()) {
                // empty result, final result will also be empty, can just return
                return {};
            }

            resultStore.insertResult(std::make_shared<Result>(result));
        }

        // need to select from the results


        return resultStore.retrieveSelect(q.selectSynonym);
    }

    Result QueryEvaluator::evaluateClause(const PQL::Clause &clause) {
        // TODO

        return {};
    }

    void QueryEvaluator::initialiseDeclaration(const PQL::Query &q) {
        // Add all declarations to the
        for (const auto &syn : q.declarations) {
            addSynonymToStore(syn);
        }
    }

    void QueryEvaluator::addSynonymToStore(const PQL::Synonym &syn) {
        std::vector<std::string> strRes;
        std::vector<int> intRes;
        switch (syn.entityType) {
            case SimpleProgram::DesignEntity::PROCEDURE:
                strRes = reader->getAllProcedures();
                resultStore.createColumn(syn, std::vector<std::string>(strRes.begin(), strRes.end()));
                break;

            case SimpleProgram::DesignEntity::STMT:
                intRes = reader->getAllStatementNum();
                resultStore.createColumn(syn, std::vector<int>(intRes.begin(), intRes.end()));
                break;

            case SimpleProgram::DesignEntity::READ:
                intRes = reader->getAllReadStmtNum();
                resultStore.createColumn(syn, std::vector<int>(intRes.begin(), intRes.end()));
                break;

            case SimpleProgram::DesignEntity::PRINT:
                intRes = reader->getAllPrintStmtNum();
                resultStore.createColumn(syn, std::vector<int>(intRes.begin(), intRes.end()));
                break;

            case SimpleProgram::DesignEntity::ASSIGN:
                intRes = reader->getAllAssignStmtNum();
                resultStore.createColumn(syn, std::vector<int>(intRes.begin(), intRes.end()));
                break;

            case SimpleProgram::DesignEntity::CALL:
                intRes = reader->getAllCallStmtNum();
                resultStore.createColumn(syn, std::vector<int>(intRes.begin(), intRes.end()));
                break;

            case SimpleProgram::DesignEntity::WHILE:
                intRes = reader->getAllWhileStmtNum();
                resultStore.createColumn(syn, std::vector<int>(intRes.begin(), intRes.end()));
                break;

            case SimpleProgram::DesignEntity::IF:
                intRes = reader->getAllIfStmtNum();
                resultStore.createColumn(syn, std::vector<int>(intRes.begin(), intRes.end()));
                break;

            case SimpleProgram::DesignEntity::VARIABLE:
                strRes = reader->getAllVariables();
                resultStore.createColumn(syn, std::vector<std::string>(strRes.begin(), strRes.end()));
                break;

            case SimpleProgram::DesignEntity::CONSTANT:
                intRes = reader->getAllConstants();
                resultStore.createColumn(syn, std::vector<int>(intRes.begin(), intRes.end()));
        }
    }
}
