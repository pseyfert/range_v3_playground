# Copyright (C) 2018  CERN for the benefit of the LHCb collaboration
# Author: Paul Seyfert <pseyfert@cern.ch>
#
# This software is distributed under the terms of the GNU General Public
# Licence version 3 (GPL Version 3), copied verbatim in the file "LICENSE".
#
# In applying this licence, CERN does not waive the privileges and immunities
# granted to it by virtue of its status as an Intergovernmental Organization
# or submit itself to any jurisdiction.

CPPFLAGS = -isystem /home/pseyfert/.local/include -march=native -std=c++17 -m64 -O3 -g -Wextra -Wall -Wshadow

all = masked

masked: masked.o

masked.o: mask.hpp

clean:
	rm -f *.o masked
