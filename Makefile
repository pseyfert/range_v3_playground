CPPFLAGS = -isystem /home/pseyfert/.local/include -march=native -std=c++17 -m64 -O3 -g -Wextra -Wall -Wshadow

all = masked

masked: masked.o
