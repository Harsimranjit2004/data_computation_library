#pragma once
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <typeinfo>
#include <algorithm>
#include <numeric>

namespace display
{

    class TableDisplay
    {
    private:
        static constexpr size_t MAX_ROWS = 20;
        static constexpr size_t MAX_ROWS_DISPLAY = 5;
        static constexpr size_t MAX_COL_WIDTH = 50;
        static constexpr size_t MIN_COL_WIDTH = 8;
        static constexpr size_t INDEX_WIDTH = 6;

        struct ColumnFormat
        {
            size_t width;
            size_t precision;
            bool isNumeric;
            std::string alignment;
            bool isDateTime = false;
        };

        struct DisplayOptions
        {
            bool showIndex = true;
            bool showRowCount = true;
            bool showColumnTypes = true;
            bool alignNumericRight = true;
            bool showNullAsNaN = true;
            bool truncateStrings = true;
            size_t maxRows = MAX_ROWS;
            size_t displayRows = MAX_ROWS_DISPLAY;
            std::string dateFormat = "%Y-%m-%d";
            std::string nullValue = "NaN";
            size_t maxPrecision = 6;
        };

    public:
        template <typename T>
        static std::string display(const std::vector<std::vector<T>> &data,
                                   const std::vector<std::string> &headers,
                                   const DisplayOptions &options = DisplayOptions())
        {
            try
            {
                validateInput(data, headers);
                return formatTable(data, headers, options);
            }
            catch (const std::exception &e)
            {
                return std::string("Error: ") + e.what();
            }
        }

        // Static methods to create different display styles
        static DisplayOptions defaultStyle()
        {
            return DisplayOptions();
        }

        static DisplayOptions compactStyle()
        {
            DisplayOptions opt;
            opt.showColumnTypes = false;
            opt.displayRows = 3;
            return opt;
        }

        static DisplayOptions detailedStyle()
        {
            DisplayOptions opt;
            opt.maxPrecision = 8;
            opt.showColumnTypes = true;
            opt.displayRows = 10;
            return opt;
        }

    private:
        // ... rest of the implementation methods ...
    };

} // namespace display