#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AVLTree.h"
#include <queue>


void themTu(AVLTree &T);
AVLNode* timKiem(AVLTree T, char tuAnh[]);
int suaTu(AVLTree &T, char tuAnh[]);
void docFile(AVLTree &T1, char filename[]);
void ghiFile(AVLTree T, char filename[]);
void ghi1TuMoiVaoFile(TU tu, char filename[]);
void xoaTu(AVLTree &T, char tuAnh[]);
void addHistorySearch(TU tu, std::queue<TU> &q);
void showHistorySearch(std::queue<TU> q);
int isAdmin(char pass[]);
int adminMenu();
int userMenu();
void admin(AVLTree &T);
void user(AVLTree &T);

int main() {
    int choice;
    char pass[10];
    AVLTree t = NULL;
    char filename[] = "tudien.txt";
    docFile(t, filename);
    system("pause");
    do {
        system("cls");
        printf("CHON CHE DO: \n");
        printf("1. Admin.\n"); //MK la 123456
        printf("2. User.\n");
        printf("0. Thoat.\n");
        printf("Nhap lua chon: ");scanf("%d", &choice);
        fflush(stdin);
    } while(choice < 0 || choice > 2);
    switch (choice)
    {
    case 1:
        printf("\nNhap mat khau: ");gets(pass);
        fflush(stdin);
        if(isAdmin(pass)) {
            system("cls");
            admin(t);
        } else {
            printf("Mat khau khong dung.\n");
            exit(0);
        }
        break;
    case 2:
        system("cls");
        user(t);
        break;
    default:
        break;
    }
    return 0;
}

void admin(AVLTree &T) {
    int choice;
    char tu[50], tuUp[50], tuDel[50];
    
    do {
        choice = adminMenu();
        fflush(stdin);
        switch (choice)
        {
        case 1:
            themTu(T);
            break;
        case 2:
            printf("Nhap tu can sua: ");gets(tuUp);
            fflush(stdin);
            if(suaTu(T, tuUp))
                printf("Sua thanh cong.\n");
            else if(suaTu(T, tuUp) == 0)
                printf("Tu khong ton tai.\n");
            else
                printf("Mang rong.\n");
                system("pause");
            break;
        case 3:
            fflush(stdin);
            printf("Nhap tu can xoa: ");gets(tuDel);
            xoaTu(T, tuDel);
            system("pause");
            break;
        case 4:
            NLR(T);
            system("pause");
            break;
        case 5: {
            fflush(stdin);
            printf("Nhap tu can tim kiem: ");gets(tu);
            AVLTree t1 = timKiem(T, tu);
            if(t1 == NULL)
                printf("Tu khong ton tai!!!\n");
            else {
                printf("%s (%s): %s. \nExample: %s\n", t1->Key.tuAnh, 
                t1->Key.loaiTu, t1->Key.nghia, t1->Key.viDu);
            }
            system("pause");
            break;
        }
        default:
            break;
        }
    } while(choice != 0);
}

void user(AVLTree &T) {
    int choice;
    char tu[50];
    std::queue<TU> q;
    system("cls");
    do {
        choice = userMenu();
        switch (choice)
        {
        case 1: {
            fflush(stdin);
            printf("Nhap tu can tim kiem: ");gets(tu);
            AVLNode* t1 = timKiem(T, tu);
            if(t1 == NULL)
                printf("Tu khong ton tai!!!\n");
            else {
                printf("%s (%s): %s. \nExample: %s\n", t1->Key.tuAnh, 
                t1->Key.loaiTu, t1->Key.nghia, t1->Key.viDu);
                addHistorySearch(t1->Key, q);
            }
            system("pause");
            break;
        }
        case 2: {
            NLR(T);
            system("pause");
            break;
        }
        case 3:
            showHistorySearch(q);
            system("pause");
            system("cls");  
            break;
        default:
            break;
        }
    } while(choice != 0);
}

void themTu(AVLTree &T) {
	TU tu;
	do {
		printf("Nhap tu can them: ");gets(tu.tuAnh);
		if(timKiem(T, tu.tuAnh) != NULL)
			printf("Tu nay da co trong tu dien. Vui long nhap lai!!!\n");
	} while(timKiem(T, tu.tuAnh) != NULL);
	printf("Nhap tu loai: ");gets(tu.loaiTu);
	printf("Nhap nghia cua tu can them: ");gets(tu.nghia);
	printf("Nhap vi du ve tu da them: ");gets(tu.viDu);
	if(insertNode(T, tu)) {
		printf("Them thanh cong.\n");
        ghi1TuMoiVaoFile(tu, "tudien.txt");
    }
	else
		printf("Khong du bo nho luu tru.\n");
}

AVLNode* timKiem(AVLTree T, char tuAnh[]) {
	if(T == NULL) {
		return NULL;
	}	
	if(stricmp(T->Key.tuAnh, tuAnh) == 0)
		return T;
	return (stricmp(T->Key.tuAnh, tuAnh) > 0)?
    timKiem(T->pLeft, tuAnh):timKiem(T->pRight, tuAnh);
}

void docFile(AVLTree &T1, char filename[]) {
	FILE *file = fopen(filename, "r"); 

    if (file == NULL) {
        perror("Khong the mo file.\n");
    }

    char line[200];
	TU tu; 

    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%49[^|]|%19[^|]|%49[^|]|%99[^\n]", 
                tu.tuAnh, tu.loaiTu, tu.nghia, tu.viDu) == 4) {
        	if(insertNode(T1, tu))
        		printf("Them thanh cong tu %s.\n", tu.tuAnh);
        	else
        		printf("Tu %s da co trong tu dien.\n", tu.tuAnh);
        } else {
            printf("Dong khong hop le: %s\n", line);
        }
    }

    fclose(file);
}

void ghiFile(AVLTree T, char filename[]) {
	FILE *f = fopen(filename, "a");
	if(f == NULL) {
		printf("Loi khong the doc file.\n");
		exit(0);
	}
	if(T != NULL) {
		fprintf(f, "%s|%s|%s|%s\n", T->Key.tuAnh, 
        T->Key.loaiTu, T->Key.nghia,T->Key.viDu);
		printf("Luu thanh cong tu vao file: %s.\n", T->Key.tuAnh);
		ghiFile(T->pLeft, filename);
		ghiFile(T->pRight, filename);
	}
	fclose(f);
}

void ghi1TuMoiVaoFile(TU tu, char filename[]) {
    FILE *f = fopen(filename, "a");
    if(f == NULL) {
        printf("Loi khong the doc file.\n");
        exit(0);
    }
    fprintf(f, "%s|%s|%s|%s\n", tu.tuAnh, tu.loaiTu, tu.nghia,tu.viDu);
    printf("Luu thanh cong tu vao file: %s.\n", tu.tuAnh);
    fclose(f);
}

void xoaTu(AVLTree &T, char tuAnh[]) {
    if(T == NULL) {
        printf("Tu dien rong.\n");
        return;
    }
    int tmp = delNode(T, tuAnh);
    if(tmp == 0)
        printf("Tu nay khong co trong tu dien.\n");
    else 
        printf("Xoa thanh cong.\n");
}

void addHistorySearch(TU tu, std::queue<TU> &q) {
    if(q.size() == 10) {
        q.pop();
    }
    q.push(tu);
}

void showHistorySearch(std::queue<TU> q) {
    if(q.empty()) {
        printf("Khong co tu trong lich su tim kiem.\n");
        return;
    }
    printf("Lich su tim kiem:\n");
    while(!q.empty()) {
        printf("%s (%s): %s. \nExample: %s\n", q.front().tuAnh, q.front().loaiTu, q.front().nghia, q.front().viDu);
        q.pop();
    }
}

int isAdmin(char pass[]) {
    if(strcmp(pass, "123456") == 0)
        return 1;
    return 0;
}

int adminMenu() {
    system("cls");
    printf("1. Them tu.\n");
    printf("2. Sua tu.\n");
    printf("3. Xoa tu.\n");
    printf("4. In tu dien.\n");
    printf("5. Tra tu.\n");
    printf("0. Thoat.\n");
    int choice;
    printf("Nhap lua chon: ");
    scanf("%d", &choice);
    fflush(stdin);
    return choice;
}

int userMenu() {
    system("cls");
    printf("1. Tra tu.\n");
    printf("2. In tu dien.\n");
    printf("3. Lich su tim kiem.\n");
    printf("0. Thoat.\n");
    int choice;
    printf("Nhap lua chon: ");
    scanf("%d", &choice);
    fflush(stdin);
    return choice;
}

int suaTu(AVLTree &T, char tuAnh[]) {
	if(T == NULL)
		return -1;
	AVLNode* T1 = timKiem(T, tuAnh);
	if(T1) {
		printf("Nhap tu loai: ");gets(T1->Key.loaiTu);
		printf("Nhap nghia cua tu: ");gets(T1->Key.nghia);
		printf("Nhap vi du ve tu: ");gets(T1->Key.viDu);
		return 1;
	}
	return 0;
}
