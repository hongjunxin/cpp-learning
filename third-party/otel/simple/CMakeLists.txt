cmake_minimum_required(VERSION 3.22)
project(otel_simple_demo)

# Because we add -DWITH_STL=ON when build otel-cpp libs
# so here must add HAVE_CPP_STDLIB to g++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations -g -DHAVE_CPP_STDLIB")

add_executable(main_exporter_stdout.out main_exporter_stdout.cc foo.cc)
# add_executable(main_exporter_otlp.out main_exporter_otlp.cc foo.cc)
add_executable(main_exporter_jaeger.out main_exporter_jaeger.cc foo.cc)

target_link_libraries(main_exporter_stdout.out
  opentelemetry_exporter_ostream_span
  opentelemetry_trace
  opentelemetry_resources
  opentelemetry_common)

# target_link_libraries(main_exporter_otlp
#   opentelemetry_exporter_ostream_span
#   opentelemetry_trace
#   opentelemetry_resources
#   opentelemetry_common)

target_link_libraries(main_exporter_jaeger.out
  opentelemetry_exporter_jaeger_trace
  thrift
  opentelemetry_trace
  opentelemetry_resources
  opentelemetry_common
  opentelemetry_http_client_curl
  curl)
