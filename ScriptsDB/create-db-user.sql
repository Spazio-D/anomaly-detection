

-- This creates the database :dbname

drop database if exists :dbname ;
CREATE DATABASE :dbname ;


-- This creates the user :username

\c :dbname postgres

-- erase role if already created
REASSIGN OWNED BY :username TO postgres ;
REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM :username ;
REVOKE ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public FROM :username ;
REVOKE ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public FROM :username ;
DROP OWNED BY :username;
DROP USER IF EXISTS :username ;
CREATE USER :username WITH ENCRYPTED PASSWORD '47002' ;
-- user created



\c :dbname postgres

-- grant privileges to user

GRANT ALL PRIVILEGES ON DATABASE :dbname to :username ;

GRANT ALL ON SCHEMA public TO :username ;

