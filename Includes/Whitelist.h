#pragma once

#include <string>
#include <vector>

class whitelist {
public:
    explicit whitelist(const std::vector<std::string>& ips);

    bool isAllowed(const std::string& ip) const;

private:
    std::vector<std::string> m_ips;
};
