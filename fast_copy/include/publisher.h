#pragma once

#include "logger.h"

#include <set>
#include <memory>
#include <tuple>
#include <functional>
#include <algorithm>

template <typename T_EVENTS_HANDLER>
class Publisher
{
public:
    void Subscribe(const std::shared_ptr<T_EVENTS_HANDLER> &spHandler)
    {
        // auto [it, inserted] = 
        m_Handlers.insert(spHandler);
        // if (!inserted)
        //     static_assert(LOG_DISABLED);
    }
protected:
    std::set<std::shared_ptr<T_EVENTS_HANDLER>> m_Handlers;
};