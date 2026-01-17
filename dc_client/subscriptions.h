#pragma once

#include "subscription.h"

struct Subscriptions {
    std::vector<Subscription> subscriptions;

    Subscriptions(nlohmann::json json);

    nlohmann::json to_json();
};