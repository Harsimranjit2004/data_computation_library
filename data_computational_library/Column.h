#pragma once
#ifndef HARSIMRAN_COLUMN_H
#define HARSIMRAN_COLUMN_H
#include <vector>
#include "BaseColumn.h"
namespace project {
	template<typename T> 
	class Column : public BaseColumn {
	private:
		set::vector<T> data;
	public:
		Column() = default;
		Column(const std::vector<T>& data);
		Column(const std::vector<T>&& data);

		Column(const Column<T>& other);
		Column(Column<T>&& other) noexcept;

		Column& operator=(const Column<T>& other);
		Column& operator=(Column<T>&& other);

		~Column() override;

		size_t size() const override;
		Cell get(size_t index) const override;
		void set(size_t index, const Cell& cell) override;
		void append(const Cell& cell) override;
		std::shared_ptr<T>& clone() const override;

		// additional
		const std::vector<T>& getData() const;
		void reserve(size_t n);

		template<typename U = T> 
		typename std::enable_if<std::is_arithmetic<U>::value, double>::type 
	    mean() const;


		template<typename U = T>
		typename std::enable_if<std::is_arithmetic<U>::value, double>::type
		sum() const;
		
		T min() const;

		T max() const;

	};
}
#endif