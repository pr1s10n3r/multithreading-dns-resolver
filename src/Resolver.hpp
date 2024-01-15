#pragma once

#include "InputLoad.hpp"
#include "Poco/Runnable.h"

class Resolver : public Poco::Runnable
{
private:
    InputLoad &_inputLoad;

public:
    Resolver(InputLoad &inputLoad);
    ~Resolver() = default;

    virtual void
    run();
};