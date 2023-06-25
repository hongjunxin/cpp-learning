## Install otel

install dependencies:
- for otlp exporter:
  - [grpc](https://github.com/grpc/grpc/tree/master)
  - [protobuf](https://github.com/protocolbuffers/protobuf). 
- for jaeger exporter:
  - [thrift](https://github.com/apache/thrift)

build protobuf
```sh
cmake -B build -Dprotobuf_BUILD_SHARED_LIBS=ON \
      -Dprotobuf_WITH_ZLIB=ON
```

install thrift cpp
```sh
$ bash mycmake_thrift.sh --target-system linux
$ cd build/linux && make -j4
$ sudo make install
```

install otel cpp
```sh
$ sudo apt-get install git cmake g++ libcurl4-openssl-dev
$ git clone https://github.com/open-telemetry/opentelemetry-cpp
$ cp mycmake_otel.sh opentelemetry-cpp
$ cd opentelemetry-cpp
$ git checkout -b v1.9.1 v1.9.1
$ git submodule update --init --recursive
$ bash mycmake_otel.sh --target-system linux
$ cd build/linux && make -j4
$ sudo make install
```

otel cmake parameter:
- DWITH_OTLP_HTTP：add otlp http exporter
- DWITH_OTLP_GRPC: add otlp grpc exporter
- DWITH_JAEGER: add jaeger exporter
- DWITH_STL: use STL instead of ABSEIL

todo:
- otlp exporter build failed

## Doc
SDK usage learning from [GettingStarted](https://github.com/open-telemetry/opentelemetry-cpp/blob/main/docs/public/sdk/GettingStarted.rst)