#include    <stdio.h>

/* Include this header file defining functions in libsndfile. */
#include    <sndfile.h>

/* This will be the length of the buffer used to hold samples while
** we process them.
*/
#define BUFFER_LEN      1024

/* libsndfile can handle more than 6 channels but we'll restrict it to 6. */
#define    MAX_CHANNELS    6

/* Function prototype. */
static void process_data (double *data, int count, int channels) ;

int main( int argc, char *argv[])
{
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

  SF_INSTRUMENT instr;
  if( sf_command (infile, SFC_GET_INSTRUMENT, &instr, sizeof (instr)) == SF_FALSE ) exit(1);

  if( instr.loops[0].start == 0 ){
    printf("no loopdata found\n");
    exit(0);
  }

  printf("[x] file=%s\n", infilename );
  printf("[x] sampleframes=%llu\n",sfinfo.frames);

  int i;
  for( i = 0; i < 16; i++ ){
    if( instr.loops[i].start == 0 ) continue;
    printf("[x] loop[%c].start=%i\n",i,instr.loops[i].start);
    printf("[x] loop[%c].end=%i\n",i,instr.loops[i].end);
    printf("[x] loop[%c].count=%i\n",i,instr.loops[i].count);
  }


  /* Close input and output files. */
  sf_close (infile) ;

	return 0;
}
