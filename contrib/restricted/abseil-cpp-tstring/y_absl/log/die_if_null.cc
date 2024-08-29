// Copyright 2022 The Abseil Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "y_absl/log/die_if_null.h"

#include "y_absl/base/config.h"
#include "y_absl/log/log.h"
#include "y_absl/strings/str_cat.h"

namespace y_absl {
Y_ABSL_NAMESPACE_BEGIN
namespace log_internal {

void DieBecauseNull(const char* file, int line, const char* exprtext) {
  LOG(FATAL).AtLocation(file, line)
      << y_absl::StrCat("Check failed: '", exprtext, "' Must be non-null");
}

}  // namespace log_internal
Y_ABSL_NAMESPACE_END
}  // namespace y_absl
