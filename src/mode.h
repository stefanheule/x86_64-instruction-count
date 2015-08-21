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

#ifndef INSTR_COUNT_MODE_H
#define INSTR_COUNT_MODE_H

#include <iostream>

enum class Mode {
  MNEMONIC_ATT = 0,
  MNEMONIC_INTEL,
  OPERAND_TYPE,
  OPERAND_WIDTH
};

struct ModeReader {
  void operator()(std::istream& is, Mode& t);
};

struct ModeWriter {
  void operator()(std::ostream& os, const Mode t);
};

#endif
