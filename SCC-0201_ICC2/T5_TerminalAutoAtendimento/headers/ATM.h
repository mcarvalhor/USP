
/*
 *	~          ATM          ~
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

#ifndef ATM_H_
#define ATM_H_
#include <hashtable.h>


	enum { audit_transfer = 0, audit_withdraw, audit_deposit };

	typedef struct {
		int Bank1, Bank2;
		double Money;
	} OPERATION;

	typedef struct {
		int Bank;
		double DepositedMoney, WithdrawnMoney;
		double TransfersTo, TransfersFrom;
	} BANK;


	int ReadOperation(char *, int *, int *, int *, int *, double *);

	int Withdraw(double, int, HASHTABLE *);

	int Deposit(double, int, HASHTABLE *);

	int TransferFrom(double, int, HASHTABLE *);

	int TransferTo(double, int, HASHTABLE *);

	int AddAudit(int, char, int, int, double, HASHTABLE *);

	int PrintATM(HASHTABLE *);

	int PrintAudit(int, char, unsigned long, HASHTABLE *);


#endif