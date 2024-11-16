#pragma once
#ifndef HARSIMRAN_BASECOLUMN_H
#define HARSIMRAN_BASECOULUM_H
#include <memory>
#include "Cell.h"
namespace project {
	class BaseColumn {
	public:
		virtual ~BaseColumn() = default;
		virtual size_t size() const = 0;
		virtual Cell get(size_t index) const = 0;
		virtual void set(size_t index, const Cell& cell) = 0;
		virtual void append(const Cell& cell) = 0;
		virtual std::shared_ptr<BaseColumn> clone() const = 0;
	};
}
#endif