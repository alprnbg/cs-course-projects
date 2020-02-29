mpic++ main.cpp -o game
time mpirun -np 17 --oversubscribe ./game input.txt output.txt 10