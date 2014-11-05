#
# OMNeT++/OMNEST Makefile for epon
#
# This file was generated with the command:
#  opp_makemake -f --deep
#

# Name of target to be created (-o option)
TARGET = epon$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I. \
    -IGBA \
    -IHost \
    -IIPACT \
    -IQueue \
    -ISLA \
    -ITEST

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS =

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/Analyzer.o \
    $O/Splitter.o \
    $O/cStat.o \
    $O/cTDMPON.o \
    $O/Scheduler.o \
    $O/Recorder.o \
    $O/GBA/GBAInterScheduler.o \
    $O/GBA/GBAIntraScheduler.o \
    $O/Host/LRDSource.o \
    $O/Host/Source.o \
    $O/Host/dummy.o \
    $O/Host/Dispatcher.o \
    $O/Host/Sink.o \
    $O/Host/SRDSource.o \
    $O/IPACT/IntraScheduler.o \
    $O/IPACT/InterScheduler.o \
    $O/Queue/TwoStageQueue.o \
    $O/Queue/Queue.o \
    $O/Queue/MultiTwoStageQueue.o \
    $O/Queue/MultiQueue.o \
    $O/SLA/SLAInterScheduler.o \
    $O/SLA/SLAIntraScheduler.o \
    $O/TEST/testInterScheduler.o \
    $O/TEST/testIntraScheduler.o \
    $O/mpcp_m.o

# Message files
MSGFILES = \
    mpcp.msg

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(CXX) -c $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	-rm -rf $O
	-rm -f epon epon.exe libepon.so libepon.a libepon.dll libepon.dylib
	-rm -f ./*_m.cc ./*_m.h
	-rm -f GBA/*_m.cc GBA/*_m.h
	-rm -f Host/*_m.cc Host/*_m.h
	-rm -f IPACT/*_m.cc IPACT/*_m.h
	-rm -f Queue/*_m.cc Queue/*_m.h
	-rm -f SLA/*_m.cc SLA/*_m.h
	-rm -f TEST/*_m.cc TEST/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc GBA/*.cc Host/*.cc IPACT/*.cc Queue/*.cc SLA/*.cc TEST/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/Analyzer.o: Analyzer.cc \
	Analyzer.h \
	Queue/Queue.h \
	Recorder.h \
	cStat.h \
	cTDMPON.h \
	mpcp_m.h
$O/Recorder.o: Recorder.cc \
	Recorder.h \
	mpcp_m.h
$O/Scheduler.o: Scheduler.cc \
	Recorder.h \
	Scheduler.h \
	cStat.h \
	cTDMPON.h \
	mpcp_m.h
$O/Splitter.o: Splitter.cc \
	Recorder.h \
	Splitter.h \
	cStat.h \
	cTDMPON.h \
	mpcp_m.h
$O/cStat.o: cStat.cc \
	cStat.h
$O/cTDMPON.o: cTDMPON.cc \
	Recorder.h \
	cStat.h \
	cTDMPON.h \
	mpcp_m.h
$O/mpcp_m.o: mpcp_m.cc \
	mpcp_m.h
$O/GBA/GBAInterScheduler.o: GBA/GBAInterScheduler.cc \
	GBA/GBAInterScheduler.h \
	Queue/MultiQueue.h \
	Queue/Queue.h \
	Recorder.h \
	Scheduler.h \
	cStat.h \
	cTDMPON.h \
	mpcp_m.h
$O/GBA/GBAIntraScheduler.o: GBA/GBAIntraScheduler.cc \
	GBA/GBAIntraScheduler.h \
	Queue/Queue.h \
	Recorder.h \
	Scheduler.h \
	cStat.h \
	cTDMPON.h \
	mpcp_m.h
$O/Host/Dispatcher.o: Host/Dispatcher.cc \
	Host/Dispatcher.h \
	Recorder.h \
	cStat.h \
	cTDMPON.h \
	mpcp_m.h
$O/Host/LRDSource.o: Host/LRDSource.cc \
	Host/LRDSource.h \
	Host/Source.h \
	Recorder.h \
	cStat.h \
	cTDMPON.h \
	mpcp_m.h
$O/Host/SRDSource.o: Host/SRDSource.cc \
	Host/SRDSource.h \
	Host/Source.h \
	Recorder.h \
	cStat.h \
	cTDMPON.h \
	mpcp_m.h
$O/Host/Sink.o: Host/Sink.cc \
	Host/Sink.h \
	Recorder.h \
	cStat.h \
	cTDMPON.h \
	mpcp_m.h
$O/Host/Source.o: Host/Source.cc \
	Host/Source.h \
	Recorder.h \
	cStat.h \
	cTDMPON.h \
	mpcp_m.h
$O/Host/dummy.o: Host/dummy.cc \
	Host/dummy.h
$O/IPACT/InterScheduler.o: IPACT/InterScheduler.cc \
	IPACT/InterScheduler.h \
	Queue/MultiQueue.h \
	Queue/Queue.h \
	Recorder.h \
	Scheduler.h \
	cStat.h \
	cTDMPON.h \
	mpcp_m.h
$O/IPACT/IntraScheduler.o: IPACT/IntraScheduler.cc \
	IPACT/IntraScheduler.h \
	Queue/Queue.h \
	Recorder.h \
	Scheduler.h \
	cStat.h \
	cTDMPON.h \
	mpcp_m.h
$O/Queue/MultiQueue.o: Queue/MultiQueue.cc \
	Queue/MultiQueue.h \
	Queue/Queue.h \
	Recorder.h \
	cStat.h \
	cTDMPON.h \
	mpcp_m.h
$O/Queue/MultiTwoStageQueue.o: Queue/MultiTwoStageQueue.cc \
	Queue/MultiTwoStageQueue.h \
	Queue/TwoStageQueue.h \
	Recorder.h \
	cStat.h \
	cTDMPON.h \
	mpcp_m.h
$O/Queue/Queue.o: Queue/Queue.cc \
	Queue/Queue.h \
	Recorder.h \
	cStat.h \
	cTDMPON.h \
	mpcp_m.h
$O/Queue/TwoStageQueue.o: Queue/TwoStageQueue.cc \
	Queue/TwoStageQueue.h \
	Recorder.h \
	cStat.h \
	cTDMPON.h \
	mpcp_m.h
$O/SLA/SLAInterScheduler.o: SLA/SLAInterScheduler.cc \
	Queue/MultiQueue.h \
	Queue/Queue.h \
	Recorder.h \
	SLA/SLAInterScheduler.h \
	Scheduler.h \
	cStat.h \
	cTDMPON.h \
	mpcp_m.h
$O/SLA/SLAIntraScheduler.o: SLA/SLAIntraScheduler.cc \
	Queue/Queue.h \
	Recorder.h \
	SLA/SLAIntraScheduler.h \
	Scheduler.h \
	cStat.h \
	cTDMPON.h \
	mpcp_m.h
$O/TEST/testInterScheduler.o: TEST/testInterScheduler.cc \
	Queue/MultiTwoStageQueue.h \
	Queue/TwoStageQueue.h \
	Recorder.h \
	Scheduler.h \
	TEST/testInterScheduler.h \
	cStat.h \
	cTDMPON.h \
	mpcp_m.h
$O/TEST/testIntraScheduler.o: TEST/testIntraScheduler.cc \
	Queue/TwoStageQueue.h \
	Recorder.h \
	Scheduler.h \
	TEST/testIntraScheduler.h \
	cStat.h \
	cTDMPON.h \
	mpcp_m.h

