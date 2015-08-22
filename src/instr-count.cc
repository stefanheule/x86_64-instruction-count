#include <iostream>
#include <map>
#include <set>
#include <string>

#include "src/ext/cpputil/include/command_line/command_line.h"
#include "src/ext/x64asm/src/opcode.h"
#include "src/ext/x64asm/src/instruction.h"

#include "src/mode.h"

using namespace std;
using namespace x64asm;
using namespace cpputil;

auto& options = Heading::create("Main arguments:");
auto& mode = ValueArg<Mode, ModeReader, ModeWriter>::create("mode")
  .usage("(mnemonic_att|mnemonic_intel|operand_type|operand_width)")
  .description("How should instructions be counted?")
  .default_val(Mode::OPERAND_WIDTH);

auto& show_instructions = FlagArg::create("show_instructions")
  .description("Should the instructions be shown (rather than just showing the count)?");

uint16_t type_size(Type t);

/** Count the number of x86_x64 instructions in different ways. */
int main(int argc, char** argv) {

  CommandLineConfig::strict_with_convenience(argc, argv);

  map<string, set<Opcode>> instructions;
  for(size_t i = 1; i < X64ASM_NUM_OPCODES; ++i) {
    auto opcode = (Opcode)i;
    auto instr = Instruction(opcode);
    stringstream ss;
    string key;

    if (mode == Mode::MNEMONIC_ATT) {
      key = opcode_write_att(opcode);
    } else if (mode == Mode::MNEMONIC_INTEL) {
      key = opcode_write_intel(opcode);
    } else if (mode == Mode::OPERAND_TYPE) {
      ss << opcode;
      key = ss.str();
      // don't count the _1 variants
      if (key.compare(key.length() - 2, 2, "_1") == 0 || key.compare(key.length() - 2, 2, "_2") == 0 || key.compare(key.length() - 2, 2, "_3") == 0) {
        key = key.substr(key.length() - 2);
      }
    } else if (mode == Mode::OPERAND_WIDTH) {
      ss << opcode_write_intel(opcode);
      for (size_t j = 0; j < instr.arity(); j++) {
        ss << "_" << type_size(instr.type(j));
      }
      key = ss.str();
    }

    if (instructions.find(key) == instructions.end()) {
      instructions.insert(pair<string, set<Opcode>>(key, set<Opcode>()));
    }
    (*instructions.find(key)).second.insert(opcode);
  }

  if (show_instructions) {
    vector<string> keys;
    for(auto& i: instructions) {
      keys.push_back(i.first);
    }
    sort(keys.begin(), keys.end());
    for (auto& key : keys) {
      cout << key << endl;
    }
  } else {
    cout << instructions.size() << endl;
  }

  return 0;
}

uint16_t type_size(Type t) {

  switch(t) {
  case Type::IMM_8:
  case Type::ZERO:
  case Type::ONE:
  case Type::THREE:
  case Type::M_8:
  case Type::MOFFS_8:
  case Type::RH:
  case Type::R_8:
  case Type::AL:
  case Type::CL:
    return 8;

  case Type::IMM_16:
  case Type::M_16:
  case Type::R_16:
  case Type::MOFFS_16:
  case Type::AX:
  case Type::DX:
    return 16;

  case Type::IMM_32:
  case Type::MOFFS_32:
  case Type::M_32:
  case Type::R_32:
  case Type::EAX:
    return 32;

  case Type::IMM_64:
  case Type::MOFFS_64:
  case Type::M_64:
  case Type::R_64:
  case Type::RAX:
    return 64;

  case Type::XMM:
  case Type::XMM_0:
  case Type::M_128:
    return 128;

  case Type::YMM:
  case Type::M_256:
    return 256;

  default:
    return 0;
  }
}