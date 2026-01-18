#include "config.h"
#include "misc.h"
#include <fstream>

Subscription::Subscription(std::string guild_id, std::vector<std::string> channel_ids) : guild_id(guild_id), channel_ids(channel_ids) {}

nlohmann::json Subscription::to_subscription() {
	nlohmann::json json = {
		{"typing", false},
		{"activities", false},
		{"threads", false},
		{"channels", nlohmann::json::object()}
	};
	for (std::string channel_id : channel_ids) {
		json["channels"][channel_id] = nlohmann::json::array({ {0, 99} });
	}
	return json;
}

Subscriptions::Subscriptions(nlohmann::json json) {
	for (auto& subscription : json) {
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

MessageSocketConfig::MessageSocketConfig(std::string useragent, std::string host, std::string port, std::string url) : useragent(useragent), host(host), port(port), url(url) {}

MessageWebsocketAuth::MessageWebsocketAuth(std::string token, Subscriptions* subscriptions) {
	std::string uuid = generate_uuid4();

	verify = {
		{"op", 2},
		{"d", {
			{"token", token},
			{"capabilities", 1734653},
			{"properties", {
					{"os", "Windows"},
					{"browser", "Firefox"},
					{"device", ""},
					{"system_locale", "en-US"},
					{"has_client_mods", false},
					{"browser_user_agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:146.0) Gecko/20100101 Firefox/146.0"},
					{"browser_version", "146.0"},
					{"os_version", "10"},
					{"referrer", ""},
					{"referring_domain", ""},
					{"referrer_current", ""},
					{"referring_domain_current", ""},
					{"release_channel", "stable"},
					{"client_build_number", 484344},
					{"client_event_source", nullptr},
					{"client_launch_id", uuid},
					{"is_fast_connect", true}
				}},
				{"client_state", {
					{"guild_versions", nlohmann::json::object()}
				}}
			}
		}
	};

	session = {
		{"op", 41},
		{"d", {
			{"initialization_timestamp", static_cast<int64_t>(time(nullptr)) * 1000},
			{"session_id", generate_uuid4()},
			{"client_launch_id", uuid}
		}}
	};

	subscribe = {
		{"op", 37},
		{"d", {
			{"subscriptions", subscriptions->to_json()}
		}}
	};

	heartbeat = {
		{"op", 1},
		{"d", 1}
	};
}

void MessageWebsocketAuth::set_sequence(int sequence) {
	this->heartbeat["d"] = sequence;
}

dc::Config::Config() {
	auto dir = get_exe_dir().append("config.json");
	std::ifstream config_file(dir);

	nlohmann::json cfg = nlohmann::json::parse(config_file);

	token = cfg.value("token", "");

	Subscriptions subscriptions = Subscriptions(cfg["subscriptions"]);
	msg_socket = MessageSocketConfig(cfg.value("useragent", ""), "gateway.discord.gg", "443", "/?encoding=json&v=9&compress=zlib-stream");
}
