#pragma once
#include "file_manager.h"
#include "configuration_manager.h"

namespace FastCopy
{
    class ThreadsCreator
    {
    public:
        ThreadsCreator(const std::shared_ptr<FastCopy::ConfigurationManager> &config)
            : m_config{config}
        {
        }

        template <typename T>
        void Create() const
        {
            T fm(m_config);
            std::vector<std::jthread> vThreadsPool;

            for (size_t i = 0; i != m_config->GetThreadsCount(); ++i)
                vThreadsPool.emplace_back([&fm]
                                          { fm.ProcessData(); });
            vThreadsPool.emplace_back([&fm]
                                      { fm.PrepareData(); });
        }

    private:
        std::shared_ptr<FastCopy::ConfigurationManager> m_config;
    };
} // !FastCopy