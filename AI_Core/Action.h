#pragma once
#include <iostream>
#include "WorldState.h"

class IAction {
public:
    IAction(const IAction& other) = delete;
    IAction(const std::string& name);
    IAction(IAction&& other) noexcept = default;
    virtual ~IAction();

    std::string Name;
    WorldState Preconditions;
    WorldState Effects;

    virtual void OnActionEnter() = 0;
    virtual bool OnActionExecute() = 0;

    virtual bool Validate() = 0;
    virtual int CalculateCost() = 0;

    IAction& operator=(IAction&& other) noexcept = default;
    friend std::ostream& operator<<(std::ostream& stream, IAction& other);
    bool operator==(const IAction& other);
};