#include <typeinfo>
#include "Cell.h"
using namespace std;
namespace project
{
	void Cell::destroy() noexcept
	{
		if (type == Type::STRING)
		{
			using std::string;
			stringValue.~string();
		}
		type = Type::NULL_TYPE;
	}
	Cell::Cell() : type(Type::NULL_TYPE)
	{
	}

	Cell::Cell(int64_t intValue) : type(Type::INT64), intValue(intValue)
	{
	}

	Cell::Cell(double doubleValue) : type(Type::DOUBLE), doubleValue(doubleValue)
	{
	}

	Cell::Cell(const std::string &stringValue) : type(Type::STRING)
	{
		new (&this->stringValue) std::string(stringValue);
	}

	Cell::Cell(std::string &&stringValue) : type(Type::STRING)
	{
		new (&this->stringValue) std::string(std::move(stringValue));
	}

	Cell &Cell::operator=(const Cell &other)
	{
		if (this != &other)
		{
			destroy();
			type = other.type;
			switch (type)
			{
			case Type::INT64:
				intValue = other.intValue;
				break;
			case Type::DOUBLE:
				doubleValue = other.doubleValue;
				break;
			case Type::STRING:
				new (&stringValue) std::string(other.stringValue);
				break;
			default:
				break;
			}
		}
		return *this;
	}

	// Move Assignment
	Cell &Cell::operator=(Cell &&other) noexcept
	{
		if (this != &other)
		{
			destroy();
			type = other.type;
			switch (type)
			{
			case Type::INT64:
				intValue = other.intValue;
				break;
			case Type::DOUBLE:
				doubleValue = other.doubleValue;
				break;
			case Type::STRING:
				new (&stringValue) std::string(std::move(other.stringValue));
				break;
			default:
				break;
			}
			other.destroy();
		}
		return *this;
	}
	// Copy Constructor
	Cell::Cell(const Cell &other) : type(other.type)
	{
		switch (type)
		{
		case Type::INT64:
			intValue = other.intValue;
			break;
		case Type::DOUBLE:
			doubleValue = other.doubleValue;
			break;
		case Type::STRING:
			new (&stringValue) std::string(other.stringValue);
			break;
		default:
			// Handle NULL_TYPE or other cases if necessary
			break;
		}
	}

	// Move Constructor
	Cell::Cell(Cell &&other) noexcept : type(other.type)
	{
		switch (type)
		{
		case Type::INT64:
			intValue = other.intValue;
			break;
		case Type::DOUBLE:
			doubleValue = other.doubleValue;
			break;
		case Type::STRING:
			new (&stringValue) std::string(std::move(other.stringValue));
			break;
		default:
			// Handle NULL_TYPE or other cases if necessary
			break;
		}
		other.destroy();
	}

	Cell::~Cell()
	{
		destroy();
	}

	bool Cell::isNull() const noexcept
	{
		return type == Type::NULL_TYPE;
	}

	void Cell::setNull() noexcept
	{
		destroy();
	}

	bool Cell::operator==(const Cell &other) const
	{
		if (type != other.type)
			return false;

		switch (type)
		{
		case project::Cell::Type::DOUBLE:
			return other.doubleValue == doubleValue;
		case project::Cell::Type::INT64:
			return other.intValue == intValue;
		case project::Cell::Type::STRING:
			return other.stringValue == stringValue;
		default:
			return true;
		}
	}

	bool Cell::operator!=(const Cell &other) const
	{
		return !(*this == other);
	}

	bool Cell::operator<(const Cell &other) const
	{
		if (type != other.type)
			throw std::runtime_error("Cannot compare Cells of different types");
		switch (type)
		{
		case Type::INT64:
			return intValue < other.intValue;
		case Type::DOUBLE:
			return doubleValue < other.doubleValue;
		case Type::STRING:
			return stringValue < other.stringValue;
		default:
			throw std::runtime_error("Cannot compare null Cells");
		}
	}

	bool Cell::operator>(const Cell &other) const { return other < *this; }
	bool Cell::operator<=(const Cell &other) const { return !(*this > other); }
	bool Cell::operator>=(const Cell &other) const { return !(*this < other); }

	// Arithmetic Operators
	Cell Cell::operator+(const Cell &other) const
	{
		if (type != other.type)
			throw std::runtime_error("Cannot add Cells of different types");
		switch (type)
		{
		case Type::INT64:
			return Cell(intValue + other.intValue);
		case Type::DOUBLE:
			return Cell(doubleValue + other.doubleValue);
		case Type::STRING:
			return Cell(stringValue + other.stringValue);
		default:
			throw std::runtime_error("Addition not supported for this type");
		}
	}

	Cell Cell::operator-(const Cell &other) const
	{
		if (type != other.type)
			throw std::runtime_error("Cannot subtract Cells of different types");
		switch (type)
		{
		case Type::INT64:
			return Cell(intValue - other.intValue);
		case Type::DOUBLE:
			return Cell(doubleValue - other.doubleValue);
		default:
			throw std::runtime_error("Subtraction not supported for this type");
		}
	}

	Cell Cell::operator*(const Cell &other) const
	{
		if (type != other.type)
			throw std::runtime_error("Cannot multiply Cells of different types");
		switch (type)
		{
		case Type::INT64:
			return Cell(intValue * other.intValue);
		case Type::DOUBLE:
			return Cell(doubleValue * other.doubleValue);
		default:
			throw std::runtime_error("Multiplication not supported for this type");
		}
	}

	Cell Cell::operator/(const Cell &other) const
	{
		if (type != other.type)
			throw std::runtime_error("Cannot divide Cells of different types");
		switch (type)
		{
		case Type::INT64:
			if (other.intValue == 0)
				throw std::runtime_error("Division by zero");
			return Cell(intValue / other.intValue);
		case Type::DOUBLE:
			if (other.doubleValue == 0.0)
				throw std::runtime_error("Division by zero");
			return Cell(doubleValue / other.doubleValue);
		default:
			throw std::runtime_error("Division not supported for this type");
		}
	}

	std::size_t Cell::hash() const
	{
		switch (type)
		{
		case Type::INT64:
			return std::hash<int64_t>{}(intValue);
		case Type::DOUBLE:
			return std::hash<double>{}(doubleValue);
		case Type::STRING:
			return std::hash<std::string>{}(stringValue);
		default:
			return 0;
		}
	}

}