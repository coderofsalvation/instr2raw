#include <cstdio>
#include <cstdlib>
#include "sf2.hpp"
#include <libgen.h>
/* Include this header file defining functions in libsndfile. */
#include    <sndfile.h>
/* This will be the length of the buffer used to hold samples while
** we process them.
*/
#define BUFFER_LEN      1024

/* libsndfile can handle more than 6 channels but we'll restrict it to 6. */
#define    MAX_CHANNELS    6

int main(int argc, char *argv[])
{
  SNDFILE      *infile;
  /* A pointer to an SF_INFO stutct is passed to sf_open and sf_open_write
  ** which fill this struct with information about the file.
  */ 
  SF_INFO      sfinfo ;
  int          readcount ;
  
  if( argc < 2 ){
    printf("Usage: sf2write <file.sf2> <file.wav> <index> <samplerate> <lokey> <hikey>  [loopstart] [loopstop]\n\n");
    printf("Note: index starts with 0, define -1 for loopstart/loopstop to disable looping\n");
    exit(1);
  }
  
  char *sf2filename = argv[1];
  char *infilename = argv[2];
  unsigned int index = atoi(argv[3]);
  unsigned int samplerate = atoi(argv[4]);
  unsigned int lokey = atoi(argv[5]);
  unsigned int hikey = atoi(argv[6]);
  signed int loopstart = -1;
  signed int loopstop  = -1;
  bool loop = (loopstart > 0 && loopstop > 0 );
  if( argc > 7 ){
    loopstart = atoi(argv[7]);
    loopstop  = atoi(argv[8]);
  }
  if (! (infile = sf_open(infilename, SFM_READ, &sfinfo))) { 
    /* Open failed so print an error message. */
    printf ("Not able to open input file %s.\n", infilename) ;
    /* Print the error message fron libsndfile. */
    sf_perror (NULL) ;
    return  1 ;
  } ;

  if (sfinfo.channels > MAX_CHANNELS){   
    printf ("Not able to process more than %d channels\n", MAX_CHANNELS) ;
    return  1 ;
  } ;
  sf_close (infile) ;

  SF2 sf(44100);
  
  FILE * sample = fopen( infilename, "rb");
  if (!sample)
  {
    puts("error sample");
    exit(-1);
  }
  
  sf.add_new_sample(  sample, 
                      SampleType::SIGNED_16, 
                      basename(infilename), 
                      0x2c, 
                      sfinfo.frames, // *sizeof(int16_t), 
                      loop, 
                      loopstart, 
                      loopstop, 
                      0x3c,
                      1, 
                      samplerate);
  sf.add_new_instrument(basename(infilename));
  
  sf.add_new_inst_bag();
  sf.add_new_inst_generator(SFGenerator::keyRange, lokey,hikey);     // specify key range
  sf.add_new_inst_generator(SFGenerator::sampleModes, loop?1:0);     // enable looping
  sf.add_new_inst_generator(SFGenerator::sampleID, index);        // link someinstr to sample 0
 
  /* 
  sf.add_new_preset("somepresset", 10, 0);
  sf.add_new_preset_bag();
  sf.add_new_preset_generator(SFGenerator::instrument, 0);
  */
  
  FILE* sfout = fopen( sf2filename, "wb");
  if (!sfout)
  {
    puts("error sfout");
    exit(-1);
  }
  
  sf.write(sfout);
}
