#pragma once
#include "pch.h"
class WorkQue {
    using size_type = size_t;
    std::vector<std::function<void()>> tasks;
    std::function<void()> remainder;
    size_type number_of_threads = std::min(SUGGESTED_THREAD_COUNT, std::thread::hardware_concurrency());
    size_type _partition_size = 0;

    template<class Callable>
    void schedule(Callable process, size_type start, size_type end, size_type part) {
         assert(start < end && part >= 0 && part <= tasks.size()); 
         tasks.emplace_back([process, start, end, part]() noexcept {                
            for (auto i = start; i < end; ++i) {                                        
                std::invoke(process, part, i);                    
            }
        });       
    }
    template<class Callable>
    void schedule_remainder(Callable process, size_type start, size_type end, size_type part) {
         assert(start < end && part >= 0 && part <= tasks.size()); 
         remainder = [process, start, end, part]() noexcept {                
            for (auto i = start; i < end; ++i) {                                        
                std::invoke(process, part, i);                    
            }
        };       
    }
public:
    WorkQue() noexcept = default;
    explicit WorkQue(size_type numberOfThreads) noexcept : number_of_threads(numberOfThreads) {}
    
    constexpr size_type calculate_partition_size(size_type itemsToProcess) noexcept {
        _partition_size = itemsToProcess / thread_count();
        return _partition_size;
    }
    constexpr size_type partition_size() const noexcept {        
        return _partition_size;
    }
    constexpr size_type thread_count() const noexcept {
        return number_of_threads;
    }   

    void run_in_parallel() const noexcept {
        std::for_each(std::execution::par, tasks.begin(), tasks.end(),
            [](const auto& task) { std::invoke(task); }
        );
        if (remainder) {
            std::invoke(remainder);
        }
    }
    void run_sequentially() const noexcept {
        std::ranges::for_each(tasks,
            [](const auto& task) { std::invoke(task); }
        );
        if (remainder) {
            std::invoke(remainder);
        }
    }          

    template<class Callable>
    void schedule(size_type itemsToProcess, Callable process) {                
        const size_type partition_size = calculate_partition_size(itemsToProcess);
        size_type end = 0;
        for (size_type part = 0; partition_size && part < thread_count(); ++part) {
            const auto start = part * partition_size;
            end = (part + 1) * partition_size;
            schedule(process, start, end, part);
        }
        if (end < itemsToProcess) {            
            size_type part = tasks.empty() ? 0 : tasks.size()-1;            
            schedule_remainder(process, end, itemsToProcess, part);
        }
    }

    void clear() noexcept {
        tasks.clear();
        remainder = {};
        calculate_partition_size(0);
    }    
};