#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <mylib.h>
#include <string>
#include <iostream>
#include <fstream>

#define MAXLOPTINCHI 100000

using namespace std;

//=============================== DS MON HOC CAY NHI PHAN TIM KIEM =====================
struct MONHOC{
	char MAMH[10];
	char TENMN[35];
	int STCLT;
	int STCTH; 
};

struct nodeMH{
	MONHOC mh;
	nodeMH *left;
	nodeMH *right;
};

typedef nodeMH *PTRMH;


//=============================== SINH VIEN DS LIEN KET DON ============================

struct SinhVien{
	char MASV[15];
	char HO[30];
	char TEN[10];
	char SDT[12];
	char PHAI[4];
	char MALOP[15];
};

struct nodeSV {
	SinhVien sv;
	nodeSV* next;
};

typedef nodeSV* PTRSV;
        
//============================ DANG KY DS LIEN KET DON =======================
struct DangKy {
	char MASV[15];
	float DIEM;
	boolean HUYDANGKY = false;	
};

struct nodeDangKy {
	DangKy dangky;
	nodeDangKy* next;
};

typedef nodeDangKy* PTRDK;


// LOP TIN CHI DS TUYEN TINH MANG CON TRO

struct LopTinChi{
	int MALOPTC;
	char MAMH[10];
	char NIENKHOA[10];
	int HOCKY;
	int NHOM;
	int SOSVMIN;
	int SOSVMAX;
	
	boolean HUYLOP = false;
	PTRDK First_DSSVDK = NULL;
};

struct LIST_LTC {
	int soluong = 0;
	LopTinChi *nodesltc[MAXLOPTINCHI];
};

// =========================== END KHAI BAO ========================

int XacNhan (char *S){
	cout<<S;
	char kt;
	do{
		kt=toupper(getch());
	}while(kt!='Y'&&kt!='N');
	cout<<kt;
	return kt=='Y';
}



//==========================MON HOC=================

// tim kiem mon hoc 
PTRMH Search_MonHoc(PTRMH root, char *x)
{
   PTRMH p;
   p = root;
   while(p != NULL && stricmp(p->mh.MAMH,x)!=0)
     if(stricmp(p->mh.MAMH,x)<0)
         p = p->left;
      else
         p = p->right;
return(p); 
}

// them node mon hoc vao cay nhi phan
void Insert_MonHoc(PTRMH &p, MONHOC mh)
{ 
   if(p == NULL)    // nut p hien tai se la nut la
   {
     p = new nodeMH;
     p->mh = mh;
     p->left = NULL;     p->right = NULL;  
   }
   else
    if(stricmp(p->mh.MAMH,mh.MAMH)<0)
    	Insert_MonHoc(p->left, mh);
    else if(stricmp(p->mh.MAMH,mh.MAMH)>0)
		Insert_MonHoc(p->right, mh);
}

// Tao cay mon hoc 
void Create_MONHOC(PTRMH &root)
{ MONHOC mh;
	PTRMH temp;
  	while(1){
	 
  	fflush(stdin);
  	cout<<"Nhap ma mon hoc: ";
  	gets(mh.MAMH);
  	if(strcmp(mh.MAMH,"0")==0) break;
  	temp=Search_MonHoc(root,mh.MAMH);
  	if(temp!=NULL) {
  		cout<<"Ma bi trung\n";
  		continue;
	  }
  	cout<<"Nhap ten mon hoc: ";
  	gets(mh.TENMN);
  	cout<<"Nhap so tin chi ly thuyet: ";
    cin >> mh.STCLT;
    cout<<"Nhap so tin chi thuc hanh: ";
    cin >> mh.STCTH;
        
    Insert_MonHoc(root,mh);
     
    }
}


//Show NLR
void Show_MonHoc(PTRMH root)
{

   if (root != NULL) 
   {
      printf("\n   %10s  %-15s %-8d %8d"   , root->mh.MAMH, root->mh.TENMN,root->mh.STCLT,root->mh.STCTH);
      Show_MonHoc(root->left);
      Show_MonHoc(root->right);
   }
}

// dem tong so nut cua cay mon hoc
void SoNut_MonHoc(PTRMH &root, int &sl)
{

   if (root != NULL) 
   {
      sl++;
      SoNut_MonHoc(root->left,sl);
      SoNut_MonHoc(root->right,sl);
   }
}

// duyet cay ghi vao file 
void traverseWriteToFile(PTRMH root,ofstream &outfile){
	MONHOC mh;
	if(root != NULL){
		outfile<<root->mh.MAMH<<endl;
		outfile<<root->mh.TENMN<<endl;
		outfile<<root->mh.STCLT<<endl;
		outfile<<root->mh.STCTH<<endl;
		traverseWriteToFile(root->left, outfile);
		traverseWriteToFile(root->right, outfile);
	}
}

// ghi file mon hoc
void ghiFileMonHoc(PTRMH &root){
	ofstream outfile;
	outfile.open("MonHoc.txt");
	int slmh =0;
	SoNut_MonHoc(root,slmh);
	outfile<<slmh<<endl;
	traverseWriteToFile(root,outfile);
	outfile.close();
}

// doc file mon hoc 
void docFileMonHoc(PTRMH &root){
	MONHOC mh;
	ifstream filein;
	root = NULL;
	filein.open("MonHoc.txt",ios::in);
	int slmh;
	filein>>slmh;
	filein.ignore();
	for(int i =0; i<slmh; i++){
		filein.getline(mh.MAMH , 10);
		filein.getline(mh.TENMN,35);
		filein>>mh.STCLT;
		filein.ignore();
		filein>>mh.STCTH;
		filein.ignore();
		cout<<"Doc file thanh cong"<<mh.MAMH;
		getch(); 
		Insert_MonHoc(root, mh);	
	}
	filein.close();
	
}

//==========================LOP TIN CHI========================


// ma LTC tu dong tang 
int maTuDongTang(LIST_LTC &list_ltc){
	if(list_ltc.soluong==0) return 1; 
	return list_ltc.nodesltc[list_ltc.soluong-1]->MALOPTC+1;
}

// search ltc theo ma
int LTC_Search_theoma(LIST_LTC list_ltc, int x)
{
	for ( int i =0 ; i <list_ltc.soluong ; i++)
  	     if (list_ltc.nodesltc[i]->MALOPTC == x) return i;
	return -1;
}


// tao lop tin chi
int Create_LTC(LIST_LTC &list_ltc, LopTinChi &ltc){	
	cout<<"Nhap thong tin lop tin chi\n";
	ltc.MALOPTC=maTuDongTang(list_ltc); fflush(stdin);
	cout<<"Nhap Ma mon hoc: ";
	gets(ltc.MAMH);
	if(stricmp(ltc.MAMH,"0")==0) return 0;
	cout<<"Nhap Nien khoa: "; fflush(stdin);
	gets(ltc.NIENKHOA);
	cout<<"Nhap Hoc ki: ";
	cin>>ltc.HOCKY;
	cout<<"Nhap Nhom: ";
	cin>>ltc.NHOM;
	cout<<"Nhap so sinh vien min: ";
	cin>>ltc.SOSVMIN;
	cout<<"Nhap so sinh vien max: ";
	cin>>ltc.SOSVMAX;
	fflush(stdin);
	return 1;
}

// tao danh sach lop tin chi
// them lop tin chi
void Create_List_LTC(LIST_LTC &list_ltc){
	LopTinChi ltc;
	
	while(list_ltc.soluong<MAXLOPTINCHI){
		if (Create_LTC(list_ltc, ltc)==0)    
		   return;
		list_ltc.nodesltc[list_ltc.soluong] = new LopTinChi;
		//gan gia tri lop tin chi them vao vao con tro lop tin chi cuoi mang
		*list_ltc.nodesltc[list_ltc.soluong] = ltc;
		list_ltc.soluong +=1;
	}
}

// duyet danh sach lop tin chi 
void duyetDS_LTC(LIST_LTC &list_ltc){
	printf ("                     DANH SACH LOP TIN CHI \n");
 	printf ("  Stt   Ma lop   Ma mon   Nhom   Nien khoa     Hoc Ky    So sv max     So sv min   \n");
	for (int i =0 ; i < list_ltc.soluong ; i++)
  		printf ("%8d %5d  %-10s  %-8d  %10s  %5d  %5d   %5d    \n", 
		  i+1,
		  list_ltc.nodesltc[i]->MALOPTC,
		  list_ltc.nodesltc[i]->MAMH,
		  list_ltc.nodesltc[i]->NHOM,
		  list_ltc.nodesltc[i]->NIENKHOA,
		  list_ltc.nodesltc[i]->HOCKY,
		  list_ltc.nodesltc[i]->SOSVMAX,
		  list_ltc.nodesltc[i]->SOSVMIN);
 	getch();
}

int LTC_Empty(LIST_LTC &list_ltc)
{     return(list_ltc.soluong == 0 ? 1 : 0);
}

// xoa LTC theo ma
void LTC_Delete_theoma (LIST_LTC &list_ltc)
{	int i;
	cout<<"Nhap ma lop tin chi muon xoa: ";
	cin>>i;
	int j;
	int temp;
	if(i <= 0 || i > list_ltc.soluong)
		printf("Vi tri xoa khong phu hop.");
	else
		if(LTC_Empty(list_ltc))
			printf("Danh sach khong co phan tu.");
		else
		{
			for(j = i;  j< list_ltc.soluong ; j++)
				list_ltc.nodesltc[j-1] = list_ltc.nodesltc[j];
			list_ltc.soluong--;
		}
}

//
int LTC_Sua_theoma(LIST_LTC &list_ltc){
	LopTinChi ltc;
	int maltc;
	cout<<"Nhap ma lop tin chi muon sua: ";
	cin>>maltc;
	int vitri=LTC_Search_theoma(list_ltc,maltc);
	if(vitri==-1) cout<<"Khong tim thay.";
	else{
		printf ("   Ma lop   Ma mon   Nhom   Nien khoa     Hoc Ky    So sv max     So sv min   \n");
  		printf (" %5d  %-10s  %-8d  %10s  %5d  %5d   %5d    \n", 
	  		list_ltc.nodesltc[vitri]->MALOPTC,
	  		list_ltc.nodesltc[vitri]->MAMH,
	  		list_ltc.nodesltc[vitri]->NHOM,
	  		list_ltc.nodesltc[vitri]->NIENKHOA,
	  		list_ltc.nodesltc[vitri]->HOCKY,
	  		list_ltc.nodesltc[vitri]->SOSVMAX,
	  		list_ltc.nodesltc[vitri]->SOSVMIN);
	  	cout<<"Nhap Ma mon hoc: ";	fflush(stdin);
		gets(ltc.MAMH);
		//if(stricmp(ltc.MAMH,"0")==0)	ltc.MAMH = list_ltc.nodesltc[vitri]->MAMH;
		//=============can bat loi nhap ma mon hoc =========//
	  	cout<<"Nhap Nien khoa: "; fflush(stdin);
		gets(ltc.NIENKHOA);
		//if(stricmp(ltc.NIENKHOA,"0")==0)	ltc.NIENKHOA = list_ltc.nodesltc[vitri]->NIENKHOA;
		cout<<"Nhap Hoc ki: ";
		cin>>ltc.HOCKY;
		if(ltc.HOCKY==0)	ltc.HOCKY = list_ltc.nodesltc[vitri]->HOCKY;
		cout<<"Nhap Nhom: ";
		cin>>ltc.NHOM;
		if(ltc.NHOM==0)	ltc.NHOM = list_ltc.nodesltc[vitri]->NHOM;
		cout<<"Nhap so sinh vien min: ";
		cin>>ltc.SOSVMIN;
		if(ltc.SOSVMIN==0)	ltc.SOSVMIN = list_ltc.nodesltc[vitri]->SOSVMIN;
		cout<<"Nhap so sinh vien max: ";
		cin>>ltc.SOSVMAX;
		if(ltc.SOSVMAX==0)	ltc.SOSVMAX = list_ltc.nodesltc[vitri]->SOSVMAX;
		ltc.MALOPTC=list_ltc.nodesltc[vitri]->MALOPTC;
		*list_ltc.nodesltc[vitri]=ltc;
		
	}
	
 	getch();
}




// =============================== SINH VIEN ======================================
// CLEAR DANH SACH SINH VIEN
void clearlist_sv(PTRSV &First)
{
   PTRSV p;   
   p = First;
   while(First != NULL)
   {
      p = First;
      First = First->next;
      delete p;
   }
}

//TIM SV THEO ....
PTRSV search_infosv(PTRSV First, char *x){
	PTRSV p;
	for(p=First; p!= NULL; p=p->next){
		if(stricmp(p->sv.MASV,x)==0)
		return p;
	}
	return NULL;
}

//KHOI TAO DANH SACH SINH VIEN
void Create_DSSV(PTRSV &First)
{ 
	PTRSV Last,p, temp;
    SinhVien sv;

    char maso[15],c;
	clearlist_sv(First);   	
    
  do
  {
  		printf("Ma so sinh vien: ");
		gets(sv.MASV);
		if(stricmp(sv.MASV,"0")==0) return;
		temp=search_infosv(First,sv.MASV);
		if(temp!=NULL)	{
			cout<<"\nMa so bi trung.\n";
			continue;	
		}
		printf("Ho sinh vien: ");
		gets(sv.HO);
		printf("Ten sinh vien: ");
		gets(sv.TEN);
		cout<<"Ma lop: ";
		gets(sv.MALOP);
		cout<<"Phai: ";
		gets(sv.PHAI);
		cout<<"So dien thoai: ";
		gets(sv.SDT);
		fflush(stdin);
		
		p = new nodeSV; //New_node();
		p->sv=sv;
		
		if (First==NULL)
		  First=p;
		else 
		 {
		  for (Last = First ; Last->next !=NULL;Last=Last->next);
		  Last->next = p;
		 }
		
		p->next=NULL;
	}while(1);
}

// duyet DS SINH VIEN
void traverse(PTRSV First)
{
   PTRSV p;
   int stt = 0;
   p = First; 
   if(p == NULL){
   	printf("\n   (Khong co sinh vien trong danh sach)");
   	getche();
   	return;
   }
      
   printf("\n       Danh sach sinh vien: ");
   printf("\n     STT       MSSV       HO           TEN            MA LOP          PHAI            SDT");
   while(p != NULL)
   {
      printf("\n   %5d %10s  %-15s %-10s %15s %5s %15s"   , ++stt, p->sv.MASV, p->sv.HO,p->sv.TEN, p->sv.MALOP, p->sv.PHAI, p->sv.SDT);
      p = p->next;
   }
   getche();
}



// menu 
char menu(){
	int chucnang;
	do {
		printf("\n\n\t\tQUAN LY SINH VIEN THEO HE TIN CHI");
		printf("\n\Cac chuc nang cua chuong trinh\n");
		printf("   1. Tao danh sach sinh vien\n");
		printf("   2: Xem danh sach sinh vien\n");
		printf("   3: Nhap mon hocn\n");
		printf("   4: Xem danh sach mon hoc\n");
		printf("   5: Tao lop tin chi\n");
		printf("   6: Xem lop tin chi\n");
		printf("   7: Xoa lop tin chi\n");
		printf("   8: Sua lop tin chi\n");
		printf("   9: Them lop tin chi\n");
		printf("   11: Ghi vao file \n");
      	printf("   12: Mo danh sach \n");
		printf("Chuc nang ban chon: ");
		cin>>chucnang;
		
	}while(chucnang < 0) ;
      return chucnang;
}

int main(){
	PTRMH treeMH = NULL;
	LIST_LTC list_ltc;
	list_ltc.soluong =0;
	PTRSV FirstSV = NULL;
		
	do {
		system("cls");
      	int chucnang = menu();
      	system("cls");
      	fflush(stdin);
      	switch(chucnang){
      		case 1:
      			{
      			Create_DSSV(FirstSV);
      			break;
			}
			case 2:{
				traverse(FirstSV);
				break;
			}
			case 3:{
				Create_MONHOC(treeMH);
				break;
			}
			case 4:{
				printf("\n       Danh sach mon hoc: ");
   				printf("\n     Ma mon       Ten mon    STCLT	STCTH"); 
				Show_MonHoc(treeMH);	
				getch();
				break;
			}
			case 5:{
				Create_List_LTC(list_ltc);
				break;
			}
			case 6:{
				duyetDS_LTC(list_ltc);
				break;
			}
			case 7 :{
				LTC_Delete_theoma(list_ltc);
				break;
			}
			case 8:{
				LTC_Sua_theoma(list_ltc);
				break;
			}
			case 9:{
				Create_List_LTC(list_ltc);
				break;
			}
			case 11:{
				ghiFileMonHoc(treeMH);
				break;
			}
			case 12: {
				docFileMonHoc(treeMH);
				break;
			}
			
		}
	}
	while(1);
	
}









