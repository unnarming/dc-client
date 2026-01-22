#include <sciter-x.h>
#include "sciter-x-window.hpp"
#include <iostream>
#include "socket.h"
#include "config.h"
#include <fstream>
#include "subscriptions.h"
#include <nlohmann/json.hpp>
#include "sciter-x-threads.h"
#include <filesystem>
#include "misc.h"

std::wstring get_html() {
	std::filesystem::path path = get_exe_dir() / "index.html";
	return path.wstring();
}

struct MessageThread {
	std::thread socket_thread;
	std::shared_ptr<MessageSocket> socket;

    MessageThread(dc::Config config, std::function<void(const std::string&)> callback) {
		socket = std::make_shared<MessageSocket>(config, callback);
        socket_thread = std::thread([this]() {
			socket->connect();
        });
    }

    ~MessageThread() {
		socket->stop();
		socket_thread.join();
    }
};

class frame : public sciter::window {
public:
    frame() : window(SW_MAIN | SW_ENABLE_DEBUG) {}

    SOM_PASSPORT_BEGIN(frame)
        SOM_FUNCS(
            SOM_FUNC(nativeMessage)
        )
    SOM_PASSPORT_END
    
    sciter::string nativeMessage() { return WSTR("test"); }
    
    template<typename... Args>
    void call_func(const std::string& func_name, Args... args) {
        sciter::value result;
        sciter::sync::gui_thread_ctx::exec([this, func_name, args...]() {
            constexpr size_t argc = sizeof...(Args);
            sciter::value argv[argc > 0 ? argc : 1] = { sciter::value(args)... };
            sciter::value result;

            SciterCall(this->get_hwnd(), func_name.c_str(), argc, argv, &result);
        });
    }

    void post_message(const std::string& message) {
        call_func("recvMessage", message);
    }
};

int uimain(std::function<int()> run) {
    sciter::sync::gui_thread_ctx gui_ctx;
    sciter::om::hasset<frame> pwin = new frame();

    dc::Config cfg;

    frame* frame_ptr = pwin;

    MessageThread mt(cfg, [frame_ptr](const std::string& msg) {
        frame_ptr->post_message(msg);
    });


	std::wstring path = L"file:///" + get_html();
    pwin->load(path.c_str());
    pwin->expand();

    return run();
}