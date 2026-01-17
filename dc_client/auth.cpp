#include "auth.h"
#include <ctime>

MessageWebsocketAuth::MessageWebsocketAuth(std::string token, Subscriptions subscriptions) : token(token), uuid(generate_uuid4()), subscriptions(subscriptions) {
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
			{"subscriptions", subscriptions.to_json()}
		}}
	};
}

nlohmann::json MessageWebsocketAuth::get_heartbeat() {
	nlohmann::json json = {
		{"op", 1},
		{"d", sequence}
	};
	return json;
}

void MessageWebsocketAuth::set_heartbeat_interval(int interval) {
	heartbeat_interval = interval / 1000;
}

void MessageWebsocketAuth::set_sequence(int seq) {
	sequence = seq;
}
