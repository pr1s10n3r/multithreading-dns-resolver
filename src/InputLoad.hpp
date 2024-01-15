#pragma once

#include <string>
#include <queue>
#include <mutex>

class InputLoad
{
private:
    std::queue<std::string> domainsQueue;
    std::mutex _mutex;

public:
    InputLoad(const std::string &path);
    ~InputLoad() = default;

    size_t size();
    bool isEmpty();
    bool isValidURI(const std::string &value) const;

    std::string pop();
};