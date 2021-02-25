CXX		:=	g++

target ?= oop

TARGET	:=	fouloscopia.bin

BUILDIR	:=	target

INCLUDE	:=	$(addprefix -I, mktoolchain/toolchain/include)

CXXFLAGS :=	    $(INCLUDE)		    \
				-Wall 				\
				-Wextra 			\
				-Winline			\
				-Wuninitialized		\

LDFLAGS	:=  -L mktoolchain/toolchain/libs    		    	\
            -l grapic 										\
            -l SDL2											\
            -l SDL2_image									\
            -l SDL2_ttf										\
            -Wl,-rpath="$(PWD)/mktoolchain/toolchain/libs"

EXTSRC := cpp
EXTOBJ := o

SRC :=	$(wildcard src/*.$(EXTSRC) src/**/*.$(EXTSRC))

OBJ := 	$(patsubst src/%.$(EXTSRC), $(BUILDIR)/%.$(EXTOBJ), $(SRC))

.PHONY: all run build clean install

build: $(TARGET)

all: build

$(TARGET): $(OBJ)
	@$(CXX) -o $(TARGET) $(OBJ) $(LDFLAGS)
	@-echo -e " LINKED      $@"

clean:
	@rm -rf $(BUILDIR) *.bin

$(BUILDIR)/%.$(EXTOBJ): src/%.$(EXTSRC)
	@mkdir -p $(shell dirname $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@-echo -e "    CXX      $@"

install:
	@./mktoolchain/mktoolchain

run: $(TARGET)
	@./$(TARGET)
