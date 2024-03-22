
-- database already exists

\c :dbname 


CREATE TABLE IF NOT EXISTS dataTable(
        sensorID VARCHAR(10) NOT NULL,
        sampleTime INT NOT NULL,
        value DOUBLE PRECISION,
        readTime TIME DEFAULT CURRENT_TIME,
        PRIMARY KEY (sensorID, sampleTime)
);

CREATE TABLE IF NOT EXISTS averageTable(
        sensorID VARCHAR(10) NOT NULL,
        value DOUBLE PRECISION,
        firstSampleTime INT NOT NULL,
        lastSampleTime INT NOT NULL,
        PRIMARY KEY (sensorID, firstSampleTime)
);

CREATE TABLE IF NOT EXISTS covarianceTable(
        sensorID1 VARCHAR(10) NOT NULL,
        sensorID2 VARCHAR(10) NOT NULL,
        value DOUBLE PRECISION,
        firstSampleTime INT NOT NULL,
        lastSampleTime INT NOT NULL,
        PRIMARY KEY (sensorID1, sensorID2, firstSampleTime)
);

CREATE TABLE IF NOT EXISTS anomalyAverageTable(
        sensorID VARCHAR(10) NOT NULL,
        firstSampleTime INT NOT NULL,
        isAnomaly BOOLEAN,
        value DOUBLE PRECISION,
        detectionTime TIME DEFAULT CURRENT_TIME,
        deltaTime INTERVAL,
        exceededTime BOOLEAN,
        upperThreshold DOUBLE PRECISION,
        lowerThreshold DOUBLE PRECISION,
        FOREIGN KEY (sensorID, firstSampleTime) REFERENCES averageTable(sensorID, firstSampleTime)
);

CREATE TABLE IF NOT EXISTS anomalyCovarianceTable(
        sensorID1 VARCHAR(10) NOT NULL,
        sensorID2 VARCHAR(10) NOT NULL,
        firstSampleTime INT NOT NULL,
        isAnomaly BOOLEAN,
        value DOUBLE PRECISION,
        detectionTime TIME,
        deltaTime INTERVAL,
        exceededTime BOOLEAN,
        threshold DOUBLE PRECISION,
        FOREIGN KEY (sensorID1, sensorID2, firstSampleTime) REFERENCES covarianceTable(sensorID1, sensorID2, firstSampleTime)
);

CREATE TABLE IF NOT EXISTS MonitorMissingDataTable(
        sensorID VARCHAR(10) NOT NULL,
        firstSampleTime INT NOT NULL,
        lastSampleTime INT NOT NULL,
        nullStreak INT NOT NULL,
        isAnomaly BOOLEAN,
        PRIMARY KEY (sensorID, firstSampleTime)
);

CREATE TABLE IF NOT EXISTS MonitorAnomalyAverageTable(
        sensorID VARCHAR(10) NOT NULL,
        firstSampleTime INT NOT NULL,
        isRight BOOLEAN,
        FOREIGN KEY (sensorID, firstSampleTime) REFERENCES averageTable(sensorID, firstSampleTime)
);

CREATE TABLE IF NOT EXISTS MonitorAnomalyCovarianceTable(
        sensorID1 VARCHAR(10) NOT NULL,
        sensorID2 VARCHAR(10) NOT NULL,
        firstSampleTime INT NOT NULL,
        isRight BOOLEAN,
        FOREIGN KEY (sensorID1, sensorID2, firstSampleTime) REFERENCES covarianceTable(sensorID1, sensorID2, firstSampleTime)
);