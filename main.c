#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 17
#define VAL 64
#define MAX_BUFFER 50000

typedef struct nodo_s {
	int valida;		//Parola ammissibile o no
	char colore; 	//R: red, B: black
	struct nodo_s *sinistro;
	struct nodo_s *destro;
	struct nodo_s *padre;
	char *parola;
} nodo;

typedef struct nodo_bst_s {
	char carattere;
	struct nodo_bst_s *sinistro;
	struct nodo_bst_s *destro;
} nodo_bst;

nodo *T_nil;	//Nodo riferimento a NIL
int cont;		//Contatore parole ammissibili

void stampa_filtrate(nodo *n, int *contatore) {

	if(n != T_nil && *contatore > 0) {
		if(n->sinistro != T_nil) {
			stampa_filtrate(n->sinistro, contatore);
		}
		if(n->valida) {
			(*contatore)--;
			puts(n->parola);
		}
		if(n->destro != T_nil) {
			stampa_filtrate(n->destro, contatore);
		}
	}
}

nodo * ruotaSinistra(nodo *albero, nodo *x) {

	nodo *y;

	y = x->destro;
	x->destro = y->sinistro;
	if(y->sinistro != T_nil) {
		y->sinistro->padre = x;
	}
	y->padre = x->padre;
	if(x->padre == T_nil) {
		albero = y;
	} else if(x == x->padre->sinistro) {
		x->padre->sinistro = y;
	} else {
		x->padre->destro = y;
	}
	y->sinistro = x;
	x->padre = y;

	return albero;
}

nodo * ruotaDestra(nodo *albero, nodo *x) {

	nodo *y;

	y = x->sinistro;
	x->sinistro = y->destro;
	if(y->destro != T_nil) {
		y->destro->padre = x;
	}
	y->padre = x->padre;
	if(x->padre == T_nil) {
		albero = y;
	} else if(x == x->padre->sinistro) {
		x->padre->sinistro = y;
	} else {
		x->padre->destro = y;
	}
	y->destro = x;
	x->padre = y;

	return albero;
}

nodo * riparaRBInserisci(nodo *albero, nodo *n) {

	nodo *x;
	nodo *y;

	if(albero == n) {
		albero->colore = 'B';
	} else {
		x = n->padre;
		if(x->colore == 'R') {
			if(x->padre->sinistro == x) {
				y = x->padre->destro;
				if(y->colore == 'R') {
					x->colore = 'B';
					y->colore = 'B';
					x->padre->colore = 'R';
					albero = riparaRBInserisci(albero, x->padre);
				} else {
					if(n == x->destro) {
						n = x;
						albero = ruotaSinistra(albero, n);
						x = n->padre;
					}
					x->colore = 'B';
					x->padre->colore = 'R';
					albero = ruotaDestra(albero, x->padre);
				}
			} else {
				y = x->padre->sinistro;
				if(y->colore == 'R') {
					x->colore = 'B';
					y->colore = 'B';
					x->padre->colore = 'R';
					albero = riparaRBInserisci(albero, x->padre);
				} else {
					if(n == x->sinistro) {
						n = x;
						albero = ruotaDestra(albero, n);
						x = n->padre;
					}
					x->colore = 'B';
					x->padre->colore = 'R';
					albero = ruotaSinistra(albero, x->padre);
				}
			}
		}
	}

	return albero;
}

int cerca_parola(nodo *n, char *parola) {

	int res;

	if(n == T_nil) {
		return 0;
	} else {
		res = strcmp(n->parola, parola);
		if(res == 0) {
			return 1;
		} else if(res < 0) {
			return cerca_parola(n->destro, parola);
		} else {
			return cerca_parola(n->sinistro, parola);
		}
	}
}

int cerca_carattere(nodo_bst *n, char c) {

	if(n == NULL) {
		return 0;
	} else {
		if(c == n->carattere) {
			return 1;
		} else if(c < n->carattere) {
			return cerca_carattere(n->sinistro, c);
		} else {
			return cerca_carattere(n->destro, c);
		}
	}
}

nodo * pulisci(nodo *n, char lettera, int indice, int *contatore) {

	if(*contatore > 0) {
		if(n->valida) {
			if(n->parola[indice] == lettera) {
				n->valida = 0;
				(*contatore)--;
				cont--;
				if(n->sinistro != T_nil) {
					n->sinistro = pulisci(n->sinistro, lettera, indice, contatore);
				}
				if(n->destro != T_nil) {
					n->destro = pulisci(n->destro, lettera, indice, contatore);
				}
			}
		}
	}

	return n;
}

nodo * sistema_valida(nodo *n, nodo_bst **reg_zero, char *reg_uno, int reg_occ[3][VAL], int *contatore, int k) {

	int occ[VAL];
	int i;
	int indice;

	if(*contatore > 0) {
		if(n->valida) {
			(*contatore)--;
			memset(occ, 0, sizeof(int) * VAL);
			for(i = 0; i < k && n->valida; i++) {
				//Calcola indice
				if(n->parola[i] == '-') {
					indice = 0;
				} else if(n->parola[i] <= '9') {
					indice = (int)n->parola[i] - 48 + 1;
				} else if(n->parola[i] <= 'Z') {
					indice = (int)n->parola[i] - 65 + 11;
				} else if(n->parola[i] == '_') {
					indice = 37;
				} else {
					indice = (int)n->parola[i] - 97 + 38;
				}
				if(reg_uno[i] != '?' && n->parola[i] != reg_uno[i]) {
					n->valida = 0;
					if(i == 0 || (k < 40 && i < 4)) {
						if(n->sinistro != T_nil) {
							n->sinistro = pulisci(n->sinistro, n->parola[i], i, contatore);
						}
						if(n->destro != T_nil) {
							n->destro = pulisci(n->destro, n->parola[i], i, contatore);
						}
					}
				}
				if(reg_occ[2][indice] == 1) {
					occ[indice]++;
					if(reg_occ[0][indice] == 0) {
						n->valida = 0;
						if(i == 0 || (k < 40 && i < 4)) {
							if(n->sinistro != T_nil) {
								n->sinistro = pulisci(n->sinistro, n->parola[i], i, contatore);
							}
							if(n->destro != T_nil) {
								n->destro = pulisci(n->destro, n->parola[i], i, contatore);
							}
						}
					} else if(reg_occ[0][indice] == 1) {
						if(cerca_carattere(reg_zero[i], n->parola[i])) {
							n->valida = 0;
							if(i == 0 || (k < 40 && i < 4)) {
								if(n->sinistro != T_nil) {
									n->sinistro = pulisci(n->sinistro, n->parola[i], i, contatore);
								}
								if(n->destro != T_nil) {
									n->destro = pulisci(n->destro, n->parola[i], i, contatore);
								}
							}
						}
						if(reg_occ[1][indice] < 0) {
							if(occ[indice] > (-1 * reg_occ[1][indice])) {
								n->valida = 0;
							}
						}
					}
				}
			}
			for(i = 0; i < VAL && n->valida; i++) {
				if(reg_occ[0][i] == 1 && reg_occ[2][i] == 1) {
					if(reg_occ[1][i] > 0) {
						if(occ[i] < reg_occ[1][i]) {
							n->valida = 0;
						}
					} else {
						if(occ[i] != (-1 * reg_occ[1][i])) {
							n->valida = 0;
						}
					}
				}
			}
			if(!n->valida) {
				cont--;
			}
		}
		if(n->sinistro != T_nil) {
			n->sinistro = sistema_valida(n->sinistro, reg_zero, reg_uno, reg_occ, contatore, k);
		}
		if(n->destro != T_nil) {
			n->destro = sistema_valida(n->destro, reg_zero, reg_uno, reg_occ, contatore, k);
		}
	}

	return n;
}

nodo * copia_albero(nodo *copia, nodo *n, nodo **buffer_n, char **buffer, int *cont_buffer, int *contatore, int k) {

	nodo *tmp;
	nodo *curr;
	int res = 0;

	if(n != T_nil && *contatore > 0) {
		if(n->valida) {
			(*contatore)--;
			//Inserimento
			tmp = T_nil;
			curr = copia;
			while(curr != T_nil) {
				tmp = curr;
				res = strcmp(n->parola, curr->parola);
				if(res < 0) {
					curr = curr->sinistro;
				} else {
					curr = curr->destro;
				}
			}
			if(*cont_buffer == MAX_BUFFER) {
				*buffer = malloc((k+1)*MAX_BUFFER);
				*buffer_n = malloc(sizeof(nodo)*MAX_BUFFER);
				*cont_buffer = 0;
			}
			curr = &(*(*buffer_n + *cont_buffer));
			curr->parola = &(*(*buffer + (k+1)*(*cont_buffer)));
			(*cont_buffer)++;
			strcpy(curr->parola, n->parola);
			curr->colore = 'R';
			curr->valida = 1;
			curr->sinistro = T_nil;
			curr->destro = T_nil;
			curr->padre = tmp;
			if(tmp == T_nil) {
				copia = curr;
			} else if(res < 0) {
				tmp->sinistro = curr;
			} else {
				tmp->destro = curr;
			}
			copia = riparaRBInserisci(copia, curr);
		}
		if(n->sinistro != T_nil) {
			copia = copia_albero(copia, n->sinistro, buffer_n, buffer, cont_buffer, contatore, k);
		}
		if(n->destro != T_nil) {
			copia = copia_albero(copia, n->destro, buffer_n, buffer, cont_buffer, contatore, k);
		}
	}

	return copia;
}

nodo * reset_valida(nodo *n) {

	if(n->sinistro != T_nil) {
		n->sinistro = reset_valida(n->sinistro);
	}
	if(n->destro != T_nil) {
		n->destro = reset_valida(n->destro);
	}
	if(!n->valida) {
		n->valida = 1;
	}

	return n;
}

nodo_bst * pulisci_albero_bst(nodo_bst *n) {

	if(n != NULL) {
		if(n->sinistro != NULL) {
			n->sinistro = pulisci_albero_bst(n->sinistro);
		}
		if(n->destro != NULL) {
			n->destro = pulisci_albero_bst(n->destro);
		}
		free(n);
	}

	return NULL;
}

int main() {

	nodo *albero;
	nodo *copia;
	nodo *curr;
	nodo *tmp;
	nodo_bst **reg_zero;
	nodo_bst *curr_bst;
	nodo_bst *tmp_bst;
	nodo *buffer_n;
	char *buffer;
	char *reg_uno;
	int reg_occ[3][VAL];
	int occ[2][VAL];
	char *parola;
	char *rif;
	char *out;
	int dim_diz;
	int k;
	int exit;
	int fine;
	int max;
	int i;
	int giusta;
	int indice;
	int valida;
	int nuova;
	int res;
	int contatore;
	int cont_buffer;
	int no_regole;

	//INIZIALIZZAZIONE T_nil
	T_nil = malloc(sizeof(nodo));
	T_nil->sinistro = NULL;
	T_nil->destro = NULL;
	T_nil->colore = 'B';
	T_nil->padre = NULL;

	albero = T_nil;
	copia = T_nil;

	//INIZIALIZZAZIONI
	if(scanf("%d", &k) == 0) {return 0;}
	if(k < MAX) {
		parola = malloc(sizeof(char)*(MAX+1+((k+1)*3)));
		parola[MAX] = '\0';
		rif = &parola[MAX+1];
	} else {
		parola = malloc(sizeof(char)*(k+1)*4);
		parola[k] = '\0';
		rif = &parola[k+1];
	}
	rif[k] = '\0';
	out = &rif[k+1];
	out[k] = '\0';

	reg_zero = malloc(sizeof(nodo_bst)*k);
	reg_uno = malloc(sizeof(char)*k);
	for(i = 0; i < k; i++) {
		reg_zero[i] = NULL;
	}
	memset(reg_uno, '?', k);
	memset(reg_occ[0], -1, sizeof(int)*VAL);
	memset(reg_occ[1], 0, sizeof(int)*VAL);
	memset(reg_occ[2], 0, sizeof(int)*VAL);
	memset(occ[0], 0, sizeof(int)*VAL);
	memset(occ[1], 0, sizeof(int)*VAL);

	//ELABORAZIONE
	cont = 0;
	exit = 0;
	indice = 0;
	res = 0;
	no_regole = 1;
	nuova = 0;
	cont_buffer = 0;
	buffer = malloc(sizeof(char)*(k+1)*MAX_BUFFER);
	buffer_n = malloc(sizeof(nodo)*MAX_BUFFER);
	//SALVATAGGIO PAROLE DIZIONARIO
	if(scanf("%s", parola) == EOF) {exit = 1;}
	while(parola[0] != '+') {
		//Inserisci
		tmp = T_nil;
		curr = albero;
		while(curr != T_nil) {
			tmp = curr;
			res = strcmp(parola, curr->parola);
			if(res < 0) {
				curr = curr->sinistro;
			} else {
				curr = curr->destro;
			}
		}
		if(cont_buffer == MAX_BUFFER) {
			buffer = malloc((k+1)*MAX_BUFFER);
			buffer_n = malloc(sizeof(nodo)*MAX_BUFFER);
			cont_buffer = 0;
		}
		curr = &(*(buffer_n + cont_buffer));
		curr->parola = &(*(buffer + (k+1)*cont_buffer));
		cont_buffer++;
		strcpy(curr->parola, parola);
		curr->colore = 'R';
		curr->valida = 1;
		curr->sinistro = T_nil;
		curr->destro = T_nil;
		curr->padre = tmp;
		if(tmp == T_nil) {
			albero = curr;
		} else if(res < 0) {
			tmp->sinistro = curr;
		} else {
			tmp->destro = curr;
		}
		albero = riparaRBInserisci(albero, curr);
		cont++;
		if(scanf("%s", parola) == EOF) {exit = 1;}
	}

	dim_diz = cont;

	//ELABORAZIONE
	while(!exit) {
		if(parola[1] == 's') {
			contatore = cont;
			if(copia != T_nil) {
				stampa_filtrate(copia, &contatore);
			} else {
				stampa_filtrate(albero, &contatore);
			}
		} else if(parola[1] == 'n') {
			//NUOVA PARTITA
			if(scanf("%s", rif) == EOF) {exit = 1; }
			for(i = 0; i < k; i++) {
				//Calcola indice
				if(rif[i] == '-') {
					indice = 0;
				} else if(rif[i] <= '9') {
					indice = (int)rif[i] - 48 + 1;
				} else if(rif[i] <= 'Z') {
					indice = (int)rif[i] - 65 + 11;
				} else if(rif[i] == '_') {
					indice = 37;
				} else {
					indice = (int)rif[i] - 97 + 38;
				}
				occ[0][indice]++;
			}
			if(scanf("%d", &max) == EOF) {exit = 1; }
			fine = 0;
			while(max > 0 && !fine) {
				if(scanf("%s", parola) == EOF) {exit = 1; }
				if(parola[0] != '+') {//ANALIZZO NUOVA PAROLA
					if(cerca_parola(albero, parola)) {
						giusta = 1;
						for(i = 0; i < k; i++) {
							out[i] = '/';
							if(rif[i] == parola[i]) {
								out[i] = '+';
								//Calcola indice
								if(parola[i] == '-') {
									indice = 0;
								} else if(parola[i] <= '9') {
									indice = (int)parola[i] - 48 + 1;
								} else if(parola[i] <= 'Z') {
									indice = (int)parola[i] - 65 + 11;
								} else if(parola[i] == '_') {
									indice = 37;
								} else {
									indice = (int)parola[i] - 97 + 38;
								}
								occ[1][indice]++;
							} else if(giusta) {
								giusta = 0;
							}
						}
						if(!giusta) {
							for(i = 0; i < k; i++) {
								if(out[i] != '+') {
									//Calcola indice
									if(parola[i] == '-') {
										indice = 0;
									} else if(parola[i] <= '9') {
										indice = (int)parola[i] - 48 + 1;
									} else if(parola[i] <= 'Z') {
										indice = (int)parola[i] - 65 + 11;
									} else if(parola[i] == '_') {
										indice = 37;
									} else {
										indice = (int)parola[i] - 97 + 38;
									}
									occ[1][indice]++;
									if(occ[0][indice] > 0 && occ[1][indice] <= occ[0][indice]) {
										out[i] = '|';
									}
								}
							}
							out[k] = '\0';
							puts(out);
							//AGGIUNGO LE NUOVE REGOLE
							if(cont > 1) {
								nuova = 0;
								for(i = 0; i < k; i++) {
									if(parola[i] == rif[i] && reg_uno[i] == '?') {
										reg_uno[i] = parola[i];
										reg_occ[2][i] = 1;
										nuova = 1;
									}
									//Calcola indice
									if(parola[i] == '-') {
										indice = 0;
									} else if(parola[i] <= '9') {
										indice = (int)parola[i] - 48 + 1;
									} else if(parola[i] <= 'Z') {
										indice = (int)parola[i] - 65 + 11;
									} else if(parola[i] == '_') {
										indice = 37;
									} else {
										indice = (int)parola[i] - 97 + 38;
									}
									if(reg_occ[0][indice] != -1) {
										if(reg_occ[0][indice] == 1) {
											if(out[i] != '+') {
												//Inserisci BST
												tmp_bst = NULL;
												curr_bst = reg_zero[i];
												while(curr_bst != NULL) {
													tmp_bst = curr_bst;
													if(parola[i] < curr_bst->carattere) {
														curr_bst = curr_bst->sinistro;
													} else {
														curr_bst = curr_bst->destro;
													}
												}
												curr_bst = malloc(sizeof(nodo_bst));
												curr_bst->carattere = parola[i];
												curr_bst->sinistro = NULL;
												curr_bst->destro = NULL;
												if(tmp_bst == NULL) {
													reg_zero[i] = curr_bst;
												} else if(parola[i] < tmp_bst->carattere) {
													tmp_bst->sinistro = curr_bst;
												} else {
													tmp_bst->destro = curr_bst;
												}
												reg_occ[2][indice] = 1;
												nuova = 1;
											}
											if(reg_occ[1][indice] > 0) {
												if(occ[1][indice] > occ[0][indice]) {
													reg_occ[1][indice] = (-1 * occ[0][indice]);
													reg_occ[2][indice] = 1;
													nuova = 1;
												} else if(reg_occ[1][indice] < occ[1][indice]) {
													reg_occ[1][indice] = occ[1][indice];
													reg_occ[2][indice] = 1;
													nuova = 1;
												}
											}
										}
									} else {
										nuova = 1;
										reg_occ[2][indice] = 1;
										if(occ[0][indice] > 0) {
											reg_occ[0][indice] = 1;
											if(occ[1][indice] > occ[0][indice]) {
												reg_occ[1][indice] = (-1 * occ[0][indice]);
											} else {
												reg_occ[1][indice] = occ[1][indice];
											}
											if(out[i] != '+') {
												//Inserisci BST
												tmp_bst = NULL;
												curr_bst = reg_zero[i];
												while(curr_bst != NULL) {
													tmp_bst = curr_bst;
													if(parola[i] < curr_bst->carattere) {
														curr_bst = curr_bst->sinistro;
													} else {
														curr_bst = curr_bst->destro;
													}
												}
												curr_bst = malloc(sizeof(nodo_bst));
												curr_bst->carattere = parola[i];
												curr_bst->sinistro = NULL;
												curr_bst->destro = NULL;
												if(tmp_bst == NULL) {
													reg_zero[i] = curr_bst;
												} else if(parola[i] < tmp_bst->carattere) {
													tmp_bst->sinistro = curr_bst;
												} else {
													tmp_bst->destro = curr_bst;
												}
											}
										} else {
											reg_occ[0][indice] = 0;
										}
									}
								}
							}
							if(nuova && no_regole) {
								no_regole = 0;
							}
							if(nuova && cont > 1) {
								contatore = cont;
								if(copia != T_nil) {
									copia = sistema_valida(copia, reg_zero, reg_uno, reg_occ, &contatore, k);
								} else {
									albero = sistema_valida(albero, reg_zero, reg_uno, reg_occ, &contatore, k);
									if(cont < 0.17 * dim_diz) {
										contatore = cont;
										copia = copia_albero(copia, albero, &buffer_n, &buffer, &cont_buffer, &contatore, k);
									}
								}
							}
							memset(reg_occ[2], 0, sizeof(int)*VAL);
							printf("%d\n", cont);
							max--;
						} else {
							puts("ok");
							fine = 1;
						}
						memset(occ[1], 0, sizeof(int)*VAL);
					} else {
						puts("not_exists");
					}
				} else {
					if(parola[1] == 's') {
						if(cont > 1) {
							contatore = cont;
							if(copia != T_nil) {
								stampa_filtrate(copia, &contatore);
							} else {
								stampa_filtrate(albero, &contatore);
							}
						} else {
							puts(rif);
						}
					} else if(parola[1] == 'i') {
						if(scanf("%s", parola) == EOF) {exit = 1; }
						while(parola[0] != '+') {
							valida = 1;
							if(!no_regole) {
								for(i = 0; i < k && valida; i++) {
									//Calcola indice
									if(parola[i] == '-') {
										indice = 0;
									} else if(parola[i] <= '9') {
										indice = (int)parola[i] - 48 + 1;
									} else if(parola[i] <= 'Z') {
										indice = (int)parola[i] - 65 + 11;
									} else if(parola[i] == '_') {
										indice = 37;
									} else {
										indice = (int)parola[i] - 97 + 38;
									}
									occ[1][indice]++;
									if(reg_uno[i] != '?' && parola[i] != reg_uno[i]) {
										valida = 0;
									}
									if(reg_occ[0][indice] == 0) {
										valida = 0;
									} else if(reg_occ[0][indice] == 1) {
										if(cerca_carattere(reg_zero[i], parola[i])) {
											valida = 0;
										}
										if(reg_occ[1][indice] < 0) {
											if(occ[1][indice] > (-1 * reg_occ[1][indice])) {
												valida = 0;
											}
										}
									}
								}
								for(i = 0; i < VAL && valida; i++) {
									if(reg_occ[0][i] == 1) {
										if(reg_occ[1][i] > 0) {
											if(occ[1][i] < reg_occ[1][i]) {
												valida = 0;
											}
										} else {
											if(occ[1][i] != (-1 * reg_occ[1][i])) {
												valida = 0;
											}
										}
									}
								}
							}
							if(valida) {
								cont++;
							}
							memset(occ[1], 0, sizeof(int)*VAL);
							if(valida && copia != T_nil) {
								//Inserisci
								tmp = T_nil;
								curr = copia;
								while(curr != T_nil) {
									tmp = curr;
									res = strcmp(parola, curr->parola);
									if(res < 0) {
										curr = curr->sinistro;
									} else {
										curr = curr->destro;
									}
								}
								if(cont_buffer == MAX_BUFFER) {
									buffer = malloc((k+1)*MAX_BUFFER);
									buffer_n = malloc(sizeof(nodo)*MAX_BUFFER);
									cont_buffer = 0;
								}
								curr = &(*(buffer_n + cont_buffer));
								curr->parola = &(*(buffer + (k+1)*cont_buffer));
								cont_buffer++;
								strcpy(curr->parola, parola);
								curr->colore = 'R';
								curr->valida = valida;
								curr->sinistro = T_nil;
								curr->destro = T_nil;
								curr->padre = tmp;
								if(tmp == T_nil) {
									copia = curr;
								} else if(res < 0) {
									tmp->sinistro = curr;
								} else {
									tmp->destro = curr;
								}
								copia = riparaRBInserisci(copia, curr);
							}
							//Inserisci
							tmp = T_nil;
							curr = albero;
							while(curr != T_nil) {
								tmp = curr;
								res = strcmp(parola, curr->parola);
								if(res < 0) {
									curr = curr->sinistro;
								} else {
									curr = curr->destro;
								}
							}
							if(cont_buffer == MAX_BUFFER) {
								buffer = malloc((k+1)*MAX_BUFFER);
								buffer_n = malloc(sizeof(nodo)*MAX_BUFFER);
								cont_buffer = 0;
							}
							curr = &(*(buffer_n + cont_buffer));
							curr->parola = &(*(buffer + (k+1)*cont_buffer));
							cont_buffer++;
							strcpy(curr->parola, parola);
							curr->colore = 'R';
							curr->valida = valida;
							curr->sinistro = T_nil;
							curr->destro = T_nil;
							curr->padre = tmp;
							if(tmp == T_nil) {
								albero = curr;
							} else if(res < 0) {
								tmp->sinistro = curr;
							} else {
								tmp->destro = curr;
							}
							albero = riparaRBInserisci(albero, curr);
							dim_diz++;
							if(scanf("%s", parola) == EOF) {exit = 1;}
						}
					}
				}
			}
			if(!fine) {
				puts("ko");
			}
			//PULIZIA REGOLE
			if(!no_regole) {
				for(i = 0; i < k; i++) {
					if(reg_zero[i] != NULL) {
						reg_zero[i] = pulisci_albero_bst(reg_zero[i]);
					}
				}
				no_regole = 1;
			}
			copia = T_nil;
			memset(reg_uno, '?', k);
			memset(occ[0], 0, sizeof(int)*VAL);
			memset(reg_occ[0], -1, sizeof(int)*VAL);
			memset(reg_occ[1], 0, sizeof(int)*VAL);
			memset(reg_occ[2], 0, sizeof(int)*VAL);
			albero = reset_valida(albero);
			cont = dim_diz;
		} else if(parola[1] == 'i') {
			if(scanf("%s", parola) == EOF) {exit = 1; }
			while(parola[0] != '+') {
				//Inserisci
				tmp = T_nil;
				curr = albero;
				while(curr != T_nil) {
					tmp = curr;
					res = strcmp(parola, curr->parola);
					if(res < 0) {
						curr = curr->sinistro;
					} else {
						curr = curr->destro;
					}
				}
				if(cont_buffer == MAX_BUFFER) {
					buffer = malloc((k+1)*MAX_BUFFER);
					buffer_n = malloc(sizeof(nodo)*MAX_BUFFER);
					cont_buffer = 0;
				}
				curr = &(*(buffer_n + cont_buffer));
				curr->parola = &(*(buffer + (k+1)*cont_buffer));
				cont_buffer++;
				strcpy(curr->parola, parola);
				curr->colore = 'R';
				curr->valida = 1;
				curr->sinistro = T_nil;
				curr->destro = T_nil;
				curr->padre = tmp;
				if(tmp == T_nil) {
					albero = curr;
				} else if(res < 0) {
					tmp->sinistro = curr;
				} else {
					tmp->destro = curr;
				}
				albero = riparaRBInserisci(albero, curr);
				cont++;
				if(scanf("%s", parola) == EOF) {exit = 1;}
			}
			dim_diz = cont;
		}
		if(scanf("%s", parola) == EOF) {exit = 1;}
	}

	return 0;
}

