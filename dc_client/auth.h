#pragma once
#include <string>
#include "misc.h"
#include <nlohmann/json.hpp>
#include "subscriptions.h"

struct MessageWebsocketAuth {
	std::string url = "wss://gateway.discord.gg/?encoding=json&v=9&compress=zlib-stream";
	std::string token;
	std::string uuid;
	Subscriptions subscriptions;
	nlohmann::json verify;
	nlohmann::json session;
	nlohmann::json subscribe;
	int heartbeat_interval = 0;
	int sequence = 1;

    nlohmann::json get_heartbeat();

	void set_heartbeat_interval(int interval);
	void set_sequence(int sequence);

	MessageWebsocketAuth(std::string token, Subscriptions subscriptions);
};