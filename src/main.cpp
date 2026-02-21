#include "crow/crow.h"
#include "crow/middlewares/cors.h"
#include "lru_cache.h"
#include <string>

int main() {
    Datastore db(3);
    crow::App<crow::CORSHandler> app;  // ← changed from SimpleApp

    // Configure CORS globally
    auto& cors = app.get_middleware<crow::CORSHandler>();
    cors.global()
        .origin("*")
        .methods("GET"_method, "POST"_method, "DELETE"_method, "OPTIONS"_method)
        .headers("Content-Type");

    CROW_ROUTE(app, "/")([](){
        return "Mini-Redis up and running";
    });

    CROW_ROUTE(app, "/set").methods("POST"_method)
    ([&db](const crow::request& req){
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "Invalid JSON");

        std::string key = body["key"].s();
        std::string value = body["value"].s();
        if (body.has("ttl")) db.set(key, value, body["ttl"].i());
        else db.set(key, value);

        return crow::response(200, "OK");
    });

    CROW_ROUTE(app, "/get").methods("GET"_method)
    ([&db](const crow::request& req){
        auto key = req.url_params.get("key");
        if (!key) return crow::response(400, "Missing key");

        std::string value = db.get(key);
        if (value == "NULL") return crow::response(404, "Key not found");

        return crow::response(200, value);
    });

    CROW_ROUTE(app, "/delete").methods("GET"_method)
    ([&db](const crow::request& req){
        auto key = req.url_params.get("key");
        if (!key) return crow::response(400, "Missing key");

        db.del(key);
        return crow::response(200, "Deleted");
    });

    CROW_ROUTE(app, "/stats").methods("GET"_method)
    ([&db](const crow::request&){
        crow::json::wvalue x;
        x["total_requests"] = db.total_requests;
        x["hits"] = db.hits;
        x["misses"] = db.misses;
        x["evictions"] = db.evictions;
        x["current_keys"] = db.cache_size();
        return crow::response(200, x.dump());
    });

    CROW_ROUTE(app, "/keys").methods("GET"_method)
    ([&db](const crow::request&){
        crow::json::wvalue x;
        auto keys = db.getKeys();
        for (size_t i = 0; i < keys.size(); i++) x[i] = keys[i];
        return crow::response(200, x.dump());
    });

    app.port(18080).multithreaded().run();
}