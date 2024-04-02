FASTJET_DIR = /Users/ycwu/Workingspace/Utilities/FASTJET/3.4.2
DELPHES_DIR = /Users/ycwu/Workingspace/MC-Generators/MG5_aMC_v3_3_1/Delphes

SRCDIR := ../src
INCDIR := ../include
OBJDIR := ../obj
CXX = g++

CXXFLAGS += $(shell $(FASTJET_DIR)/bin/fastjet-config --cxxflags)
CXXFLAGS += $(shell root-config --cflags)
CXXFLAGS += -I$(DELPHES_DIR) -I$(DELPHES_DIR)/external -I$(INCDIR)

LDFLAGS += $(shell $(FASTJET_DIR)/bin/fastjet-config --libs)
LDFLAGS += $(shell root-config --libs) -lEG
LDFLAGS += -Wl,-rpath,$(DELPHES_DIR) -L$(DELPHES_DIR) -lDelphes


SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))

all: test_EFlowObjs.x test_EFlowJet.x test_ConfigParser.x

%.x: %.cpp $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f *.x
	rm -f $(OBJDIR)/*.o
