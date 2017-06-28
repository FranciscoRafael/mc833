#!/bin/bash

gcc client.c -o client 
./client dijkstra S B 2 Y 2 2 && ./client dijkstra S A 0 X 1 1

