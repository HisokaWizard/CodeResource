# Project: BudComplan (Application)
# Target: Linux

include ../../../../cbp$(Build_Suffix).mk

MF_DIR=.

### Compiler/linker options

Global_CFLAGS=$(cflags)

Project_CFLAGS= \
-W \
-Winvalid-pch \
-include "gccPrecompile.h" \
-DNO_GCC_PRAGMA \
-DUSE_PCH

All_CFLAGS=$(Project_CFLAGS) $(Global_CFLAGS) $(CXXCFLAGS)

OBJ_DEPS=-MT$@ -MF$@.d -MD -MP

Project_INCS= \
-I$(MF_DIR)/ \
-I$(MF_DIR)/./Include \
-I$(MF_DIR)/../../../../AMG/AMGLib/Include \
-I$(MF_DIR)/../../../../sclHTTP/sclHTTPLib/Include \
-I$(MF_DIR)/../../../../amgSCL/amgSCLLib/Include \
-I$(MF_DIR)/../../../../LotApplication/LotApplicationLib/Include \
-I$(MF_DIR)/../../../../sclYang/sclYangLib/Include \
-I$(MF_DIR)/../../../CakApiLib/Include \
-I$(MF_DIR)/../../../../LotData/LotDataLib/Include

Project_LIBDIRS= \
-L$(MF_DIR)/../../../../AMG/Lib/gcc_linux$(Build_Suffix) \
-L$(MF_DIR)/../../../../sclHTTP/Lib/gcc_linux$(Build_Suffix) \
-L$(MF_DIR)/../../../../LotApplication/Lib/gcc_linux$(Build_Suffix) \
-L$(MF_DIR)/../../../../amgSCL/Lib/gcc_linux$(Build_Suffix) \
-L$(MF_DIR)/../../../../sclYang/Lib/gcc_linux$(Build_Suffix) \
-L$(MF_DIR)/../../../Lib/gcc_linux$(Build_Suffix) \
-L$(MF_DIR)/../../../../LotData/Lib/gcc_linux$(Build_Suffix)

Project_LIBS= \
-lCakApi \
-lLotApplication \
-lsclYang \
-lsclHTTP \
-lamgSCL \
-lAMG \
-ljsoncpp \
-lLotData

Target_LIBS= \

All_LDFLAGS=$(Project_LIBS) $(ldflags) $(Target_LIBS) $(LIBS)

LINK=$(CXX)

RM=rm -rf

### Objects used in this Makefile

Project_OUT=$(MF_DIR)/../Bin/gcc_linux$(Build_Suffix)/BudComplan

ifndef OBJ_DIR
   OBJ_DIR=$(MF_DIR)/Obj/gcc_linux$(Build_Suffix)
endif

Project_OBJS= \
$(OBJ_DIR)/Source/Application.o \
$(OBJ_DIR)/Source/Endpoint.o \
$(OBJ_DIR)/Source/EventMsg.o \
$(OBJ_DIR)/Source/EventSource.o \
$(OBJ_DIR)/Source/Frequency.o \
$(OBJ_DIR)/Source/Mode.o \
$(OBJ_DIR)/Source/RestconfRequests.o \
$(OBJ_DIR)/Source/Service.o \
$(OBJ_DIR)/Source/ServiceSet.o \
$(OBJ_DIR)/Source/Types.o \
$(OBJ_DIR)/Source/XmlMerger.o \
$(OBJ_DIR)/Source/main.o \

Project_DEPS=$(patsubst %.o,%.o.d,$(Project_OBJS))

LINK_DEPS= \
$(MF_DIR)/../../../../AMG/Lib/gcc_linux$(Build_Suffix)/libAMG.a \
$(MF_DIR)/../../../../LotApplication/Lib/gcc_linux$(Build_Suffix)/libLotApplication.a \
$(MF_DIR)/../../../../amgSCL/Lib/gcc_linux$(Build_Suffix)/libamgSCL.a \
$(MF_DIR)/../../../../LotData/Lib/gcc_linux$(Build_Suffix)/libLotData.a \
$(MF_DIR)/../../../../sclYang/Lib/gcc_linux$(Build_Suffix)/libsclYang.a \
$(MF_DIR)/../../../Lib/gcc_linux$(Build_Suffix)/libCakApi.a \
$(MF_DIR)/../../../../sclHTTP/Lib/gcc_linux$(Build_Suffix)/libsclHTTP.a


.PHONY: all clean

all: Make_DIRS $(Project_OUT)

clean:
	$(RM) $(Project_OUT) $(Project_OBJS) $(Project_DEPS)

Make_DIRS:
	mkdir -p "$(MF_DIR)/../Bin/gcc_linux$(Build_Suffix)"
	mkdir -p "$(OBJ_DIR)/Source"

$(Project_OUT): $(LINK_DEPS) $(Project_OBJS)
	$(LINK) -o $(Project_OUT) $(Project_OBJS) $(Project_LIBDIRS) $(All_LDFLAGS)

$(OBJ_DIR)/Source/Application.o: $(MF_DIR)/Source/Application.cpp
	$(CXX) $(All_CFLAGS) $(Project_INCS) -c -o $@ $(OBJ_DEPS) $<

$(OBJ_DIR)/Source/Endpoint.o: $(MF_DIR)/Source/Endpoint.cpp
	$(CXX) $(All_CFLAGS) $(Project_INCS) -c -o $@ $(OBJ_DEPS) $<

$(OBJ_DIR)/Source/EventMsg.o: $(MF_DIR)/Source/EventMsg.cpp
	$(CXX) $(All_CFLAGS) $(Project_INCS) -c -o $@ $(OBJ_DEPS) $<

$(OBJ_DIR)/Source/EventSource.o: $(MF_DIR)/Source/EventSource.cpp
	$(CXX) $(All_CFLAGS) $(Project_INCS) -c -o $@ $(OBJ_DEPS) $<

$(OBJ_DIR)/Source/Frequency.o: $(MF_DIR)/Source/Frequency.cpp
	$(CXX) $(All_CFLAGS) $(Project_INCS) -c -o $@ $(OBJ_DEPS) $<

$(OBJ_DIR)/Source/Mode.o: $(MF_DIR)/Source/Mode.cpp
	$(CXX) $(All_CFLAGS) $(Project_INCS) -c -o $@ $(OBJ_DEPS) $<

$(OBJ_DIR)/Source/RestconfRequests.o: $(MF_DIR)/Source/RestconfRequests.cpp
	$(CXX) $(All_CFLAGS) $(Project_INCS) -c -o $@ $(OBJ_DEPS) $<

$(OBJ_DIR)/Source/Service.o: $(MF_DIR)/Source/Service.cpp
	$(CXX) $(All_CFLAGS) $(Project_INCS) -c -o $@ $(OBJ_DEPS) $<

$(OBJ_DIR)/Source/ServiceSet.o: $(MF_DIR)/Source/ServiceSet.cpp
	$(CXX) $(All_CFLAGS) $(Project_INCS) -c -o $@ $(OBJ_DEPS) $<

$(OBJ_DIR)/Source/Types.o: $(MF_DIR)/Source/Types.cpp
	$(CXX) $(All_CFLAGS) $(Project_INCS) -c -o $@ $(OBJ_DEPS) $<

$(OBJ_DIR)/Source/XmlMerger.o: $(MF_DIR)/Source/XmlMerger.cpp
	$(CXX) $(All_CFLAGS) $(Project_INCS) -c -o $@ $(OBJ_DEPS) $<

$(OBJ_DIR)/Source/main.o: $(MF_DIR)/Source/main.cpp
	$(CXX) $(All_CFLAGS) $(Project_INCS) -c -o $@ $(OBJ_DEPS) $<

$(MF_DIR)/../../../../AMG/Lib/gcc_linux$(Build_Suffix)/libAMG.a:
	$(MAKE) -C $(MF_DIR)/../../../../AMG/AMGLib

$(MF_DIR)/../../../../LotApplication/Lib/gcc_linux$(Build_Suffix)/libLotApplication.a: $(MF_DIR)/../../../../AMG/Lib/gcc_linux$(Build_Suffix)/libAMG.a
	$(MAKE) -C $(MF_DIR)/../../../../LotApplication/LotApplicationLib

$(MF_DIR)/../../../../amgSCL/Lib/gcc_linux$(Build_Suffix)/libamgSCL.a: $(MF_DIR)/../../../../AMG/Lib/gcc_linux$(Build_Suffix)/libAMG.a
	$(MAKE) -C $(MF_DIR)/../../../../amgSCL/amgSCLLib

$(MF_DIR)/../../../../LotData/Lib/gcc_linux$(Build_Suffix)/libLotData.a: $(MF_DIR)/../../../../AMG/Lib/gcc_linux$(Build_Suffix)/libAMG.a $(MF_DIR)/../../../../amgSCL/Lib/gcc_linux$(Build_Suffix)/libamgSCL.a
	$(MAKE) -C $(MF_DIR)/../../../../LotData/LotDataLib

$(MF_DIR)/../../../../sclYang/Lib/gcc_linux$(Build_Suffix)/libsclYang.a: $(MF_DIR)/../../../../AMG/Lib/gcc_linux$(Build_Suffix)/libAMG.a $(MF_DIR)/../../../../amgSCL/Lib/gcc_linux$(Build_Suffix)/libamgSCL.a
	$(MAKE) -C $(MF_DIR)/../../../../sclYang/sclYangLib

$(MF_DIR)/../../../Lib/gcc_linux$(Build_Suffix)/libCakApi.a: $(MF_DIR)/../../../../AMG/Lib/gcc_linux$(Build_Suffix)/libAMG.a $(MF_DIR)/../../../../amgSCL/Lib/gcc_linux$(Build_Suffix)/libamgSCL.a
	$(MAKE) -C $(MF_DIR)/../../../CakApiLib

$(MF_DIR)/../../../../sclHTTP/Lib/gcc_linux$(Build_Suffix)/libsclHTTP.a: $(MF_DIR)/../../../../AMG/Lib/gcc_linux$(Build_Suffix)/libAMG.a $(MF_DIR)/../../../../amgSCL/Lib/gcc_linux$(Build_Suffix)/libamgSCL.a $(MF_DIR)/../../../../LotData/Lib/gcc_linux$(Build_Suffix)/libLotData.a
	$(MAKE) -C $(MF_DIR)/../../../../sclHTTP/sclHTTPLib


-include $(OBJ_DIR)/Source/*.d