include $(SWB_PROJECT_ROOT)/Sources/Common/mk/config.mk

VPATH := . 

CPPS := $(foreach dir,.,$(wildcard $(dir)/*.cpp)) 


OBJS := $(patsubst %.cpp,%.o,$(CPPS))


RELEASE_OBJS=$(addprefix Release/, $(OBJS))
RELEASE_CPP_DEPS=$(RELEASE_OBJS:.o=.d)

DEBUG_OBJS=$(addprefix Debug/, $(OBJS))
DEBUG_CPP_DEPS=$(DEBUG_OBJS:.o=.d)
DEBUG_OBJS=$(addprefix Debug/, $(OBJS))
DEBUG_CPP_DEPS=$(DEBUG_OBJS:.o=.d)

DEBUG_CFLAGS = -O0 -g3 -D_UNIX -DGTEST
RELEASE_CFLAGS = -O3 -g1 -D_UNIX -DGTEST

FEATURE_DIR=ApexSim
TARGET=ApexSim
TARGET_DIR=$(SWB_PROJECT_ROOT)/Sources/Tools/$(FEATURE_DIR)

include makefile.defs

all: $(RELEASE_OBJS)
	@echo 'Building target: $@'
	$(CXX) $(RELEASE_LDFLAGS) -oRelease/$(TARGET) $(RELEASE_OBJS) $(RELEASE_LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory release-post-build

debug: $(DEBUG_OBJS)
	@echo 'Building target: $@'
	$(CXX) $(DEBUG_LDFLAGS) -oDebug/$(TARGET) $(DEBUG_OBJS) $(DEBUG_LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory debug-post-build

clean:
	-$(RM) $(RELEASE_OBJS) $(DEBUG_OBJS) $(RELEASE_CPP_DEPS) $(DEBUG_CPP_DEPS) Release/$(TARGET) Debug/$(TARGET) Bin/$(TARGET)
	-$(RM) $(SWB_PROJECT_ROOT)/Builds/bin/$(TARGET)
	-$(RM) $(SWB_PROJECT_ROOT)/Builds/bind/$(TARGET)
	-@echo ' '

release-post-build:
	-cp $(TARGET_DIR)/Release/$(TARGET) $(SWB_PROJECT_ROOT)/Sources/Tools/$(FEATURE_DIR)/Bin
	-@echo ' '

debug-post-build:
	-cp $(TARGET_DIR)/Debug/$(TARGET) $(SWB_PROJECT_ROOT)/Builds/bind
	-@echo ' '

Release/%.o: %.cpp
	@echo 'Building file: $<'
	$(CXX) -DGTEST $(RELEASE_INCLUDES) $(RELEASE_CFLAGS) -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


Debug/%.o: %.cpp
	@echo 'Building file: $<'
	$(CXX) -DGTEST $(DEBUG_INCLUDES) $(DEBUG_CFLAGS) -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


.PHONY: UT clean
.SECONDARY: release-post-build

#include $(SWB_PROJECT_ROOT)/Sources/Common/mk/cdtbuild.mk

