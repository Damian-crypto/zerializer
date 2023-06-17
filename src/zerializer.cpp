#include "zerializer/zerializer.hpp"
#include <limits>
#include <cassert>

namespace zeus
{
    Zerializer::Zerializer()
    {
        std::cout << "Zerializer started!" << std::endl;
    }

    Zerializer::~Zerializer()
    {
        std::cout << "Zerializer stopped!" << std::endl;
    }

    void Zerializer::StartSerialize(std::string filename, SerialInfo serialInfo)
    {
        m_Info = serialInfo;
        m_Filename = filename;

        m_FileOut = std::ofstream(filename);

        std::string err = "\"" + filename + "\" couldn't find!";
        assert(m_FileOut.is_open() && err.c_str());

        std::cout << "Serialization started!" << std::endl;
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

    void Zerializer::Flush()
    {
        m_FileOut.flush();
        m_FileOut.close();
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

    void Zerializer::StartDeserialize(std::string filename, SerialInfo serialInfo)
    {
        m_Filename = filename;

        if (m_FileOut.is_open())
        {
            m_FileOut.flush();
            m_FileOut.close();
        }

        m_FileIn = std::ifstream(filename, std::ios::ate);
        std::string err = "\"" + filename + "\" couldn't find!";
        assert(m_FileIn.is_open() && err.c_str());
        long filesize = m_FileIn.tellg();
        assert(filesize > 0 && "Deserializing an empty file!");
        m_FileIn.seekg(0, std::ios::beg);

        std::cout << "Deserialization started!" << std::endl;

        std::string line;
        while (getline(m_FileIn, line))
        {
            if (line.find("#") != std::string::npos)
                continue;

            strip(line);

            // If there is a colon, it is a key-value pair other wise it is a header
            size_t separator = line.find(":");
            if (separator != std::string::npos)
            {
                std::string key = line.substr(0, separator);
                strip(key);
                std::string value = line.substr(separator + 1);
                strip(value);

                key = m_ReadingHeader + key;

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

                    // If the key has [] brackets then it has dimensions otherwise normal
                    begpos = key.find('[');
                    endpos = key.find_last_of(']');
                    if (begpos != std::string::npos)
                        key.erase(begin(key) + begpos, begin(key) + endpos + 1);

                    // This take into consideration when serializing and deserializing lists
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
            else if (!line.empty())
            {
                if (!m_HeadersStack.empty())
                {
                    std::string key = m_HeadersStack.top();
                    std::string rev = key;
                    std::reverse(begin(rev), end(rev));
                    if (rev == line)
                    {
                        key = key + "_";
                        auto it = m_ReadingHeader.find(key);
                        if (it != std::string::npos)
                        {
                            m_ReadingHeader.erase(it, key.length());
                            m_HeadersStack.pop();
                        }
                        continue;
                    }
                }

                m_ReadingHeader.append(line).append("_");
                m_HeadersStack.push(line);
                m_Data[line] = "";
            }
            //else if (!m_HeadersStack.empty())
            //{
            //    const std::string& most_recent_header = m_HeadersStack.top();
            //    m_Data[most_recent_header].append(" ").append(line);
            //}
        }

        std::cout << "Deserialization finished!" << std::endl;

        // Clearing up variables
        m_ReadingHeader = "";
        m_WritingHeader = "";
        while (!m_HeadersStack.empty())
            m_HeadersStack.pop();
    }

    std::string Zerializer::DeserializeStr(const std::string& key)
    {
        std::string tag = m_ReadingHeader + key;
        assert(m_Data.find(tag) != m_Data.end() && "Deserializing invalid key!");
        return m_Data[tag];
    }

    int Zerializer::DeserializeInt(const std::string& key)
    {
        std::string tag = m_ReadingHeader + key;
        assert(m_Data.find(tag) != m_Data.end() && "Deserializing invalid key!");
        return std::stoi(m_Data[tag]);
    }

    double Zerializer::DeserializeDbl(const std::string& key)
    {
        std::string tag = m_ReadingHeader + key;
        assert(m_Data.find(tag) != m_Data.end() && "Deserializing invalid key!");
        return std::stod(m_Data[tag]);
    }

    /**
     * Start reading a subsection of the script. From now all the readings
     * are specific to the current header (not global).
     *
     * @param title: header name/title
     *
     * @return return true if the header exists otherwise false.
     */
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

    bool Zerializer::HasProperty(const std::string& key)
    {
        std::string tag = m_ReadingHeader + key;
        return m_Data.find(tag) != m_Data.end();
    }

    /**
     * Stop reading the current header. Back to the previous header if exists.
     * Otherwise to the global scope.
     */
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