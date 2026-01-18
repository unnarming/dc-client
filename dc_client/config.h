#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "subscriptions.h"

struct Subscription {
	Subscription(std::string guild_id, std::vector<std::string> channel_ids);
	Subscription() = default;

	std::string guild_id;
	std::vector<std::string> channel_ids;

	nlohmann::json to_subscription();
};

struct Subscriptions {
	std::vector<Subscription> subscriptions;

	Subscriptions(nlohmann::json json);
	Subscriptions() = default;

	nlohmann::json to_json();
};

struct MessageSocketConfig {
	MessageSocketConfig(std::string useragent, std::string host, std::string port, std::string url);
	MessageSocketConfig() = default;

	std::string useragent;
	std::string host;
	std::string port;
	std::string url;
};

struct MessageWebsocketAuth {
	nlohmann::json verify;
	nlohmann::json session;
	nlohmann::json subscribe;
	nlohmann::json heartbeat;
	int heartbeat_interval = 0;

	void set_sequence(int sequence);

	MessageWebsocketAuth(std::string token, Subscriptions* subscriptions);
	MessageWebsocketAuth() = default;
};

namespace dc {
	struct Config {
		std::string token;
		Subscriptions subscriptions;
		MessageSocketConfig msg_socket;

		Config();
	};
}