# Copyright (C) 2017-2018, 2021 taylor.fish <contact@taylor.fish>
#
# This file is part of Fish AmpMod.
#
# Fish AmpMod is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Fish AmpMod is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Fish AmpMod.  If not, see <http://www.gnu.org/licenses/>.

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -fpic -Ofast -MMD \
         -fvisibility=hidden -flto -fwhole-program
LDFLAGS = -shared -Wl,--no-undefined,--no-allow-shlib-undefined
LDLIBS = -lm
OBJECTS = ampmod.o
LIBRARY = ampmod.so

.PHONY: all
all: $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

-include $(OBJECTS:.o=.d)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(OBJECTS:.o=.d) $(LIBRARY)
