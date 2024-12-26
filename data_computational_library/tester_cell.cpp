#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <random>
#include "Cell.h"

using namespace project;

int main()
{
    try
    {
        // Test default constructor
        {
            Cell c;
            assert(c.isNull());
            try
            {
                c.get<int64_t>();
                assert(false); // Should not reach here
            }
            catch (const std::bad_cast &e)
            {
                std::cout << "Caught expected std::bad_cast exception in default constructor test.\n";
            }
            catch (const std::exception &e)
            {
                std::cerr << "Caught unexpected std::exception: " << e.what() << "\n";
                assert(false);
            }
            catch (...)
            {
                std::cerr << "Caught unexpected unknown exception.\n";
                assert(false);
            }
        }

        // Test int64_t constructor
        {
            Cell c(int64_t(42));
            assert(!c.isNull());
            assert(c.is<int64_t>());
            assert(!c.is<double>());
            assert(!c.is<std::string>());
            assert(c.get<int64_t>() == 42);
        }

        // Test double constructor
        {
            Cell c(3.14);
            assert(!c.isNull());
            assert(c.is<double>());
            assert(c.get<double>() == 3.14);
        }

        // Test std::string constructor (lvalue)
        {
            std::string s = "hello";
            Cell c(s);
            assert(c.is<std::string>());
            assert(c.get<std::string>() == "hello");
        }

        // Test std::string constructor (rvalue)
        {
            Cell c(std::string("world"));
            assert(c.is<std::string>());
            assert(c.get<std::string>() == "world");
        }

        // Test const char* constructor
        {
            Cell c("test");
            assert(c.is<std::string>());
            assert(c.get<std::string>() == "test");
        }

        // Test copy constructor
        {
            Cell c1(int64_t(100));
            Cell c2(c1);
            assert(c2.is<int64_t>());
            assert(c2.get<int64_t>() == 100);
        }

        // Test move constructor
        {
            Cell c1(200.5);
            Cell c2(std::move(c1));
            assert(c2.is<double>());
            assert(c2.get<double>() == 200.5);
            assert(c1.isNull()); // After move, c1 should be null
        }

        // Test copy assignment operator
        {
            Cell c1("copy");
            Cell c2;
            c2 = c1;
            assert(c2.is<std::string>());
            assert(c2.get<std::string>() == "copy");
        }

        // Test move assignment operator
        {
            Cell c1("move");
            Cell c2;
            c2 = std::move(c1);
            assert(c2.is<std::string>());
            assert(c2.get<std::string>() == "move");
            assert(c1.isNull()); // After move, c1 should be null
        }

        // Test setNull method
        {
            Cell c(int64_t(500));
            c.setNull();
            assert(c.isNull());
        }

        // Test set method with int64_t
        {
            Cell c;
            c.set(int64_t(1000));
            assert(c.is<int64_t>());
            assert(c.get<int64_t>() == 1000);
        }

        // Test set method with double
        {
            Cell c;
            c.set(123.456);
            assert(c.is<double>());
            assert(c.get<double>() == 123.456);
        }

        // Test set method with std::string
        {
            Cell c;
            c.set(std::string("set_string"));
            assert(c.is<std::string>());
            assert(c.get<std::string>() == "set_string");
        }

        // Test set method with const char*
        {
            Cell c;
            c.set("const_char");
            assert(c.is<std::string>());
            assert(c.get<std::string>() == "const_char");
        }

        // Test is<T> method
        {
            Cell c(int64_t(42));
            assert(c.is<int64_t>());
            assert(!c.is<double>());
            assert(!c.is<std::string>());
        }

        // Test get<T> method with correct type
        {
            Cell c(3.1415);
            double val = c.get<double>();
            assert(val == 3.1415);
        }

        // Test get<T> method with incorrect type
        {
            Cell c("error_test");
            try
            {
                c.get<int64_t>();
                assert(false); // Should not reach here
            }
            catch (const std::bad_cast &e)
            {
                std::cout << "Caught expected std::bad_cast exception in get<T> test.\n";
            }
            catch (const std::exception &e)
            {
                std::cerr << "Caught unexpected std::exception: " << e.what() << "\n";
                assert(false);
            }
            catch (...)
            {
                std::cerr << "Caught unexpected unknown exception.\n";
                assert(false);
            }
        }

        // Test comparison operators
        {
            Cell c1(int64_t(50));
            Cell c2(int64_t(50));
            Cell c3(int64_t(100));

            assert(c1 == c2);
            assert(c1 != c3);
            assert(c1 < c3);
            assert(c3 > c1);
            assert(c1 <= c2);
            assert(c3 >= c2);
        }

        // Test arithmetic operators with int64_t
        {
            Cell c1(int64_t(10));
            Cell c2(int64_t(20));
            Cell c_add = c1 + c2;
            Cell c_sub = c1 - c2;
            Cell c_mul = c1 * c2;
            Cell c_div = c2 / c1;

            assert(c_add.get<int64_t>() == 30);
            assert(c_sub.get<int64_t>() == -10);
            assert(c_mul.get<int64_t>() == 200);
            assert(c_div.get<int64_t>() == 2);
        }

        // Test arithmetic operators with double
        {
            Cell c1(5.5);
            Cell c2(2.0);
            Cell c_add = c1 + c2;
            Cell c_sub = c1 - c2;
            Cell c_mul = c1 * c2;
            Cell c_div = c1 / c2;

            assert(c_add.get<double>() == 7.5);
            assert(c_sub.get<double>() == 3.5);
            assert(c_mul.get<double>() == 11.0);
            assert(c_div.get<double>() == 2.75);
        }

        // Test arithmetic operators with mixed types
        {
            Cell c1(int64_t(10));
            Cell c2(3.0);
            Cell c_add = c1 + c2;
            assert(c_add.is<double>());
            assert(c_add.get<double>() == 13.0);
        }

        // Test arithmetic operators with strings (should throw exception)
        {
            Cell c1("foo");
            Cell c2("bar");
            try
            {
                Cell c_add = c1 + c2;
                assert(false); // Should not reach here
            }
            catch (const std::runtime_error &e)
            {
                std::cout << "Caught expected std::runtime_error exception in arithmetic with strings test.\n";
            }
            catch (const std::exception &e)
            {
                std::cerr << "Caught unexpected std::exception: " << e.what() << "\n";
                assert(false);
            }
            catch (...)
            {
                std::cerr << "Caught unexpected unknown exception.\n";
                assert(false);
            }
        }

        // Test hash function
        {
            Cell c1(int64_t(123));
            Cell c2(int64_t(123));
            Cell c3("hash_test");
            assert(c1.hash() == c2.hash());
            assert(c1.hash() != c3.hash());
        }

        // Test destructor (no memory leaks)
        {
            Cell *c = new Cell("memory_test");
            delete c;
        }

        // Test self-assignment
        {
            Cell c(int64_t(999));
            c = c;
            assert(c.get<int64_t>() == 999);
        }

        // Test set method with unsupported type (should trigger static_assert)
        /*
        {
            Cell c;
            // Uncommenting the following line should cause a compile-time error
            // c.set(true);
        }
        */

        // Test move semantics with std::string
        {
            std::string s = "move_string";
            Cell c(std::move(s));
            assert(c.get<std::string>() == "move_string");
            // Note: After moving, 's' may or may not be empty; it's in a valid but unspecified state.
        }

        // Test isNull after setting value
        {
            Cell c;
            c.set(int64_t(0));
            assert(!c.isNull());
        }

        // Test extreme values for int64_t
        {
            Cell c1(std::numeric_limits<int64_t>::max());
            Cell c2(std::numeric_limits<int64_t>::min());
            assert(c1.get<int64_t>() == std::numeric_limits<int64_t>::max());
            assert(c2.get<int64_t>() == std::numeric_limits<int64_t>::min());

            // Test overflow behavior
            try
            {
                Cell result = c1 + Cell(int64_t(1));
                assert(false); // Should not reach here
            }
            catch (const std::overflow_error &e)
            {
                std::cout << "Caught expected overflow exception.\n";
            }
        }

        // Test floating-point precision and special values
        {
            Cell c1(std::numeric_limits<double>::infinity());
            Cell c2(-std::numeric_limits<double>::infinity());
            Cell c3(std::numeric_limits<double>::quiet_NaN());

            assert(std::isinf(c1.get<double>()));
            assert(std::isinf(c2.get<double>()));
            assert(std::isnan(c3.get<double>()));

            // Test very small and very large doubles
            Cell c4(std::numeric_limits<double>::min());
            Cell c5(std::numeric_limits<double>::max());
            assert(c4.get<double>() == std::numeric_limits<double>::min());
            assert(c5.get<double>() == std::numeric_limits<double>::max());
        }

        // Test string edge cases
        {
            // Empty string
            Cell c1("");
            assert(c1.get<std::string>().empty());

            // Very long string
            std::string longStr(1000000, 'a'); // 1M characters
            Cell c2(longStr);
            assert(c2.get<std::string>() == longStr);

            // String with special characters
            Cell c3("Hello\n\t\r\0World");
            assert(c3.get<std::string>().length() > 0);

            // Unicode string
            Cell c4("Hello 世���！");
            assert(c4.get<std::string>() == "Hello 世界！");
        }

        // Test complex chain of operations
        {
            Cell c1(int64_t(100));
            Cell c2(200.5);
            Cell c3("test");

            // Chain of assignments
            Cell result = c1;
            result = c2;
            result = c3;
            result = Cell(int64_t(42));
            assert(result.get<int64_t>() == 42);

            // Multiple moves
            Cell temp1(std::move(c1));
            Cell temp2(std::move(temp1));
            Cell temp3(std::move(temp2));
            assert(c1.isNull());
            assert(temp1.isNull());
            assert(temp2.isNull());
            assert(!temp3.isNull());
        }

        // Test arithmetic operation chains
        {
            Cell a(int64_t(10));
            Cell b(int64_t(20));
            Cell c(int64_t(30));

            Cell result = ((a + b) * c) / (b - a);
            assert(result.get<int64_t>() == 90); // ((10 + 20) * 30) / (20 - 10) = 900 / 10 = 90
        }

        // Test comparison edge cases
        {
            Cell nullCell1;
            Cell nullCell2;
            assert(nullCell1 == nullCell2);

            Cell zero1(int64_t(0));
            Cell zero2(0.0);
            assert(zero1 != zero2); // Different types should not be equal

            Cell inf1(std::numeric_limits<double>::infinity());
            Cell inf2(std::numeric_limits<double>::infinity());
            assert(inf1 == inf2);
        }

        // Test exception safety with nested operations
        {
            try
            {
                Cell c1("string");
                Cell c2(int64_t(42));
                Cell result = (c1 + c2) * Cell(2.0); // Should throw
                assert(false);                       // Should not reach here
            }
            catch (const std::runtime_error &)
            {
                // Expected
            }
        }

        // Test memory management under stress
        {
            std::vector<Cell> cells;
            for (int i = 0; i < 1000; ++i)
            {
                cells.emplace_back(std::string(1000, 'x')); // Create large strings
            }

            // Move cells around
            std::shuffle(cells.begin(), cells.end(), std::random_device{});

            // Clear half the cells
            for (size_t i = 0; i < cells.size() / 2; ++i)
            {
                cells[i].setNull();
            }

            // Verify remaining cells are intact
            for (size_t i = cells.size() / 2; i < cells.size(); ++i)
            {
                assert(cells[i].get<std::string>() == std::string(1000, 'x'));
            }
        }

        // Test hash consistency
        {
            Cell c1(int64_t(42));
            Cell c2(int64_t(42));
            Cell c3(42.0);

            assert(c1.hash() == c2.hash()); // Same type and value
            assert(c1.hash() != c3.hash()); // Different types

            // Hash should remain consistent after moves
            Cell c4(std::move(c1));
            assert(c4.hash() == c2.hash());
        }

        std::cout << "All enhanced tests passed successfully!" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
