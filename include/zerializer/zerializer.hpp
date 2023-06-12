#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <stack>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <sstream>

namespace zeus
{
    const char lineEnd = '\n';

    enum class SerialType
    {
        XML,
        JSON,
        YAML,
        PROPERTIES
    };

    class SerialInfo
    {
    public:
        SerialType SerializationType;
    };

    class Zerializer
    {
    private:
        SerialInfo m_Info;
        std::string m_Filename;
        std::ifstream m_FileIn;
        std::ofstream m_FileOut;

        std::stack<std::string> m_HeadersStack;
        std::string m_WritingHeader{""};
        std::stack<std::string> m_ReadingHeaderStack;
        std::string m_ReadingHeader{""};
        std::unordered_map<std::string, std::string> m_Data; // [key, value]
        std::unordered_map<std::string, std::vector<unsigned int>> m_HeaderData; // [name, dimensions]

        std::function<std::string(int)> IntToString = [](int val) {
            return std::to_string(val);
        };

    public:
        Zerializer() = default;

        void StartSerialize(const std::string& filename, SerialInfo serialInfo);
        void InsertHeader(const std::string& title);

        void Serialize(const std::string& key, const std::string& value);
        void Serialize(const std::string& key, int value);
        void Serialize(const std::string& key, double value);
        template <typename T, typename ToString>
        void Serialize(const std::string& key, const std::vector<T>& value, ToString convert);
        template <typename T, typename ToString>
        void Serialize(const std::string& key, const std::vector<std::vector<T>>& value, ToString convert);

        void StartDeserialize(const std::string& filename, SerialInfo serialInfo);
        bool ReadHeader(const std::string& title);
        std::string DeserializeStr(const std::string& key);
        int DeserializeInt(const std::string& key);
        double DeserializeDbl(const std::string& key);

        template <typename T, typename ToVec>
        std::vector<T> DeserializeVec(const std::string& key, ToVec convert);
        template <typename T, typename ToVec2>
        std::vector<std::vector<T>> DeserializeVec2(const std::string& key, ToVec2 convert);

        void EndHeader();
    };
}

template <typename T, typename ToString>
void zeus::Zerializer::Serialize(const std::string& key, const std::vector<T>& value, ToString convert)
{
    const int n = (int)value.size();
    m_FileOut << m_WritingHeader << key << "[" << n << "]: ";
    for (int i = 0; i < n; i++)
    {
        m_FileOut << convert(value[i]) << " ";
    }
    m_FileOut << lineEnd;
}

template <typename T, typename ToString>
void zeus::Zerializer::Serialize(const std::string& key, const std::vector<std::vector<T>>& value, ToString convert)
{
    const int rows = (int)value.size();
    const int cols = (int)value.size();

    m_FileOut << m_WritingHeader << key << "[" << rows << "][" << cols << "]" << ":" << lineEnd;
    for (const auto& row : value)
    {
        for (const auto& data : row)
        {
            m_FileOut << convert(data);
        }
        m_FileOut << lineEnd;
    }
    m_FileOut << lineEnd;
}

template <typename T, typename ToVec>
std::vector<T> zeus::Zerializer::DeserializeVec(const std::string& key, ToVec convert)
{
    std::string tag = m_ReadingHeader + key;
    std::stringstream ss;
    ss << tag;
    for (int dim : m_HeaderData[key])
    {
        ss << "[" << dim << "]";
    }
    tag = ss.str();

    std::vector<T> res;
    if (m_Data.find(tag) != m_Data.end())
    {
        std::string value = m_Data[m_ReadingHeader + key];
        ss.str(value);
        std::string token;
        while (ss >> token)
        {
            res.push_back(convert(token));
        }
    }

    return res;
}

template <typename T, typename ToVec2>
std::vector<std::vector<T>> zeus::Zerializer::DeserializeVec2(const std::string& key, ToVec2 convert)
{
    std::string tag = m_ReadingHeader + key;
    size_t rows = m_HeaderData[key][0];
    size_t cols = m_HeaderData[key][1];

    std::vector<std::vector<T>> res(rows);
    if (m_Data.find(tag) != m_Data.end())
    {
        std::string value = m_Data[m_ReadingHeader + key];
        size_t begpos = value.find("(");
        size_t endpos = value.find(")", begpos + 1);
        for (size_t row = 0; row < rows; row++)
        {
            int col = 0;
            while (begpos != std::string::npos)
            {
                std::string token = value.substr(begpos + 1, endpos - begpos - 1);
                res[row].push_back(convert(token));
                begpos = value.find("(", endpos + 1);
                endpos = value.find(")", begpos + 1);
                col++;
                if (col == cols)
                    break;
            }
        }
    }

    return { res };
}
