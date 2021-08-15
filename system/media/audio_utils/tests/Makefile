# TODO Incorporate testing into audio_utils/tests/Android.bp and remove this file.

INC=../include/audio_utils

main : logplot_tests.cpp $(INC)/LogPlot.h
	g++ -I$(INC) -o logplot_tests logplot_tests.cpp

clean :
	rm -f logplot_tests
