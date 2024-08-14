#include <cstdint>
#include <ctime>
#include <dpp/dpp.h>

#include "user.h"
#include "base.h"
#include "colors.h"
#include "message.h"

void update(std::shared_ptr<drogon::orm::DbClient> dbClient, std::string user_id) {
    auto row = dbClient->execSqlSync("select exists(select 1 from users where id=$1)",
        user_id
    );

    if (!row[0][0].as<bool>()) {
        dbClient->execSqlSync("insert into users values($1)",
            user_id
        );
    }
}

std::string getBalance(std::shared_ptr<drogon::orm::DbClient> dbClient, dpp::guild_member& user) {
    update(dbClient, std::to_string(user.user_id));
    
    auto row = dbClient->execSqlSync("select balance from users where id=$1",
        user.user_id
    );
    return row[0][0].as<std::string>();
}

void User::getProfile (const dpp::slashcommand_t& event, 
std::shared_ptr<drogon::orm::DbClient> dbClient, dpp::guild_member& user) {

    update(dbClient, std::to_string(user.user_id));
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

void User::topUsers (const dpp::slashcommand_t& event, std::shared_ptr<drogon::orm::DbClient> dbClient) {

    update(dbClient, std::to_string(event.command.usr.id));

    auto rows = dbClient->execSqlSync("select id, balance from users order by balance desc limit 10");

    dpp::embed stata = dpp::embed()
        .set_title("Топ " + std::to_string(std::min((int)rows.size(), 10)) + " пользователей `🍪`")
        .set_color(dpp::colors::brown_sand);


    for (size_t i = 0; i < rows.size(); ++i) {
        stata.add_field("", "<@"+ rows[i][0].as<std::string>() + "> `" + rows[i][1].as<std::string>() + "🍪`");
    }

    dpp::message msg;
    msg.add_embed(stata);

    event.reply(msg);
}

void User::getTimely(const dpp::slashcommand_t &event, std::shared_ptr<drogon::orm::DbClient> dbClient) {

    auto user = event.command.usr;
    update(dbClient, std::to_string(user.id));
    
    auto row = dbClient->execSqlSync("select last_get, balance from users where id=$1",
        user.id
    );

    dpp::embed timely = dpp::embed()
        .set_color(dpp::colors::baby_blue);


    int64_t last_get = row[0][0].as<int64_t>();
    int bonus = 20 + rand() % 30;

    if (time(NULL) - last_get >= H24_SECONDS) {
        dbClient->execSqlSync("update users set balance=$1, last_get=$2 where id=$3", 
            row[0][1].as<int64_t>() + bonus, time(NULL), user.id
        );
        timely.set_description("Вы получили свой ежедневный бонус " + std::to_string(bonus) +" `🍪`");
    }
    else {
        timely.set_description("Бонус можно получать один раз в 24 часа");
    }

    dpp::message msg;
    msg.add_embed(timely);
    event.reply(msg);
}