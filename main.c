#include "chunk.h"
#include "common.h"
#include "debug.h"

int main(int argc, const char *argv[]) {
  Chunk chunk;
  initChunk(&chunk);
  for (int i = 0; i <= 257; i++) {
    writeConstant(&chunk, i+1.1, i / 10);
  }
  disassembleChunk(&chunk, "test chunk");
  freeChunk(&chunk);
  return 0;
}
