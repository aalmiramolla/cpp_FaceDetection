# COPYRIGHT (c) 2016 Alejandro Almira Molla <aalmiramolla at gmail dot com>
#
# GNU GENERAL PUBLIC LICENSE
#    Version 3, 29 June 2007
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


# COMPILER
CC:=g++
CFLAGS:=-Wall -ggdb -std=c++11

# APP
APPNAME:=FaceDetection

# APP DIRECTORIES
BUILDDIR:=bin
INCLUDEDIR:=include
LIBDIR:=lib
SRCDIR:=src
TMPDIR:=.obj

# SOURCES
MAINFILE=$(SRCDIR)/main.cpp
SOURCES:=$(shell find $(SRCDIR) -name '*.cpp')
_SOURCESFOLDERS:=$(shell find $(SRCDIR) -type d)
SOURCESFOLDERS:=$(subst ./,,$(_SOURCESFOLDERS))2

# OBJECTS
_OBJECTS:=$(addprefix $(TMPDIR)/,$(SOURCES:%.cpp=%.o))
_OBJECTS:=$(subst ./,,$(_OBJECTS))
OBJECTS:=$(subst $(TMPDIR)/$(SRCDIR)/main.o,,$(_OBJECTS))

# EXTERNAL DEPENDENCIES
#SHAREDIR:=../shared
#SHAREDLIB:=$(SHAREDIR)/lib
#SHAREDHEADERS:=$(SHAREDIR)/include
#_EXTLIBS:=
#EXTLIBS:=$(addprefix -l,$(_EXTLIBS))
_PKGCONFIGLIBS:=opencv
PKGCONFIG:=`pkg-config --cflags --libs $(_PKGCONFIGLIBS)`


# ================================== TESTS ===================================

# TEST DIRECTORIES
TESTDIR:=tests
TMPTESTDIR:=.obj_tests

# SOURCES
TESTMAINFILE:=$(TESTDIR)/main.cpp
TESTOUTPUTFILE:=test-runner
TESTSOURCES:=$(shell find $(TESTDIR) -name '*.cpp')
TESTSOURCES:=$(subst $(TESTMAINFILE),,$(TESTSOURCES))
_TESTSOURCESFOLDERS:=$(shell find $(TESTDIR) -type d)
TESTSOURCESFOLDERS:=$(subst ./,,$(_TESTSOURCESFOLDERS))

# OBJECTS
_TESTOBJECTS:=$(addprefix $(TMPTESTDIR)/,$(TESTSOURCES:%.cpp=%.o))
TESTOBJECTS:=$(subst ./,,$(_TESTOBJECTS))

# EXTERNAL DEPENDENCIES
_TESTEXTLIBS:=cppunit
TESTEXTLIBS:=$(addprefix -l,$(_TESTEXTLIBS))

# ================================== COMPILE ===================================
$(APPNAME): check compile $(MAINFILE)
	@echo "========================="
	@echo " Building $(APPNAME) ..."
	@echo "========================="
	$(CC) $(CFLAGS) -I$(INCLUDEDIR) $(MAINFILE) $(OBJECTS) $(PKGCONFIG) -o $(BUILDDIR)/$(APPNAME)

compile: $(OBJECTS)

$(TMPDIR)/%.o: %.cpp
	@echo "------------------------"
	@echo "> COMPILING $< ..."
	$(CC) $(CFLAGS) -I$(INCLUDEDIR) $(PKGCONFIG) $< -c -o $@

check:
	@test -d $(BUILDDIR) || mkdir $(BUILDDIR)
	@$(shell find $(SRCDIR) -depth -type d -print | cpio -pd $(TMPDIR))

# =================================== TEST ====================================
check-test:
	@echo "Checking test dirs ..."
	@test -d $(TESTDIR) || { echo "The directory '$(TESTDIR)' doesn't exists. No tests to run."; exit 1; }
	@test -d $(TMPTESTDIR) || mkdir $(TMPTESTDIR)
	@$(shell find $(TESTSRCDIR) -depth -type d -print | cpio -pd $(TMPTESTDIR))

$(TMPTESTDIR)/%.o: %.cpp
	@echo "------------------------"
	@echo "> COMPILING $< ..."
	$(CC) $(CFLAGS) -I$(INCLUDEDIR) -I$(TESTDIR) $< -c -o $@

compile_test: $(TESTOBJECTS)

test: $(APPNAME) check-test compile_test
	@echo "==============================="
	@echo "> Building $(APPNAME) tests ..."
	@echo "==============================="
	@echo "$(TESTOBJECTS)"
	$(CC) $(CFLAGS) -I$(INCLUDEDIR) -I$(TESTDIR) $(TESTMAINFILE) $(OBJECTS) $(TESTOBJECTS) $(TESTEXTLIBS) -o $(BUILDDIR)/$(TESTOUTPUTFILE)

# =================================== OTHER ====================================
run: $(BUILDDIR)/$(APPNAME)
	 $(BUILDDIR)/$(APPNAME) -r -D -p /mnt/hdd/misc/Photos/ -c /usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml

clean:
	-rm -Rf $(TMPDIR)
	-rm -Rf $(TMPTESTDIR)

.PHONY: clean all
