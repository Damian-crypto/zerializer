#include <zerializer/zerializer.hpp>
#include <stdexcept>

namespace zeus
{
    void Zerializer::Start(const std::string& filename, SerialInfo serialInfo)
    {
        m_Info = serialInfo;
        m_Filename = filename;

        m_FileIn = std::ifstream(filename);
        m_FileOut = std::ofstream(filename);

        if (!m_FileIn.is_open() || !m_FileOut.is_open())
        {
            throw std::runtime_error("Runtime Error: " + filename + " is not open!");
        }
    }

    void Zerializer::InsertHeader(const std::string& title)
    {
        m_HeadersStack.push(title);
        if (m_Info.SerializationType == SerialType::PROPERTIES)
        {
            m_FileOut << title << ":" << lineEnd;
        }
    }

    void Zerializer::EndHeader()
    {
        string recentHeader = m_HeadersStack.top();
        m_HeadersStack.pop();
        if (m_Info.SerializationType == SerialType::PROPERTIES)
        {
            m_FileOut << lineEnd;
        }
    }

    void Zerializer::Serialize(const std::string& key, const std::string& value)
    {
        if (m_Info.SerializationType == SerialType::PROPERTIES)
        {
            m_FileOut << key << " : " << value << lineEnd;
        }
    }
    
    void Zerializer::Serialize(const std::string& key, int value)
    {
        if (m_Info.SerializationType == SerialType::PROPERTIES)
        {
            m_FileOut << key << " : " << std::to_string(value) << lineEnd;
        }
    }
    
    void Zerializer::Serialize(const std::string& key, double value)
    {
        if (m_Info.SerializationType == SerialType::PROPERTIES)
        {
            m_FileOut << key << " : " << std::to_string(value) << lineEnd;
        }
    }
    
    template <typename T, typename ToString>
    void Zerializer::Serialize(const std::string& key, const std::vector<T>& value)
    {
        const int n = (int)value.size();
        m_FileOut << key << "[ ";
        for (int i = 0; i < n; i++)
        {
            m_FileOut << ToString(value[i]);
            if (i < n - 1)
            {
                m_FileOut << ", ";
            }
        }
        m_FileOut << " ]" << lineEnd;
    }
    
    template <typename T, typename ToString>
    void Zerializer::Serialize(const std::string& key, const std::vector<std::vector<T>>& value)
    {
    }
    
    std::string Zerializer::DeserializeStr(const std::string& key) const
    {
        return "";
    }
    
    int Zerializer::DeserializeInt(const std::string& key) const
    {
        return 0;
    }
    
    double Zerializer::DeserializeDbl(const std::string& key) const
    {
        return 0.0;
    }
    
    template <typename T>
    std::vector<T> Zerializer::DeserializeVec(const std::string& key) const
    {
        return {};
    }
    
    template <typename T>
    std::vector<std::vector<T>> Zerializer::DeserializeVec2(const std::string& key) const
    {
        return {{}};
    }

    void Zerializer::End()
    {
    }
}
