How to massage .XI and .SF2 instruments to AIFF/WAV with looppoints
===================================================================
These are handy utilities for SF2/XI instrument inspection/conversion and what not more.
The tools might be buggy, but serve as a good startingpoint, as I didn't find any other 
simple sourcecode which does this.

### Author ###

Coder of Salvation (Leon van Kammen / BUGMENOTinfo@leon.vankammen.eu (remove BUGMENOT)

### Problem ###

There are many looped instruments out there (.xi/.sf2), and there are many software DAW's (cubase/ableton) and hardware tools
(electribe esx/old samplers) which support .wav looppoints.
However, there isnt many software out there which uses these features.
Also, conversion tools usually cannot be used in batch, which can be a big pita.

### The purpose of this package ###

  * convert the free legacy of public domain .xi and .sf2 instruments to newer formats
  * purely noncommercial
  * educational
  * conversion of `royalty-`, `license-`, free and/or owned instruments

### How it works ###

The samples and loopingmetadata are extracted from the instrumentfiles, and converted to raw- and txt-files.
The shellscript 'convert' easifies batchprocessing of directories.

`(IMPORT)NOTE: libsndfile generally always produces 16bit PCM (little endian) output`

### Dependancies ###

  * gnu compiler, make, unzip and wget
  * libsndfile library
  * sox library (optional, used for normalizing samples)
 
### Installation ###

    git clone http://
    cd instr2raw
    make all
 
### Usage ###

conversion:

    ./sf2toraw in/bass.sf2 outdir
    ./xitoraw in/chord1.xi outdir

inject loopppoints:

    ./raw2wav out/chord1.raw out/chord1.wav
    ./loopinject out/chord1.wav 100 200 out/chord1-looped.wav

or to batch directories:

    mkdir in out
    # copy all your instrument files to directory 'in' 
    ./convert instr2raw foo.xi out
    ./convert instr2raw foo.sf2 out
    ./convert rawdir2wav outdir

or a whole collection at once:

    mkdir outdir
    ./convert dir myinstrumentdir outdir

`sf2toraw` and `xi2raw` also generate loopfiles which the shellscript `convert` uses for batchpurposes.
But with little effort you can inject loopdata into aiff/wav-files yourself.
You can just call the `loopinject` command manually and insert the looppoints.

### Limitations ###

  * handle stereo files (mono only since my hardware supports monolooped instruments only) 
  * aiff generation/injection possible but `convert` shellscript is not finished concerning aiff output

### License ###

GNU AGPL
