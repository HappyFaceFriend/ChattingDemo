#pragma once

#include <atomic>
#include <memory>

namespace RamenNetworking
{
	/// <summary>
	/// SPSC(Single Producer Single Consumer) lock-free circular message queue for networking
	/// </summary>
	template <typename T>
	class MessageQueue
	{
	public:
		MessageQueue(size_t maxElementCount)
			: m_Capacity(maxElementCount + 1), m_Buffer(new T[maxElementCount + 1])
		{
		}
		MessageQueue(const MessageQueue&) = delete;
		MessageQueue& operator=(const MessageQueue&) = delete;
		MessageQueue(MessageQueue&&) = delete;
		MessageQueue& operator=(MessageQueue&&) = delete;

		~MessageQueue()
		{
			delete[] m_Buffer;
		}

		// TODO
		// void Resize(size_t maxElementCount);


		bool TryPush(T&& element)
		{
			auto popIndex = m_PopIndex.load(std::memory_order_acquire);
			auto pushIndex = m_PushIndex.load(std::memory_order_relaxed);
			if (popIndex == (pushIndex + 1) % m_Capacity)
			{
				// Queue is full
				return false;
			}
			m_Buffer[pushIndex] = std::move(element);
			m_PushIndex.store((pushIndex + 1) % m_Capacity, std::memory_order_release);
			return true;
		}

		bool TryPush(const T& element)
		{
			auto popIndex = m_PopIndex.load(std::memory_order_acquire);
			auto pushIndex = m_PushIndex.load(std::memory_order_relaxed);
			if (popIndex == (pushIndex + 1) % m_Capacity)
			{
				// Queue is full
				return false;
			}
			m_Buffer[pushIndex] = element;
			m_PushIndex.store((pushIndex + 1) % m_Capacity, std::memory_order_release);
			return true;
		}

		bool TryPop(T& element)
		{
			auto popIndex = m_PushIndex.load(std::memory_order_acquire);
			auto pushIndex = m_PopIndex.load(std::memory_order_relaxed);
			if (pushIndex == popIndex)
			{
				// Queue is empty
				return false;
			}
			element = std::move(m_Buffer[pushIndex]);
			m_PopIndex.store((pushIndex + 1) % m_Capacity, std::memory_order_release);
			return true;
		}



	private:
		const size_t m_Capacity = {};
		T* m_Buffer = {};
		std::atomic<uint32_t> m_PopIndex = {}; 
		std::atomic<uint32_t> m_PushIndex = {};

	};
}