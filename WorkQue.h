#pragma once
#include "pch.h"

class WorkQue {
    std::vector<std::function<void()>> tasks;
    size_t number_of_threads = std::max(2u, std::thread::hardware_concurrency());
public:    
    void run_in_parallel() const noexcept {
        std::for_each(std::execution::par, tasks.begin(), tasks.end(),
            [](const auto& task) {
                std::invoke(task);
            }
        );
    }
    void run_sequentially() const noexcept {
        std::ranges::for_each(tasks,
            [](const auto& task) {
                std::invoke(task);
            }
        );
    }
    size_t thread_count() const noexcept {
        return number_of_threads;
    }    
    void push_back(std::function<void()> task) {        
        tasks.emplace_back(std::move(task));
    }
    void clear() noexcept {
        tasks.clear();
    }    
};