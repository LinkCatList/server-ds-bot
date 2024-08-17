#define WELCOME_CHANNEL_ID 1271060677129404478
#define UNVERIFIED_ROLE_ID 1271114229541245010 
#define H24_SECONDS 86400

#include <drogon/drogon.h>
#include <dpp/dpp.h>

inline void update(std::shared_ptr<drogon::orm::DbClient> dbClient, std::string user_id) {
    auto row = dbClient->execSqlSync("select exists(select 1 from users where id=$1)",
        user_id
    );

    if (!row[0][0].as<bool>()) {
        dbClient->execSqlSync("insert into users values($1)",
            user_id
        );
    }
}

inline std::string getBalance(std::shared_ptr<drogon::orm::DbClient> dbClient, std::string user_id) {
    update(dbClient, user_id);
        
    auto row = dbClient->execSqlSync("select balance from users where id=$1",
        user_id
    );
    return row[0][0].as<std::string>();
}