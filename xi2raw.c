
#include    <stdio.h>
#include    <unistd.h>

/* Include this header file defining functions in libsndfile. */
#include    <sndfile.h>

/* This will be the length of the buffer used to hold samples while
** we process them.
*/
#define BUFFER_LEN      1024

/* libsndfile can handle more than 6 channels but we'll restrict it to 6. */
#define    MAX_CHANNELS    6

#include "global.h"

/* Function prototype. */
static void process_data (double *data, int count, int channels) ;

int main( int argc, char *argv[])
{
  if( argc < 3 ){
    printf("Usage: xi2raw <infile.xi> <outfile.raw>\n");
    exit(1);
  } 
  /* This is a buffer of double precision floating point values
  ** which will hold our data while we process it.
  */
  static double data [BUFFER_LEN] ;

  /* A SNDFILE is very much like a FILE in the Standard C library. The
  ** sf_open and sf_open_write functions return an SNDFILE* pointer
  ** when they sucessfully open the specified file.
  */
  SNDFILE      *infile, *outfile ;

  /* A pointer to an SF_INFO stutct is passed to sf_open and sf_open_write
  ** which fill this struct with information about the file.
  */ 
  SF_INFO      sfinfo ;
  int          readcount ;

  /* Here's where we open the input file. We pass sf_open the file name and
  ** a pointer to an SF_INFO struct.
  ** On successful open, sf_open returns a SNDFILE* pointer which is used
  ** for all subsequent operations on that file. 
  ** If an error occurs during sf_open, the function returns a NULL pointer.
  */
  char *infilename = argv[1];
  if (! (infile = sf_open(infilename, SFM_READ, &sfinfo)))
  {   /* Open failed so print an error message. */
      printf ("Not able to open input file %s.\n", infilename) ;
      /* Print the error message fron libsndfile. */
      sf_perror (NULL) ;
      return  1 ;
      } ;

  if (sfinfo.channels > MAX_CHANNELS)
  {   printf ("Not able to process more than %d channels\n", MAX_CHANNELS) ;
      return  1 ;
      } ;

  /* Open the output file. */
  char *outfilename[255];
  sprintf(outfilename, "%s/%s.raw", argv[2], basename(argv[1]) );
  sfinfo.format     = SF_FORMAT_RAW | SF_FORMAT_PCM_16; 
  if (! (outfile = sf_open(outfilename, SFM_WRITE, &sfinfo)))
  {   printf ("Not able to open output file %s.\n", outfilename) ;
      sf_perror (NULL) ;
      return  1 ;
      } ;

  /* While there are samples in the input file, read them, process
  ** them and write them to the output file.
  */
  while ((readcount = sf_read_double (infile, data, BUFFER_LEN)))
  {   process_data (data, readcount, sfinfo.channels) ;
      sf_write_double (outfile, data, readcount) ;
      } ;

  SF_INSTRUMENT inst ;
  sf_command (infile, SFC_GET_INSTRUMENT, &inst, sizeof (inst)) ;

  if( inst.loop_count > 0 ){
    char loopfile[255];
    sprintf("%s.raw.txt", argv[2] );
    writeLoopFile( loopfile, inst.loops[0].start, inst.loops[0].end, sfinfo.samplerate, sfinfo.channels );
  }

  /* Close input and output files. */
  sf_close (infile) ;
  sf_close (outfile) ;

	return 0;
}

static void
process_data (double *data, int count, int channels)
{    double channel_gain [MAX_CHANNELS] = { 0.5, 0.8, 0.1, 0.4, 0.4, 0.9 } ;
    int k, chan ;

    /* Process the data here. 
    ** If the soundfile contains more then 1 channel you need to take care of 
    ** the data interleaving youself.
    ** Current we just apply a channel dependant gain. 
    */

    for (chan = 0 ; chan < channels ; chan ++)
        for (k = chan ; k < count ; k+= channels)
            data [k] *= channel_gain [chan] ;

    return ;
} /* process_data */

