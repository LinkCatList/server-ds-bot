#pragma once

#include <dpp/dpp.h>
#include <drogon/drogon.h>
#include <memory>

class User {
public:     
    User(std::string& id, int64_t balance, time_t joined_at)
    : id(id), balance(balance), joined_at(joined_at)
    {}

    static void getProfile (const dpp::slashcommand_t&, std::shared_ptr<drogon::orm::DbClient>, dpp::guild_member&);
    static void topUsers (const dpp::slashcommand_t&, std::shared_ptr<drogon::orm::DbClient>);
    static void getTimely (const dpp::slashcommand_t&, std::shared_ptr<drogon::orm::DbClient>);

private:
    std::string id;
    int64_t balance;
    time_t joined_at;
};
