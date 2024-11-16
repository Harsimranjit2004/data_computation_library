#pragma once
#ifndef HARSIMRAN_CELL_H
#define HARSIMRAN_CELL_H
#include <string> 
#include <stdexcept>
#include <type_traits>
namespace project {

	class Cell {
	private:
	
		enum class Type { NULL_TYPE, INT64, DOUBLE, STRING };
		Type type;
		union {
			int64_t intValue;
			double doubleValue;
			std::string stringValue;
		};
		void destroy();
		struct always_false : std::false_type {};
	public:
		Cell();
		Cell(int64_t intValue);
		Cell(double doubleValue);
		Cell(const std::string& stringValue);
		Cell(std::string&& stringValue);


		Cell(const Cell& other) ;
		Cell(Cell&& other) noexcept;

		Cell& operator=(const Cell& other);
		Cell& operator=(Cell&& other);

		~Cell();

		bool isNull() const noexcept;
		bool setNull() noexcept;

		template<typename T>
		T get() const;

		template<typename T> 
		void set(T&& val);

		template<typename T> 
		bool is() const noexcept;

		bool operator==(const Cell& other) const;
		bool operator!=(const Cell& other) const;
		bool operator<(const Cell& other) const;
		bool operator>(const Cell& other) const;
		bool operator<=(const Cell& other) const;
		bool operator>=(const Cell& other) const;

		Cell operator+(const Cell& other) const;
		Cell operator-(const Cell& other) const;
		Cell operator*(const Cell& other) const;
		Cell operator/(const Cell& other) const;

		std::size_t hash() const;
	};
	template<typename T>
	T Cell::get() const {
		static_assert(always_false<T>::value, "Unsupported type for Cell::get()");
	}

	// Specializations for get<T>()
	template<>
	int64_t Cell::get<int64_t>() const {
		if (type != Type::INT64) {
			throw "bad cast";
		}
		return intValue;
	}

	template<>
	double Cell:: get<double>() const {
		if (type != Type::DOUBLE) {
			throw "bad cast";
		}
		return doubleValue;
	}

	template<>
	std::string Cell::get<std::string>() const {
		if (type != Type::STRING) {
			throw "bad cast";
		}
		return stringValue;
	}
	 template<typename T>
	 inline void Cell::set(T&& val)
	 {
		 if (type == Type::STRING) {
			 stringValue.~basic_string();
		 }
		 using DecayT = std::decay_t<T>;

		 if constexpr (std::is_same_v<DecayT, int64_t>) {
			 type = Type::INT64;
			 intValue = val;
		 }
		 else if  constexpr (std::is_same_v<DecayT, double>) {
			 type = Type::DOUBLE;
			 doubleValue = val;
		 }
		 else if constexpr (std::is_same_v <DecayT, std::string>) {
			 new (&this->stringValue) std::string(std::forward<T>(val));
			 type = Type::STRING;
		 }
		 else if constexpr (std::is_same_v < DecayT, const char*>) {
			 new (&this->stringValue) std::string(val);
			 type = Type::STRING;
		 }
		 else {
			 static_assert(always_false<T>::value, "Unsupported type");
		 }
	 }
	 template<typename T>
	 inline bool Cell::is() const noexcept
	 {
		 if constexpr (std::is_same_v<T, int64_t>) {
			 return type == Type::INT64;
		 }
		 else if constexpr (std::is_same_v<T, double>) {
			 return type == Type::DOUBLE;
		 }
		 else if constexpr (std::is_same_v<T, std::string>) {
			 return type == Type::STRING;
		 }
		 else {
			 return false;
		 }
	 }
}
#endif