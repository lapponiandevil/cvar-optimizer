CPPC = g++
CPPFLAGS = -g -Wall -pipe
XTRAFLAGS = -msse2 -DDSFMT_MEXP=19937 -DHAVE_SSE2
COMPILE = $(CPPC) $(CPPFLAGS)
OBJECTS =  main.o CVaR.o Drawdown.o normsinv.o Normcdf.o \
           Asset.o Share.o Option.o Portfolio.o \
           Parameters.o Scenarios.o writeToFile.o dSFMT.o

default: Optimizer

Optimizer: $(OBJECTS)
	$(COMPILE) $(XTRAFLAGS) -o Optimizer $(OBJECTS)

dSFMT.o: dSFMT.cpp dSFMT.h
	$(COMPILE) -c $(XTRAFLAGS) dSFMT.cpp

Normcdf.o: Normcdf.cpp Normcdf.hpp
	$(COMPILE) -c Normcdf.cpp

normsinv.o: normsinv.cpp normsinv.h
	$(COMPILE) -c normsinv.cpp

Asset.o: Asset.cpp Asset.hpp
	$(COMPILE) -c Asset.cpp

Share.o: Share.cpp Share.hpp Asset.hpp
	$(COMPILE) -c Share.cpp

Option.o: Option.cpp Option.hpp Normcdf.hpp Asset.hpp Share.hpp
	$(COMPILE) -c Option.cpp 

Portfolio.o: Portfolio.cpp Portfolio.hpp Asset.hpp Share.hpp Option.hpp \
             normsinv.h Normcdf.hpp noSuchAssetException.hpp
	$(COMPILE) -c Portfolio.cpp

Parameters.o: Parameters.cpp Parameters.hpp
	$(COMPILE) -c Parameters.cpp

Scenarios.o: Scenarios.cpp Scenarios.hpp Matrix.hpp Portfolio.hpp \
             normsinv.h Normcdf.hpp Asset.hpp Share.hpp Option.hpp \
             noSuchAssetException.hpp dSFMT.h
	$(COMPILE) -c $(XTRAFLAGS) Scenarios.cpp

writeToFile.o: writeToFile.cpp writeToFile.hpp Matrix.hpp Portfolio.hpp \
               normsinv.h Normcdf.hpp Asset.hpp Share.hpp Option.hpp \
               noSuchAssetException.hpp Parameters.hpp
	$(COMPILE) -c writeToFile.cpp

CVaR.o: CVaR.cpp CVaR.hpp Scenarios.hpp writeToFile.hpp Matrix.hpp \
        Portfolio.hpp normsinv.h Normcdf.hpp Asset.hpp Share.hpp Option.hpp \
        noSuchAssetException.hpp dSFMT.h  Parameters.hpp
	$(COMPILE) -c $(XTRAFLAGS) CVaR.cpp

Drawdown.o: Drawdown.cpp Drawdown.hpp Scenarios.hpp writeToFile.hpp \
            Matrix.hpp Portfolio.hpp normsinv.h Normcdf.hpp \
            Asset.hpp Share.hpp Option.hpp Parameters.hpp \
            noSuchAssetException.hpp dSFMT.h
	$(COMPILE) -c $(XTRAFLAGS) Drawdown.cpp

main.o: main.cpp CVaR.hpp Drawdown.hpp Scenarios.hpp writeToFile.hpp \
        Matrix.hpp Portfolio.hpp normsinv.h Normcdf.hpp \
        Asset.hpp Share.hpp Option.hpp Parameters.hpp \
        noSuchAssetException.hpp dSFMT.h
	$(COMPILE) -c $(XTRAFLAGS) main.cpp

view: Optimizer
	./Optimizer

clean:
	rm $(OBJECTS)
