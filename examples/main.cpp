#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include "zerializer/zerializer.hpp"

typedef struct Person
{
    char Name[100];
    int Age;
    char Gender;
} Person;

int main()
{
    zeus::Zerializer zerializer;
    zeus::SerialInfo info;
    info.SerializationType = zeus::SerialType::PROPERTIES;

    std::vector<std::vector<std::vector<int>>> levelmap = {
        {{1, 1, 1, -1}, {2, 1, 1, -1},    {2, 1, 1, -1},   {2, 1, 1, -1},   {2, 1, 1, -1},   {2, 1, 1, -1},   {2, 1, 1, -1},   {2, 1, 1, -1},   {2, 1, 1, -1}, {3, 1, 1, -1}},
        {{1, 2, 3, -1}, {-1, -1, 3, -1},  {2, 2, 3, -1}, {-1, -1, 3, -1},   {2, 2, 3, -1}, {-1, -1, 3, -1}, {-1, -1, 3, -1}, {-1, -1, 3, -1}, {-1, -1, 3, -1}, {3, 2, 3, -1}},
        {{1, 2, 3, -1}, {-1, -1, 3, -1},  {2, 2, 3, -1}, {-1, -1, 3, -1},   {2, 2, 3, -1}, {-1, -1, 3, -1}, {-1, -1, 3, -1}, {  5, 2, 3, -1}, {-1, -1, 3, -1}, {3, 2, 1, -1}},
        {{1, 2, 3, -1}, {-1, -1, 3, -1},  {2, 2, 3, -1}, {-1, -1, 3, -1}, {-1, -1, 3, -1}, {-1, -1, 3, -1}, {-1, -1, 3, -1}, {-1, -1, 3, -1}, {-1, -1, 3, -1}, {3, 2, 1, -1}},
        {{1, 2, 3, -1}, {-1, -1, 3, -1},  {2, 2, 3, -1}, {-1, -1, 3, -1},   {2, 2, 3, -1}, {-1, -1, 3, -1}, {-1, -1, 3, -1}, {-1, -1, 3, -1}, {-1, -1, 3, -1}, {3, 2, 1, -1}},
        {{1, 2, 3, -1}, {-1, -1, 3, -1},  {2, 2, 3, -1}, {-1, -1, 3, -1},   {2, 2, 3, -1}, {-1, -1, 3, -1}, {-1, -1, 3, -1}, {-1, -1, 3, -1}, {-1, -1, 3, -1}, {3, 2, 1, -1}},
        {{1, 2, 3, -1}, {-1, -1, 3, -1},  {2, 2, 3, -1}, {-1, -1, 3, -1},   {2, 2, 3, -1}, {-1, -1, 3, -1},   {5, 2, 3, -1}, {-1, -1, 3, -1}, {-1, -1, 3, -1}, {3, 2, 1, -1}},
        {{1, 2, 3, -1}, {-1, -1, 3, -1},  {2, 2, 3, -1}, {-1, -1, 3, -1},   {2, 2, 3, -1}, {-1, -1, 3, -1}, {-1, -1, 3, -1}, {-1, -1, 3, -1}, {-1, -1, 3, -1}, {3, 2, 1, -1}},
        {{1, 2, 3, -1}, {-1, -1, 3, -1},{-1, -1, 3, -1}, {-1, -1, 3, -1},   {2, 2, 3, -1}, {-1, -1, 3, -1}, {-1, -1, 3, -1}, {-1, -1, 3, -1}, {-1, -1, 3, -1}, {3, 2, 1, -1}},
        {{1, 3, 1, -1}, {2, 3, 1, -1},    {2, 3, 1, -1},   {2, 3, 1, -1},   {2, 3, 1, -1},   {2, 3, 1, -1},   {2, 3, 1, -1},   {2, 3, 1, -1},   {2, 3, 1, -1}, {3, 3, 2, -1}}
    };

    const std::function<std::string(const std::vector<int>&)> ivToString = [](const std::vector<int>& data) {
        std::string str = "(";
        for (int i = 0; i < (int)data.size(); i++)
        {
            str += std::to_string(data[i]);
            if (i < data.size() - 1)
                str += ", ";
        }
        str += ") ";

        return str;
    };

    const std::function<std::string(int)> iToString = [](int v) {
        return std::to_string(v);
    };

    const std::function<int(const std::string&)> sToInt = [](const std::string& s) {
        return std::stoi(s);
    };

    const std::function<std::vector<int>(const std::string&)> sToVec = [](const std::string& s) {
        std::vector<int> res;
        std::stringstream ss(s);
        std::string token;
        size_t pos;
        while (ss >> token)
        {
            pos = token.find(",");
            if (pos != std::string::npos)
                token.erase(pos, 1);
            // std::cout << "token: " << token << std::endl;
            res.push_back(std::stoi(token));
        }

        return res;
    };

    zerializer.StartSerialize("person_data.dat", info);
    zerializer.Serialize("cols", 10);
    zerializer.Serialize("rows", 10);
    zerializer.Serialize("cellsize", 32);
    zerializer.Serialize("cellgap", 2.0);
    zerializer.Serialize("levelmap", levelmap, ivToString);
    zerializer.InsertHeader("enemy1");
    zerializer.Serialize("type", "human");
    zerializer.Serialize("pos", std::vector<int>{ 100, 100 }, iToString);
    zerializer.Serialize("weapon", "gun");
    zerializer.Serialize("walk", "updown");
    zerializer.EndHeader();

    zerializer.StartDeserialize("person_data.dat", info);
    std::cout << "cols: " << zerializer.DeserializeInt("cols") << std::endl;
    std::cout << "rows: " << zerializer.DeserializeInt("rows") << std::endl;
    std::cout << "cellsize: " << zerializer.DeserializeInt("cellsize") << std::endl;
    std::cout << "cellgap: " << zerializer.DeserializeDbl("cellgap") << std::endl;
    std::vector<std::vector<std::vector<int>>> mp = zerializer.DeserializeVec2<std::vector<int>>("levelmap", sToVec);
    std::cout << "levelmap:" << std::endl;
    int r = 0;
    for (const auto& row : mp)
    {
        int c = 0;
        for (const auto& cell : row)
        {
            std::cout << "==> [" << r << "][" << c << "] (";
            for (int i = 0; i < cell.size(); i++)
            {
                std::cout << cell[i];
                if (i < cell.size() - 1)
                    std::cout << ", ";
            }
            c++;
            std::cout << ") ";
        }
        r++;
        std::cout << std::endl;
    }

    if (zerializer.ReadHeader("enemy1"))
    {
        std::cout << "Start deserializing enemy1" << std::endl;
        std::cout << "type: " << zerializer.DeserializeStr("type") << std::endl;
        std::vector<int> pos = zerializer.DeserializeVec<int>("pos", sToInt);
        std::cout << "pos: " << pos[0] << " " << pos[1] << std::endl;
        std::cout << "weapon: " << zerializer.DeserializeStr("weapon") << std::endl;
        std::cout << "walk: " << zerializer.DeserializeStr("walk") << std::endl;
        zerializer.EndHeader();
    }

    return 0;
}
