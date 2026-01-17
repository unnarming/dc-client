#include "misc.h"
#include <random>

std::string generate_uuid4() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<> dis(0, 15);
	static std::uniform_int_distribution<> dis2(8, 11);
	std::string uuid = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";
	for (auto& ch : uuid) {
		if (ch == 'x') {
			ch = "0123456789abcdef"[dis(gen)];
		} else if (ch == 'y') {
			ch = "89ab"[dis2(gen) - 8];
		}
	}
	return uuid;
}