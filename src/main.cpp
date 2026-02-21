#include "crow/crow.h"
#include "lru_cache.h"  //  LRU implementation
#include <string>

int main() {
    Datastore db(3);  // Max 3 elements
    crow::SimpleApp app;

    // --------------------
    // SET endpoint
    // POST /set
    // JSON body: {"key":"a", "value":"10", "ttl":5}
    // ttl is optional
    // --------------------

    CROW_ROUTE(app, "/")([](){
        return "Mini-Redis up and running";
    });

    CROW_ROUTE(app, "/set").methods("POST"_method)
    ([&db](const crow::request& req){
        auto body = crow::json::load(req.body);
        if (!body)
            return crow::response(400, "Invalid JSON");

        std::string key = body["key"].s();
        std::string value = body["value"].s();

        if (body.has("ttl"))
            db.set(key, value, body["ttl"].i());
        else
            db.set(key, value);

        return crow::response(200, "OK");
    });

    // --------------------
    // GET endpoint
    // GET /get?key=...
    // --------------------
    CROW_ROUTE(app, "/get")
    ([&db](const crow::request& req){
        auto key = req.url_params.get("key");
        if (!key)
            return crow::response(400, "Missing key");

        std::string value = db.get(key);
        if (value == "NULL")
            return crow::response(404, "Key not found");
        return crow::response(200, value);
    });

    // --------------------
    // DELETE endpoint
    // DELETE /delete?key=...
    // --------------------
    CROW_ROUTE(app, "/delete")
    ([&db](const crow::request& req){
        auto key = req.url_params.get("key");
        if (!key)
            return crow::response(400, "Missing key");

        db.del(key);
        return crow::response(200, "Deleted");
    });

    // --------------------
    // STATS endpoint
    // GET /stats
    // --------------------
    CROW_ROUTE(app, "/stats")
    ([&db]{
        crow::json::wvalue x;
        x["total_requests"] = db.total_requests;
        x["hits"] = db.hits;
        x["misses"] = db.misses;
        x["evictions"] = db.evictions;
        x["current_keys"] = db.cache_size(); // add a function to return cache.size()
        return x;
    });

    // --------------------
    // KEYS endpoint
    // GET /keys
    // --------------------
    CROW_ROUTE(app, "/keys")
    ([&db]{
        crow::json::wvalue x;
        auto keys = db.getKeys();  // implement getKeys() to return std::vector<std::string>
        for (size_t i=0; i<keys.size(); i++)
            x[i] = keys[i];
        return x;
    });

    app.port(18080).multithreaded().run();
}