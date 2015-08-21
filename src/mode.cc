// Copyright 2013-2015 Stanford University
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

#include <array>
#include <string>
#include <utility>
#include <cassert>

#include "src/mode.h"

using namespace std;

namespace {
array<pair<string, Mode>, 4> pts {{
    {"mnemonic_att", Mode::MNEMONIC_ATT},
    {"mnemonic_intel", Mode::MNEMONIC_INTEL},
    {"operand_type", Mode::OPERAND_TYPE},
    {"operand_width", Mode::OPERAND_WIDTH}
  }
};

} // namespace

/** Forward lookup; returns true iff lookup succeeds. */
template <size_t N, typename T1, typename T2>
bool generic_read(const std::array<std::pair<T1, T2>, N>& elems, const T1& key, T2& val) {
  for (const auto& elem : elems) {
    if (key == elem.first) {
      val = elem.second;
      return true;
    }
  }
  return false;
}

/** Backward lookup; returns true iff lookup succeeds. */
template <size_t N, typename T1, typename T2>
void generic_write(const std::array<std::pair<T1, T2>, N>& elems, T1& key, const T2& val) {
  for (const auto& elem : elems) {
    if (val == elem.second) {
      key = elem.first;
      return;
    }
  }
  assert(false);
}

void ModeReader::operator()(std::istream& is, Mode& t) {
  string s;
  is >> s;
  if (!generic_read(pts, s, t)) {
    is.setstate(ios::failbit);
  }
}

void ModeWriter::operator()(std::ostream& os, const Mode t) {
  string s;
  generic_write(pts, s, t);
  os << s;
}
