#pragma once

class IPKBWriter {

public:
    virtual ~IPKBWriter() = default;
    virtual void insertFollows(int followeeStmtNum, int followerStmtNum) = 0;
};