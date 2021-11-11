# define this to turn on debug
DEBUG ?= y

# Application name
TARGET := cmm

# Source files
SRCS := \
  cmm.cpp \
  data.cpp \
  expression.cpp \
  funs.cpp \
  operand.cpp \
  script.cpp

CXXFLAGS += -pipe -Wall

ifeq ($(DEBUG), y)
  CXXFLAGS += -g -O -DDEBUG
else
  CXXFLAGS += -O2
endif

OBJS := $(SRCS:.cpp=.o)

DEPFILE := .depend

.PHONY: all clean dep

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

dep:
	$(CXX) $(CXXFLAGS) -MM $(SRCS) > $(DEPFILE)

clean:
	-rm -f $(OBJS)
	-rm -f $(TARGET)
	-rm -f $(DEPFILE)

ifeq ($(DEPFILE), $(wildcard $(DEPFILE)))
  include $(DEPFILE)
endif
