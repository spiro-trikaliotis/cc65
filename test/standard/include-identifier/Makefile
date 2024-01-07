# Makefile for the regression tests that return an error code on failure

ifneq ($(shell echo),)
  CMD_EXE = 1
endif

ifdef CMD_EXE
  S = $(subst /,\,/)
  NULLDEV = nul:
  MKDIR = mkdir $(subst /,\,$1)
  RMDIR = -rmdir /s /q $(subst /,\,$1)
  EXESUFFIX = .exe
else
  S = /
  NULLDEV = /dev/null
  MKDIR = mkdir -p $1
  RMDIR = $(RM) -r $1
  EXESUFFIX =
endif

ifdef QUIET
  .SILENT:
  NULLOUT = >$(NULLDEV)
  NULLERR = 2>$(NULLDEV)
endif

CC65 := $(if $(wildcard ../../../bin/cc65*),..$S..$S..$Sbin$Scc65,cc65)

WORKDIR = ../../../testwrk/standard/include-identifier

OPTIONS = c89 c99 cc65

.PHONY: all clean

SOURCES        = $(wildcard *.cc)
HEADERS        = $(wildcard *.h)
TESTPRG        = $(SOURCES:%.cc=$(WORKDIR)/%$(EXESUFFIX))
TESTFILES      = $(wildcard *.identifier)
TESTFILESEMPTY = $(wildcard *.defaultidentifier)
TESTS          = \
                 $(foreach option,$(OPTIONS),$(TESTFILESEMPTY:%.defaultidentifier=$(WORKDIR)/%.$(option)._test)) \
                 $(foreach option,$(OPTIONS),$(TESTFILES:%.identifier=$(WORKDIR)/%.$(option)._test))


all: $(TESTS)

$(WORKDIR):
	$(call MKDIR,$(WORKDIR))

.PRECIOUS: $(WORKDIR)/%$(EXESUFFIX)

$(WORKDIR)/%$(EXESUFFIX): %.cc | $(WORKDIR)
	$(CXX) $(CXXFLAGS) -o "$@" "$<"

$(TESTPRG): $(SOURCES) $(HEADERS)

define TEST_template

$(WORKDIR)/%.$1._test: %.identifier $(TESTPRG)
	$(eval BASE=$$(notdir $$(@:._test=)))
	@$(WORKDIR)/testidentifier$(EXESUFFIX) "$$(CC65)" "$$<" "$$(BASE:.$1=)" "$1" "$$(WORKDIR)"

endef # TEST_template

define TEST_EMPTY_template

$(WORKDIR)/%.$1._test: %.defaultidentifier $(TESTPRG)
	$(eval BASE=$$(notdir $$(@:._test=)))
	@$(WORKDIR)/testidentifier$(EXESUFFIX) "$$(CC65)" "$$<" "" "$1" "$$(WORKDIR)"

endef # TEST_EMPTY_template


$(foreach option,$(OPTIONS),$(eval $(call TEST_template,$(option))))
$(foreach option,$(OPTIONS),$(eval $(call TEST_EMPTY_template,$(option))))

clean:
	@$(call RMDIR,$(WORKDIR))
