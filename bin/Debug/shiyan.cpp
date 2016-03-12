#include<stdio.h>
#include<string.h>
#include<windows.h>
#include<time.h>
#include<iostream>
#include "uselei.h"
using namespace std;

typedef int (*dll_fun)(int*);//注意这里有个声明形参的地方，而且这句话意义什么，以后要深究。
giveplayer tmp;

void showmap(){
    for (int i=0;i<tmp.mpsize;i++){
        for (int j=0;j<tmp.mpsize;j++)
            cout<<tmp.smap[i][j];
        cout<<endl;
    }
    cout<<endl;
}
void randomfood(){
    srand( (unsigned)time( NULL ) );
    int x=rand()%200;
    int sum=0;
    for (int i=0;i<tmp.mpsize;i++){
        for (int j=0;j<tmp.mpsize;j++){
            if (!tmp.smap[i][j]) sum++;
            if (sum==x) {
                tmp.food.x=i;
                tmp.food.y=j;
                tmp.smap[i][j]=2;
                return ;
            }
        }
    }
}
void fillall(int x){
    for (int i=0;i<tmp.mpsize;i++){
        for (int j=0;j<tmp.mpsize;j++){
            if (tmp.smap[i][j]==x) tmp.smap[i][j]=0;
        }
    }
}
void getcmd(int *a){
    a[0]=tmp.food.x;
    a[1]=tmp.food.y;
    a[4]=tmp.mpsize;
    for (int i=0;i<40;i++)
        for (int j=0;j<40;j++)
            a[40+i*40+j]=tmp.smap[i][j];
    a[2000]=tmp.p1.tail;
    a[2002]=tmp.p1.head;
    a[2005]=tmp.p1.bodylen;
    for (int i=0;i<76;i++){
        a[2006+2*i]=tmp.p1.body[i].x;
        a[2006+2*i+1]=tmp.p1.body[i].y;
    }

    a[3000]=tmp.p2.tail;
    a[3002]=tmp.p2.head;
    a[3005]=tmp.p2.bodylen;

    for (int i=0;i<76;i++){
        a[3006+2*i]=tmp.p2.body[i].x;
        a[3006+2*i+1]=tmp.p2.body[i].y;
    }
}
int RESULTA,RESULTB;
int main(int argv,char * argc[])
{
    HMODULE A,B;
    A = LoadLibrary(argc[1]);//这个位置是dll文件的相对位置，这个意思是在同文件夹
    B = LoadLibrary(argc[2]);

    if(A==NULL)//出错处理。
    {
        printf("load library fail!\n");
        FreeLibrary(A);
        getchar();
        return 0;
    }
    if(B==NULL)//出错处理。
    {
        printf("load library fail!\n");
        FreeLibrary(B);
        getchar();
        return 0;
    }

    dll_fun FUNA,FUNB;
    FUNA=(dll_fun)GetProcAddress(A,"player_1");//在这里声明dll函数名字
    FUNB=(dll_fun)GetProcAddress(B,"player_2");//在这里声明dll函数名字
    if (!FUNA){
        cout<<"NOT FOUND function player_1"<<endl;
        return 0;
    }
    if (!FUNB){
        cout<<"NOT FOUND function player_2"<<endl;
        return 0;
    }

    memset(tmp.smap,0,sizeof tmp.smap);
    tmp.mpsize=39;
    tmp.p1.body[1].x=tmp.mpsize-1;
    tmp.p1.body[1].y=0;
    tmp.p1.body[0].x=tmp.mpsize-2;
    tmp.p1.body[0].y=0;
    tmp.p2.body[1].x=tmp.mpsize-1;
    tmp.p2.body[1].y=tmp.mpsize-1;
    tmp.p2.body[0].x=tmp.mpsize-2;
    tmp.p2.body[0].y=tmp.mpsize-1;
    tmp.p1.bodylen=tmp.p2.bodylen=2;
    tmp.p1.head=tmp.p2.head=0;
    tmp.p1.tail=tmp.p2.tail=1;
    tmp.smap[tmp.mpsize-1][0]=tmp.smap[tmp.mpsize-2][0]=1;
    tmp.smap[tmp.mpsize-1][tmp.mpsize-1]=tmp.smap[tmp.mpsize-2][tmp.mpsize-1]=3;

    FILE *fp=fopen("result.txt","w");
    int i;

    for (i=0;i<5000;i++){
        if (i%100==0) randomfood();
        int cmd[10000];
        getcmd(cmd);
        showmap();
        if (!RESULTA) {
            int da=FUNA(cmd);
            if (da<0||da>3) {
                RESULTA=tmp.p1.bodylen;
                fillall(1);
                tmp.p1.bodylen=0;
                for (int j=0;j<76;j++)
                    tmp.p1.body[j].x=tmp.p1.body[j].y=0;
            }
            else {
                tmp.smap[tmp.p1.body[tmp.p1.tail].x][tmp.p1.body[tmp.p1.tail].y]=0;
                int newx,newy;
                if (da==0){
                    newx=tmp.p1.body[tmp.p1.head].x-1;
                    newy=tmp.p1.body[tmp.p1.head].y;
                }
                if (da==1){
                    newx=tmp.p1.body[tmp.p1.head].x;
                    newy=tmp.p1.body[tmp.p1.head].y+1;
                }
                if (da==2){
                    newx=tmp.p1.body[tmp.p1.head].x+1;
                    newy=tmp.p1.body[tmp.p1.head].y;
                }
                if (da==3){
                    newx=tmp.p1.body[tmp.p1.head].x;
                    newy=tmp.p1.body[tmp.p1.head].y-1;
                }

                if (newx<0||newx>=tmp.mpsize||newy<0||newy>=tmp.mpsize){
                    RESULTA=tmp.p1.bodylen;
                    fillall(1);
                    tmp.p1.bodylen=0;
                    for (int j=0;j<76;j++)
                        tmp.p1.body[j].x=tmp.p1.body[j].y=0;
                }
                else {
                    if (tmp.smap[newx][newy]==1||tmp.smap[newx][newy]==3){
                        RESULTA=tmp.p1.bodylen;
                        fillall(1);
                        tmp.p1.bodylen=0;
                        for (int j=0;j<76;j++)
                            tmp.p1.body[j].x=tmp.p1.body[j].y=0;
                    }
                    else{
                        if (tmp.smap[newx][newy]==2){
                            for (int j=tmp.p1.bodylen;j>=1;j--){
                                tmp.p1.body[j].x=tmp.p1.body[j-1].x;
                                tmp.p1.body[j].y=tmp.p1.body[j-1].y;
                            }
                            tmp.p1.body[0].x=newx;
                            tmp.p1.body[0].y=newy;
                            tmp.p1.bodylen++;
                        }
                        else {
                            for (int j=tmp.p1.bodylen-1;j>=1;j--){
                                tmp.p1.body[j].x=tmp.p1.body[j-1].x;
                                tmp.p1.body[j].y=tmp.p1.body[j-1].y;
                            }
                            tmp.p1.body[0].x=newx;
                            tmp.p1.body[0].y=newy;
                        }

                        for (int j=0;j<tmp.mpsize;j++)
                            for (int k=0;k<tmp.mpsize;k++)
                                if (tmp.smap[j][k]==1) tmp.smap[j][k]=0;
                        for (int j=0;j<tmp.p1.bodylen;j++)
                            tmp.smap[tmp.p1.body[j].x][tmp.p1.body[j].y]=1;
                    }
                }
            }
        }

        getcmd(cmd);
        if (!RESULTB) {
            int db=FUNB(cmd);
            if (db<0||db>3) {
                RESULTA=tmp.p2.bodylen;
                fillall(3);
                tmp.p2.bodylen=0;
                for (int j=0;j<76;j++)
                    tmp.p2.body[j].x=tmp.p2.body[j].y=0;
            }
            else {
                tmp.smap[tmp.p2.body[tmp.p2.tail].x][tmp.p2.body[tmp.p2.tail].y]=0;
                int newx,newy;
                if (db==0){
                    newx=tmp.p2.body[tmp.p2.head].x-1;
                    newy=tmp.p2.body[tmp.p2.head].y;
                }
                if (db==1){
                    newx=tmp.p2.body[tmp.p2.head].x;
                    newy=tmp.p2.body[tmp.p2.head].y+1;
                }
                if (db==2){
                    newx=tmp.p2.body[tmp.p2.head].x+1;
                    newy=tmp.p2.body[tmp.p2.head].y;
                }
                if (db==3){
                    newx=tmp.p2.body[tmp.p2.head].x;
                    newy=tmp.p2.body[tmp.p2.head].y-1;
                }

                if (newx<0||newx>=tmp.mpsize||newy<0||newy>=tmp.mpsize){
                    RESULTB=tmp.p2.bodylen;
                    fillall(3);
                    tmp.p2.bodylen=0;
                    for (int j=0;j<76;j++)
                        tmp.p2.body[j].x=tmp.p2.body[j].y=0;
                }
                else {
                    if (tmp.smap[newx][newy]==1||tmp.smap[newx][newy]==3){
                        RESULTB=tmp.p2.bodylen;
                        fillall(3);
                        tmp.p2.bodylen=0;
                        for (int j=0;j<76;j++)
                            tmp.p2.body[j].x=tmp.p2.body[j].y=0;
                    }
                    else{
                        if (tmp.smap[newx][newy]==2){
                            for (int j=tmp.p2.bodylen;j>=1;j--){
                                tmp.p2.body[j].x=tmp.p2.body[j-1].x;
                                tmp.p2.body[j].y=tmp.p2.body[j-1].y;
                            }
                            tmp.p2.body[0].x=newx;
                            tmp.p2.body[0].y=newy;
                            tmp.p2.bodylen++;
                        }
                        else {
                            for (int j=tmp.p2.bodylen-1;j>=1;j--){
                                tmp.p2.body[j].x=tmp.p2.body[j-1].x;
                                tmp.p2.body[j].y=tmp.p2.body[j-1].y;
                            }
                            tmp.p2.body[0].x=newx;
                            tmp.p2.body[0].y=newy;
                        }

                        for (int j=0;j<tmp.mpsize;j++)
                            for (int k=0;k<tmp.mpsize;k++)
                                if (tmp.smap[j][k]==3) tmp.smap[j][k]=0;
                        for (int j=0;j<tmp.p2.bodylen;j++)
                            tmp.smap[tmp.p2.body[j].x][tmp.p2.body[j].y]=3;
                    }
                }
            }
        }
        if (RESULTA&&RESULTB) break;
    }

    fclose(fp);
    cout<<i<<endl;
    return 0;
}
