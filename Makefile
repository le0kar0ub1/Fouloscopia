CXX		:=	g++

target  := src/oop

TARGET	:=	fouloscopia.bin

BUILDIR	:=	target

INCLUDE	:=	$(addprefix -I, $(target) mktoolchain/toolchain/include)

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

SRC :=	$(wildcard $(target)/*.$(EXTSRC) $(target)/**/*.$(EXTSRC))

OBJ := 	$(patsubst $(target)/%.$(EXTSRC), $(BUILDIR)/%.$(EXTOBJ), $(SRC))

.PHONY: all run clean install target-dir

target-dir:
	@(test -d $(target)) || (echo "Invalid target directory" && exit 1)

build: target-dir $(TARGET)

project:
	@mkdir -p $(target)

$(TARGET): $(OBJ)
	@$(CXX) -o $(TARGET) $(OBJ) $(LDFLAGS)
	@-echo -e " LINKED      $@"

clean:
	@rm -rf $(BUILDIR) *.bin

$(BUILDIR)/%.$(EXTOBJ): $(target)/%.$(EXTSRC)
	@mkdir -p $(shell dirname $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@-echo -e "    CXX      $@"

install:
	@./mktoolchain/mktoolchain

run: $(TARGET)
	@./$(TARGET)
