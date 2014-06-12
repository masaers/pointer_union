# ==============================================================================
# Basic building and testing.
#
# author: Markus Saers <masaers@gmail.com>
# ==============================================================================

CXXFLAGS += -Wall -pedantic -std=c++11 -O3 -g

.PRECIOUS : build/%.o %/.STAMP

%/.STAMP :
	@mkdir -p $(@D)
	@touch $@

build/%.o : %.cpp build/.STAMP
	$(CXX) $(CXXFLAGS) -c -o $@ $<

% : %.o
	$(CXX) $(CXXFLAGS) -o $@ $<

all : test

test : build/pointer_union_test
	@build/pointer_union_test || echo "unit test failed: pointer_union_test"

clean :
	@rm -rf build || true

build/pointer_union_test.o : pointer_union_test.cpp pointer_union.hpp

