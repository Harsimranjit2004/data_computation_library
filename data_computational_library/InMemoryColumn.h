#pragma once
#ifndef HARSIMRAN_INMEMORYCOLUMN_H
#define HARSIMRAN_INMEMEORYCOLUMN_H
#include <vector>
#include <exception>
#include <execution>
#include <sstream>
#include <shared_mutex>
#include <atomic>
#include "IBaseColumn.h"
#include "Cell.h"
namespace project
{
	template <typename T>
	class InMemoryColumn : public IBaseColumn
	{
	private:
		//////////////////// INTERNAL STRUCTURES ////////////////////
		struct MemoryStats
		{
			size_t allocated{0};
			size_t used{0};
			size_t peak{0};
			size_t allocations{0};
			d

				void
				update(size_t newAllocated, size_t newUsed)
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

		//////////////////// MEMBER VARIABLES ////////////////////
		std::vector<T> m_data;

		static constexpr double GROWTH_FACTOR = 1.5;
		MemoryStats m_memStats;
		mutable std::shared_mutex m_mutex;
		std::atomic<bool> m_threadSafe{false};

		//////////////////// PRIVATE HELPER METHODS ////////////////////
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
		//////////////////// PUBLIC STRUCTURES ////////////////////
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

		InMemoryColumn<T> diff(size_t periods = 1) const;
		InMemoryColumn<T> pct_change(size_t periods = 1) const;
		InMemoryColumn<double> normalize() const;

		size_t count_na() const;
		bool has_na() const;
		InMemoryColumn<T> dropna() const;
		InMemoryColumn<T> interpolate(const std::string &method = "linear") const;

		InMemoryColumn<T> rolling_min(size_t window) const;
		InMemoryColumn<T> rolling_max(size_t window) const;
		InMemoryColumn<T> rolling_std(size_t window) const;

		// Cumulative operations
		InMemoryColumn<T> cumsum() const;
		InMemoryColumn<T> cummin() const;
		InMemoryColumn<T> cummax() const;
		InMemoryColumn<T> cumprod() const;
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

		template <typename U = T>
		typename std::enable_if<std::is_arithmetic<U>::value, U>::type
		median() const;

		template <typename U = T>
		typename std::enable_if<std::is_arithmetic<U>::value, U>::type
		variance(bool population = false) const;

		template <typename U = T>
		typename std::enable_if<std::is_arithmetic<U>::value, U>::type
		stddev(bool population = true) const;

		void fillna(const T &value);
		std::vector<size_t> unique() const;
		std::map<T, size_t> value_counts() const;
		void sort(bool ascending = true);

		template <typename U = T>
		typename std::enable_if<std::is_arithmetic<U>::value, U>::type
		skew() const;

		template <typename U = T>
		typename std::enable_if<std::is_arithmetic<U>::value, U>::type
		kurtosis() const;
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

	/////////////////// IMPLEMENTATION //////////////////////////
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
												   U(0)); });
	}
	template <typename T>
	template <typename U>
	inline typename std::enable_if<std::is_arithmetic<U>::value, U>::type InMemoryColumn<T>::min() const
	{
		return withReadLock([this]
							{
			if(m_data.empty()){
				throw std::runtime_error("Column is empty");
			}
			return *std::min_element(std::execution::par, m_data.begin(), m_data.end()); })
	}
	template <typename T>
	template <typename U>
	inline typename std::enable_if<std::is_arithmetic<U>::value, U>::type InMemoryColumn<T>::max() const
	{
		return withReadLock([this]
							{
			if (m_data.empty())
			{
				throw std::runtime_error("Column is empty");
			}
			return *std::max_element(std::execution::par, m_data.begin(), m_data.end()); })
	}
	template <typename T>
	template <typename U>
	inline typename std::enable_if<std::is_arithmetic<U>::value, U>::type InMemoryColumn<T>::median() const
	{
		return withReadLock([this]
							{
			if(m_data.empty()){
				throw std::runtime_error("Column is empty");
			}
			std::vector<T> temp_data = m_data;
			const size_t n = temp_data.size();
			const size_t mid = n/2;

			if(n %2 == 0) // even number
			{
				std::nth_element(std::execution::par_unseq,temp_data.begin(), temp_data.begin() + mid, temp_data.end());
				const T right_mid = temp_data[mid];

				std::nth_element(std::execution::par_unseq,temp_data.begin(), temp_data.begin() + mid -1 , temp_data.end());

				const T left_mid = temp_data[mid-1];
				return static_cast<U>(left_mid + right_mid);
			}else{// odd number of elements
				std::nth_element(std::execution::par_unseq,temp_data.begin(), temp_data.begin() + mid, temp_data.end());
				return static_cast<U>(temp_data[mid]);
			} })
	}
	template <typename T>
	template <typename U>
	inline typename std::enable_if<std::is_arithmetic<U>::value, U>::type InMemoryColumn<T>::variance(bool population = false) const
	{
		return withReadLock([this]() -> U
							{
								const size_t n = m_data.size();
								if (n == 0)
									throw std::runtime_error("Column is empty");
								if (n == 1)
									return 0;

								struct AccumulatorBlock
								{
									U mean{0};
									U m2{0};
									size_t count{0};

									void merge(const AccumulatorBlock &other)
									{
										if (other.count == 0)
											return;

										const size_t combined_count = count + other.count;
										const U delta = other.mean - mean;
										mean += delta * other.count / combined_count;
										m2 += other.m2 + delta * delta * count * other.count / combined_count;
										count = combined_count;
									}
								};

								// paralle reduction
								const size_t chunk_size = 1024;
								const size_t num_chunks = (n + chunk_size - 1) / chunk_size;
								std::vector<AccumulatorBlock> accumulators(num_chunks);

#pragma omp parallel for
								for (size_t i = 0; i < num_chunks; i++)
								{
									const size_t start = i * chunk_size;
									const size_t end = std::min(start + chunk_size, n);

									U chunk_mean = 0;
									U chunk_m2 = 0;
									size_t chunk_count = 0;

									for (size_t j = start; j < end; j++)
									{
										const U value = static_cast<U>(m_data[j]);
										const U delta = value - chunk_count;
										chunk_count++;
										chunk_mean += delta / chunk_count;
										const U delta2 = value - chunk_mean;
										chunk_m2 += delta * delta2;
									}
									accumulators[i] = {chunk_mean, chunk_m2, chunk_count};
								}
								AccumulatorBlock final_acc = std::reduce(
									std::execution::par_unseq,
									accumulators.begin(),
									accumulators.end(),
									AccumulatorBlock{},
									[](AccumulatorBlock a, AccumulatorBlock b)
									{
										a.merge(b);
										return a;
									}) })
	}
	template <typename T>
	template <typename U>
	inline typename std::enable_if<std::is_arithmetic<U>::value, U>::type InMemoryColumn<T>::stddev(bool population = false) const
	{
		return std::sqrt(variance<U>(population));
	}
	template <typename T>
	template <typename U>
	inline typename std::enable_if<std::is_arithmetic<U>::value, U>::type InMemoryColumn<T>::skew() const
	{
		return withReadLock([this]
							{
			if(m_data.size() < 3){
				throw std::runtime_error("Need at least 3 points");
			}
			U m = mean<U>();
			U s = stddev<U>();
			U n = static_cast<U>(m_data.size());

			U sum = std::transform_reduce(
				std::execution::par, 
				m_data.begin(), 
				m_data.end(),
				0.0, 
				std::plus<>();
				[m, s](const T& x){return std::pow((x-m)/s , 3);}
			) 

			return (n/ ((n-1) * (n-2))) * sum; })
	}
	template <typename T>
	template <typename U>
	inline typename std::enable_if<std::is_arithmetic<U>::value, U>::type InMemoryColumn<T>::kurtosis() const
	{
		return withReadLock([this]
							{
			if(m_data.size() < 4){
				throw std::runtime_error("need at least 4 data pointsS");
			}
			U m  = mean<U>();
			U s = stddev<U>();
			U n = static_cast<U>(m_data.size());

			U sum = std::transform_reduce(
				std::execution::par, 
				m_data.begin(), 
				m_data.end(), 
				0.0, 
				std::plus<>(),
				[m, s](const T& x){ return std::pow((x-m)/s, 4);}
			);
			U a = (n* (n+1))/ ((n-1)* (n-2)* (n-3));
			U b = 3* std::pow(n-1, 2)/ ((n-2)* (n-3));

			return a*sum - b; })
	}
	template <typename T>
	inline void InMemoryColumn<T>::sort(bool ascending)
	{
		withWriteLock([this, ascending]
					  {
			if(m_data.empty()){
				throw std::runtime_error("Column is empty");
			}
			std::sort(std::execution::par_unseq, m_data.begin(), m_data.end(), ascending ? std::less<T>() : std::greater<T>()); });
	}
}
#endif
// column ERROR need to bne handledd in future