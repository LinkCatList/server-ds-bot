FROM postgres:latest

COPY schema/000001_init.up.sql /docker-entrypoint-initdb.d/10-schema.sql
