#pragma once
#include "pch.h"

class WorkQue {
    using size_type = uint16_t;
    std::vector<std::function<void()>> tasks;
    size_type number_of_threads = narrow_cast<size_type>(std::max(2u, std::thread::hardware_concurrency()));
public:
    WorkQue() noexcept = default;
    explicit WorkQue(size_type numberOfThreads) noexcept : number_of_threads(numberOfThreads) {}
    
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
    size_type thread_count() const noexcept {
        return number_of_threads;
    }    
    void push_back(std::function<void()> task) {        
        tasks.emplace_back(std::move(task));
    }
    void clear() noexcept {
        tasks.clear();
    }    
};