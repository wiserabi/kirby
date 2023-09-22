#include "stdafx.h"
#include "ReadCoordinate.h"
#include <fstream>
#include <sstream>

vector<Vector3> FileReader::ReadFile(const string& filename)
{
    ifstream file(filename);
    vector<Vector3> data;

    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            Vector3 vector;
            istringstream iss(line);

            if (iss >> vector.x >> vector.y >> vector.z)
            {
                data.push_back(vector);
            }
        }

        file.close();
    }

    return data;
}
