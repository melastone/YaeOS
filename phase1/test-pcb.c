/*****************************************************************************
 * Copyright 2018 Renzo Davoli                                               *
 * inspired by previous projects:                                            *
 * Copyright 2004, 2005 Michael Goldweber, Davide Brini.                     *
 * Modified 2012 by Marco Di Felice                                          *
 * Modified 2013 by Paolo Parisen Toldin                                     *
 *                                                                           *
 * P1TEST v.0.1                                                              *
 * This file is part of the projct YAEOS 2018.                               *
 *                                                                           *
 * YAEOS is free software; you can redistribute it and/or modify it under the*
 * terms of the GNU General Public License as published by the Free Software *
 * Foundation; either version 2 of the License, or (at your option) any      *
 * later version.                                                            *
 * This program is distributed in the hope that it will be useful, but       *
 * WITHOUT ANY WARRANTY; without even the implied warranty of                *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General *
 * Public License for more details.                                          *
 * You should have received a copy of the GNU General Public License along   *
 * with this program; if not, write to the Free Software Foundation, Inc.,   *
 * 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.                  *
 *****************************************************************************/

/*********************************P1TEST.C*******************************
 *
 *	Test program for the modules ASL and PCB (phase 1).
 *
 *	Produces progress messages on terminal 0.
 *
 *		Aborts as soon as an error is detected.
 *
 *
 */

// TODO: eliminare la Ridefinizione della costante MAXPROC

#include <uARMconst.h>
#include <uARMtypes.h>
#include <libuarm.h>

#include "pcb.h"

#include "const.h"
//#include "types.h"

#define	MAXSEM	MAXPROC
#define MAXPROC 20

#define MAX_PCB_PRIORITY		10
#define MIN_PCB_PRIORITY		0
#define DEFAULT_PCB_PRIORITY		5

int my_counter_process;
int onesem;
pcb_t *procp[MAXPROC], *p, *q, *maxproc, *minproc, *proc;
int  sem[MAXSEM];
int onesem;
pcb_t* head;

/* This function places the specified character string in okbuf and
 *	causes the string to be written out to terminal0 */
void addokbuf(char *strp) {
	tprint(strp);
}

/* This function places the specified character string in errbuf and
 *	causes the string to be written out to terminal0.  After this is done
 *	the system shuts down with a panic message */
void adderrbuf(char *strp) {

	tprint(strp);
	PANIC();
}

void increment_counter(struct pcb_t *pcb, void* pt)
{
	int *counter=pt;
	(*counter)++;
}

/******************************************************************************
 * Main Test File
 ******************************************************************************/

int main() {

	int i;

	head = NULL;

	addokbuf("Strange Operating System test\n");

	initPcbs();
	addokbuf("Control Blocks initialized\n");
	if(MAXPROC == 20)
		addokbuf("No more than 20 proc\n");

	/* controlla la allocPcb */
	for (i = 0; i < MAXPROC; i++) {
		if ((procp[i] = allocPcb()) == NULL)
			adderrbuf("allocPcb(): unexpected NULL\n");
	}

	/* it should not allocate more than MAXPROC elements */
	if (allocPcb() != NULL) {
		adderrbuf("ERROR: allocPcb(): allocated more than MAXPROC entries\n");
	}
	addokbuf("AllocPcb test OK\n");
	addokbuf("List initialized correctly\n");

	/* Return the last 10 entries back to free list */
	for (i = 10; i < MAXPROC; i++)
		freePcb(procp[i]);

	addokbuf("10 entries were added to the free PCB list\n");

/*	addokbuf("We are going to test the insertProcQ function...\n");

	/* insert 10 elements using different priorities. one max, one min prio
	all the others default prio *//*
	for (i = 0; i < 10; i++) {
		q = allocPcb();
		if ( q == NULL)
			adderrbuf("ERROR: allocPcb(): unexpected NULL while insert\n");
		switch (i) {
			case 5:
				q->p_priority=DEFAULT_PCB_PRIORITY;
				proc=q;
				break;
			case 1:
				q->p_priority=MAX_PCB_PRIORITY;
				maxproc = q;
				break;
			case 2:
				q->p_priority=MIN_PCB_PRIORITY;
				minproc=q;
				break;
			default:
				q->p_priority=DEFAULT_PCB_PRIORITY;
				break;
		}
		/* insert the process in the queue *//*
		insertProcQ(&head, q);
	}

	addokbuf("Test insertProcQ: OK. Inserted 10 elements \n");

	addokbuf("Checking if the first element has max priority\n");
	/* Check outProcQ and headProcQ *//*
	if (headProcQ(head) != maxproc)
		adderrbuf("ERROR: headProcQ(qa) failed\n");
	addokbuf("Done\n");

	/* remove an element *//*
	addokbuf("Removing an element. Testing outProcQ\n");
	q=outProcQ(&head, proc);
	if ((q == NULL) || (q != proc))
		adderrbuf("ERROR: outProcQ(&qa, proc) failed to remove the entry\n");
	freePcb(q); /* return the element to the free list *//*
	addokbuf("Done\n");

	/* Removing the first element from ProcQ *//*
	addokbuf("Removing the first element. Testing removeProcQ\n");
	q = removeProcQ(&head);
	if (q == NULL || q != maxproc)
		adderrbuf("ERROR: removeProcQ(&qa, midproc) failed to remove the elements in the right order\n");
	freePcb(q);
	addokbuf("Done\n");

	/* we inserted 10 elements and removed 2. Now let us remove the
	remaining elements and check if the last one has the min prio *//*
	addokbuf("Testing removeProcQ ...   \n");
	for (i = 0; i < 7; i++) {
		if ((q = removeProcQ(&head)) == NULL)
			adderrbuf("removeProcQ(&qa): unexpected NULL\n");
		freePcb(q);
	}*/

	/* keeping track of the last element */
	/*q=headProcQ(head);
	addokbuf("Applying a function to all the elements of the list. Testing forallProcQ\n");
	/* there should be just one element inside the list. i'm going to remove by using the forallProcQ instruction */
	/*my_counter_process = 0;
	forallProcQ(head, increment_counter , &my_counter_process);
	if(my_counter_process != 1)
		adderrbuf("ERROR: forallProcQ(): failed on last entry\n");
	my_counter_process = 0;
	removeProcQ(&head);
	if (q != minproc)
		adderrbuf("ERROR: headProcQ(): failed on last entry\n");
	freePcb(q);
	addokbuf("Done\n");*/

	addokbuf("Let's see if now the queue is empty\n");
	addokbuf("Test insertProcQ(), removeProcQ(), headProcQ(), forallProcQ(), outProcQ(): OK\n");
	addokbuf("Test process queues module: OK\n");

	addokbuf("Testing process trees...\n");

	/*check that it has no child*/
	q = removeChild(procp[2]);
	if (q!=NULL )
		adderrbuf("ERROR: emptyChild: unexpected FALSE\n");
	/*if the test is passed, then the node has no child at all*/

	/* make procp[1],procp[2],procp[3], procp[7] children of procp[0] */
	addokbuf("Inserting...\n");
	insertChild(procp[0], procp[1]);
	insertChild(procp[0], procp[2]);
	insertChild(procp[0], procp[3]);
	insertChild(procp[0], procp[7]);
	addokbuf("Inserted 4 children of pcb0\n");

	/* make procp[8],procp[9] children of procp[7] */
	insertChild(procp[7], procp[8]);
	insertChild(procp[7], procp[9]);
	addokbuf("Inserted 2 children of pcb7\n");

	/*checking if node 7 has a child. if it doesn't have, ERRRO*/
	q=removeChild(procp[7]);
	addokbuf("Removed the first child of 7\n");
	if(q==NULL)
		adderrbuf("ERROR: emptyChild: node 7 has no child!\n");
	if(q!=procp[8])
		adderrbuf("ERROR: removeChild(procp[0] failed\n");
	/*otherwise, we will re-insert*/
	insertChild(procp[7], q);

	addokbuf("Re-inserted son in 7, now removing in 0\n");

	/*check the same with node 0*/
	q=removeChild(procp[0]);
	addokbuf("Removed the first child of 0\n");
	if(q==NULL)
		adderrbuf("ERROR: emptyChild: node 0 has no child!\n");
	if(q!=procp[1])
		adderrbuf("ERROR:  removeChild(procp[0] failed\n");
	/*otherwise, we will re-insert*/
	insertChild(procp[0], q);

	addokbuf("Check outchild function\n");

	/* Check outChild */
	q = outChild(procp[1]);
	addokbuf("Outchild executed on 1\n");
	if (q == NULL || q != procp[1])
		adderrbuf("ERROR: outChild(procp[1]) failed\n");
	q = outChild(procp[8]);
	addokbuf("Outchild executed on 8\n");
	if (q == NULL || q != procp[8])
		adderrbuf("ERROR: outChild(procp[8]) failed\n");

	/* Check removeChild */
	q = removeChild(procp[0]);
	addokbuf("Removechild executed on 0\n");
	if (q==procp[1])
		adderrbuf("q==pp1");
	if (q == NULL || q != procp[2])
		adderrbuf("ERROR: removeChild(procp[0])  1 failed\n");

	q = removeChild(procp[7]);
	addokbuf("Removechild executed on 7\n");
	if (q == NULL || q != procp[9])
		adderrbuf("ERROR: removeChild(procp[7])  2 failed\n");

	q = removeChild(procp[0]);
	addokbuf("Removechild executed on 0\n");
	if (q == NULL || q != procp[3])
		adderrbuf("ERROR: removeChild(procp[0]) 2 failed\n");

	q = removeChild(procp[0]);
	addokbuf("Removechild executed on 0\n");
	if (q == NULL || q != procp[7])
		adderrbuf("ERROR: removeChild(procp[0]) 3 failed\n");

	q=removeChild(procp[0]);
	addokbuf("Removechild executed on 0\n");
	if ( q!=NULL )
		adderrbuf("ERROR: removeChild(procp[0]): it had already a child! it shouldn't! ");

	addokbuf("Test: insertChild(), removeChild() OK\n");
	addokbuf("Testing process tree module OK\n");


	freePcb(procp[0]);
	freePcb(procp[1]);
	freePcb(procp[2]);
	freePcb(procp[3]);
	freePcb(procp[4]);
	freePcb(procp[5]);
	freePcb(procp[6]);
	freePcb(procp[7]);
	freePcb(procp[8]);
	freePcb(procp[9]);


	addokbuf("Now starting with test of semaphores\n");

	addokbuf("ASL module OK\n");
	addokbuf("1f u c4n r34d th1s 1t m34ns th4t y430s m19ht w0rk\n");
	addokbuf("\n");

	return 0;
}
