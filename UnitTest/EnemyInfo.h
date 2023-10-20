#pragma once

typedef struct EnemyData {
    int moves;//(walk, jump, fly, attack) 1,2,4,8
    string ability;
    int invincible;
    int follow;
    int split;
} EnemyData;


class EnemyInfo {
public:
    EnemyInfo();
    ~EnemyInfo();
    map<string, EnemyData> infos;
    void LoadEnemyInfo();
    map<string, EnemyData> GetData();
};