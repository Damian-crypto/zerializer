#include <zerializer.hpp>

namespace zeus
{
    void Zerializer::Start(const std::string& filename, SerialInfo serialInfo)
    {
        m_Info = serialInfo;
        m_Filename = filename;
    }

    void Zerializer::Serialize(const std::string& key, std::string& value)
    { 
    }

    std::string Zerializer::Deserialize(const std::string& key) const
    {
    }

    void Zerializer::End()
    {
    }
}
