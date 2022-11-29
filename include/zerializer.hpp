#pragma once

#include <string>
#include <vector>
#include <fstream>

namespace zeus
{
    const char lineEnd = '\n';

    enum class SerialType
    {
        XML,
        JSON,
        YAML
    };

    class SerialInfo
    {
    public:
        SerialType SerializationType;
    };

    struct IntToString
    {
        std::string operator()(int val)
        {
            return std::to_string(val);
        }
    };

    class Zerializer
    {
    private:
        SerialInfo m_Info;
        std::string m_Filename;
        std::ifstream m_FileIn;
        std::ofstream m_FileOut;

    public:
        Zerializer() = default;

        void Start(const std::string& filename, SerialInfo serialInfo);
        void InsertHeader(const std::string& title);

        void Serialize(const std::string& key, const std::string& value);
        void Serialize(const std::string& key, int value);
        void Serialize(const std::string& key, double value);
        template <typename T, typename ToString = IntToString>
        void Serialize(const std::string& key, const std::vector<T>& value);
        template <typename T, typename ToString = IntToString>
        void Serialize(const std::string& key, const std::vector<std::vector<T>>& value);

        bool ReadHeader(const std::string& title);
        std::string DeserializeStr(const std::string& key) const;
        int DeserializeInt(const std::string& key) const;
        double DeserializeDbl(const std::string& key) const;
        template <typename T>
        std::vector<T> DeserializeVec(const std::string& key) const;
        template <typename T>
        std::vector<std::vector<T>> DeserializeVec2(const std::string& key) const;
        void End();
    };
}
