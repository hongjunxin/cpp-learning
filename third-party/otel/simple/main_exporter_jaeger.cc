// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/exporters/jaeger/jaeger_exporter_factory.h"
#include "opentelemetry/sdk/trace/simple_processor_factory.h"
#include "opentelemetry/sdk/trace/tracer_provider_factory.h"
#include "opentelemetry/sdk/trace/samplers/parent_factory.h"
#include "opentelemetry/sdk/trace/samplers/always_on_factory.h"
#include "opentelemetry/sdk/resource/resource.h"
#include "opentelemetry/trace/provider.h"
#include "opentelemetry/sdk/common/global_log_handler.h"

#include "foo.h"

namespace trace     = opentelemetry::trace;
namespace trace_sdk = opentelemetry::sdk::trace;
namespace jaeger    = opentelemetry::exporter::jaeger;
namespace resource  = opentelemetry::sdk::resource;
namespace common    = opentelemetry::sdk::common;

namespace
{

// url is like http://localhost:14268/api/traces
std::unique_ptr<trace_sdk::SpanExporter> 
createExporterToJaegerCollector(std::string url)
{
    jaeger::JaegerExporterOptions opts;
    opts.transport_format  = jaeger::TransportFormat::kThriftHttp;
    opts.endpoint = url;
    opts.headers = {{}}; // optional headers
    return jaeger::JaegerExporterFactory::Create(opts);
}

std::unique_ptr<trace_sdk::SpanExporter> 
createExporterToJaegerAgent(std::string host, uint16_t port)
{
    jaeger::JaegerExporterOptions opts;
    opts.endpoint = host;
    opts.server_port = port;
    return jaeger::JaegerExporterFactory::Create(opts);
}

std::string endpoint_type = "collector";

void InitTracer()
{
    // Capture log raised by otel sdk and print to stdout as dafault.
    common::internal_log::GlobalLogHandler::SetLogLevel(common::internal_log::LogLevel::Debug);

    std::unique_ptr<trace_sdk::SpanExporter> exporter;
    if (endpoint_type == "collector") {
        exporter = createExporterToJaegerCollector("http://jaeger.ysjtest.com:14268/api/traces");
    } else {
        exporter = createExporterToJaegerAgent("localhost", 6831);
    }

    // Immediately forwards ended spans to the exporter
    auto processor = trace_sdk::SimpleSpanProcessorFactory::Create(std::move(exporter));

    auto attributes = resource::ResourceAttributes{
        {"service.name", "otel_cpp_demo"},
        {"version", (uint32_t)1}
    };
    auto resource = resource::Resource::Create(attributes);

    // Either respects the parent span's sampling decision or delegates to
    // delegateSampler for root spans.
    // Note: lack of remote sampling schema so far?
    auto sampler = trace_sdk::ParentBasedSamplerFactory::Create(std::make_shared<trace_sdk::AlwaysOnSampler>());

    // auto sampler = trace_sdk::AlwaysOnSamplerFactory::Create();

    std::shared_ptr<opentelemetry::trace::TracerProvider> provider =
        trace_sdk::TracerProviderFactory::Create(std::move(processor), resource, std::move(sampler));

    // Set the global trace provider
    trace::Provider::SetTracerProvider(provider);
}

void CleanupTracer()
{
    std::shared_ptr<opentelemetry::trace::TracerProvider> none;
    trace::Provider::SetTracerProvider(none);
}
}  // namespace

int main(int argc, char *argv[])
{
    if (argc == 2) {
        endpoint_type = std::string(argv[1]);
    }
    if (endpoint_type != "collector" && endpoint_type != "agent") {
        endpoint_type = "collector";
    }

    // Removing this line will leave the default noop TracerProvider in place.
    InitTracer();

    foo_library();

    CleanupTracer();
}
