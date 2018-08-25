--
-- Create the 'trep' user on the MySQL database
--
drop user if exists 'trep'@'localhost';
create user 'trep'@'localhost' identified by 'trep';
grant SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, FILE, INDEX, 
	ALTER, CREATE TEMPORARY TABLES, CREATE VIEW, EVENT, TRIGGER, SHOW VIEW, 
	CREATE ROUTINE, ALTER ROUTINE, EXECUTE ON *.* 
	to 'trep'@'localhost';

drop user if exists 'trep'@'%';
create user 'trep'@'%' identified by 'trep';
grant SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, FILE, INDEX, 
	ALTER, CREATE TEMPORARY TABLES, CREATE VIEW, EVENT, TRIGGER, SHOW VIEW, 
	CREATE ROUTINE, ALTER ROUTINE, EXECUTE ON *.* 
	to 'trep'@'%';

flush privileges;

--
-- Create a 'trep_trep' database on the MySQL database
--
drop database if exists trep_trep;
create database if not exists trep_trep
	   default character set utf8
	   collate utf8_unicode_ci;
