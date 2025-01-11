INSERT INTO logs (id, timestamp, level, message) VALUES
    (generateUUIDv4(), now64(3), 'INFO', 'Application started successfully'),
    (generateUUIDv4(), now64(3), 'DEBUG', 'User logged in with ID 123'),
    (generateUUIDv4(), now64(3), 'WARNING', 'Disk space is running low'),
    (generateUUIDv4(), now64(3), 'ERROR', 'Failed to load configuration file'),
    (generateUUIDv4(), now64(3), 'FATAL', 'Database connection lost'),
    (generateUUIDv4(), now64(3), 'TRACE', 'Entering user authentication method');