#include <AppCore/App.h>
#include <AppCore/Window.h>
#include <Ultralight/Ultralight.h>
#include <AppCore/Overlay.h>
#include <iostream>
#include "socket.h"
#include <fstream>
#include "subscriptions.h"
#include <nlohmann/json.hpp>

using namespace ultralight;

int main() {
    auto dir = get_exe_dir().append("config.json");
    std::ifstream config_file(dir);

    nlohmann::json cfg = nlohmann::json::parse(config_file);

    std::string token = cfg.value("token", "");
    Subscriptions subscriptions = Subscriptions(cfg["subscriptions"]);
	std::cout << "test" << std::endl;

	MessageWebsocketAuth auth(token, subscriptions);

	MessageSocket socket(auth);
	socket.connect();
    
    Settings settings;
    settings.file_system_path = "./";

    Config config;
    config.resource_path_prefix = "resources/";

    auto app = App::Create(settings, config);

    auto monitor = app->main_monitor();

    auto window = Window::Create(app->main_monitor(), 800, 600, false, kWindowFlags_Titled | kWindowFlags_Resizable);

    window->SetTitle("Client");

    auto overlay = Overlay::Create(window, window->width(), window->height(), 0, 0);

    overlay->view()->LoadHTML(R"(
        <html>
            <body>
                <h1>Hello, World!</h1>
            </body>
        </html>
    )");

    app->Run();

    return 0;
}
