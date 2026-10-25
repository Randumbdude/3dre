#pragma once

#include <any>
#include <cstdint>
#include <exception>
#include <functional>
#include <future>
#include <memory>
#include <stdexcept>
#include <stop_token>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

namespace scheduler
{
    using task_id = std::size_t;

    namespace detail
    {
        struct task
        {
            std::function<std::any(std::stop_token)> function;
            std::shared_future<std::any> result;
            std::jthread worker;
            bool started = false;
        };

        inline std::vector<task> tasks;
    }

    template <typename F, typename... Args>
    task_id add(F &&func, Args &&...args)
    {
        detail::task t;

        if constexpr (std::is_invocable_v<F, Args..., std::stop_token>)
        {
            auto bound = std::bind(
                std::forward<F>(func),
                std::forward<Args>(args)...,
                std::placeholders::_1);
            using bound_type = decltype(bound);
            auto callable = std::make_shared<bound_type>(std::move(bound));

            t.function = [callable](std::stop_token stop_token) -> std::any
            {
                using result_type = std::invoke_result_t<bound_type &, std::stop_token>;
                if constexpr (std::is_void_v<result_type>)
                {
                    std::invoke(*callable, stop_token);
                    return {};
                }
                else
                {
                    return std::any(std::invoke(*callable, stop_token));
                }
            };
        }
        else
        {
            auto bound = std::bind(
                std::forward<F>(func),
                std::forward<Args>(args)...);
            using bound_type = decltype(bound);
            auto callable = std::make_shared<bound_type>(std::move(bound));

            t.function = [callable](std::stop_token) -> std::any
            {
                using result_type = std::invoke_result_t<bound_type &>;
                if constexpr (std::is_void_v<result_type>)
                {
                    std::invoke(*callable);
                    return {};
                }
                else
                {
                    return std::any(std::invoke(*callable));
                }
            };
        }

        detail::tasks.push_back(std::move(t));
        return detail::tasks.size() - 1;
    }

    template <typename T>
    T get(task_id id)
    {
        auto &t = detail::tasks.at(id);
        if (!t.started)
            throw std::logic_error("Task has not executed");

        return std::any_cast<T>(t.result.get());
    }

    void run(task_id id);
    void run_all();
    void stop_all();
    bool is_executed(task_id id);
    std::size_t size();
    void clear();
}