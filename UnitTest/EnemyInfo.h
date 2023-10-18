#pragma once

typedef struct EnemyData {
    string name;
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
    vector<EnemyData> infos;
    void LoadEnemyInfo();
    vector<EnemyData> GetData();
};