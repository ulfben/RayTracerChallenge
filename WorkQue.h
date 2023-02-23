#pragma once
#include "pch.h"

class WorkQue {
    using size_type = size_t;
    std::vector<std::function<void()>> tasks;
    size_type number_of_threads = std::max(2u, std::thread::hardware_concurrency());
    size_type _partition_size = 0;
public:
    constexpr WorkQue() noexcept = default;
    constexpr explicit WorkQue(size_type numberOfThreads) noexcept : number_of_threads(numberOfThreads) {}
    
    constexpr void calculate_partition_size(size_type itemsToProcess) noexcept {
        _partition_size = itemsToProcess / thread_count(); //TODO: assumes buffer is evenly divisible    
    }
    constexpr size_type partition_size() const noexcept {
        return _partition_size;
    }

    constexpr size_type thread_count() const noexcept {
        return number_of_threads;
    }   

    void run_in_parallel() const noexcept {
        std::for_each(std::execution::par, tasks.begin(), tasks.end(),
            [](const auto& task) {
                std::invoke(task);
            }
        );
    }
    constexpr void run_sequentially() const noexcept {
        std::ranges::for_each(tasks,
            [](const auto& task) {
                std::invoke(task);
            }
        );
    }
         
    void push_back(std::function<void()> task) {        
        tasks.emplace_back(std::move(task));
    }   

    template<class Callable>
    void schedule(size_type itemsToProcess, Callable process) {                
        //if (buffer.size() % thread_count()) { //not evenly divisible
        //   push_back([task, end = buffer.size()]() noexcept {                
        //        std::invoke(task, 0, end);
        //   });
        //   return;
        //}        
        calculate_partition_size(itemsToProcess);        
        for (size_type part = 0; part < thread_count(); ++part) {
            const auto start = part * partition_size();
            const auto end = (part + 1) * partition_size();
            push_back([process, part, start, end]() noexcept {                
                for (auto i = start; i < end; ++i) {                                        
                    std::invoke(process, part, i);                    
                }
            });        
        }         
    }

    constexpr void clear() noexcept {
        tasks.clear();
    }    
};