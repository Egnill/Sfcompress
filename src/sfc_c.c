#include "sfc.h"
#include <math.h>

static ptab ptable[256];
static char codes[256][8];

void compress(const char *input, const char *output)
{
	int c, i = 0, j;
	char string[256];

	FILE *infile = fopen(input, "r");

	int size = ptablebuild(infile, ptable);

	encode(0, size - 1);

	printf("Code-table size: %d\n", size);

	FILE *codetable = fopen("file/codetable", "wb");
	for (i = 0; i < size; ++i) {
		fprintf(codetable, "%c %s %f \n", ptable[i].ch, codes[ptable[i].ch], ptable[i].p);
	}
	fclose(codetable);
	
	FILE *output_w = fopen("file/output_w", "wb");
	
	fseek(infile, 0, SEEK_SET);
	fgets(string, 255, infile);

	i = 0;
	while (string[i] != '\0') {
		for (j = 0; j <= size; ++j) {
			if (string[i] == ptable[j].ch) {
				fprintf(output_w, "%s", codes[ptable[j].ch]);
				fprintf(output_w, " ");
			}
		}

		++i;
	}
	j = 0;
	i = 0;
	fclose(output_w);

	fseek(infile, 0, SEEK_SET);
	FILE *outfile = fopen("file/output", "wb");
	
	putc(size - 1, outfile);

	buffer buff;
	buff.size = buff.v = 0;

	char codesize[8], codebit[8], *ch;
	for (i = 0; i < size; ++i) {
		c = ptable[i].ch;
		chartobit(c, codebit);
		for (j = 0; j < 8; ++j) {
			writebit(outfile, &buff, codebit[j]); // 8 bits of the code
		}

		chartobit(strlen(codes[c]) - 1, codesize);
		
		for (j = 0; j < 8; ++j) {
			writebit(outfile, &buff, codesize[j]); // size of code
		}

		j = -1;
		ch = codes[c];
		while (ch[++j] != '\0') {
			writebit(outfile, &buff, ch[j]); // code
		}
	}

	fseek(infile, 0, SEEK_SET);
	
	while ((c = getc(infile)) != EOF){
		ch = codes[c];
		j = -1;
		while (ch[++j] != '\0') {
			writebit(outfile, &buff, ch[j]);
		}	
	}

	if (buff.size != 8) {
		putc(buff.v, outfile);
	}
	
	putc(buff.size, outfile);

	fclose(outfile);
	fclose(infile);
}

int ptablebuild(FILE *infile, ptab ptable[])
{
	int freq_table[256], i, c;

	int total = 0;
	for (i = 0; i < 256; ++i) {
		freq_table[i] = 0;
	}

	while ((c = getc(infile)) != EOF) {
		freq_table[c]++;
		++total;
	}

	int ftot = total;

	int size = 0;
	for (i = 0; i < 256; ++i) {
		if (!freq_table[i]) {
			continue;
		}

		ptable[size].ch = i;
		ptable[size].p = (float)freq_table[i] / ftot;
		size++;
	}
	
	quicksort(ptable, 0, size);
	return size;
}

void encode(int li, int ri)
{
	if (li != ri) {

		int i, isp;
		float p, phalf;

		if (ri - li == 1) {
			charcat(codes[ptable[li].ch], '0');
			charcat(codes[ptable[ri].ch], '1');
		} else {
			phalf = 0;
			for(i = li; i <= ri; ++i) {
				phalf += ptable[i].p;
			}

			p = 0;
			isp = -1;
			phalf *= 0.5f;
			for(i = li; i <= ri; ++i) {
				if(p <= phalf) {
					charcat(codes[ptable[i].ch], '0');
				} else {
					charcat(codes[ptable[i].ch], '1');
					if(isp < 0) {
						isp = i;
					}
				}
				p += ptable[i].p;
			}

			if (isp < 0) {
				isp = li + 1;
			}

			encode(li, isp - 1);
			encode(isp, ri);

		}
	}
}

void charcat(char s[], char t)
{
	int i = 0;
	while (s[i] != '\0') {
		i++;
	}

	s[i++] = t;
	s[i++] = '\0';
}