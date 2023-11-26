//Người viết lại thư viện AVLTree.h từ code đã viết có trong main: Nguyễn Hoàng Khải

//Hàm khai báo
//Người viết: Nguyễn Võ Hiệp

#define LH -1  	 /* Cay con trai cao hon */
#define EH 0	 /* Hai cay con bang nhau */
#define RH 1 

#ifndef AVLTREE_H
#define AVLTREE_H

typedef struct tudien {
	char tuAnh[20];
	char loaiTu[10];
	char nghia[20];
	char viDu[50];		
}TU;

struct AVLNode{
	char balFactor; //Chi so can bang
	TU Key;
	AVLNode* pLeft;
	AVLNode* pRight;
};
typedef AVLNode* AVLTree;

#endif


/*
Các hàm xoay cây
Người viết: Nguyễn Võ Hiệp
Ngày viết: 1/11/2023
*/

void rotateRR(AVLTree &T) {
	AVLNode* T1 = T->pRight;
	T->pRight = T1->pLeft;
	T1->pLeft = T;
	switch(T1->balFactor) {
        case RH: T->balFactor = EH;
        T1->balFactor= EH; 
        break;
        case EH: T->balFactor = RH; 
        T1->balFactor= LH; 
        break;
	}
	T = T1;
}

void rotateLL(AVLTree &T) {
	AVLNode* T1 = T->pLeft;
	T->pLeft = T1->pRight;
	T1->pRight = T;
	switch(T1->balFactor) {
        case LH: T->balFactor = EH; 
        T1->balFactor = EH; 
        break;
        case EH: T->balFactor = LH;
        T1->balFactor = RH; 
        break;
	}
	T = T1;
}

void rotateLR(AVLTree &T) { 
	AVLNode* T1 = T->pLeft;
	AVLNode* T2 = T1->pRight;
	T->pLeft = T2->pRight;
	T2->pRight = T;
	T1->pRight = T2->pLeft;
	T2->pLeft = T1;
	switch(T2->balFactor) {
		case LH: 
			T->balFactor = RH; 
			T1->balFactor = EH; 
			break;
		case EH: 
			T->balFactor = EH; 
			T1->balFactor = EH; 
			break;
		case RH: 
			T->balFactor = EH; 
			T1->balFactor = LH; 
			break;
	}
	T2->balFactor = EH;
	T = T2;
}

void rotateRL(AVLTree &T) { 
	AVLNode* T1 = T->pRight;
	AVLNode* T2 = T1->pLeft;
	T->pRight = T2->pLeft;
	T2->pLeft = T;
	T1->pLeft = T2->pRight;
	T2->pRight = T1;
	switch(T2->balFactor) {
		case RH: 
			T->balFactor = LH; 
			T1->balFactor = EH; 
			break;
		case EH: 
			T->balFactor = EH; 
			T1->balFactor = EH; 
			break;
		case LH: 
			T->balFactor = EH; 
			T1->balFactor = RH; 
			break;
	}
	T2->balFactor = EH;
	T = T2;
}

/*
Các hàm cân bằng cây
Người viết: Nguyễn Võ Hiệp
Ngày viết: 1/11/2023
*/
int balanceLeft(AVLTree &T) {
	AVLNode* T1 = T->pLeft;
	switch(T1->balFactor) {
		case LH: 
			rotateLL(T); return 2;
		case EH: 
			rotateLL(T); return 1;
		case RH: 
			rotateLR(T); return 2;
	}
	return 0;
}

int balanceRight(AVLTree &T ) {
	AVLNode* T1 = T->pRight;
	switch(T1->balFactor) {
		case LH: rotateRL(T); return 2;
		case EH: rotateRR(T); return 1;
		case RH: rotateRR(T); return 2;
	}
	return 0;
}

/*
Các hàm duyệt cây
Người viết: Nguyễn Gia Hào
Ngày viết: 1/11/2023
*/
void NLR(AVLTree T) {
	if(T != NULL) {
		printf("%s (%s): %s. \nExample: %s\n", T->Key.tuAnh, T->Key.loaiTu, T->Key.nghia, T->Key.viDu);
		NLR(T->pLeft);
		NLR(T->pRight);
	}
}

void LNR(AVLTree T){
	if(T != NULL){
		LNR(T->pLeft);
		printf("%s (%s): %s. \nExample: %s\n", T->Key.tuAnh, T->Key.loaiTu, T->Key.nghia, T->Key.viDu);
		LNR(T->pRight);
	}
}

void LRN(AVLTree T) {
	if(T != NULL) {
		LRN(T->pLeft);
		LRN(T->pRight);
		printf("%s (%s): %s. \nExample: %s\n", T->Key.tuAnh, T->Key.loaiTu, T->Key.nghia, T->Key.viDu);
	}
}

/*
Các hàm thêm, xóa, tìm kiếm
Người viết: Nguyễn Võ Hiệp
Ngày viết: 2/11/2023
*/

int insertNode(AVLTree &T, TU tu) { 
	int res;
	if (T) { 
		if(stricmp(T->Key.tuAnh, tu.tuAnh) == 0) return 0; //da co trong tu dien
		if(stricmp(T->Key.tuAnh, tu.tuAnh) > 0) { 
			res = insertNode(T->pLeft, tu);
			if(res < 2) return res;
			switch(T->balFactor) { 
				case RH: T->balFactor = EH; return 1;
				case EH: T->balFactor = LH; return 2;
				case LH: balanceLeft(T); return 1;
			}
		}
		else { 
			res = insertNode(T->pRight, tu);
			if(res < 2) return res;
			switch(T->balFactor) { 
				case LH: T->balFactor = EH; return 1;
				case EH: T->balFactor = RH; return 2;
				case RH: balanceRight(T); return 1;
			}
		}
	}
	T = new AVLNode;
	if(T == NULL) 
		return -1; //thieu bo nho 
	T->Key = tu; 
	T->balFactor = EH;
	T->pLeft = T->pRight = NULL;
	return 2; // thanh cong, chieu cao tang	
}

int searchStandFor(AVLTree &p, AVLTree &q) { 
	int res;
	if(q->pLeft) { 
		res = searchStandFor(p, q->pLeft);
		if(res < 2) return res;
		switch(q->balFactor) { 
			case LH: q->balFactor = EH; return 2;
			case EH: q->balFactor = RH; return 1;
			case RH: return balanceRight(q); 
		}
	} else { 
		p->Key = q->Key; 
		p = q; 
		q = q->pRight; 
		return 2;
	}
}

int delNode(AVLTree &T, char tuAnh[]) { 
	int res;
	if(T==NULL) 
		return 0;
	if(stricmp(T->Key.tuAnh, tuAnh) > 0) {
		res = delNode(T->pLeft, tuAnh);
		if(res < 2) return res;
		switch(T->balFactor) { 
			case LH: T->balFactor = EH; return 2;
			case EH: T->balFactor = RH; return 1;
			case RH: return balanceRight(T); 
		}
	}
	if(stricmp(T->Key.tuAnh, tuAnh) < 0) { 
		res = delNode(T->pRight, tuAnh);
		if(res < 2) return res;
		switch(T->balFactor) { 
			case RH: T->balFactor = EH; return 2;
			case EH: T->balFactor = LH; return 1;
			case LH: return balanceLeft(T); 
		}
	}
	else { 
		AVLNode* p = T;
		if(T->pLeft == NULL) { 
			T = T->pRight; 
			res = 2; 
		}
		else if(T->pRight == NULL) { 
			T = T->pLeft; 
			res = 2; 
		}
		else { 
			res = searchStandFor(p,T->pRight);
			if(res < 2) return res;
			switch(T->balFactor) { 
				case RH: T->balFactor = EH; return 2;
				case EH: T->balFactor = LH; return 1;
				case LH: return balanceLeft(T); 
			}
			delete p; 
			return res;
		}
	}
}

/*
Hàm tính chiều cao của cây
Người viết: Nguyễn Gia Hào
Ngày viết: 1/11/2023
*/
int tinhH(AVLTree T) {
	if(T == NULL) {
		return 0;
	}
	return tinhH(T->pLeft)>tinhH(T->pRight) ? tinhH(T->pLeft) + 1:tinhH(T->pRight) + 1;
}
