#include <iostream>
#include <cassert>
#include "Cell.h"

using namespace project;

int main() {
    try {
        // Test default constructor
        {
            Cell c;
            assert(c.isNull());
            try {
                c.get<int64_t>();
                assert(false); // Should not reach here
            }
            catch (const std::bad_cast& e) {
                std::cout << "Caught expected std::bad_cast exception in default constructor test.\n";
            }
            catch (const std::exception& e) {
                std::cerr << "Caught unexpected std::exception: " << e.what() << "\n";
                assert(false);
            }
            catch (...) {
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
            try {
                c.get<int64_t>();
                assert(false); // Should not reach here
            }
            catch (const std::bad_cast& e) {
                std::cout << "Caught expected std::bad_cast exception in get<T> test.\n";
            }
            catch (const std::exception& e) {
                std::cerr << "Caught unexpected std::exception: " << e.what() << "\n";
                assert(false);
            }
            catch (...) {
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
            try {
                Cell c_add = c1 + c2;
                assert(false); // Should not reach here
            }
            catch (const std::runtime_error& e) {
                std::cout << "Caught expected std::runtime_error exception in arithmetic with strings test.\n";
            }
            catch (const std::exception& e) {
                std::cerr << "Caught unexpected std::exception: " << e.what() << "\n";
                assert(false);
            }
            catch (...) {
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
            Cell* c = new Cell("memory_test");
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

        // All tests passed
        std::cout << "All tests passed successfully!" << std::endl;
    }
    catch (const std::bad_cast& e) {
        std::cerr << "Unhandled std::bad_cast exception: " << e.what() << "\n";
        return 1; // Indicate failure
    }
    catch (const std::exception& e) {
        std::cerr << "Unhandled std::exception: " << e.what() << "\n";
        return 1; // Indicate failure
    }
    catch (...) {
        std::cerr << "Unhandled unknown exception.\n";
        return 1; // Indicate failure
    }

    return 0;
}
