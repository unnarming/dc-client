#include "subscriptions.h"

Subscriptions::Subscriptions(nlohmann::json json) {
    for (auto& subscription : json["subscriptions"]) {
        subscriptions.push_back(Subscription(subscription["guild_id"], subscription["channel_ids"]));
    }
}

nlohmann::json Subscriptions::to_json() {
    nlohmann::json json = nlohmann::json::object();
    for (auto& subscription : subscriptions) {
        json[subscription.guild_id] = subscription.to_subscription();
    }
    return json;
}