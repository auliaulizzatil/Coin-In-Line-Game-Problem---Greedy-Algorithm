#include <iostream>
#include <stdlib.h>
using namespace std;

typedef int infotype;
typedef struct TElmtList *address;
typedef struct TElmtList{
    infotype info;
    address next;
    address prev;
} ElmtList;

typedef struct {
    address first;
    address last;
}DList ;

 //SELEKTOR
#define Info(P) (P)->info
#define Next(P) (P)->next
#define Prev(P) (P)->prev
#define First(L) ((L).first)
#define Last(L) ((L).last)

//KONSTANTA
#define Nil NULL
#define Infinity 99999


/****** TEST LIST KOSONG ******/
bool IsEmpty(DList L) {
/* Mengirim true jika list kosong. Lihat definisi di atas. */
    return (First(L) == NULL && Last(L) == NULL);
}


/****** PEMBUATAN LIST KOSONG ******/
void createList(DList *L) {
/* I.S. L sembarang  */
/* F.S. Terbentuk list kosong. Lihat definisi di atas. */
    First(*L) = NULL;
    Last(*L) = NULL;
}


/****** Manajemen Memori ******/
address Allocation(infotype x) {
/* Mengirimkan address hasil alokasi sebuah elemen */
/* Jika alokasi berhasil, maka address tidak nil. */
/* Misalnya: menghasilkan P, maka Info(P)=X, Next(P)=Nil, Prev(P)=Nil */
/* Jika alokasi gagal, mengirimkan Nil. */
    address NewElmt;
    NewElmt = new ElmtList;//(ElmtList*) malloc (sizeof(ElmtList));
    Info(NewElmt) = x;
    Next(NewElmt) = Nil;
    Prev(NewElmt) = Nil;
    return NewElmt;
}

void Deallocation(address hapus) {
/* I.S. P terdefinisi */
/* F.S. P dikembalikan ke sistem */
/* Melakukan dealokasi/pengembalian address P */
    delete hapus;//free(hapus);
}


/****** PENCARIAN SEBUAH ELEMEN LIST ******/
address Search (DList L, infotype X){
/* Mencari apakah ada elemen list dengan Info(P)=X */
/* Jika ada, mengirimkan address elemen tersebut. */
/* Jika tidak ada, mengirimkan Nil */
    address P = First(L);
        if (Info(P)==X){
            return P;
        }
        else {
            P = Last(L);
            if (Info(P)==X){
                return P;
            }
        }

    return Nil;
}


/****** PRIMITIF BERDASARKAN NILAI ******/
/* PENAMBAHAN ELEMEN */
void InsertFirst(DList *L, infotype x) {
/* I.S. L mungkin kosong */
/* F.S. Melakukan alokasi sebuah elemen dan */
/* menambahkan elemen pertama dengan nilai X jika alokasi berhasil */
    address NewElmt;
    NewElmt = Allocation(x);
    if (NewElmt != NULL) {
        if(IsEmpty(*L)){
            First(*L) = NewElmt;
            Last(*L) = NewElmt;
        } else{
            Next(NewElmt) = First(*L);
            Prev(First(*L)) = NewElmt;
            First(*L) = NewElmt;
        }
    }
}

void InsertLast(DList *L, infotype x) {
/* I.S. L mungkin kosong */
/* F.S. Melakukan alokasi sebuah elemen dan */
/* menambahkan elemen list di akhir: elemen terakhir yang baru */
/* bernilai X jika alokasi berhasil. Jika alokasi gagal: I.S.= F.S. */
    address NewElmt;
    NewElmt = Allocation(x);
    if (NewElmt != NULL) {
        if(IsEmpty(*L)){
            InsertFirst(L,x);
        } else{
            Next(Last(*L)) = NewElmt;
            Prev(NewElmt) = Last(*L);
            Last(*L) = NewElmt;
        }
    }
}


/****** PRIMITIF BERDASARKAN ALAMAT ******/
/* PENAMBAHAN ELEMEN BERDASARKAN ALAMAT */
void InsertFirstA (DList *L, address P){
/* I.S. Sembarang, P sudah dialokasi  */
/* F.S. Menambahkan elemen ber-address P sebagai elemen pertama */
    if(IsEmpty(*L)){
        First(*L) = P;
        Last(*L) = P;
    } else{
        Next(P) = First(*L);
        Prev(First(*L)) = P;
        First(*L) = P;
    }
}

void InsertLastA (DList *L, address P){
/* I.S. Sembarang, P sudah dialokasi  */
/* F.S. P ditambahkan sebagai elemen terakhir yang baru */
    if(IsEmpty(*L)){
        InsertFirstA(L,P);
    } else{
        Next(Last(*L)) = P;
        Prev(P) = Last(*L);
        Last(*L) = P;
    }
}


/* PENGHAPUSAN SEBUAH ELEMEN */

void DelFirst (DList *L, address *P){
/* I.S. DList tidak kosong */
/* F.S. P adalah alamat elemen pertama list sebelum penghapusan */
/*      Elemen list berkurang satu (mungkin menjadi kosong) */
/* First element yg baru adalah suksesor elemen pertama yang lama */
    *P = First(*L);
    First(*L) = Next(*P);
    if(First(*L) != NULL) Prev(First(*L)) = Nil;
    else L->last = NULL;
    Deallocation(*P);
}

void DelLast (DList *L, address *P){
/* I.S. DList tidak kosong */
/* F.S. P adalah alamat elemen terakhir list sebelum penghapusan  */
/*      Elemen list berkurang satu (mungkin menjadi kosong) */
/* Last element baru adalah predesesor elemen pertama yg lama, jika ada */
    *P = Last(*L);
    Last(*L) = Prev(*P);
    Next(Last(*L)) = Nil;
    Prev(*P) = Nil;
    Deallocation(*P);
}

void DelAfter (DList *L, address *Pdel, address Prec){
/* I.S. DList tidak kosong. Prec adalah anggota list. */
/* F.S. Menghapus Next(Prec): */
/*      Pdel adalah alamat elemen list yang dihapus  */
    *Pdel = Next(Prec);
    Next(Prec) = Next(*Pdel);
    Prev(Next(*Pdel)) = Prec;
    Prev(*Pdel) = Nil;
    Next(*Pdel)= Nil;
    Deallocation(*Pdel);
}


bool DelP (DList *L, DList *A, infotype X){
/* I.S. Sembarang */
/* F.S. Jika ada elemen list beraddress P, dengan Info(P)=X  */
/* maka P dihapus dari list dan didealokasi */
/* Jika tidak ada elemen list dengan Info(P)=X, maka list tetap */
/* DList mungkin menjadi kosong karena penghapusan */
    address P = Search(*L,X);
    if (P != Nil){
            InsertLast(A, X);
            if (P == L->first){
                DelFirst(L, &P);
            }
            else {
                DelLast(L, &P);
            }
            return true;
        }

    else {
        return false;
    }
}

int start (DList *L){
    int odd, even;
    address P = (L)->first;
    while (P != Nil){
        odd= odd + P->info;
        P = Next(P);
            even= even + P->info;
        P = Next(P);
    }

    if(odd<even){
        return L->first->info;
    }
    else if (even<odd){
        return L->last->info;
    }

}

int cek(DList *L, infotype x){
    if(L->first->next != Nil && L->last->prev != Nil){

    if (x==L->first->info){
        return L->first->next->info;
    }
    else if (x==L->last->info){
        return L->last->prev->info;
    }
}
}

int ceksum (DList *L){
    int sum;
    if(!IsEmpty(*L)){

    address P = (L)->first;
    while (P != Nil){
        sum= sum + Info(P);
        P = Next(P);
    }
}
cout<<sum;
return sum;

}

/****** PROSES SEMUA ELEMEN LIST ******/
void PrintForward (DList L){
/* I.S. DList mungkin kosong */
/* F.S. Jika list tidak kosong, isi list dicetak dari elemen pertama */
/* ke elemen terakhir secara horizontal ke kanan: [e1,e2,...,en] */
/* Contoh : jika ada tiga elemen bernilai 1, 20, 30 akan dicetak: [1,20,30] */
/* Jika list kosong : menulis [] */
/* Tidak ada tambahan karakter apa pun di awal, akhir, atau di tengah */
    if(!IsEmpty(L)){
        address temp =First(L);
        while(temp!=NULL){
            cout<<Info(temp)<<" ";
            temp = Next(temp);
        }
    }
    else{
        cout<<"[Belum ada data yang bisa ditampilkan]\n";
    }
}

void PrintBackward (DList L){
/* I.S. DList mungkin kosong */
/* F.S. Jika list tidak kosong, isi list dicetak dari elemen terakhir */
/*      ke elemen pertama secara horizontal ke kanan: [en,en-1,...,e2,e1] */
/*      Contoh : jika ada tiga elemen bernilai 1, 20, 30 akan dicetak: [30,20,1] */
/* Jika list kosong : menulis [] */
/* Tidak ada tambahan karakter apa pun di awal, akhir, atau di tengah */
    if(!IsEmpty(L)){
        address temp =Last(L);
        while(temp!=NULL){
            cout<<Info(temp)<<endl;
            temp = Prev(temp);
        }
    }
    else{
        cout<<"[Belum ada data yang bisa ditampilkan] ";
    }
}

int main(){
    infotype y;
    address temp;
    DList koin;
    DList pemain;
    DList ai;
    createList(&koin);
    createList(&pemain);
    createList(&ai);
    int n;

    cout<<"======     GAME     ======\n";
    cout<<"====== COIN IN LINE ======\n\n";
	cout<<"The number of coins: ";
    cin>>n;
    int i = 0;
    while ( i<n ) {
        InsertLast(&koin, rand()%11);
        i++;
    }

    int pilih;
    int pilih_ai = start(&koin);
    int score_ai, score_p;
    while(!IsEmpty(koin)){

        system("cls");

            cout<<"\nYou : ";
            PrintForward(pemain);
            cout<<" = ";
            //ceksum(&pemain);
            score_p=ceksum(&pemain);
            cout<<"\nComputer : ";
            DelP(&koin, &ai, pilih_ai);
            PrintForward(ai);
            cout<<" = ";
            //ceksum(&ai);
            score_ai=ceksum(&ai);
            cout<<endl<<endl;
            PrintForward(koin);
            cout<<endl<<endl;

        cout<<"You choose coins : ";
        cin>>pilih;
        pilih_ai = cek(&koin, pilih);
        while( !(DelP(&koin, &pemain, pilih)) ){
            cout<<"\nSelect coins that are on the edge! : ";
            cin>>pilih;

        };
    }

system("cls");

    cout<<"LAST SCORE : ";
    cout<<"\nYou : ";
            PrintForward(pemain);
            cout<<" = ";
            ceksum(&pemain);
    cout<<"\nComputer : ";
            PrintForward(ai);
            //score_ai=ceksum(&ai);
            cout<<" = "<<score_ai;
            cout<<endl<<endl;

    if (score_ai > score_p){
        cout<<"========================";
		cout<<"\nGame Over!";
        cout<<"\nComputer Wins!!!"<<endl;
    }
}

