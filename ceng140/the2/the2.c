#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
char *Curpos;
int read_and_eval(int *ptr, char *htr, int len)
{
	int left, right, i;
	char oper;
	if (*Curpos == '-')
	{
		Curpos++;
		return !(read_and_eval(ptr, htr, len));
	}
	else if (*Curpos == '(')
	{
		Curpos++;
		left = read_and_eval(ptr, htr, len);
		Curpos++;
		oper = *Curpos;
		Curpos++;
		right = read_and_eval(ptr, htr, len);
		Curpos++;
		switch (oper)
		{
		case '>':
			return left <= right;
			break;
		case '=':
			return left == right;
			break;
		case '&':
			return left && right;
			break;
		case '|':
			return left || right;
			break;
		}
	}

	else
	{
		if (islower(*Curpos))
		{
			for (i = 0; i < len; i++)
			{
				if (*Curpos == htr[i])
				{
					left = ptr[i];
				}
			}
			return left;
		}
	}
}

void sort_ptr(char *P, int n)
{
	int i, j = 0;
	char temp;
	for (i = 1; i < n; i++)
	{
		temp = P[i];
		j = i - 1;
		while ((j >= 0) && (P[j] > temp))
		{
			P[j + 1] = P[j];
			j = j - 1;
		}
		P[j + 1] = temp;
	}
}

int in(char a, char *b)
{
	int i;
	for (i = 0; i < 30; i++)
	{
		if (a == b[i])
		{
			return 0;
		}
	}
	return 1;
}
int find_the_num(char *s)
{
	int i = 0, l = 0, k = 0;
	char array[30] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	while (s[k] != '\0')
	{
		if ((islower(s[k])) && (in(s[k], array)))
		{
			i++;
			array[l] = s[k];
			l++;
		}
		k++;
	}

	return i;
}
char *letter_arr(int i, char *S)
{
	char *p = malloc(sizeof(char) * (i + 1));
	int k, l;
	for (k = 0, l = 0; S[k] != '\0'; k++)
	{
		if (islower(S[k]) && in(S[k], p))
		{
			p[l] = S[k];
			l++;
		}
	}

	sort_ptr(p, l);
	p[l] = 'R';
	return p;
}
char *letter(int i, char *S)
{
	char *z = malloc(sizeof(char) * (i));
	int k, l;
	for (k = 0, l = 0; S[k] != '\0'; k++)
	{
		if (islower(S[k]) && in(S[k], z))
		{
			z[l] = S[k];
			l++;
		}
	}

	sort_ptr(z, l);
	return z;
}
void print_str(char *s, int len)
{
	int i;
	for (i = 0; i < len - 1; i++)
	{
		printf("%c ", s[i]);
	}
	printf("%c\n", s[len - 1]);
}
void print_(char *s, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		printf("%c ", s[i]);
	}
}
int *tf_rakam(char *tf, int len)
{
	int k, *y = malloc(sizeof(int) * (len));

	for (k = 0; k < len; k++)
	{
		if (tf[k] == 'T')
		{
			y[k] = 1;
		}
		else
			y[k] = 0;
	}

	return y;
}
char eval(char *K, int *P, char *H, int len)
{
	Curpos = K;
	if (read_and_eval(P, H, len))
	{
		return 'T';
	}
	else
	{
		return 'F';
	}
}
void make_tf(int *counter, char *tf, int len, char *S)
{
	int i, j, *Eint = NULL;
	char R, *Echa = NULL;
	for (i = 0, j = len - 1; i < len && j >= 0; i++, j--)
	{
		if (counter[j] == pow(2, i) - 1)
		{
			if (tf[j] == 'T')
			{
				tf[j] = 'F';
				counter[j] = 0;
				continue;
			}
			else if (tf[j] == 'F')
			{
				tf[j] = 'T';
				counter[j] = 0;
				continue;
			}
		}
		counter[j]++;
	}
	Eint = tf_rakam(tf, len);
	Echa = letter(len, S);
	R = eval(S, Eint, Echa, len);

	print_(tf, len);
	printf("%c\n", R);
	free(Eint);
	free(Echa);
}
void make_the_tt(char *S)
{
	int num = find_the_num(S);
	char *x = NULL, *tf = malloc(sizeof(char) * (num)), R, *EC = NULL;
	int *counter = malloc(sizeof(int) * num), *EI = NULL;
	int i, k;
	x = letter_arr(num, S);
	print_str(x, num + 1);
	free(x);
	for (i = 0; i < num; i++)
	{
		counter[i] = 0;
	}
	for (i = 0; i < num; i++)
	{
		tf[i] = 'T';
	}
	EI = tf_rakam(tf, num);
	EC = letter(num, S);
	R = eval(S, EI, EC, num);
	print_(tf, num);
	printf("%c\n", R);
	free(EC);
	free(EI);
	for (k = 0; k < pow(2, num) - 1; k++)
	{
		make_tf(counter, tf, num, S);
	}

	free(tf);
	free(counter);
}

int main(void)
{
	char *S = malloc(sizeof(char));
	int i = 0, temp;
	while (1)
	{
		temp = getchar();
		if (temp == EOF)
		{
			break;
		}
		else if (temp != '\n' && temp != ' ')
		{
			S[i] = temp;
			i++;

			S = (char *)realloc(S, sizeof(char) * (i + 1));
		}
	}
	make_the_tt(S);
	free(S);
	return 0;
}
