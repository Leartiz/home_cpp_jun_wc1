-- +goose Up
-- +goose StatementBegin
CREATE TABLE `logs` (
    `id` UUID DEFAULT generateUUIDv4(),
    `timestamp` DateTime64 DEFAULT now64(3),
    `level` Enum('TRACE', 'DEBUG', 'INFO', 'WARNING', 'ERROR', 'FATAL'),
    `message` Text,
) ENGINE = MergeTree()
ORDER BY `timestamp`;
-- +goose StatementEnd

-- +goose Down
-- +goose StatementBegin
DROP TABLE IF EXISTS `logs`;
-- +goose StatementEnd
