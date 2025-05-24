#include "FIFO.hpp"
#include "LFU.hpp"
#include "LRU.hpp"
#include "Clock.hpp"
#include "MFU.hpp"
#include "NRU.hpp"
#include "Second_Chance.hpp"
#include "includes.hpp"


int main(int argc, char *argv[]) {
  FIFO();
  LFU();
  LRU();
  Clock();
  MFU();
  NRU();
  Second_Chance();

    return 0;
}