/*
** EPITECH PROJECT, 2024
** src
** File description:
** threadSafeQueue
*/

#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T>
class ThreadSafeQueue
{
    public:
        ThreadSafeQueue() = default;
        ~ThreadSafeQueue() = default;

        void push(T element)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_queue.push(element);
            m_cond.notify_one();
        }

        T pop()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cond.wait(lock, [this]() {return !m_queue.empty(); });
            T element = m_queue.front();
            m_queue.pop();
            return element;
        }

        const T& front()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            return m_queue.front();
        }

        [[nodiscard]] bool isEmpty()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            return m_queue.empty();
        }

    private:
        std::queue<T> m_queue;
        std::mutex m_mutex;
        std::condition_variable m_cond;
};
