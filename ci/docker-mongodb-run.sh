#!/bin/bash

docker run -ti --rm --name elos-mongo \
	--net elos-net \
	-e MONGO_INITDB_ROOT_USERNAME=mongoadmin \
	-e MONGO_INITDB_ROOT_PASSWORD=secret \
	mongo

