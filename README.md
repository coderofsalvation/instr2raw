How to massage .XI and .SF2 instruments to AIFF/WAV with looppoints
===================================================================
These are handy utilities for SF2/XI instrument inspection/conversion and what not more.
The tools might be buggy, but serve as a good startingpoint, as I didn't find any other 
simple sourcecode which does this.

### Author ###

Coder of Salvation (Leon van Kammen / BUGMENOTinfo@leon.vankammen.eu (remove BUGMENOT)

### Usage ###

    $ ./xi2raw 
    Usage: xi2raw <infile.xi> <outfile.raw>

    $ ./loopdump 
    Usage: loopdump <file.wav>

    $ ./loopinject 
    Usage: loopinject <infile.wav|aiff> <loopstart> <loopend> <outfile.wav|aiff>

    $ ./raw2wav 
    Usage: raw2wavloop <rawfile> <samplerate> <channels> <outfile.wav>

    $ ./sf2toraw 
    Usage: sf2toraw <sf2file> <outdir>

    $ ./sf2write 
    Usage: sf2write <file.sf2> <file.wav> <index> <samplerate> <lokey> <hikey>  [loopstart] [loopstop]

    Note: index starts with 0, define -1 for loopstart/loopstop to disable looping

    $ ./instr2raw 
        ./instr2raw instr2raw            # <file.xi/file.sf2> <outdir>      <-- converts an .xi or .sf2 file to .raw & .wav
        ./instr2raw rawdir2wav           # <dir-with-raw-files> <outdir>    <-- converts .raw files to wav (with loopstart/end)
        ./instr2raw loopInject           # <file.wav/file.aiff>             <-- injects looppoints based on file.raw.txt 
        ./instr2raw dir                  # <indir> <outdir>                 <-- process all .xi and .sf2 in indir, and convert to wav/aiff

        related commands: instr2raw, loopdump, loopinject, raw2wav, sf2toraw, sf2write, xi2raw

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


### Problem ###

There are many looped instruments out there (.xi/.sf2), and there are many software DAW's (cubase/ableton) and hardware tools
(electribe esx/old samplers) which support .wav looppoints.
However, there isnt many software out there which uses these features.
Also, conversion tools usually cannot be used in batch, which can be a big pita.

### The purpose of this package ###

  * convert the free legacy of public domain .xi and .sf2 instruments to newer formats
  * purely noncommercial
  * educational
  * conversion of `royalty-`, `license-free` and/or owned instruments

### How it works ###

The samples and loopingmetadata are extracted from the instrumentfiles, and converted to raw- and txt-files.
The shellscript 'convert' easifies batchprocessing of directories.

`(IMPORT)NOTE: libsndfile generally always produces 16bit PCM (little endian) output`

### Dependancies ###

  * gnu compiler, make, unzip and wget
  * libsndfile library
  * sox library (optional, used for normalizing samples)
  * libc6-dev-i386 package (on my ubuntu)
 
### Installation ###

    git clone http://
    cd instr2raw
    make all
 
### NOTES ###

For more verbose output run the `convert` command with '--verbose' as last argument.
Also, if you feel up to it, turn on the DEBUG-flag in the Makefile for crazy verboseness =].
`sf2toraw` and `xi2raw` also generate loopfiles which the shellscript `convert` uses for batchpurposes.
But with little effort you can inject loopdata into aiff/wav-files yourself.
You can just call the `loopinject` command manually and insert the looppoints.

### Binaries ###

Precompiled are available (fingers crossed!) on my blog (http://leon.vankammen.eu/tech/index.html)

### Limitations ###

  * handle stereo files (mono only since my hardware supports monolooped instruments only) 
  * aiff generation/injection possible but `convert` shellscript is not finished concerning aiff output

### License ###

GNU AGPL
