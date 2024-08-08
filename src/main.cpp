#include <dpp/dpp.h>
#include <dotenv.h>
#include <dpp/message.h>

#include "base.h"
 
int main() {

    dotenv::init();
    const std::string BOT_TOKEN = std::getenv("BOT_TOKEN");

    dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_guild_members);
 
    bot.on_log(dpp::utility::cout_logger());
 
    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "ping") {
            event.reply("Pong!");
        }
        
    });

    bot.on_guild_member_add([&bot](const dpp::guild_member_add_t& event) {

        const dpp::snowflake guild_id = event.adding_guild->id;
        const dpp::snowflake user_id = event.added.user_id;

        bot.message_create(dpp::message(
                WELCOME_CHANNEL_ID,
                "<@" + std::to_string(user_id) + ">, добро пожаловать на сервер " +
                event.adding_guild->name + "!"
            )
        );

        bot.guild_member_add_role(guild_id, user_id, UNVERIFIED_ROLE_ID);

    });

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("ping", "Ping pong!", bot.me.id));
        }
    });
 
    bot.start(dpp::st_wait);
}