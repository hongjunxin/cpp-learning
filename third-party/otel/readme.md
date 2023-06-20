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

build thrift for cpp
```sh
$ ./bootstrap.sh
$ ./configure --enable-shared=no --enable-tests=no \
        --with-cpp=yes \
        --with-boost=no \
        --with-java=no \
        --with-kotlin=no \
        --with-erlang=no \
        --with-nodejs=no \
        --with-nodets=no \
        --with-lua=no \
        --with-php=no \
        --with-dart=no \
        --with-ruby=no \
        --with-go=no \
        --with-swift=no
$ cd thrift/lib/cpp
$ make
$ sudo make install
```

install otel cpp
```sh
$ sudo apt-get install git cmake g++ libcurl4-openssl-dev
$ git clone https://github.com/open-telemetry/opentelemetry-cpp
$ cd opentelemetry-cpp
$ git checkout -b v1.9.1 v1.9.1
$ git submodule update --init --recursive
$ cmake -B build -DBUILD_TESTING=OFF -DWITH_EXAMPLES_HTTP=ON -DWITH_OTLP_HTTP=ON -DWITH_OTLP_GRPC=ON -DWITH_STL=ON 
$ make -C build/
$ sudo make install
```

parameter:
- DWITH_OTLP_HTTP：add otlp http exporter
- DWITH_OTLP_GRPC: add otlp grpc exporter
- DWITH_JAEGER: add jaeger exporter
- DWITH_STL: use STL instead of ABSEIL

todo:
- otlp exporter build failed

## Doc
SDK usage learning from [GettingStarted](https://github.com/open-telemetry/opentelemetry-cpp/blob/main/docs/public/sdk/GettingStarted.rst)