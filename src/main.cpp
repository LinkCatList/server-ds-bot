#include <dpp/dpp.h>
#include <dotenv.h>
#include <dpp/message.h>
#include <drogon/drogon.h>
#include <memory>


#include "appcommand.h"
#include "base.h"
#include "dispatcher.h"
#include "guild.h"
#include "user.h"
 
int main() {

    dotenv::init();

    const std::string BOT_TOKEN = std::getenv("BOT_TOKEN");
    const std::string POSTGRES_CONN = std::getenv("POSTGRES_CONN");

    auto dbClient = drogon::orm::DbClient::newPgClient(POSTGRES_CONN, 1);

    dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_guild_members);
 
    bot.on_log(dpp::utility::cout_logger());
    
    bot.on_slashcommand([dbClient, &bot](const dpp::slashcommand_t& event) {

        if (event.command.get_command_name() == "ping") {
            event.reply("Pong!");
        }

        if (event.command.get_command_name() == "profile") {

            std::string value = "";

            try {
                value = std::get<std::string>(event.get_parameter("user"))
                    .substr(2, 18);
            }
            catch (...) {
                value = event.command.usr.id.str();
            }


            auto guild = event.command.get_guild();

            bot.guild_get_member(guild.id, value, 
            [event = std::move(event), dbClient](const dpp::confirmation_callback_t& callback) {
                if (callback.is_error()) {
                    std::cerr << "Error fetching user: " << callback.get_error().message << std::endl;
                    event.reply("Ебать ты пидорас");
                } else {
                    auto fetchedUser = std::get<dpp::guild_member>(callback.value);
                    User::getProfile(event, dbClient, fetchedUser);
                }
            });
                
        }

        if (event.command.get_command_name() == "stata") {
            User::topUsers(event, dbClient);
        }

        if (event.command.get_command_name() == "bonus") {
            User::getTimely(event, dbClient);
        }
    });

    bot.on_guild_member_add([&bot, dbClient](const dpp::guild_member_add_t& event){

        const dpp::snowflake guild_id = event.adding_guild->id;
        auto user = event.added;

        bot.message_create(dpp::message(
                WELCOME_CHANNEL_ID,
                "<@" + std::to_string(user.user_id) + ">, добро пожаловать на сервер " +
                event.adding_guild->name + "! \n verificate plz :3"
            )
        );

        bot.guild_member_add_role(guild_id, user.user_id, UNVERIFIED_ROLE_ID);  
    });

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            dpp::slashcommand pingcommand("ping", "Ping pong!", bot.me.id);
            dpp::slashcommand infocommand("profile", "User's info", bot.me.id);
            infocommand.add_option(
                dpp::command_option(dpp::co_string, "user", "The username")
            );
            dpp::slashcommand topcommand("stata", "Top users", bot.me.id);
            dpp::slashcommand bonuscommand("bonus", "Get bonus", bot.me.id);
            bot.global_bulk_command_create({ infocommand, pingcommand, topcommand, bonuscommand});
        }
    });

    bot.start(dpp::st_wait);
}
