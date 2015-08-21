#include <iostream>
#include <string>

#include "src/ext/cpputil/include/command_line/command_line.h"
#include "src/ext/x64asm/src/opcode.h"

using namespace std;
using namespace x64asm;
using namespace cpputil;

auto& options = Heading::create("Main arguments:");
auto& mode = ValueArg<string>::create("mode")
  .usage("(abc|def)")
  .description("How should instructions be counted?")
  .default_val("test");

/** Count the number of x86_x64 instructions in different ways. */
int main(int argc, char** argv) {

  CommandLineConfig::strict_with_convenience(argc, argv);

  cout << "Hello" << endl;

  return 0;
}
