#include <zerializer/zerializer.hpp>

#include <stdexcept>
#include <limits>

namespace zeus
{
    void Zerializer::StartSerialize(const std::string& filename, SerialInfo serialInfo)
    {
        m_Info = serialInfo;
        m_Filename = filename;

        m_FileOut = std::ofstream(filename);

        if (!m_FileOut.is_open())
        {
            throw std::runtime_error("Runtime Error: " + filename + " is not open!");
        }
    }

    void Zerializer::InsertHeader(const std::string& title)
    {
        m_HeadersStack.push(title);

        if (m_Info.SerializationType == SerialType::PROPERTIES)
        {
            m_FileOut << m_WritingHeader << title << ":" << lineEnd;
        }

        m_WritingHeader.append(title).append("_");
    }

    void Zerializer::Serialize(const std::string& key, const std::string& value)
    {
        if (m_Info.SerializationType == SerialType::PROPERTIES)
        {
            m_FileOut << m_WritingHeader << key << ": " << value << lineEnd;
        }
    }
    
    void Zerializer::Serialize(const std::string& key, int value)
    {
        if (m_Info.SerializationType == SerialType::PROPERTIES)
        {
            m_FileOut << m_WritingHeader << key << ": " << std::to_string(value) << lineEnd;
        }
    }
    
    void Zerializer::Serialize(const std::string& key, double value)
    {
        if (m_Info.SerializationType == SerialType::PROPERTIES)
        {
            m_FileOut << m_WritingHeader << m_WritingHeader << key << ": " << std::to_string(value) << lineEnd;
        }
    }

    inline static void lstrip(std::string& str)
    {
        str.erase(begin(str), find_if(begin(str), end(str), [](unsigned char c) { return !std::isspace(c); }));
    }

    inline static void rstrip(std::string& str)
    {
        str.erase(std::find_if(rbegin(str), rend(str), [](unsigned char c) { return !isspace(c); }).base(), end(str));
    }

    inline void strip(std::string& str)
    {
        lstrip(str);
        rstrip(str);
    }

    void Zerializer::StartDeserialize(const std::string& filename, SerialInfo serialInfo)
    {
        if (m_FileOut.is_open())
        {
            m_FileOut.flush();
            m_FileOut.close();
        }

        m_FileIn = std::ifstream(filename, std::ios::ate);
        long filesize = m_FileIn.tellg();
        m_FileIn.seekg(0, std::ios::beg);

        std::string line;
        while (getline(m_FileIn, line))
        {
            size_t separator = line.find(":");
            if (separator != std::string::npos)
            {
                std::string key = line.substr(0, separator);
                strip(key);
                std::string value = line.substr(separator + 1);
                strip(value);

                if (key.find('[') != std::string::npos)
                {
                    std::vector<unsigned int> dim;

                    size_t begpos = key.find('[');
                    size_t endpos = std::string::npos;
                    while (begpos != std::string::npos)
                    {
                        endpos = key.find(']', begpos + 1);
                        dim.push_back(std::stoi(key.substr(begpos + 1, endpos - begpos - 1)));
                        begpos = key.find('[', endpos);
                    }

                    begpos = key.find('[');
                    endpos = key.find_last_of(']');
                    if (begpos != std::string::npos)
                        key.erase(begin(key) + begpos, begin(key) + endpos + 1);

                    m_HeadersStack.push(key);
                    m_HeaderData[key] = dim;

                    if (!value.empty())
                    {
                        m_Data[key] = value;
                    }
                }
                else
                {
                    m_Data[key] = value;
                }
            }
            else
            {
                const std::string& most_recent_header = m_HeadersStack.top();
                m_Data[most_recent_header].append(" ").append(line);
            }
        }
    }

    std::string Zerializer::DeserializeStr(const std::string& key)
    {
        return m_Data[m_ReadingHeader + key];
    }
    
    int Zerializer::DeserializeInt(const std::string& key)
    {
        return std::stoi(m_Data[m_ReadingHeader + key]);
    }
    
    double Zerializer::DeserializeDbl(const std::string& key)
    {
        return std::stod(m_Data[m_ReadingHeader + key]);
    }

    bool Zerializer::ReadHeader(const std::string& title)
    {
        if (m_Data.find(title) != m_Data.end())
        {
            m_ReadingHeader.append(title).append("_");
            m_ReadingHeaderStack.push(title);

            return true;
        }

        return false;
    }

    void Zerializer::EndHeader()
    {
        if (m_FileOut.is_open() && !m_HeadersStack.empty())
        {
            std::string key = m_HeadersStack.top() + "_";
            auto it = m_WritingHeader.find(key);
            if (it != std::string::npos)
            {
                m_WritingHeader.erase(it, key.length());
                m_HeadersStack.pop();
            }
        }
        
        if (m_FileIn.is_open() && !m_ReadingHeaderStack.empty())
        {
            std::string key = m_ReadingHeaderStack.top() + "_";
            auto it = m_ReadingHeader.find(key);
            if (it != std::string::npos)
            {
                m_ReadingHeader.erase(it, key.length());
                m_ReadingHeaderStack.pop();
            }
        }
    }
}
