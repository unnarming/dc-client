#include "subscription.h"

Subscription::Subscription(std::string guild_id, std::vector<std::string> channel_ids) : guild_id(guild_id), channel_ids(channel_ids) {}

nlohmann::json Subscription::to_subscription() {
    nlohmann::json json = {
        {"typing", false},
        {"activities", false},
        {"threads", false},
        {"channels", nlohmann::json::object()}
    };
    for (std::string channel_id : channel_ids) {
        json["channels"][channel_id] = nlohmann::json::array({{0, 99}});
    }
    return json;
}