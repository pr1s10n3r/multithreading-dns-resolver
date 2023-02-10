#include "Resolver.hpp"

#include <iostream>
#include <string>
#include <memory>

#include "Poco/Thread.h"
#include "Poco/Logger.h"
#include "Poco/Net/DNS.h"
#include "Poco/Exception.h"

Resolver::Resolver(InputLoad &inputLoad) : _inputLoad(inputLoad)
{
}

Resolver::~Resolver()
{
}

void Resolver::run()
{
    Poco::Logger &logger = Poco::Logger::get("Resolver");

    while (!_inputLoad.isEmpty())
    {
        const std::string URI = _inputLoad.pop();
        const long osThreadID = Poco::Thread::currentOsTid();

        try
        {
            const Poco::Net::IPAddress addr = Poco::Net::DNS::resolveOne(URI);
            logger.information("Thread [%ld]: Resolved IP %s for domain %s", osThreadID, addr.toString(), URI);
        }
        catch (const Poco::Exception &ex)
        {
            std::string msg(ex.what());
            logger.error("Thread [%ld]: Unable to resolve %s: %s", osThreadID, URI, msg);
        }
    }

    logger.information("Is Empty: %s, Size: %lu", _inputLoad.isEmpty() ? "true" : "false", _inputLoad.size());
}
