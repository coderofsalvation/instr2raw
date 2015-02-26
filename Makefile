# comment/uncomment the line below to toggle between verbose/nonverbose information
#DEBUG=-DDEBUG_ENAB -DDEBUG
CPPC=g++ -std=gnu++11
# Parameters used for compilation
CPPFLAGS=-Wall -fdata-sections -ffunction-sections -Os -fmax-errors=5 -Isrc/sf2write
# Additional parameters used for linking whole programs
CPPWHOLE=-s -fwhole-program -static
SHELL=/bin/bash
CFLAGS_SF2=-m32 -fpermissive $(DEBUG) -DCPP_MEMORY -I./include -I/usr/include/x86_64-linux-gnu/c++/4.8
all: bin/xi2raw bin/loopdump bin/loopinject bin/raw2wav sf2sdk src/libsf2.a bin/sf2toraw bin/sf2write

clean:
	rm -rf src/sf2sdk src/*.o src/sf2write/*.o
	rm bin/* src/libsf2.a

install:
	@find bin -type f -executable -exec cp {} /usr/local/bin \;

bin/sf2toraw: src/libsf2.a src/sf2toraw.cpp
	g++ -o bin/sf2toraw src/sf2toraw.cpp  $(CFLAGS_SF2) -L. -Lsrc -lsf2 

sf2sdk:
	@echo "[x] We are now going to download + build the creative soundfont sdk"
	@echo -e "[x] (Initially Creative labs insisted in not redistributing this code directly,\n so this makefile used to download it during build.\n However as of 2015 the link died, so the zip is included in the repo now.\n\n contact maintainer when there's any problem with that)"
	@[[ ! -d src/sf2sdk ]] && mkdir src/sf2sdk && mkdir src/sf2sdk/obj; cd src/sf2sdk;\
	echo "[x] unzipping sdk";\
	unzip -o ../enabler.zip | while read line; do printf .; done; echo "";\
	echo "[x] converting all uppercase (DOS)-style files to more nowadays lowercase-format";\
	for f in `find .`; do mv -v $$f `echo $$f | tr '[A-Z]' '[a-z]'` &>/dev/null; done;\
	for f in `find .`; do mv -v $$f `echo $$f | tr '[A-Z]' '[a-z]'` &>/dev/null; done;\
	echo "[x] patching a bit";\
	sed -i 'N;s/iostream\.h>/iostream>\nusing namespace std;/g' src/enab.cpp;\
	sed -i 'N;s/"iostream\.h"/<iostream>\nusing namespace std;/g' src/omega.cpp;\
	sed -i 'N;s/iostream\.h>/iostream>\nusing namespace std;/g' src/sfreader.cpp;\
	sed -i 'N;s/iostream\.h>/iostream>\nusing namespace std;/g' src/riff.cpp;\
	exit 0

src/libsf2.a: src/sf2sdk
	cd src/sf2sdk;\
	g++ $(CFLAGS_SF2) -o obj/hydra.o -c src/hydra.cpp;\
	g++ $(CFLAGS_SF2) -o obj/riff.o -c src/riff.cpp;\
	g++ $(CFLAGS_SF2) -o obj/sfreader.o -c src/sfreader.cpp;\
	g++ $(CFLAGS_SF2) -o obj/sfnav.o -c src/sfnav.cpp;\
	g++ $(CFLAGS_SF2) -o obj/win_mem.o -c src/win_mem.cpp;\
	g++ $(CFLAGS_SF2) -o obj/omega.o -c src/omega.cpp;\
	g++ $(CFLAGS_SF2) -o obj/sfnav.o -c src/sfnav.cpp;\
	g++ $(CFLAGS_SF2) -o obj/sflookup.o -c src/sflookup.cpp;\
	g++ $(CFLAGS_SF2) -o obj/sfdetect.o -c src/sfdetect.cpp;\
	g++ $(CFLAGS_SF2) -o obj/enab.o -c src/enab.cpp;\
	# creating libsf2.a (which contains sf2-reader functionality);\
	# creating libsf2.a (which contains sf2-reader functionality);\
	ar rvs ../libsf2.a obj/hydra.o obj/riff.o obj/sfreader.o obj/sfnav.o obj/win_mem.o obj/omega.o obj/sfnav.o obj/enab.o obj/sfdetect.o obj/sflookup.o

bin/xi2raw: src/xi2raw.c
	gcc src/xi2raw.c -o bin/xi2raw -lsndfile 

bin/raw2wav: src/raw2wav.c
	gcc src/raw2wav.c -o bin/raw2wav -lsndfile 

bin/loopdump: src/loopdump.c
	gcc src/loopdump.c -o bin/loopdump -lsndfile 

bin/loopinject: src/loopinject.c
	gcc src/loopinject.c -o bin/loopinject -lsndfile 

bin/loopinject: src/loopinject.c
	gcc src/loopinject.c -o bin/loopinject -lsndfile 

bin/sf2write: src/sf2write.cpp 
	$(CPPC) $(CPPFLAGS) src/sf2write.cpp src/sf2write/sf2.cpp src/sf2write/midi.cpp -o bin/sf2write 

