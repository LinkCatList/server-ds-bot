#include <ctime>
#include <dpp/dpp.h>

#include "user.h"
#include "drogon/orm/Result.h"
#include "guild.h"

std::string getBalance(std::shared_ptr<drogon::orm::DbClient> dbClient, dpp::guild_member& user) {
    auto row = dbClient->execSqlSync("select exists(select 1 from users where id=$1)",
        user.get_user()->id
    );

    if (!row[0][0].as<bool>()) {
        dbClient->execSqlSync("insert into users values($1)",
            user.get_user()->id
        );
    }
    
    row = dbClient->execSqlSync("select balance from users where id=$1",
        user.user_id
    );
    return row[0][0].as<std::string>();
}

void User::getProfile (const dpp::slashcommand_t& event, 
std::shared_ptr<drogon::orm::DbClient> dbClient, dpp::guild_member& user) {

    dpp::embed info = dpp::embed()
        .set_title("Информация о пользователе `" + user.get_user()->global_name + "`")
        .set_color(dpp::colors::brown_sugar)
        .set_thumbnail(user.get_user()->get_avatar_url())

        .add_field("Пользователь", "<@" + std::to_string(user.user_id) + ">")
        .add_field("На сервере с", "`" + (std::string)(ctime(&user.joined_at)) + "`", true)
        .add_field("Баланс", "`" + getBalance(dbClient, user)+ "🍪`", true);


    dpp::message msg;
    msg.add_embed(info);

    event.reply(msg);
}
