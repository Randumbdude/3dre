#include "scheduler.h"

#include <chrono>

namespace scheduler
{
    void run(task_id id)
    {
        auto &t = detail::tasks.at(id);

        if (t.started)
            throw std::logic_error("Task already executed");

        std::promise<std::any> promise;
        t.result = promise.get_future().share();
        auto function = t.function;
        t.worker = std::jthread([function = std::move(function), promise = std::move(promise)](std::stop_token stop_token) mutable
                                {
            try
            {
                promise.set_value(function(stop_token));
            }
            catch (...)
            {
                promise.set_exception(std::current_exception());
            } });
        t.started = true;
    }

    void run_all()
    {
        for (task_id i = 0; i < detail::tasks.size(); ++i)
        {
            if (!detail::tasks[i].started)
            {
                run(i);
            }
        }
    }

    void stop_all()
    {
        for (auto &task : detail::tasks)
        {
            if (task.started && task.worker.joinable())
                task.worker.request_stop();
        }

        for (auto &task : detail::tasks)
        {
            if (task.started && task.worker.joinable())
                task.worker.join();
        }
    }

    bool is_executed(task_id id)
    {
        const auto &t = detail::tasks.at(id);
        return t.started &&
               t.result.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
    }

    std::size_t size()
    {
        return detail::tasks.size();
    }

    void clear()
    {
        stop_all();
        detail::tasks.clear();
    }
}