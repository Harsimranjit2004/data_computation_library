#pragma once
#ifndef HARSIMRAN_INMEMORYCOLUMN_H
#define HARSIMRAN_INMEMEORYCOLUMN_H
#include <vector>
#include <exception>
#include <execution>
#include <sstream>
#include <shared_mutex>
#include <atomic>
#include "Cell.h"
#include "IBaseColumn.h"
namespace project
{
	template <typename T>
	class InMemoryColumn : public IBaseColumn
	{
		struct MemoryStats
		{
			size_t allocated{0};
			size_t used{0};
			size_t peak{0};
			size_t allocations{0};

			void update(size_t newAllocated, size_t newUsed)
			{
				allocated = newAllocated;
				used = newUsed;
				peak = std::max(peak, used);
				allocations++;
			}

			void reset()
			{
				allocated = used = peak = allocations = 0;
			}
		};

		std::vector<T> m_data;
		static constexpr double GROWTH_FACTOR = 1.5;
		MemoryStats m_memStats;
		mutable std::shared_mutex m_mutex;
		std::atomic<bool> m_threadSafe{false};

		template <typename Func>
		auto withReadLock(Func &&func) const
		{
			if (!m_threadSafe)
				return func();
			std::shared_lock lock(m_mutex);
			return func();
		}

		template <typename Func>
		auto withWriteLock(Func &&func)
		{
			if (!m_threadSafe)
				return func();
			std::unique_lock lock(m_mutex);
			return func();
		}

		void updateMemoryUsage()
		{
			const size_t vectorOverhead = sizeof(std::vector<T>);
			const size_t elementSize = sizeof(T);

			size_t allocatedMem = vectorOverhead + (elementSize * m_data.capacity());
			size_t usedMem = vectorOverhead + (elementSize * m_data.size());

			m_memStats.update(allocatedMem, usedMem);
		}

		void smartReserve(size_t requiredSize)
		{
			if (requiredSize > m_data.capacity())
			{
				size_t oldCapacity = m_data.capacity();
				size_t newCapacity = std::max(
					static_cast<size_t>(m_data.capacity() * GROWTH_FACTOR),
					requiredSize);
				m_data.reserve(newCapacity);

				if (m_data.capacity() != oldCapacity)
				{
					updateMemoryUsage();
				}
			}
		}

	public:
		struct MemoryInfo
		{
			size_t allocated;
			size_t used;
			size_t peak;
			size_t allocations;
			double utilizationRate;

			std::string toString() const
			{
				std::ostringstream oss;
				oss << "Memory Info:\n"
					<< "  Allocated: " << (allocated / 1024.0) << " KB\n"
					<< "  Used: " << (used / 1024.0) << " KB\n"
					<< "  Peak: " << (peak / 1024.0) << " KB\n"
					<< "  Allocations: " << allocations << "\n"
					<< "  Utilization: " << (utilizationRate * 100) << "%";
				return oss.str();
			}
		};

		InMemoryColumn(size_t initialReserve = 1000)
		{
			smartReserve(initialReserve);
			updateMemoryUsage();
		}

		InMemoryColumn(const InMemoryColumn<T> &other) : m_data(other.m_data)
		{
			updateMemoryUsage();
		}

		InMemoryColumn(InMemoryColumn<T> &&other) : m_data(std::move(other.m_data))
		{
			updateMemoryUsage();
		}

		MemoryInfo getMemoryInfo() const
		{
			return withReadLock([this]
								{ return MemoryInfo{
									  m_memStats.allocated,
									  m_memStats.used,
									  m_memStats.peak,
									  m_memStats.allocations,
									  static_cast<double>(m_memStats.used) / m_memStats.allocated}; });
		}

		void append(const Cell &cell) override
		{
			withWriteLock([this, &cell]
						  {
				if (!cell.is<T>())
				{
					throw std::runtime_error("type of value not same");
				}

				size_t oldCapacity = m_data.capacity();
				m_data.push_back(cell.get<T>());

				if (m_data.capacity() != oldCapacity)
				{
					updateMemoryUsage();
				} });
		}

		void shrinkToFit()
		{
			m_data.shrink_to_fit();
			updateMemoryUsage();
		}

		void clear()
		{
			m_data.clear();
			m_data.shrink_to_fit();
			m_memStats.reset();
		}

		size_t size() const override
		{
			return withReadLock([this]
								{ return m_data.size(); });
		}

		Cell get(size_t index) const override
		{
			return withReadLock([this, index]
								{
				if (index >= m_data.size())
				{
					throw std::runtime_error("Index out of Bound");
				}
				return Cell(m_data[index]); });
		}

		void set(size_t index, const Cell &cell) override
		{
			withWriteLock([this, index, &cell]
						  {
				if (index >= m_data.size())
				{
					throw std::runtime_error("Index out of Bound");
				}
				if (!cell.is<T>())
				{
					throw std::runtime_error("type of value not same");
				}
				m_data[index] = cell.get<T>(); });
		}

		std::shared_ptr<IBaseColumn> clone() const
		{
			return std::make_shared<InMemoryColumn<T>>(*this);
		}

		////////////////////////////////////////////////////////////// ARITHMETIC OPERATIONS ////////////////////////////////////////////////////////////////
		template <typename U = T>
		typename std::enable_if<std::is_arithmetic<U>::value, U>::type
		mean() const;

		template <typename U = T>
		typename std::enable_if<std::is_arithmetic<U>::value, U>::type
		sum() const;

		template <typename U = T>
		typename std::enable_if<std::is_arithmetic<U>::value, U>::type
		min() const;

		template <typename U = T>
		typename std::enable_if<std::is_arithmetic<U>::value, U>::type
		max() const;

		////////////////////////////////////////////////////////////// ARITHMETIC OPERATIONS ////////////////////////////////////////////////////////////////

		void setThreadSafe(bool threadSafe)
		{
			m_threadSafe = threadSafe;
		}

		bool isThreadSafe() const
		{
			return m_threadSafe;
		}
	};
	template <typename T>
	inline InMemoryColumn<T>::InMemoryColumn(size_t initialReserve)
	{
		smartReserve(initialReserve);
		updateMemoryUsage();
	}
	template <typename T>
	inline InMemoryColumn<T>::InMemoryColumn(const InMemoryColumn<T> &other) : m_data(other.m_data)
	{
		updateMemoryUsage();
	}
	template <typename T>
	inline InMemoryColumn<T>::InMemoryColumn(InMemoryColumn<T> &&other) : m_data(std::move(other.m_data))
	{
		updateMemoryUsage();
	}
	template <typename T>
	inline size_t InMemoryColumn<T>::size() const
	{
		return withReadLock([this]
							{ return m_data.size(); });
	}
	template <typename T>
	inline Cell InMemoryColumn<T>::get(size_t index) const
	{
		return withReadLock([this, index]
							{
			if (index >= m_data.size())
			{
				throw std::runtime_error("Index out of Bound");
			}
			return Cell(m_data[index]); });
	}

	template <typename T>
	inline void InMemoryColumn<T>::set(size_t index, const Cell &cell)
	{
		withWriteLock([this, index, &cell]
					  {
			if (index >= m_data.size())
			{
				throw std::runtime_error("Index out of Bound");
			}
			if (!cell.is<T>())
			{
				throw std::runtime_error("type of value not same");
			}
			m_data[index] = cell.get<T>(); });
	}

	template <typename T>
	inline std::shared_ptr<IBaseColumn> InMemoryColumn<T>::clone() const
	{
		return std::make_shared<InMemoryColumn<T>>(*this);
	}

	template <typename T>
	template <typename U>
	inline typename std::enable_if<std::is_arithmetic<U>::value, U>::type InMemoryColumn<T>::mean() const
	{
		return withReadLock([this]
							{
            if(m_data.empty())
            {
                throw std::runtime_error("Column is empty");
            }
			return std::reduce(std::execution::par,
								m_data.begin(),
								m_data.end())/m_data.size(); });
	}

	template <typename T>
	template <typename U>
	inline typename std::enable_if<std::is_arithmetic<U>::value, U>::type InMemoryColumn<T>::sum() const
	{
		return withReadLock([this]
							{
								if (m_data.empty())
								{
									throw std::runtime_error("Column is empty");
								}
								return std::reduce(std::execution::par,
												   m_data.begin(),
												   m_data.end(),
												   U(0));
							});
	}
}
#endif
// column ERROR need to bne handledd in future
