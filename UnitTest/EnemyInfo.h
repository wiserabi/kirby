#pragma once

typedef struct EnemyData {
    string name;
    int moves;
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