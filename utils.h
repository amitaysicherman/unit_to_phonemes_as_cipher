//
// utils.h
//
// Efficient Cryptanalysis of Homophonic Substitution Ciphers
// CS 298, Department of Computer Science, San José State University
// Copyright © 2011 Amrapali Dhavare. All rights reserved.
//
// Modified 2016 by Markus Amalthea Magnuson <markus@polyscopic.works>
//

#ifndef ATTACK_UTILS_H__
#define ATTACK_UTILS_H__

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

char * get_file_to_buffer(char *filename);

int get_distinct_symbols(char *buffer, int distinct[256]);
int** read_2d(std::string filename, int N,int M);
int** read_2d(std::string filename, int N) ;
int* read_1d(std::string filename, int N) ;

void print_2d(int** matrix, int N) ;
void print_1d(int* matrix, int N) ;
void save_results(int score,int* values,int N);
int superv_err(int** superv_mapping,int* mapping,int superv_tot,int m,int n);

#endif  // ATTACK_UTILS_H__
