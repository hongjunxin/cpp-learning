project(otel_http_demo)

# Because we add -DWITH_STL=ON when build otel-cpp libs
# so here must add HAVE_CPP_STDLIB to g++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_CPP_STDLIB")

add_executable(http_client client.cc)
add_executable(http_server server.cc)

target_link_libraries(http_client
  opentelemetry_exporter_ostream_span
  opentelemetry_trace
  opentelemetry_http_client_curl
  curl
  opentelemetry_resources
  opentelemetry_common)

target_link_libraries(http_server
  opentelemetry_exporter_ostream_span
  opentelemetry_trace
  opentelemetry_http_client_curl
  curl
  opentelemetry_resources
  opentelemetry_common)
