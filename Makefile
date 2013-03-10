# comment/uncomment the line below to toggle between verbose/nonverbose information
DEBUG=-DDEBUG_ENAB -DDEBUG
SHELL=/bin/bash
CFLAGS_SF2=-fpermissive $(DEBUG) -DCPP_MEMORY -I./include 
all: xi2raw loopdump loopinject raw2wav sf2sdk libsf2.a sf2toraw

clean:
	rm xi2raw loopdump loopinject raw2wav sf2toraw libsf2.a sf2sdk/obj/*
	rm -rf sf2sdk

sf2toraw: libsf2.a sf2toraw.cpp
	g++ -o sf2toraw sf2toraw.cpp  $(CFLAGS_SF2) -L. -lsf2

sf2sdk:
	@echo "[x] We are now going to download + build the creative soundfont sdk"
	@echo "[x] (Creative labs insist in not redistributing this code directly, contact me if the following doesnt work)"
	@read -p "press a key to continue.."
	@[[ ! -d sf2sdk ]] && mkdir sf2sdk && mkdir sf2sdk/obj; cd sf2sdk;\
	[[ ! -f enabler.zip ]] && wget http://connect.creativelabs.com/developer/SoundFont/enabler.zip;\
	echo "[x] unzipping sdk";\
	unzip enabler.zip | while read line; do printf .; done; echo "";\
	echo "[x] converting all uppercase (DOS)-style files to more nowadays lowercase-format";\
	for f in `find .`; do mv -v $$f `echo $$f | tr '[A-Z]' '[a-z]'` &>/dev/null; done;\
	for f in `find .`; do mv -v $$f `echo $$f | tr '[A-Z]' '[a-z]'` &>/dev/null; done;\
	echo "[x] patching a bit";\
	sed -i 'N;s/iostream\.h>/iostream>\nusing namespace std;/g' src/enab.cpp;\
	sed -i 'N;s/"iostream\.h"/<iostream>\nusing namespace std;/g' src/omega.cpp;\
	sed -i 'N;s/iostream\.h>/iostream>\nusing namespace std;/g' src/sfreader.cpp;\
	sed -i 'N;s/iostream\.h>/iostream>\nusing namespace std;/g' src/riff.cpp;\
	exit 0

libsf2.a: sf2sdk
	cd sf2sdk;\
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

xi2raw: xi2raw.c
	gcc xi2raw.c -o xi2raw -lsndfile 

raw2wav: raw2wav.c
	gcc raw2wav.c -o raw2wav -lsndfile 

loopdump: loopdump.c
	gcc loopdump.c -o loopdump -lsndfile 

loopinject: loopinject.c
	gcc loopinject.c -o loopinject -lsndfile 
