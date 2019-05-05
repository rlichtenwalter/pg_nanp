MODULES = pg_nanp
EXTENSION = pg_nanp
DATA = pg_nanp--1.0.sql
DOCS = README.pg_nanp

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
