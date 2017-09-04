# Project: BudMobile (Application)
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

Project_OUT=$(MF_DIR)/../Bin/gcc_linux$(Build_Suffix)/BudMobile

ifndef OBJ_DIR
   OBJ_DIR=$(MF_DIR)/Obj/gcc_linux$(Build_Suffix)
endif

Project_OBJS= \
$(OBJ_DIR)/Sources/Application.o \
$(OBJ_DIR)/Sources/EndPoint.o \
$(OBJ_DIR)/Sources/Group.o \
$(OBJ_DIR)/Sources/main.o \

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
	mkdir -p "$(OBJ_DIR)/Sources"

$(Project_OUT): $(LINK_DEPS) $(Project_OBJS)
	$(LINK) -o $(Project_OUT) $(Project_OBJS) $(Project_LIBDIRS) $(All_LDFLAGS)

$(OBJ_DIR)/Sources/Application.o: $(MF_DIR)/Sources/Application.cpp
	$(CXX) $(All_CFLAGS) $(Project_INCS) -c -o $@ $(OBJ_DEPS) $<

$(OBJ_DIR)/Sources/EndPoint.o: $(MF_DIR)/Sources/EndPoint.cpp
	$(CXX) $(All_CFLAGS) $(Project_INCS) -c -o $@ $(OBJ_DEPS) $<

$(OBJ_DIR)/Sources/Group.o: $(MF_DIR)/Sources/Group.cpp
	$(CXX) $(All_CFLAGS) $(Project_INCS) -c -o $@ $(OBJ_DEPS) $<

$(OBJ_DIR)/Sources/main.o: $(MF_DIR)/Sources/main.cpp
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


-include $(OBJ_DIR)/Sources/*.d