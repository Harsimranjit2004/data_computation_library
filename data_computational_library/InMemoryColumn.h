#pragma once
#ifndef HARSIMRAN_INMEMORYCOLUMN_H
#define HARSIMRAN_INMEMEORYCOLUMN_H
#include <vector>
#include <exception>
#include "Cell.h"
#include "IBaseColumn.h"
namespace project {
	template<typename T>
	class InMemoryColumn : public IBaseColumn {
		std::vector<T> m_data;

	public:
		

		InMemoryColumn(size_t initialReserve = 1000);
		InMemoryColumn(const InMemoryColumn<T>& other);
		InMemoryColumn(InMemoryColumn<T>&& other);

		//InMemoryColumn& operator=(const InMemoryColumn& other);
		//InMemoryColumn& operator=(InMemoryColumn&& other);


		size_t size() const;
		Cell get(size_t index) const;
		void set(size_t index, const Cell& cell);
		void append(const Cell& cell);
		std::shared_ptr<IBaseColumn> clone() const;
	};
	template<typename T>
	inline InMemoryColumn<T>::InMemoryColumn(size_t initialReserve)
	{
		m_data.reserve(initialReserve);
	}
	template<typename T>
	inline InMemoryColumn<T>::InMemoryColumn(const InMemoryColumn<T>& other): m_data(other.m_data)
	{
	}
	template<typename T>
	inline InMemoryColumn<T>::InMemoryColumn(InMemoryColumn<T>&& other): m_data(std::move(other.m_data))
	{
	}
	template<typename T>
	inline size_t InMemoryColumn<T>::size() const
	{
		return m_data.size();
	}
	template<typename T>
	inline Cell InMemoryColumn<T>::get(size_t index) const
	{
		if (index >= m_data.size()) {
			throw std::runtime_error("Index out of Bound");
		}
		else {
			return Cell(m_data[index]);
		}
	}

	template<typename T>
	inline void InMemoryColumn<T>::set(size_t index, const Cell& cell)
	{
		if (index >= m_data.size()) {
			throw std::runtime_error("Index out of Bound");
		}
		else if (!cell.is<T>()) {
			throw std::runtime_error("type of value not same");
		}
		else {
			m_data[index] = cell.get<T>();
		}
	}

	template<typename T>
	inline void InMemoryColumn<T>::append(const Cell& cell)
	{
		if (!cell.is<T>()) {
			throw std::runtime_error("type of value not same");
		}
		else {
			m_data.push_back(cell.get<T>());
		}
	}

	template<typename T>
	inline std::shared_ptr<IBaseColumn> InMemoryColumn<T>::clone() const
	{
		return std::make_shared<InMemoryColumn<T>>(*this);
	}
	
}
#endif