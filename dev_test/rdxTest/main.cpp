#include <cstdio>
#include <iostream>
#include <cstring>
#include "adlist.h"
#include "zmalloc.h"


#define  LOOP_CNT    100

typedef struct student {
	char	name_[30];
	char	school_[30];
	int		age_;
} tagST;

int main() {
	list *pt_lst = listCreate();
	if (!pt_lst) {
		return -1;
	}
	listSetFreeMethod(pt_lst, free);
	for (int i = 0; i < LOOP_CNT; i++) {
		tagST *pSt = (tagST *)malloc(sizeof(*pSt));
		if (!pSt) {
			return -1;
		}
		std::strcpy(pSt->name_, "Stephen");
		std::strcpy(pSt->school_, "Tong Ji University");
		pSt->age_ = i;
		listAddNodeTail(pt_lst, pSt);
		pSt = NULL;
	}

	listNode *pShow = NULL;
	listIter *lstIter = listGetIterator(pt_lst, AL_START_HEAD);
	if (!lstIter) {
		return -1;
	}
	for (int i = 0; i < LOOP_CNT; i++) {
		pShow = listNext(lstIter);
		std::cout << ((tagST *)pShow->value)->age_ << ", " << ((tagST *)pShow->value)->name_
			<< ", " << ((tagST *)pShow->value)->school_ << std::endl;
		pShow = NULL;
	}
	if (lstIter) {
		listReleaseIterator(lstIter);
		lstIter = NULL;
		std::cout << "delete iter" << std::endl;
	}
	if (pt_lst) {
		listRelease(pt_lst);
		pt_lst = NULL;
		std::cout << "delete list" << std::endl;
	}

	std::cout << "press enter to exit..." << std::endl;
	std::cin.get();
	return 0;
}
