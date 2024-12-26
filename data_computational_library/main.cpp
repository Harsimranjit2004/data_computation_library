#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <variant>
#include <random>
#include <typeinfo>

class TableVisualizer
{
private:
    static constexpr size_t MAX_ROWS = 20;
    static constexpr size_t MAX_ROWS_DISPLAY = 5; // Show first/last N rows
    static constexpr size_t MAX_COL_WIDTH = 50;   // Increased for better display
    static constexpr size_t MIN_COL_WIDTH = 8;
    static constexpr size_t INDEX_WIDTH = 6;

    struct ColumnFormat
    {
        size_t width;
        size_t precision;
        bool isNumeric;
        std::string alignment;
        bool isDateTime = false; // Add datetime handling
    };

    // Add error handling class
    class TableError : public std::runtime_error
    {
    public:
        explicit TableError(const std::string &msg) : std::runtime_error(msg) {}
    };

public:
    class TableStyle
    {
    public:
        std::string topBorder;
        std::string bottomBorder;
        std::string headerSep;
        std::string verticalSep;
        bool showIndex = true;
        bool showRowCount = true;
        bool showColumnTypes = true;   // Show data types in summary
        bool alignNumericRight = true; // Right-align numbers
        bool showNullAsNaN = true;     // Show NULL as NaN
        bool truncateStrings = true;   // Truncate long strings
        size_t maxRows = MAX_ROWS;
        size_t maxColWidth = MAX_COL_WIDTH;
        size_t displayRows = MAX_ROWS_DISPLAY; // Rows to show at start/end
        std::string dateFormat = "%Y-%m-%d";   // Date format string
        std::string timeFormat = "%H:%M:%S";   // Time format string
        bool showTruncationIndicator = true;
        bool showRowCount = true;
        std::string nullValue = "NaN";
        std::string numberAlignment = "right";
        std::string textAlignment = "left";
        size_t maxPrecision = 6;

        TableStyle()
            : topBorder(" "), bottomBorder(" "), headerSep(" "), verticalSep(" "),
              maxRows(MAX_ROWS), maxColWidth(MAX_COL_WIDTH)
        {
        }
    };

    template <typename T>
    static std::string format(
        const std::vector<std::vector<T>> &data,
        const std::vector<std::string> &headers,
        const TableStyle &style = TableStyle())
    {
        try
        {
            validateInput(data, headers);
            return formatImpl(data, headers, style);
        }
        catch (const TableError &e)
        {
            return std::string("Error: ") + e.what();
        }
        catch (const std::exception &e)
        {
            return "Unexpected error: " + std::string(e.what());
        }
    }

    // Add method to get column types summary
    template <typename T>
    static std::string getColumnTypes(const std::vector<std::vector<T>> &data,
                                      const std::vector<std::string> &headers)
    {
        std::ostringstream oss;
        oss << "dtypes:\n";
        for (size_t i = 0; i < headers.size(); ++i)
        {
            oss << headers[i] << ": " << typeid(T).name() << "\n";
        }
        return oss.str();
    }

    // Add method for memory usage estimation
    template <typename T>
    static std::string getMemoryUsage(const std::vector<std::vector<T>> &data)
    {
        size_t totalBytes = 0;
        for (const auto &row : data)
        {
            totalBytes += row.size() * sizeof(T);
        }
        return formatBytes(totalBytes);
    }

private:
    template <typename T>
    static void validateInput(const std::vector<std::vector<T>> &data, const std::vector<std::string> &headers)
    {
        if (headers.empty())
        {
            throw TableError("Headers cannot be empty");
        }

        if (!data.empty())
        {
            size_t expectedCols = headers.size();
            for (size_t i = 0; i < data.size(); ++i)
            {
                if (data[i].size() != expectedCols)
                {
                    throw TableError("Row " + std::to_string(i) +
                                     " has inconsistent number of columns");
                }
            }
        }
    }

    template <typename T>
    static std::string formatValue(const T &value, const ColumnFormat &format)
    {
        std::ostringstream oss;

        // Handle null values (if T is a pointer or optional type)
        if constexpr (std::is_pointer_v<T>)
        {
            if (value == nullptr)
                return "NULL";
        }

        if (format.isNumeric)
        {
            oss << std::fixed
                << std::setprecision(format.precision)
                << std::setfill(' ');

            // Handle special numeric cases
            if constexpr (std::is_floating_point_v<T>)
            {
                if (std::isnan(value))
                    return "NaN";
                if (std::isinf(value))
                    return value > 0 ? "Inf" : "-Inf";
            }
        }

        oss << value;
        std::string str = oss.str();

        // Truncate and align
        if (str.length() > format.width)
        {
            if (format.isNumeric)
            {
                return std::string(format.width, '#');
            }
            return str.substr(0, format.width - 3) + "...";
        }

        // Alignment
        size_t padding = format.width - str.length();
        if (format.alignment == "right")
        {
            return std::string(padding, ' ') + str;
        }
        else if (format.alignment == "center")
        {
            size_t leftPad = padding / 2;
            size_t rightPad = padding - leftPad;
            return std::string(leftPad, ' ') + str + std::string(rightPad, ' ');
        }
        return str + std::string(padding, ' '); // left alignment
    }

    // Add method to handle column resizing
    static void optimizeColumnWidths(std::vector<ColumnFormat> &formats, size_t terminalWidth)
    {
        size_t totalWidth = std::accumulate(formats.begin(), formats.end(), 0,
                                            [](size_t sum, const ColumnFormat &f)
                                            { return sum + f.width + 3; }); // +3 for padding and separator

        if (totalWidth > terminalWidth && formats.size() > 1)
        {
            size_t excess = totalWidth - terminalWidth;
            size_t reduction = excess / formats.size();

            for (auto &format : formats)
            {
                format.width = std::max(MIN_COL_WIDTH, format.width - reduction);
            }
        }
    }

    static std::string formatBytes(size_t bytes)
    {
        const char *units[] = {"B", "KB", "MB", "GB"};
        int i = 0;
        double size = bytes;
        while (size >= 1024 && i < 3)
        {
            size /= 1024;
            i++;
        }
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << size << " " << units[i];
        return oss.str();
    }

    template <typename T>
    static std::string formatImpl(
        const std::vector<std::vector<T>> &data,
        const std::vector<std::string> &headers,
        const TableStyle &style)
    {
        std::ostringstream oss;
        std::vector<ColumnFormat> formats = calculateFormats(data, headers);

        // Show header and first n rows
        printHeaderAndRows(oss, data, headers, formats, style, 0,
                           std::min(style.displayRows, data.size()));

        // Show ellipsis if data is truncated
        if (data.size() > 2 * style.displayRows)
        {
            oss << "...\n";
            // Show last n rows
            printHeaderAndRows(oss, data, headers, formats, style,
                               data.size() - style.displayRows, data.size());
        }

        // Show summary information
        if (style.showRowCount)
        {
            oss << "\n[" << data.size() << " rows x " << headers.size() << " columns]";
        }

        if (style.showColumnTypes)
        {
            oss << "\n"
                << getColumnTypes(data, headers);
        }

        oss << "\nMemory usage: " << getMemoryUsage(data) << "\n";

        return oss.str();
    }

    // Add method to print header and rows
    template <typename T>
    static void printHeaderAndRows(std::ostringstream &oss, const std::vector<std::vector<T>> &data,
                                   const std::vector<std::string> &headers, const std::vector<ColumnFormat> &formats,
                                   const TableStyle &style, size_t startRow, size_t endRow)
    {
        // Draw headers
        if (style.showIndex)
        {
            oss << std::setw(INDEX_WIDTH) << " ";
            oss << " ";
        }

        // Draw header row
        for (size_t i = 0; i < headers.size(); ++i)
        {
            oss << std::setw(formats[i].width) << headers[i];
            oss << " ";
        }
        oss << "\n";

        // Draw data rows
        for (size_t row = startRow; row < endRow; ++row)
        {
            if (style.showIndex)
            {
                oss << std::setw(INDEX_WIDTH) << row;
                oss << " ";
            }

            for (size_t col = 0; col < data[row].size(); ++col)
            {
                if (formats[col].isNumeric)
                {
                    oss << std::fixed
                        << std::setprecision(formats[col].precision)
                        << std::setw(formats[col].width)
                        << std::right;
                }
                oss << data[row][col];
                oss << " ";
            }
            oss << "\n";
        }
    }
};

// Test function that demonstrates various features of the TableVisualizer
void runTests()
{
    std::cout << "Running TableVisualizer Tests\n";
    std::cout << "=============================\n\n";

    // Test 1: Basic numeric data with default style
    {
        std::cout << "Test 1: Basic numeric data (Default style)\n";
        std::vector<std::vector<double>> data = {
            {1.234, 2.345, 3.456},
            {4.567, 5.678, 6.789},
            {7.890, 8.901, 9.012}};
        std::vector<std::string> headers = {"Column A", "Column B", "Column C"};
        std::cout << TableVisualizer::format(data, headers) << "\n\n";
    }

    // Test 2: Minimal style
    {
        std::cout << "Test 2: Minimal style\n";
        std::vector<std::vector<double>> data = {
            {1.234, 2.345, 3.456},
            {4.567, 5.678, 6.789}};
        std::vector<std::string> headers = {"First", "Second", "Third"};
        auto style = TableVisualizer::TableStyle();
        std::cout << TableVisualizer::format(data, headers, style) << "\n\n";
    }

    // Test 3: ASCII style with more rows
    {
        std::cout << "Test 3: ASCII style with more rows\n";
        std::vector<std::vector<double>> data;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0, 100);

        for (int i = 0; i < 25; i++)
        { // More than MAX_ROWS
            data.push_back({dis(gen), dis(gen), dis(gen)});
        }

        std::vector<std::string> headers = {"Random 1", "Random 2", "Random 3"};
        auto style = TableVisualizer::TableStyle();
        std::cout << TableVisualizer::format(data, headers, style) << "\n\n";
    }

    // Test 4: Long headers and values
    {
        std::cout << "Test 4: Long headers and values\n";
        std::vector<std::vector<std::string>> data = {
            {"This is a very long value that should be truncated", "Short", "Medium length"},
            {"Short", "Also very long value that needs truncation", "Normal"},
            {"Medium", "Normal", "Super long value that will definitely be truncated"}};
        std::vector<std::string> headers = {
            "Very Long Header That Should Be Truncated",
            "Another Long Header",
            "Third Long Header"};
        std::cout << TableVisualizer::format(data, headers) << "\n\n";
    }

    // Test 5: Integer data
    {
        std::cout << "Test 5: Integer data\n";
        std::vector<std::vector<int>> data = {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12}};
        std::vector<std::string> headers = {"A", "B", "C", "D"};
        std::cout << TableVisualizer::format(data, headers) << "\n\n";
    }
}

int main()
{
    runTests();
    return 0;
}