#!/bin/sh

# This defines parameters
# parameters.sql 

# This creates db and user
# create-db-user.sql

# This creates db schema (tables)
# schema.sql

# Grant priviliges to user
# grant.sql


# This does all
cd ../ScriptsDB
sudo -u postgres psql postgres -f parameters.sql -f create-db-user.sql  -f schema.sql  -f grant.sql
cd ../src