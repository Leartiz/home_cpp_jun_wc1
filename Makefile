build_svr_image_on_centos:
	podman build --progress=plain . -t wc1_server -f ./centos.server.Dockerfile

run_server: build_svr_image_on_centos 
	podman run -d -p 24445:24444 wc1_server

only_run_server:
	podman run -d -p 24445:24444 wc1_server

# migrations for `ClickHouse`!
# ------------------------------------------------------------------------

GOOSE_DRIVER=clickhouse
GOOSE_DBSTRING="clickhouse://default:12345678@localhost:9000/default"
CLICKHOUSE_MIGRATIONS=./server/app/adapters/infrastructure/storage/log/impl/clickhouse/migrations

print_mig_clickhouse_env:
	echo GOOSE_DRIVER has value: $(GOOSE_DRIVER) && \
	echo GOOSE_DBSTRING has value: $(GOOSE_DBSTRING) 

create_mig_clickhouse: print_mig_clickhouse_env
	cd $(CLICKHOUSE_MIGRATIONS) && \
	goose $(GOOSE_DRIVER) $(GOOSE_DBSTRING) create first sql

up_mig_clickhouse:
	cd $(CLICKHOUSE_MIGRATIONS) && \
	goose $(GOOSE_DRIVER) $(GOOSE_DBSTRING) up

down_mig_clickhouse:
	cd $(CLICKHOUSE_MIGRATIONS) && \
	goose $(GOOSE_DRIVER) $(GOOSE_DBSTRING) down

# ------------------------------------------------------------------------
	