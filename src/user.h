#pragma once

#include <cstdint>
#include <dpp/dispatcher.h>
#include <dpp/dpp.h>
#include <dpp/message.h>
#include <drogon/drogon.h>
#include <memory>

#include "base.h"

class User {
public:     
    User(std::string id)
    : id(id)
    {}

    static void getProfile (const dpp::slashcommand_t&, std::shared_ptr<drogon::orm::DbClient>, dpp::guild_member&);
    static void topUsers (const dpp::slashcommand_t&, std::shared_ptr<drogon::orm::DbClient>);
    static void getTimely (const dpp::slashcommand_t&, std::shared_ptr<drogon::orm::DbClient>);

    inline void transferCookie (const dpp::slashcommand_t& event, 
    std::shared_ptr<drogon::orm::DbClient> dbClient, dpp::guild_member& user, int64_t amount) const {
        int64_t balance_member = std::stol(getBalance(dbClient, id));
        int64_t balance_user = std::stol(getBalance(dbClient, std::to_string(user.user_id)));

        if (balance_member < amount) {
            dpp::embed result = dpp::embed()
                .add_field("", "Недостаточно средств для перевода");

            dpp::message msg;
            msg.add_embed(result);
            event.reply(result);
            return;
        }

        dbClient->execSqlSync("update users set balance=$1 where id=$2",
            balance_user + amount, user.user_id
        );

        dbClient->execSqlSync("update users set balance=$1 where id=$2",
            balance_member - amount, id
        );

        dpp::embed result = dpp::embed()
            .add_field("", "Перевод успешно выполнен");

        dpp::message msg;
        msg.add_embed(result);
        event.reply(result);
    }

private:
    std::string id;
};
