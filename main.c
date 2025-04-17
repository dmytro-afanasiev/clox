#include "chunk.h"
#include "common.h"
#include "debug.h"

// TODO: if opconstant has two bytes: one for op and second for index. Does it
// mean that there is only 256 constants available

int main(int argc, const char *argv[]) {
  Chunk chunk;
  initChunk(&chunk);
  int constant = addConstant(&chunk, 1.2);
  writeChunk(&chunk, OP_CONSTANT, 1);
  writeChunk(&chunk, constant, 1);
  writeChunk(&chunk, OP_RETURN, 2);
  writeChunk(&chunk, OP_RETURN, 2);
  writeChunk(&chunk, OP_RETURN, 2);
  writeChunk(&chunk, OP_RETURN, 4);
  writeChunk(&chunk, OP_CONSTANT, 4);
  writeChunk(&chunk, constant, 4);
  writeChunk(&chunk, OP_RETURN, 123);
  disassembleChunk(&chunk, "test chunk");
  freeChunk(&chunk);
  return 0;
}

