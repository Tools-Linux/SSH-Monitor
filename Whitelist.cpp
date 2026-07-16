#include "Includes/Whitelist.h"

whitelist::whitelist(const std::vector<std::string>& ips) : m_ips(ips) {}

bool whitelist::isAllowed(const std::string &ip) const {
    for (const auto& allowed : m_ips) {
        if (allowed == ip)
            return true;
    }

    return false;
}
