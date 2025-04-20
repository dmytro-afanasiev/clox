#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk *chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  chunk->linesCount = 0;
  chunk->linesCapacity = 0;
  initValueArray(&chunk->constants);
}
void freeChunk(Chunk *chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->lines, chunk->linesCapacity);
  freeValueArray(&chunk->constants);
  initChunk(chunk);
}

void writeChunk(Chunk *chunk, uint8_t byte, int line) {
  if (chunk->capacity < chunk->count + 1) {
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code =
        GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
  }
  // Updating lines
  if (chunk->linesCount != 0 && chunk->lines[chunk->linesCount - 1] == line) {
    chunk->lines[chunk->linesCount - 2] = chunk->count;
  } else {
    if (chunk->linesCapacity < chunk->linesCount + 2) {
      int oldLinesCapacity = chunk->linesCapacity;
      chunk->linesCapacity = GROW_CAPACITY(oldLinesCapacity);
      chunk->lines = GROW_ARRAY(int, chunk->lines, oldLinesCapacity,
                                chunk->linesCapacity);
    }
    chunk->lines[chunk->linesCount] = chunk->count;
    chunk->lines[chunk->linesCount + 1] = line;
    chunk->linesCount += 2;
  }

  chunk->code[chunk->count] = byte;
  chunk->count++;
}
int getLine(Chunk *chunk, int offset) {
  for (int i = 0; i < chunk->linesCount; i += 2) {
    if (chunk->lines[i] >= offset) {
      return chunk->lines[i + 1];
    }
  }
  return -1; // never happens?
}
int addConstant(Chunk *chunk, Value value) {
  writeValueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
}
void writeConstant(Chunk *chunk, Value value, int line){
  int index = addConstant(chunk, value);
  if (index >= 256) {
    writeChunk(chunk, OP_CONSTANT_LONG, line);

    writeChunk(chunk, index>>16, line);
    writeChunk(chunk, index>>8, line);
    writeChunk(chunk, index, line);
  } else {
    writeChunk(chunk, OP_CONSTANT, line);
    writeChunk(chunk, index, line);
  }
}
