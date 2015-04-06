#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#import "sched.h"

char 	*ch;
int 	*timer;
int 	*tarcpuvet;
int 	soma = 0;
int 	numtarefas = 0;
tarefa  *temp;
tarefa  *sent = NULL;
vtar    *vet;
vtar 	*encerrados;
vtar 	*pilhaio;
long 	pid_filho;
int 	shmid1;
key_t 	key1;
int 	shmid2;
key_t 	key2;

void inserirFCFSeRR(vtar *v, tarefa *t){
	printf("desempilha %d, tempo: %d, texec: %d, inicio %d, fimio: %d\n",t->id, timer[0], t->texec,t->incio,t->fimio);
	tarefa *posatual = v->primeira;
	if(posatual == sent){
		v->primeira = t;
		v->ultima = t;
		t->prox = sent;
		t->prev = sent;
		//printf("1 if\n");
	}
	else{
		while(posatual->prox != sent && t->chegada >= posatual->chegada) posatual = posatual->prox;
		if(posatual->prev == sent){
			//printf("2 if\n");
			if(t->chegada >= posatual->chegada){
				posatual->prox = t;
				t->prox = sent;
				t->prev = posatual;
				v->ultima = t;
			}
			else{
				posatual->prev = sent;
				t->prox = posatual;
				t->prev = sent;
				v->primeira = t;
			}
		}
		else if(posatual->prox == sent){
			//printf("3 if\n");
			if(t->chegada >= posatual->chegada){	
				v->ultima->prox = t;
				t->prox = sent;
				t->prev = v->ultima;
				v->ultima = t;
			}
			else{
				v->ultima->prev->prox = t;
				t->prev = posatual->prev;
				t->prox = posatual;
				posatual->prev = t;
			}
		}
		else{
			//printf("4 if\n");
			posatual->prev->prox = t;
			t->prev = posatual->prev;
			t->prox = posatual;
			posatual->prev = t;
		}
	}
}

void inserirSJF(vtar *v, tarefa *t){
	printf("desempilha %d, tempo: %d, texec: %d, inicio %d, fimio: %d\n",t->id, timer[0], t->texec,t->incio,t->fimio);
	tarefa *posatual = v->primeira;
	if(posatual == sent){
		v->primeira = t;
		v->ultima = t;
		t->prox = sent;
		t->prev = sent;
		//printf("1 if\n");
	}
	else{
		while(posatual->prox != sent && t->texec >= posatual->texec) posatual = posatual->prox;
		if(posatual->prev == sent){
			//printf("2 if\n");
			if(t->texec >= posatual->texec){
				posatual->prox = t;
				t->prox = sent;
				t->prev = posatual;
				v->ultima = t;
			}
			else{
				posatual->prev = sent;
				t->prox = posatual;
				t->prev = sent;
				v->primeira = t;
			}
		}
		else if(posatual->prox == sent){
			//printf("3 if\n");
			if(t->texec >= posatual->texec){	
				v->ultima->prox = t;
				t->prox = sent;
				t->prev = v->ultima;
				v->ultima = t;
			}
			else{
				v->ultima->prev->prox = t;
				t->prev = posatual->prev;
				t->prox = posatual;
				posatual->prev = t;
			}
		}
		else{
			//printf("4 if\n");
			posatual->prev->prox = t;
			t->prev = posatual->prev;
			t->prox = posatual;
			posatual->prev = t;
		}
	}
}

void terminar(vtar *v, tarefa *t){
	printf("terminar %d, tempo: %d\n",t->id,timer[0]);
	tarefa *posatual = v->primeira;
	t->fimexec = timer[0];
	if(posatual == sent){
		v->primeira = t;
		v->ultima = t;
		t->prox = sent;
		t->prev = sent;
		//printf("1 if\n");
	}
	else{
		while(posatual->prox != sent && t->id >= posatual->id) posatual = posatual->prox;
		if(posatual->prev == sent){
			//printf("2 if\n");
			if(t->id >= posatual->id){
				posatual->prox = t;
				t->prox = sent;
				t->prev = posatual;
				v->ultima = t;
			}
			else{
				posatual->prev = sent;
				t->prox = posatual;
				t->prev = sent;
				v->primeira = t;
			}
		}
		else if(posatual->prox == sent){
			//printf("3 if\n");
			if(t->id >= posatual->id){	
				v->ultima->prox = t;
				t->prox = sent;
				t->prev = v->ultima;
				v->ultima = t;
			}
			else{
				v->ultima->prev->prox = t;
				t->prev = posatual->prev;
				t->prox = posatual;
				posatual->prev = t;
			}
		}
		else{
			//printf("4 if\n");
			posatual->prev->prox = t;
			t->prev = posatual->prev;
			t->prox = posatual;
			posatual->prev = t;
		}
	}
}

void empilhar(vtar *v, tarefa *t, int tipo){
	printf("empilhar %d, tempo: %d\n",t->id,timer[0]);
	int diff = t->fimio - t->incio;
	t->texec -= (t->fimio+1);
	t->fimio = timer[0] + diff;
	tarefa *posatual = v->primeira;
	printf("termino io %d\n", t->fimio);
	if(posatual == sent){
		v->primeira = t;
		v->ultima = t;
		t->prox = sent;
		t->prev = sent;
		//printf("1 if\n");
	}
	else{
		while(posatual->prox != sent && t->fimio >= posatual->fimio) posatual = posatual->prox;
		if(posatual->prev == sent){
			//printf("2 if\n");
			if(t->fimio >= posatual->fimio){
				posatual->prox = t;
				t->prox = sent;
				t->prev = posatual;
				v->ultima = t;
			}
			else{
				posatual->prev = sent;
				t->prox = posatual;
				t->prev = sent;
				v->primeira = t;
			}
		}
		else if(posatual->prox == sent){
			//printf("3 if\n");
			if(t->fimio >= posatual->fimio){	
				v->ultima->prox = t;
				t->prox = sent;
				t->prev = v->ultima;
				v->ultima = t;
			}
			else{
				v->ultima->prev->prox = t;
				t->prev = posatual->prev;
				t->prox = posatual;
				posatual->prev = t;
			}
		}
		else{
			//printf("4 if\n");
			posatual->prev->prox = t;
			t->prev = posatual->prev;
			t->prox = posatual;
			posatual->prev = t;
		}
	}
}

void verificapilha(vtar *v, int tipo){
	tarefa *posatual = v->primeira;
	while(posatual != sent){
		if(posatual->fimio <= timer[0]){
			printf("pilha %d, fim %d\n", posatual->id, posatual->fimio);
			posatual->chegada = timer[0];
			posatual->incio = posatual->texec;
			posatual->fimio = -1;
			tarefa *aux = posatual;
			posatual = posatual->prox;
			if(tipo) inserirFCFSeRR(vet,aux);
			else inserirSJF(vet,aux);
			v->primeira = posatual;
			posatual->prev = sent;
		}
		else return;
	}
}

void finalizar(char *argv[]){
	printf("finalizar--\n");
	FILE *out = fopen(argv[3], "w");
	tarefa *posatual = encerrados->primeira;
	while(posatual != sent){
		fprintf(out, "%d;%d;%d\n", posatual->id, posatual->iniexec, posatual->fimexec);
		soma += posatual->fimexec;
		posatual = posatual->prox;
	}
	fprintf(out,"tempo medio: %f\n", (float)soma/(float)numtarefas);
	fprintf(out,"tempo total: %d\n", timer[0]);
	exit(0);
}

int pegartarefa(char *in){
	//printf("pegartarefa--\n");
	temp = (tarefa*)malloc(sizeof(tarefa));
	temp->id = 0;
	temp->chegada = 0;
	temp->texec = 0;
	temp->incio = 0;
	temp->fimio = 0;
	temp->iniexec = 0;
	temp ->fimexec = 0;
	while((ch = fgetc(in)) != ';'){
		temp->id = temp->id*10 + ((int)ch-48);
	}
	while((ch = fgetc(in)) != ';'){
		temp->chegada = temp->chegada*10 + ((int)ch-48);
	}
	while((ch = fgetc(in)) != ';'){
		temp->texec = temp->texec*10 + ((int)ch-48);
	}
	while((ch = fgetc(in)) != ',' && ch != ';'){
		temp->incio = temp->incio*10 + ((int)ch-48);
	}
	while((ch = fgetc(in)) != ';' && ch != EOF && ch != '\n'){
		temp->fimio = temp->fimio*10 + ((int)ch-48);
	}
	if(ch == ';') ch = fgetc(in);
	if(ch == EOF) return 0;
	else return 1;
}

void montarlista(char *in, vtar *v, int metodo){
	//printf("montarlista--\n");
	v->primeira = sent;
	v->ultima = sent;
	int flag = 1;
	while(flag){
		numtarefas++;
		if(metodo == 1){
			flag = pegartarefa(in);
			inserirFCFSeRR(vet,temp);
		}
		else if(metodo == 2){
			flag = pegartarefa(in);
			inserirSJF(vet,temp);
		}
		else flag = 0;
	}
}

void main(int argc, char *argv[]){
	
	key1 = 50;
	key2 = 51;

	timer = (int*) calloc(1, sizeof(int));
	tarcpuvet = (int*) calloc(6, sizeof(int));
	sent = (tarefa*) malloc(sizeof(tarefa));
	vet = (vtar*)malloc(sizeof(vtar));
	encerrados = (vtar*)malloc(sizeof(vtar));
	pilhaio = (vtar*)malloc(sizeof(vtar));
	encerrados->primeira = sent;
	encerrados->ultima = sent;
	pilhaio->primeira = sent;
	pilhaio->ultima = sent;

	FILE *inp;
	inp = fopen(argv[2], "r");
	
	if(argc != 4){
		printf("Erro na linha de argumentos\n");
		exit(0);
	}

	shmid1 = shmget(key1, sizeof(int), IPC_CREAT | 0666);
	tarcpuvet = (tarefa*)shmat(shmid1, NULL, 0);
	shmid2 = shmget(key2, sizeof(int), IPC_CREAT | 0666);
	timer = (int*)shmat(shmid2, NULL, 0);
	timer[0] = 0;

	if(!strcmp(argv[1], "FCFS") || !strcmp(argv[1], "RR")) montarlista(inp,vet,1);
	else if(!strcmp(argv[1], "SJF")) montarlista(inp,vet,2);
	else return 0;

	tarcpuvet[0] = vet->primeira->texec;
	tarcpuvet[1] = vet->primeira->incio;
	tarcpuvet[4] = vet->primeira->fimio;
	tarcpuvet[2] = 2;
	tarcpuvet[3] = vet->primeira->id;

	pid_filho = fork();

	while(1){

		if(pid_filho == 0){

			if(tarcpuvet[2] == 1 || tarcpuvet[2] == 2){
				tarcpuvet[2] = 1;
				if(!strcmp(argv[1], "FCFS") || !strcmp(argv[1], "SJF")){
					
					int i=0;
					int cont=0;
					if(tarcpuvet[4] != 0) cont = tarcpuvet[1];
					else cont = tarcpuvet[0];
					for(i=0;i<cont;i++){
						sleep(1);
						timer[0]++;
						printf("novo tempo %d, executando %d\n", timer[0],tarcpuvet[3]);
					}
					
				}
				else{
					sleep(1);
					timer[0]++;
				}
				tarcpuvet[2] = 0;
			}
		}

		else{
			
			if (tarcpuvet[2] == 0){

				if(!strcmp(argv[1], "FCFS") || !strcmp(argv[1], "SJF")){
					
					if(vet->primeira->fimio == 0){
						tarefa *aux = vet->primeira;
						vet->primeira->prox->prev = sent;
						vet->primeira = vet->primeira->prox;
						terminar(encerrados, aux);
					}
					else if(vet->primeira->fimio > 0){
						tarefa *aux = vet->primeira;
						vet->primeira->prox->prev = sent;
						vet->primeira = vet->primeira->prox;
						empilhar(pilhaio, aux, 1);
					}
					


					

					if(!strcmp(argv[1], "FCFS") || !strcmp(argv[1], "RR")) verificapilha(pilhaio,1);
					else verificapilha(pilhaio,0);

					if(vet->primeira != sent){
						if(!strcmp(argv[1], "FCFS")){
							if(vet->primeira->chegada <= timer[0]){
								if(vet->primeira->fimio == -1) vet->primeira->fimio = 0;
								tarcpuvet[0] = vet->primeira->texec;
								tarcpuvet[1] = vet->primeira->incio;
								tarcpuvet[2] = 2;
								tarcpuvet[3] = vet->primeira->id;
								tarcpuvet[4] = vet->primeira->fimio;
								printf("jogou %d, tempo: %d, texec: %d, inicio %d, fimio: %d\n",vet->primeira->id, timer[0], vet->primeira->texec,vet->primeira->incio,vet->primeira->fimio);
							}
							else{
								sleep(1);
								timer[0]++;
							}
						}
						else{
							if(vet->primeira->chegada <= timer[0]){
								if(vet->primeira->fimio == -1) vet->primeira->fimio = 0;
								tarcpuvet[0] = vet->primeira->texec;
								tarcpuvet[1] = vet->primeira->incio;
								tarcpuvet[2] = 2;
								tarcpuvet[3] = vet->primeira->id;
								tarcpuvet[4] = vet->primeira->fimio;
								printf("jogou %d, tempo: %d, texec: %d, inicio %d, fimio: %d\n",vet->primeira->id, timer[0], vet->primeira->texec,vet->primeira->incio,vet->primeira->fimio);
							}
							else{
								int go = 1;
								tarefa *posatual = vet->primeira;
								while(go && posatual != sent){
									printf("procurando %d, cheg %d\n", posatual->id, posatual->chegada);
									if(posatual->chegada <= timer[0]){
										posatual->prox->prev = posatual->prev;
										posatual->prev->prox = posatual->prox;
										posatual->prox = vet->primeira;
										posatual->prev = sent;
										vet->primeira->prev = posatual;
										vet->primeira = posatual;
										tarcpuvet[0] = vet->primeira->texec;
										tarcpuvet[1] = vet->primeira->incio;
										tarcpuvet[2] = 2;
										tarcpuvet[3] = vet->primeira->id;
										tarcpuvet[4] = vet->primeira->fimio;
										printf("jogou %d, tempo: %d, texec: %d, inicio %d, fimio: %d\n",vet->primeira->id, timer[0], vet->primeira->texec,vet->primeira->incio,vet->primeira->fimio);
										go = 0;
									}
									else posatual = posatual->prox;
								}
								if(go == 0);
								else{
									sleep(1);
									timer[0]++;
									printf("nao achou ninguem\n");
								
								}
							}
						}
					}
					else{
						if(pilhaio->primeira == sent){
							kill(pid_filho,SIGTERM);
							finalizar(argv);
						}
						else{
							sleep(1);
							timer[0]++;
						}
					}
				}

				else{
					vet->primeira->texec--;
					vet->primeira->incio--;
					vet->primeira->fimio--;
					if(vet->primeira->texec == 0 && tarcpuvet[1] <= 0){
						tarefa *aux = vet->primeira;
						vet->primeira->prox->prev = sent;
						vet->primeira = vet->primeira->prox;
						terminar(encerrados, aux);
					}
					else if(vet->primeira->texec > 0 && tarcpuvet[1] == 0){
						tarefa *aux = vet->primeira;
						vet->primeira->prox->prev = sent;
						vet->primeira = vet->primeira->prox;
						empilhar(pilhaio, aux, 0);
					}
					else{
						if(vet->primeira != sent){
							if(vet->primeira->chegada <= timer[0]){
								tarefa *aux = vet->primeira;
								vet->primeira->prox->prev = sent;
								vet->primeira = vet->primeira->prox;
								aux->chegada = timer[0];
								inserirFCFSeRR(vet, aux);
								tarcpuvet[0] = vet->primeira->texec;
								tarcpuvet[1] = vet->primeira->incio;
								tarcpuvet[2] = 2;
								tarcpuvet[3] = vet->primeira->id;
								tarcpuvet[4] = vet->primeira->fimio;
							}
							else{
								sleep(1);
								timer[0]++;
							}
						}
						else{
						kill( pid_filho, SIGTERM );
						finalizar(argv);
						}
					}
				}
			}
		}
	}
}
