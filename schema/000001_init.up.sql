create table if not exists users (
    id bigint primary key,
    balance bigint default 0,
    last_get bigint default 0
);
