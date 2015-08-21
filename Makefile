
# Copyright 2015 Stefan Heule

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#     http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

##### CONSTANT DEFINITIONS

GCC=g++ -Werror -Wextra -Wall -Wfatal-errors -pedantic -Wno-unused-parameter -Wno-reorder -std=c++11 -fPIC

LIB=src/ext/x64asm/lib/libx64asm.a

BIN=instr-count

INC_FOLDERS=\
	./ \
	src/ext/cpputil/ \
	src/ext/x64asm

INC=$(addprefix -I./, $(INC_FOLDERS))

# number of threads used for compiling
ifndef NTHREADS
	NTHREADS=8
endif

ifndef OPT
	OPT=-O3 -DNDEBUG
endif

##### TARGETS

all: release

release:
	$(MAKE) -C . -j$(NTHREADS) $(BIN) OPT="-O3 -DNDEBUG"

debug:
	$(MAKE) -C . -j$(NTHREADS) $(BIN) OPT="-g"

.PHONY: cpputil
cpputil:
	./scripts/submodule-init.sh src/ext/cpputil

.PHONY: x64asm
x64asm:
	./scripts/submodule-init.sh src/ext/x64asm
	$(MAKE) -C src/ext/x64asm

src/mode.o: src/mode.cc
	$(GCC) $(OPT) $(INC) -c $< -o $@

instr-count: src/instr-count.cc src/mode.o cpputil x64asm
	$(GCC) $(OPT) $(INC) src/instr-count.cc -o instr-count $(LIB) src/mode.o

clean:
	rm -f *.o
	rm -f instr-count
