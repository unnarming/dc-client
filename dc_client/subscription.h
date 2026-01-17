#pragma once
#include <vector>
#include <nlohmann/json.hpp>
#include <string>

struct Subscription {
	Subscription(std::string guild_id, std::vector<std::string> channel_ids);

	std::string guild_id;
	std::vector<std::string> channel_ids;

	nlohmann::json to_subscription();
};