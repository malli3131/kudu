// Copyright 2015 Cloudera, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef KUDU_UTIL_FAULT_INJECTION_H
#define KUDU_UTIL_FAULT_INJECTION_H

#include "kudu/gutil/macros.h"

// With some probability, crash at the current point in the code
// by issuing LOG(FATAL).
//
// The probability is determined by the 'fraction_flag' argument.
//
// Typical usage:
//
//   DEFINE_double(fault_crash_before_foo, 0.0,
//                 "Fraction of the time when we will crash before doing foo");
//   TAG_FLAG(fault_crash_before_foo, unsafe);
//
// This macro should be fast enough to run even in hot code paths.
#define MAYBE_FAULT(fraction_flag) \
  kudu::fault_injection::MaybeFault(AS_STRING(fraction_flag), fraction_flag)

// Inject a uniformly random amount of latency between 0 and the configured
// number of milliseconds.
//
// As with above, if the flag is configured to be <= 0, then this will be evaluated
// inline and should be fast, even in hot code path.
#define MAYBE_INJECT_RANDOM_LATENCY(max_ms_flag) \
  kudu::fault_injection::MaybeInjectRandomLatency(max_ms_flag);

// Implementation details below.
// Use the MAYBE_FAULT macro instead.
namespace kudu {
namespace fault_injection {

// Out-of-line implementation.
void DoMaybeFault(const char* fault_str, double fraction);
void DoInjectRandomLatency(double max_latency);

inline void MaybeFault(const char* fault_str, double fraction) {
  if (PREDICT_TRUE(fraction <= 0)) return;
  DoMaybeFault(fault_str, fraction);
}

inline void MaybeInjectRandomLatency(double max_latency) {
  if (PREDICT_TRUE(max_latency <= 0)) return;
  DoInjectRandomLatency(max_latency);
}

} // namespace fault_injection
} // namespace kudu
#endif /* KUDU_UTIL_FAULT_INJECTION_H */
