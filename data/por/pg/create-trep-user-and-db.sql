--
-- File: https://github.com/trep/opentrep/tree/master/data/por/pg/create-trep-user-and-db.sql
--

--
-- Create a 'trep_trep' database on the PostgreSQL database
--
drop database if exists trep_trep;
create database trep_trep;

--
-- Create the 'trep' user on the PostgreSQL database
--
create user trep with encrypted password 'trep';
grant all privileges on database trep_trep to trep;

--
-- Grant access to the trep_trep database
-- For that, the database connection has first to be switched
--
\c trep_trep
grant all on schema public to trep;
