// TDE.cpp : define o ponto de entrada para o aplicativo do console.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Texto.h"
#include "Filtro.h"
#include "Cadeia.h"

char *RemoveCharAt(char *c, int pos, int len) {
	char *copy = (char*)malloc((len - 1) * sizeof(char));
	c[pos] = '\0'; // life hack
	strcpy(copy, c);
	c[pos] = '6';
	strcpy(&copy[pos], &c[pos + 1]);
	free(c);
	return copy;
}

char *AddString(char *destiny, char *src) {
	int a = (destiny == 0 ? 0 : strlen(destiny));
	int b = strlen(src);
	char *neww = (char*)realloc(destiny, (a + b +1) * sizeof(char));
	strcpy(&neww[a], src);
	return neww;
}


Cadeia **AdicionaListaCadeia(Cadeia **lista,int tamanho, Cadeia *dado) {
	lista = (Cadeia**)realloc(lista, (tamanho + 1) * sizeof(Cadeia*));
	lista[tamanho] = dado;
	return lista;
}

int main()
{
	char f[] = "hue.txt";
	Texto *t = CarregaTexto(f);
	Filtro *filtro = CreateFilter("ATCG>");
	int ponteiro = 0;
	char *buffer = 0;
	char *dnafiltrado = 0;
	int i = 1;
	int j = 0;
	//etapa 1: separar cabeçalho e remover caracteres que não estão contidos no filtro
	for (; i < t->QntLinhas; i++) {
		buffer = AddString(buffer,t->Linhas[ponteiro++].Caracteres);
		if (buffer) {
			if (buffer[0] == '>') {
				//achamos o outro carinha
				free(buffer);
				buffer = 0;
			}
			else {
				for (j = 0; j < t->Linhas[ponteiro - 1].QntCaracteres; j++) {
					if (false == IsInFilter(filtro, buffer[j])) {
						buffer = RemoveCharAt(buffer, j, t->Linhas[ponteiro - 1].QntCaracteres);
					}
				}
				dnafiltrado = AddString(dnafiltrado, buffer);
				free(buffer);
				buffer = 0;
			}
		}
	}
	//etapa 2: calcular entropia

	//etapa 3: transformar
	Cadeia *cadeiatmp;
	Cadeia **listacadeias = (Cadeia**)malloc(sizeof(Cadeia*));
	int sizeoflistacadeias = 0;
	int fim = strlen(dnafiltrado);
	int debug = 0;
	for (int i = 0; i < fim; i++) {
		debug = 0;
		debug = i;
		cadeiatmp = CadeiaDuplaIgual(&dnafiltrado[i], &i);
		if (!cadeiatmp) {
			debug = i;
			cadeiatmp = CadeiaMetadeIgual(&dnafiltrado[i], &i);
			if (!cadeiatmp) {
				debug = i;
				cadeiatmp = CadeiaTudoIgual(&dnafiltrado[i], &i);
				if (!cadeiatmp) {
					debug = i;
					cadeiatmp = CadeiaPar(&dnafiltrado[i], &i);
					if (!cadeiatmp) {
						debug = i;
						cadeiatmp = CadeiaUmDiferente(&dnafiltrado[i], &i);
						if (!cadeiatmp) {
							debug = i;
							cadeiatmp = CadeiaPrimeiroeUltimoDiferentes(&dnafiltrado[i], &i);
							if (!cadeiatmp) {
								debug = i;
								cadeiatmp = CadeiaEhPalindromo(&dnafiltrado[i],fim, &i,0);
								if (!cadeiatmp) {
									cadeiatmp = CadeiaEhPalindromo(&dnafiltrado[i], fim, &i, 1);
									if (!cadeiatmp) {
										cadeiatmp = cadeiatmp;
									}
								}
							}
						}
					}
				}
			}
		}
		if (cadeiatmp) {
			listacadeias = AdicionaListaCadeia(listacadeias, sizeoflistacadeias++, cadeiatmp);
		}
	}

	printf("Lido: \n%s", dnafiltrado);
	printf("\n\nFiltrado: \n");
	for (j = 0; j < sizeoflistacadeias; j++) {
		printf("%s", listacadeias[j]->Dna);
	}
	printf("\n\nCompactado: \n");
	for (j = 0; j < sizeoflistacadeias; j++) {
		printf("%s", listacadeias[j]->Compactado);
	}
	return 0;
}

