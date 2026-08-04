#include "Includes/Webhook.h"

#include <iostream>
#include <ctime>

#include <nlohmann/json.hpp>

#include "curl/curl.h"

Webhook::Webhook(const std::string& url) : m_url(url) {}

bool Webhook::sendAlert(const std::string& username, const std::string& ip)
{
    CURL* curl = curl_easy_init();

    if (!curl)
        return false;

    // Build JSON payload using nlohmann::json for correctness
    std::string timestamp;
    {
        char buffer[64];
        std::time_t now = std::time(nullptr);
        std::tm* utc = std::gmtime(&now);
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", utc);
        timestamp = buffer;
    }

    nlohmann::json payload;
    nlohmann::json embed;
    embed["title"] = "SSH Unauthorized Access";
    embed["description"] = "Une tentative de connexion SSH inconues a été détectée.";
    embed["color"] = 15158332;
    embed["fields"] = nlohmann::json::array();
    embed["fields"].push_back({{"name", "Utilisateur"}, {"value", username}, {"inline", true}});
    embed["fields"].push_back({{"name", "Adresse IP"}, {"value", ip}, {"inline", true}});
    embed["fields"].push_back({{"name", "Luser"}, {"value", username}, {"inline", true}});
    embed["footer"] = nlohmann::json::object();
    embed["footer"]["text"] = "SSH Security Monitor";
    embed["timestamp"] = timestamp;
    payload["embeds"] = nlohmann::json::array();
    payload["embeds"].push_back(embed);

    std::string json = payload.dump();

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, m_url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());

    CURLcode result = curl_easy_perform(curl);

    long httpCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    if (result != CURLE_OK)
    {
        std::cerr << "CURL Error: "
                  << curl_easy_strerror(result)
                  << std::endl;
    }
    else
    {
        std::cout << "Discord HTTP Code: "
                  << httpCode
                  << std::endl;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return result == CURLE_OK && httpCode == 204;
}