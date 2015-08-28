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
  .description("How should instructions be counted?");

auto& show_instructions = FlagArg::create("show_instructions")
  .description("Should the instructions be shown (rather than just showing the count)?");

uint16_t type_size(Type t);
string type_name(Type t);
map<string, set<Opcode>> compute_instructions(Mode mode);
int count_instructions(Mode mode) {
  return compute_instructions(mode).size();
}

/** Count the number of x86_x64 instructions in different ways. */
int main(int argc, char** argv) {

  CommandLineConfig::strict_with_convenience(argc, argv);

  if (mode.has_been_provided()) {
    map<string, set<Opcode>> instructions = compute_instructions(mode);
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
  } else {
    cout << "Mode          " << " | " << "Count" << endl;
    cout << "--------------" << " + " << "-----" << endl;
    cout << "mnemonic_att  " << " | " << count_instructions(Mode::MNEMONIC_ATT) << endl;
    cout << "mnemonic_intel" << " | " << count_instructions(Mode::MNEMONIC_INTEL) << endl;
    cout << "operand_type  " << " | " << count_instructions(Mode::OPERAND_TYPE) << endl;
    cout << "operand_width " << " | " << count_instructions(Mode::OPERAND_WIDTH) << endl;
  }

  return 0;
}

map<string, set<Opcode>> compute_instructions(Mode mode) {
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
      ss << opcode_write_intel(opcode);
      for (size_t j = 0; j < instr.arity(); j++) {
        ss << "_" << type_name(instr.type(j));
      }
      key = ss.str();
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
  return instructions;
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

string type_name(Type t) {

  switch(t) {
  case Type::NONE:
    return "none";
  case Type::HINT:
    return "hint";
  case Type::IMM_8:
    return "imm8";
  case Type::IMM_16:
    return "imm16";
  case Type::IMM_32:
    return "imm32";
  case Type::IMM_64:
    return "imm64";
  case Type::ZERO:
    return "zero";
  case Type::ONE:
    return "one";
  case Type::THREE:
    return "three";
  case Type::LABEL:
    return "label";
  case Type::M_8:
    return "m8";
  case Type::M_16:
    return "m16";
  case Type::M_32:
    return "m32";
  case Type::M_64:
    return "m64";
  case Type::M_128:
    return "m128";
  case Type::M_256:
    return "m256";
  case Type::M_16_INT:
    return "m16int";
  case Type::M_32_INT:
    return "m32int";
  case Type::M_64_INT:
    return "m64int";
  case Type::M_32_FP:
    return "m32fp";
  case Type::M_64_FP:
    return "m64fp";
  case Type::M_80_FP:
    return "m80fp";
  case Type::M_80_BCD:
    return "m80bcd";
  case Type::M_2_BYTE:
    return "m2byte";
  case Type::M_28_BYTE:
    return "m28byte";
  case Type::M_108_BYTE:
    return "m108byte";
  case Type::M_512_BYTE:
    return "m512byte";
  case Type::FAR_PTR_16_16:
    return "farptr1616";
  case Type::FAR_PTR_16_32:
    return "farptr1632";
  case Type::FAR_PTR_16_64:
    return "farptr1664";
  case Type::MM:
    return "mm";
  case Type::PREF_66:
    return "pref66";
  case Type::PREF_REX_W:
    return "prefrexw";
  case Type::FAR:
    return "far";
  case Type::MOFFS_8:
    return "moffs8";
  case Type::MOFFS_16:
    return "moffs16";
  case Type::MOFFS_32:
    return "moffs32";
  case Type::MOFFS_64:
    return "moffs64";
  case Type::R_8:
    return "r8";
  case Type::RH:
    return "rh";
  case Type::AL:
    return "al";
  case Type::CL:
    return "cl";
  case Type::R_16:
    return "r16";
  case Type::AX:
    return "ax";
  case Type::DX:
    return "dx";
  case Type::R_32:
    return "r32";
  case Type::EAX:
    return "eax";
  case Type::R_64:
    return "r64";
  case Type::RAX:
    return "rax";
  case Type::REL_8:
    return "rel8";
  case Type::REL_32:
    return "rel32";
  case Type::SREG:
    return "sreg";
  case Type::FS:
    return "fs";
  case Type::GS:
    return "gs";
  case Type::ST:
    return "st";
  case Type::ST_0:
    return "st0";
  case Type::XMM:
    return "xmm";
  case Type::XMM_0:
    return "xmm0";
  case Type::YMM:
    return "ymm";
  }
  assert(false);
  return "";
}