#include <iostream>
#include <stdlib.h>
#include <string>
#include "libsf2.h"
#include  "global.h"
#include <iostream>
using namespace std;

char *outdir;

typedef struct { 
  
 CHAR       *bankFileName; 
 HydraClass *pHydra; 
 DWORD       sampleCkOffset;

}bankNodeType; 

int dumpPresets( sfBankID bankId, SFPRESETHDRPTR presets, WORD cnt ){
  // read smapledata
  int offset = sfGetSMPLOffset( bankId );
  printf("[x] SMP offset : %i\n", offset);
  for( int i = 0; i < cnt; i++ ){
    printf("[x] preset found: (%i/%i) %s\n", presets[i].wPresetNum,
                                             presets[i].wPresetBank,
                                             presets[i].achPresetName );
    WORD dataCount;
    sfData *data = sfNav( bankId, presets[i].wPresetBank, presets[i].wPresetNum, 60, 127, &dataCount ); 
    for( int j = 0; j < dataCount; j++ ){
      int size = (data[j].dwEnd-data[j].dwStart)+1;
      int loopstart = data[j].dwStartloop-data[j].dwStart;
      int loopend   = data[j].dwEndloop-data[j].dwStart;
      printf("   %i -> sample addr start: %i\n", j, data[j].dwStart );
      printf("   %i -> sample addr end  : %i\n", j, data[j].dwEnd );
      printf("   %i -> sample frames    : %i\n", j, size );
      printf("   %i -> sample loop start: %i\n", j, loopstart );
      printf("   %i -> sample loop end  : %i\n", j, loopend );
      printf("   %i -> sample rate      : %i\n", j, data[j].dwSampleRate );
      char outfile[255];
      char cmd[255];
      exit(0);
    }
  }
  return 0; 
}

int main(int argc, char *argv[] ){
  if( argc < 3 ){
    printf("Usage: sf2toraw <sf2file> <outdir>\n");
    exit(1);
  }
  outdir = argv[2];
  sfBankID bankId;
  bankId = sfReadSFBFile( argv[1] );
  if(bankId != -1) { 
    cout <<endl; 
    cout <<"...Bank Loaded, ID returned : " << bankId <<endl<<endl; 
  }

  bankNodeType bankNodes[MAXLOADEDBANKS]; 
  sfReader reader = sfReader();
  enaErrors omega        = enaSUCCESS; 
  sfDetectErrors       detect;
  RIFFClass              riff; 
  SoundFontNavigator      navit;
  char *bankFileName = argv[1];
  char outfilename[12255];
  for( int i = 0; i < MAXLOADEDBANKS; i++ ) { 
    if(bankNodes[i].pHydra == NULL ) { // use first we find...
      HydraClass *pHF;
      // Note there is no WaveTable ID in this call because this is
      // only displaying numbers on a screen. 
      reader.SetupToFillSampleBuckets();
      pHF = reader.ReadSFBFile( bankFileName); 
      WORD cnt;
      SFPRESETHDRPTR presets = sfGetPresetHdrs( bankId, &cnt ); 
      //if( cnt > 0 ) return dumpPresets(bankId, presets, cnt);
      bankNodes[i].pHydra = pHF;
      bankNodes[i].bankFileName = new CHAR[strlen(bankFileName)+1]; 
      strcpy(bankNodes[i].bankFileName , bankFileName); 

      riff.OpenRIFF(bankFileName);
      riff.FindCk("SMPL"); 
      bankNodes[i].sampleCkOffset = riff.RIFFTell();
      riff.RIFFClose(); 

      #ifdef DEBUG_ENAB
       cout<< endl;
       cout<<"File Name     : " << bankFileName <<endl; 
       cout<<"Bank Name     : " << bankNodes[i].pHydra->GetBankName()<< endl
           <<"Enab ID       : " << i << endl; 
       cout<<"SMPL Offset   : " << bankNodes[i].sampleCkOffset << endl; 
       cout<<"pSHDR         : " << bankNodes[i].pHydra->pSHdr[0].achSampleName << endl; 
      #endif
      for( char x = 0; bankNodes[i].pHydra->pSHdr[x].dwSampleRate != 0; x++ ){
        #ifdef DEBUG_ENAB
        printf( "%c: %s->dwStart = %i\n", x, bankNodes[i].pHydra->pSHdr[x].achSampleName,
                                        bankNodes[i].pHydra->pSHdr[x].dwStart );
        printf( "%c: %s->dwEnd = %i\n", x,   bankNodes[i].pHydra->pSHdr[x].achSampleName,
                                        bankNodes[i].pHydra->pSHdr[x].dwEnd );
        printf( "%c: %s->dwStartloop = %i\n", x,   bankNodes[i].pHydra->pSHdr[x].achSampleName,
                                        bankNodes[i].pHydra->pSHdr[x].dwStartloop );
        printf( "%c: %s->dwEndloop = %i\n", x,   bankNodes[i].pHydra->pSHdr[x].achSampleName,
                                        bankNodes[i].pHydra->pSHdr[x].dwEndloop );
        #endif
        int sampleFrames  = bankNodes[i].pHydra->pSHdr[x].dwEnd - bankNodes[i].pHydra->pSHdr[x].dwStart;
        sampleFrames    *= 2;
        DWORD bucketSize = sampleFrames;
        #ifdef DEBUG_ENAB
        printf("reading %i bytes\n", bucketSize);
        #endif
        BYTE *buffer = (BYTE *) malloc( sampleFrames);
        reader.SetupToFillSampleBuckets(  bankNodes[i].pHydra->pSHdr[x].dwStart,
                                          bankNodes[i].pHydra->pSHdr[x].dwEnd );
        while( bucketSize != 0 ){
          //printf("bucketSize = %\n", bucketSize );
          reader.FillSampleBucket( buffer, &bucketSize );
        }

        sprintf(outfilename, "%s/%s.raw", argv[2],bankNodes[i].pHydra->pSHdr[x].achSampleName);
        FILE *fout = fopen( outfilename, "wb" );
        size_t byteswritten = 0;
        if( fout != NULL ){ 
          byteswritten += fwrite( buffer, 1, sampleFrames, fout ); 
        }else printf("error file open %s\n", outfilename );
        fclose( fout);
        // write loopfile
        char loopfile[255];
        sprintf(loopfile, "%s/%s.raw.txt",argv[2],bankNodes[i].pHydra->pSHdr[x].achSampleName );
        writeLoopFile( loopfile, 
                       bankNodes[i].pHydra->pSHdr[x].dwStartloop,
                       bankNodes[i].pHydra->pSHdr[x].dwEndloop,
                       bankNodes[i].pHydra->pSHdr[x].dwSampleRate,
                       1 );
        #ifdef DEBUG_ENAB
        printf("--> written %s (%i bytes)\n",outfilename,byteswritten);
        #endif
        //printf("writing %s\n", outfilename);
      }
    }
  }
  return 0;
}
