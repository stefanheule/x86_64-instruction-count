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

/** Count the number of x86_x64 instructions in different ways. */
int main(int argc, char** argv) {

  CommandLineConfig::strict_with_convenience(argc, argv);

  map<string, set<Opcode>> instructions;
  for(size_t i = 0; i < X64ASM_NUM_OPCODES; ++i) {
    auto opcode = (Opcode)i;
    //auto instr = Instruction(opcode);
    string key;

    if (mode == Mode::MNEMONIC_ATT) {
      key = opcode_write_att(opcode);
    } else if (mode == Mode::MNEMONIC_INTEL) {
      key = opcode_write_intel(opcode);
    } else if (mode == Mode::OPERAND_TYPE) {
      
    } else if (mode == Mode::OPERAND_WIDTH) {
      
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
