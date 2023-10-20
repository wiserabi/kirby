#include "stdafx.h"
#include "EnemyInfo.h"
#include <fstream>
#include <sstream>

EnemyInfo::EnemyInfo()
{
    LoadEnemyInfo();
}

EnemyInfo::~EnemyInfo()
{
    infos.clear();
}

void EnemyInfo::LoadEnemyInfo()
{
    string filename = EnemyListPath + "enemylist.txt";
    ifstream file(filename);

    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            Vector3 vector;
            istringstream iss(line);
            EnemyData info;
            string name;
            iss >> name >> info.moves >> info.ability >> info.invincible >> info.follow >> info.split;
            infos[name] = { info.moves , info.ability , info.invincible , info.follow, info.split };
        }

        file.close();
    }
}

map<string, EnemyData> EnemyInfo::GetData()
{
    return infos;
}
