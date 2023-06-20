// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/sdk/version/version.h"
#include "opentelemetry/trace/provider.h"

namespace trace = opentelemetry::trace;
namespace nostd = opentelemetry::nostd;

namespace
{
nostd::shared_ptr<trace::Tracer> get_tracer()
{
  auto provider = trace::Provider::GetTracerProvider();
  return provider->GetTracer("foo_library", OPENTELEMETRY_SDK_VERSION);
}

void f1()
{
  auto span = get_tracer()->StartSpan("f1");
  auto scoped_span = trace::Scope(span);
  span->End();
}

void f2()
{
  auto span = get_tracer()->StartSpan("f2");
  auto scoped_span = trace::Scope(span);

  f1();
  f1();

  span->End();
}
}  // namespace

void foo_library()
{
  auto span = get_tracer()->StartSpan("foo_library");
  auto scoped_span = trace::Scope(span);
  
  f2();

  span->End();
}
