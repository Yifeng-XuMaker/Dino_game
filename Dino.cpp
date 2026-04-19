#include <iostream>
using namespace std;
int obs[8];
int h=0;

struct dino {
int y; // 恐龍底部位置 (0=地面)
int h; // 恐龍高度 (1=蹲下, 2=正常站立)
int jumplevl; // 跳躍階段 (0~3)
bool crouching;// 正在蹲下?
bool jumping; // 正在跳躍?
};

void intputstr(string num){ //判讀輸入
    for(int i=0;i<8;i++){
        obs[i]=num[i]-'0';//ASCII碼的概念'0'==48
    }
}

void fps(dino d1){//畫圖
    for(int i=3;i>=0;i--){ //列(從上數下來)
        for(int j=0;j<=8;j++){//行
            cout<<"|";
            if(j==0){
                    if(i>=d1.y&&i<d1.y+d1.h) {cout<<" D"; }
                    else if(h==1&&i==0){cout<<" C";}//暴力增加-
                    else if(h==3&&i==1){cout<<" P";}
                    else if(h==4&&i==2){cout<<" P";}
                    else if((h==2&&i==0)||(h==2&&i==1)){cout<<" C";}
                    else cout<<"  ";
            }
            else{
                int k=obs[j-1];//第1行是恐龍
                if(    (k==1&&i==0)    ||    (k==2&&(i==0||i==1))    ){cout<<" C";}//仙人掌
                else if(  (k==3&&i==1)  ||  (k==4&&i==2)   ){cout<<" P";}//翼龍
                else{cout<<"  ";}
            }
        }
        cout<<"|"<<endl;
    }
    cout<<endl;
}

void condition(dino &d2,int c){ //處理恐龍狀態//確保傳進去的d可以被實際的讀取，而不是只作用在複製出來的d2
    if(c==2){//蹲
        d2.h=1;
        d2.y=0;
        d2.crouching=1;
        d2.jumping = 0;
        d2.jumplevl=0;
        return;//最高權限
    }
    else if(c==1){//跳
        if(d2.jumping==0/*&&d2.crouching==0*/){d2.jumping=1;d2.jumplevl=0;d2.crouching=0;}
        //else if(d2.jumping==0){d2.jumping=1;d2.jumplevl=0;d2.crouching=0;}
    }else{//0
        if(d2.crouching==1){
            d2.crouching=0;
            d2.h=2;
            d2.y=0;
        }
    }
    if(d2.jumping==1){
            d2.h=2;
        switch(d2.jumplevl){
        case 0:
            d2.y=1;
            break;
        case 1:
            d2.y=2;
            break;
        case 2:
            d2.y=1;
            break;
        case 3:
            d2.y=0;d2.jumping=0; d2.jumplevl=0; return; //
            break;
                }
        d2.jumplevl++;
    }
}


void changefps(int nextobs){
     h=obs[0];
    for(int i=0;i<7;i++){
        obs[i]=obs[i+1];
    }
    obs[7]=nextobs;
}


bool hit(dino d3){//碰撞判斷
    int k=obs[0];//恐龍欄位
    bool con1,con2,con3;//恐龍位置
    if(d3.y<=0&&d3.y+d3.h>0){ con1=1;}else{ con1=0;}//佔第一列
    if(d3.y<=1&&d3.y+d3.h>1){ con2=1;}else{ con2=0;}//佔第二列
    if(d3.y<=2&&d3.y+d3.h>2){ con3=1;}else{ con3=0;}//佔第三列

    if(k==0){return 0;}
    else if(k==1){return con1;}//c1
    else if(k==2){return con1||con2;} //or gate的概念//c2
    else if(k==3){return con2;}//p1
    else if(k==4){return con3;}//p2
    else{return 0;}
}



int main() {
    cout << "======Welcome to the Dinosaur Game======\nPlease enter the initial obstacle setup of the Dinosaur Game. Enter -1 to quit: ";
    string n;
    cin>>n;

    if(n[0]=='-'&&n[1]=='1'){cout<<"The game ended when it just started.\n======Thanks for playing Dinosaur Game, see you next time======"<<endl; return 0;}

    intputstr(n); //輸障礙
    dino d ={0,2,0,0,0}; //struct內的參數
    for(;;){
        fps(d);//畫圖
        cout<<"Please enter a command to control the dinosaur, 0 for no action, 1=jump, 2=crouch: ";
        int c;
        cin>>c;
        for(;;){

        if(c==0||c==1||c==2){//防呆
            if(c==1&&d.jumping==1){
                cout<<"The dinosaur cannot jump again, please enter another command: ";
                cin>>c;
                continue;
            }
            break;
        }else{
            cout<<"The dinosaur doesn't understand this command, try another one: ";
            cin>>c;}
        }

        condition(d,c);//處理新狀態

        cout<<"Please enter the next round's obstacle setup of the Dinosaur Game. Enter -1 to quit: ";
        string n2;
        cin>>n2;
        if(n2[0]=='-'&&n2[1]=='1'){cout<<"Congratulations, you WIN the game!!!\n======Thanks for playing Dinosaur Game, see you next time======"<<endl; break;;}

        int newobs=n2[0]-'0';
        if(hit(d)==1){//碰撞!?
            cout<<"What a pity, it was close :(\n======Thanks for playing Dinosaur Game, see you next time======"<<endl;
            break;
        }
        changefps(newobs);//更新障礙物
        }
    return 0;
}
