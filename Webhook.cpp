#include "Includes/Webhook.h"

#include <iostream>
#include <ctime>

#include "curl/curl.h"

Webhook::Webhook(const std::string& url) : m_url(url) {}

bool Webhook::sendAlert(const std::string& username, const std::string& ip)
{
    CURL* curl = curl_easy_init();

    if (!curl)
        return false;

    std::string json =
        "{"
            "\"embeds\":["
                "{"
                    "\"title\":\"SSH Unauthorized Access\","
                    "\"description\":\"Une tentative de connexion SSH non autorisée a été détectée.\","
                    "\"color\":15158332,"
                    "\"fields\":["
                        "{"
                            "\"name\":\"Utilisateur\","
                            "\"value\":\"" + username + "\","
                            "\"inline\":true"
                        "},"
                        "{"
                            "\"name\":\"Adresse IP\","
                            "\"value\":\"" + ip + "\","
                            "\"inline\":true"
                        "}"
                    "],"
                    "\"footer\":{"
                        "\"text\":\"SSH Security Monitor\""
                    "},"
                    "\"timestamp\":\"" + []() {
                        char buffer[64];
                        std::time_t now = std::time(nullptr);
                        std::tm* utc = std::gmtime(&now);
                        std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", utc);
                        return std::string(buffer);
                    }() + "\""
                "}"
            "]"
        "}";

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