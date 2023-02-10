#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <cassert>

#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/ThreadPool.h"
#include "Poco/Exception.h"
#include "Poco/Message.h"

#include "InputLoad.hpp"
#include "Resolver.hpp"

#define ARG_THREADS_COUNT_NAME "THREADS_COUNT"
#define ARG_INPUT_DIRECTORY_NAME "INPUT_DIRECTORY_PATH"

constexpr uint32_t defaultThreadsCount = 4;

using Poco::Util::Option;
using Poco::Util::OptionCallback;

class Application : public Poco::Util::Application
{
protected:
    void initialize(Poco::Util::Application &app)
    {
        loadConfiguration();
        Poco::Util::Application::initialize(app);
    }

    void uninitialize()
    {
        Poco::Util::Application::uninitialize();
    }

    void defineOptions(Poco::Util::OptionSet &options)
    {
        Poco::Util::Application::defineOptions(options);

        options.addOption(Option("verbose", "v").required(false).repeatable(false).binding("debug"));

        options.addOption(
            Option("help", "h", "display help information")
                .required(false)
                .repeatable(false)
                .callback(OptionCallback<Application>(this, &Application::handleHelpFlag)));

        options.addOption(
            Option("threads", "t")
                .required(false)
                .repeatable(false)
                .argument(ARG_THREADS_COUNT_NAME, true)
                .callback(OptionCallback<Application>(this, &Application::handleThreadsFlag)));

        options.addOption(
            Option("input", "i")
                .required(true)
                .argument(ARG_INPUT_DIRECTORY_NAME, true)
                .callback(OptionCallback<Application>(this, &Application::handleInputDirectoryFlag)));
    }

    int main(const std::vector<std::string> &args)
    {
        if (_helpRequested)
            return EXIT_SUCCESS;

        Poco::Logger &logger = Poco::Logger::get("Main");
        if (_debugFlag)
            logger.setLevel(Poco::Message::PRIO_DEBUG);

        if (_setThreadCount && !_goodThreadCount)
            return EXIT_FAILURE;

        uint32_t threadsCount = defaultThreadsCount;
        if (_goodThreadCount)
            threadsCount = config().getUInt(ARG_THREADS_COUNT_NAME);

        const std::string inputPath = config().getString(ARG_INPUT_DIRECTORY_NAME);

        InputLoad il(inputPath);
        std::cout << il.size() << " domains loaded" << std::endl;

        Poco::ThreadPool threadPool(threadsCount);
        for (int i = 0; i < threadsCount; i++)
        {
            Resolver resolver(il);
            threadPool.start(resolver);
        }

        threadPool.joinAll();
        logger.information("All threads joined");
        assert(il.size() == 0);

        return EXIT_SUCCESS;
    }

private:
    bool _helpRequested = false;
    bool _setThreadCount = false;
    bool _goodThreadCount = false;
    bool _debugFlag = false;

    void handleInputDirectoryFlag(const std::string &name, const std::string &value)
    {
        config().setString(ARG_INPUT_DIRECTORY_NAME, value);
    }

    void handleThreadsFlag(const std::string &name, const std::string &value)
    {
        _setThreadCount = true;
        int32_t threads = 0;

        try
        {
            const uint32_t maxThreadsCount = std::thread::hardware_concurrency();

            threads = std::stoul(value, nullptr, 10);
            if (threads < 0)
                throw std::invalid_argument("threads count cannot be negative");
            else if (threads > maxThreadsCount)
            {
                const std::string msg = "your machine does not have that much threads (max available threads: " + std::to_string(maxThreadsCount) + ")";
                throw std::invalid_argument(msg);
            }

            _goodThreadCount = true;
        }
        catch (const std::invalid_argument &ex)
        {
            std::cerr << "Unable to use \"" << value << "\" as thread paramenter: " << ex.what() << std::endl;
            stopOptionsProcessing();
        }
        catch (const std::out_of_range &ex)
        {
            std::cerr << "Unable to use \"" << value << "\" as thread paramenter: " << ex.what() << std::endl;
            stopOptionsProcessing();
        }

        config().setUInt(ARG_THREADS_COUNT_NAME, threads);
    }

    void handleHelpFlag(const std::string &name, const std::string &value)
    {
        _helpRequested = true;

        Poco::Util::HelpFormatter helpFormatter(options());
        helpFormatter.setCommand(commandName());
        helpFormatter.setUsage("[--threads=<THREADS>]");
        helpFormatter.setHeader("Simple application for DNS stress test using POCO threading library");
        helpFormatter.format(std::cout);

        stopOptionsProcessing();
    }
};
