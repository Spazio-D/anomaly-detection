
\c :dbname postgres

-- user already exists
GRANT ALL PRIVILEGES ON DATABASE :dbname to :username ;


ALTER TABLE dataTable OWNER TO :username ;
ALTER TABLE averageTable OWNER TO :username ;
ALTER TABLE covarianceTable OWNER TO :username ;
ALTER TABLE anomalyAverageTable OWNER TO :username ;
ALTER TABLE anomalyCovarianceTable OWNER TO :username ;
ALTER TABLE MonitorMissingDataTable OWNER TO :username ;

-- grant all privileges on all tables in schema public to :username ;
-- grant all privileges on all sequences in schema public to :username ;

GRANT ALL ON SCHEMA public TO :username ;
GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO :username ;
