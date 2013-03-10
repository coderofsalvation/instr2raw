
void writeLoopFile( char *file, 
                    int loopstart,
                    int loopend,
                    int samplerate,
                    int channels )
{
  char data[1024];
  sprintf(data, "loopstart=%i\nloopend=%i\nsamplerate=%i\nchannels=%i",
          loopstart, loopend, samplerate, channels );
  FILE *out = fopen( file, "w" );
  fwrite( data, 1, strlen(data), out );
  fclose(out);
}
                    
