#!/bin/bash
	/home/ttmv/opensource/PB2.6.1/bin/protoc -I=./ --cpp_out=./ ./presence.proto;
	/home/ttmv/opensource/PB2.6.1/bin/protoc -I=./ --cpp_out=./ ./user_management_service.proto;
	/home/ttmv/opensource/PB2.6.1/bin/protoc -I=./ --cpp_out=./ ./terminal_protocol.proto;
	/home/ttmv/opensource/PB2.6.1/bin/protoc -I=./ --cpp_out=./ ./MessageTransferServer.proto;
	/home/ttmv/opensource/PB2.6.1/bin/protoc -I=./ --cpp_out=./ ./GroupId.def.proto;
