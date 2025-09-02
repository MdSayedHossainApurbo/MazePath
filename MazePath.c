#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define NMAX 50

int n, g[NMAX][NMAX];

int inb(int x,int y){return x>=0&&x<n&&y>=0&&y<n;}
int open(int x,int y){return inb(x,y)&&g[x][y]==1;}

int sbt[NMAX][NMAX], vbt[NMAX][NMAX], pxb[NMAX*NMAX], pyb[NMAX*NMAX], lb;
int dbt(int x,int y){
    if(!(open(x,y)&&!vbt[x][y])) return 0;
    vbt[x][y]=1; sbt[x][y]=1; pxb[lb]=x; pyb[lb]=y; lb++;
    if(x==n-1&&y==n-1) return 1;
    static int dx[4]={0,1,0,-1}, dy[4]={1,0,-1,0};
    for(int k=0;k<4;k++){
        int nx=x+dx[k], ny=y+dy[k];
        if(dbt(nx,ny)) return 1;
    }
    vbt[x][y]=0; sbt[x][y]=0; lb--;
    return 0;
}
int run_bt(void){
    for(int i=0;i<n;i++) for(int j=0;j<n;j++){sbt[i][j]=0; vbt[i][j]=0;}
    lb=0;
    if(!open(0,0)||!open(n-1,n-1)) return 0;
    return dbt(0,0);
}

int sbf[NMAX][NMAX], vbf[NMAX][NMAX], px[NMAX][NMAX], py[NMAX][NMAX];
int run_bfs(void){
    for(int i=0;i<n;i++) for(int j=0;j<n;j++){sbf[i][j]=0; vbf[i][j]=0; px[i][j]=-1; py[i][j]=-1;}
    if(!open(0,0)||!open(n-1,n-1)) return 0;
    int qx[NMAX*NMAX], qy[NMAX*NMAX], f=0, b=0;
    vbf[0][0]=1; qx[b]=0; qy[b]=0; b++;
    int dx[4]={0,1,0,-1}, dy[4]={1,0,-1,0};
    while(f<b){
        int x=qx[f], y=qy[f]; f++;
        if(x==n-1&&y==n-1) break;
        for(int k=0;k<4;k++){
            int nx=x+dx[k], ny=y+dy[k];
            if(open(nx,ny)&&!vbf[nx][ny]){
                vbf[nx][ny]=1; px[nx][ny]=x; py[nx][ny]=y;
                qx[b]=nx; qy[b]=ny; b++;
            }
        }
    }
    if(!vbf[n-1][n-1]) return 0;
    int cx=n-1, cy=n-1;
    while(!(cx==0&&cy==0)){
        sbf[cx][cy]=1;
        int tx=px[cx][cy], ty=py[cx][cy];
        cx=tx; cy=ty;
    }
    sbf[0][0]=1;
    return 1;
}

void printg(int a[NMAX][NMAX]){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++) printf("%d ", a[i][j]);
        printf("\n");
    }
}
int count1(int a[NMAX][NMAX]){
    int c=0; for(int i=0;i<n;i++) for(int j=0;j<n;j++) if(a[i][j]) c++; return c;
}

void flush_line(void){
    int ch;
    while((ch=getchar())!='\n' && ch!=EOF) { }
}

int read_row_digits(int row){
    char buf[512];
    if(!fgets(buf, sizeof(buf), stdin)) return 0;
    int col = 0;
    for(int i=0; buf[i]; i++){
        if(buf[i]=='\n') break;
        if(isspace((unsigned char)buf[i])) continue;
        if(buf[i]=='0' || buf[i]=='1'){
            if(col >= n) return 0;
            g[row][col++] = (buf[i]=='1') ? 1 : 0;
        }else{
            return 0;
        }
    }
    return (col==n);
}

int main(void){
    printf("Enter maze size n (1..%d): ", NMAX);
    if(scanf("%d",&n)!=1 || n<=0 || n>NMAX){
        printf("Invalid n\n");
        return 0;
    }
    flush_line();

    printf("Enter %dx%d maze (each row with 0/1, with or without spaces):\n", n, n);
    for(int i=0;i<n;i++){
        while(1){
            printf("Row %d: ", i);
            if(read_row_digits(i)) break;
            printf("Invalid input! Try again (only 0/1, exactly %d values)\n", n);
        }
    }

    int ok1=run_bt();
    int ok2=run_bfs();

    if(ok1){
        printf("BT\n");
        printg(sbt);
        for(int i=0;i<lb;i++) printf("(%d,%d)%s", pxb[i], pyb[i], (i+1<lb)?" ":"\n");
        printf("%d\n", lb>0?lb-1:0);
    }else{
        printf("BT\nNONE\n");
    }

    if(ok2){
        printf("BFS\n");
        printg(sbf);
        int pxs[NMAX*NMAX], pys[NMAX*NMAX], L=0, cx=n-1, cy=n-1;
        while(!(cx==0&&cy==0)){ pxs[L]=cx; pys[L]=cy; L++; int tx=px[cx][cy], ty=py[cx][cy]; cx=tx; cy=ty; }
        pxs[L]=0; pys[L]=0; L++;
        for(int i=L-1;i>=0;i--) printf("(%d,%d)%s", pxs[i], pys[i], (i?" ":"\n"));
        int steps=count1(sbf)-1; if(steps<0) steps=0; printf("%d\n", steps);
    }else{
        printf("BFS\nNONE\n");
    }
    return 0;
}
