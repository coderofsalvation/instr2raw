#include <cstdio>
#include <cstdlib>
#include "sf2.hpp"

int main()
{
  SF2 sf(44100);
  
  FILE * sample = fopen("./test.wav", "rb");
  if (!sample)
  {
    puts("error sample");
    exit(-1);
  }
  
  sf.add_new_sample(  sample, 
                      SampleType::SIGNED_16, 
                      "someSample", 
                      0x2c, 
                      100000*sizeof(int16_t), 
                      true, 
                      1, 
                      111, 
                      0x3c,
                      1, 
                      44100);
  sf.add_new_instrument("someinstr");
  
  sf.add_new_inst_bag();
  sf.add_new_inst_generator(SFGenerator::sampleModes, 1); //enable looping
  sf.add_new_inst_generator(SFGenerator::sampleID, 0); // link someinstr to sample 0
  sf.add_new_inst_generator(SFGenerator::keyRange, 0,80); // specify key range
  
  sf.add_new_inst_bag();
  sf.add_new_inst_generator(SFGenerator::sampleModes, 1); //enable looping
  sf.add_new_inst_generator(SFGenerator::sampleID, 0); // link someinstr to sample 0
  sf.add_new_inst_generator(SFGenerator::keyRange, 81, 127); //specify key range
  
  sf.add_new_preset("somepresset", 10, 0);
  sf.add_new_preset_bag();
  sf.add_new_preset_generator(SFGenerator::instrument, 0);
  
  FILE* sfout = fopen("./sftest.sf2", "wb");
  if (!sfout)
  {
    puts("error sfout");
    exit(-1);
  }
  
  sf.write(sfout);
}
