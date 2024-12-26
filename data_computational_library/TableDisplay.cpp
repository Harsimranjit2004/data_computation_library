#include "TableDisplay.h"

namespace display
{

    std::string TableDisplay::formatBytes(size_t bytes)
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

    // ... other non-template method implementations ...

} // namespace display