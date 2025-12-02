#ifndef ID_GENERATOR_H
#define ID_GENERATOR_H

#include <queue>
#include <memory>

namespace GLib {

		template<typename T>
		class IDGenerator
		{
		public:
			IDGenerator(const T firstValue, const T incrementAmount = 100)  
			{ 
				this->Reset(firstValue); 
				this->m_IncrementValue = incrementAmount;
			}

			T GetNextId()
			{
				
				if (m_IdQueue.empty())
				{
					for (T i = m_LastId + 1; i < m_LastId + m_IncrementValue; i++)
						m_IdQueue.push(i);
				}

				// Get the next ID from the queue and pop it.
				m_LastId = m_IdQueue.front();
				m_IdQueue.pop();

				return m_LastId;
			}

			void Reset(const T firstValue)
			{
				// Clear out any remaining values.
				while (!m_IdQueue.empty())
					m_IdQueue.pop();

				// Give the 100 IDs to start with
				for (T i = firstValue; i < firstValue + 100; i++)
					m_IdQueue.push(i);

				m_FirstValue = m_LastId = firstValue;
			}

			
			void Release(const T id)
			{
				if (id > m_LastId)
					return; // Can't release something we never had assigned.

				m_IdQueue.push(id);
			}

			T GetLatestId() { return m_LastId; }
			T GetFirstValue() { return m_FirstValue; }

		private:
			T m_LastId;
			T m_FirstValue;
			T m_IncrementValue;

			std::queue<T> m_IdQueue;
	};
} // namespace GApi

#endif
