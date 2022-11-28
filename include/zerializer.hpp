#pragma once

#include <string>

namespace zeus
{
    enum class SerialType
    {
        XML,
        JSON,
        YAML
    };

    class SerialInfo
    {
        SerialType SerializationType;
    };

    class Zerializer
    {
    private:
        SerialInfo m_Info;
        std::string m_Filename;

    public:
        Zerializer() = default;

        void Start(const std::string& filename, SerialInfo serialInfo);
        void Serialize(const std::string& key, std::string& value);
        std::string Deserialize(const std::string& key) const;
        void End();
    };
}
