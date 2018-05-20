/*
 *	~  Gerenc. de Processos ~
 *
 *	Matheus Carvalho Raimundo
 *	mcarvalhor@usp.br
 *	 _______ _______ _______
 *	|   |   |               \
 *	|   |   |      \    |___|
 *	|   |    \      |   |
 *	|_______ _______|___|
 *
*/

#ifndef __PROCESSMAN_H_
#define __PROCESSMAN_H_

	typedef struct __processlist_t PROC_LIST;
	enum __proc_man_t { proc_fifo = 0, proc_round_robin };

	struct __processlist_t *NewSession(enum __proc_man_t);

	char IncludeProcessesFrom(FILE *,struct __processlist_t *);

	char RunProcesses(struct __processlist_t *);

	char PrintResults(FILE *,struct __processlist_t *);

	char DestroySession(struct __processlist_t *);

#endif
