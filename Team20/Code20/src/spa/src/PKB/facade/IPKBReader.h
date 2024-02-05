#pragma once

#include <unordered_map>
#include <string>

// interface for FacadeAPI to implement
class IPKBReader {

public:
    virtual ~IPKBReader() = default;
    virtual std::vector<int> getFollows(int followeeStmtNum) = 0;
    virtual std::vector<int> getFollowing(int followerStmtNum) = 0;
    virtual bool containsFollows(int followeeStmtNum) = 0;
    virtual bool containsFollowing(int followerStmtNum) = 0;
};