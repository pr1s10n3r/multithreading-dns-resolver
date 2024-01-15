#include "InputLoad.hpp"

#include <string>
#include <fstream>
#include <algorithm>

#include "Poco/URI.h"
#include "Poco/DirectoryIterator.h"

static inline void ltrim(std::string &s);
static inline void rtrim(std::string &s);
static inline void trim(std::string &s);

InputLoad::InputLoad(const std::string &path)
{
    Poco::DirectoryIterator it(path);
    Poco::DirectoryIterator end;

    while (it != end)
    {
        const Poco::Path path(it->path());
        if (!it->isDirectory() || !it->canRead())
        {
            std::ifstream file(path.toString());

            std::string line;
            while (std::getline(file, line))
            {
                trim(line);
                if (line.length() < 1)
                    continue;
                if (isValidURI(line))
                    domainsQueue.push(line);
            }
        }

        it++;
    }
}

size_t InputLoad::size()
{
    const std::lock_guard<std::mutex> lock(_mutex);
    return domainsQueue.size();
}

bool InputLoad::isEmpty()
{
    const std::lock_guard<std::mutex> lock(_mutex);
    return domainsQueue.empty();
}

std::string InputLoad::pop()
{
    const std::lock_guard<std::mutex> lock(_mutex);

    const std::string domain = domainsQueue.front();
    domainsQueue.pop();

    return domain;
}

bool InputLoad::isValidURI(const std::string &value) const
{
    try
    {
        Poco::URI uri(value);
    }
    catch (...)
    {
        return false;
    }

    return true;
}

// trim from start (in place)
static inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                    { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                         { return !std::isspace(ch); })
                .base(),
            s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s)
{
    rtrim(s);
    ltrim(s);
}
