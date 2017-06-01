#ifndef SFC_H
#define SFC_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct ptab {
	int ch;
	float p;
} ptab;

typedef struct buffer {
	char v;
	int size;
} buffer;


typedef struct tree {
  int data;
  struct tree *left;
  struct tree *right;
} tree;

// sfc_c.c
void compress(const char *input, const char *output);
void encode(int li, int ri);
void charcat(char s[], char t);
int ptablebuild(FILE *infile, ptab v[]);

// sfc_d.c
void decompress(const char *input, const char *output);
void buildtree(FILE *infile, tree *root, buffer *buff);

// sfc_lib.c
void swap(ptab v[], int i, int j);
void quicksort(ptab v[],int first, int last);
void clear(tree *root);
int gototree(char c, tree **t);
tree* addelement(int v);
void writebit(FILE *outfile, buffer *buff, char bit);
int readbit(FILE *infile, buffer *buff);
void chartobit(int val, char v[]);
int bittochar(char v[]);

#endif