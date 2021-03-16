#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define maxcid 55
#define maxcnt 5005

int participated[maxcnt][maxcid];   //participated[uid][cid]表示uid这个人有没有参加过cid这场比赛

typedef struct{
    int solnum[maxcnt];     //con[cid].solnum[uid]表示uid这个人在cid这场比赛中ac题目数
    int pplsum[maxcnt];     //con[cid].pplsum[j]表示在cid这场比赛中做出题目数 大于等于j 的有多少人
} Contest;
Contest con[maxcid];

typedef struct done{
    int cid;
    int actime;
    struct done* next;
} done;
done* d[maxcnt][maxcnt];    //d[i][j]是一个链表，记录i这个人对j这个题，在哪些比赛中提交过，通过次数是多少
                            //d[uid][pid-1000]


typedef struct{
    int cid;
    int pid;
    int uid;
    int res;  //0->unac; 1->ac
}sub;
sub sb[300005];

void createContest();
void getRank();
// void rejudge();
// void submission();
// done* findNode(int uid, int pid, int cid);
void freelist(done* head);
void myfree();

int main()
{
    char s[20];
    int pcnt,ucnt,m;
    scanf("%d%d%d",&pcnt,&ucnt,&m);

    for( int i = 0 ; i < maxcnt ; ++i ){
        for( int j = 0 ; j < maxcnt ; ++j ){
            d[i][j] = NULL;
        }
    }

    for( int i = 0 ; i < m ; i++ ){
        scanf("%s",s);

        if(strcmp(s,"createContest")==0)    createContest();

        if(strcmp(s,"submission")==0){
            int sid,cid,pid,uid;                        //读入
            char result[10];
            scanf("%d%d%d%d%s",&sid,&cid,&pid,&uid,result);

            sb[sid].cid = cid;          //创建提交记录
            sb[sid].pid = pid;
            sb[sid].uid = uid;
            if(strcmp(result,"AC")==0)  sb[sid].res = 1;
            else        sb[sid].res = 0;

            if(participated[uid][cid]==0){
                participated[uid][cid] = 1;
                con[cid].pplsum[0]++;
            }
     
            done* p = d[uid][pid-1000];             //findNode
            int flag = 0;
            while(p){
                if(p->cid==cid){
                    flag = 1;
                    break;
                }
                p = p->next;
            }
            if(!flag){
                done* q = (done*)malloc(sizeof(done));
                q->cid = cid;
                q->actime = 0;
                q->next = d[uid][pid-1000];
                d[uid][pid-1000] = q;
                p = q;
            }

            if(sb[sid].res==1){
                if(p->actime==0){
                    con[cid].solnum[uid]++;
                    con[cid].pplsum[con[cid].solnum[uid]]++;
                }
                p->actime++;
            }
        }

        if(strcmp(s,"getRank")==0)          getRank();

        if(strcmp(s,"rejudge")==0){
            int sid;
            scanf("%d",&sid);

            if(sb[sid].res==0)  continue;
            sb[sid].res = 0;
            int cid = sb[sid].cid;
            int pid = sb[sid].pid;
            int uid = sb[sid].uid;

            done* p = d[uid][pid-1000];
            while(p){
                if(p->cid==cid) break;
                p = p->next;
            }

            p->actime--;
            if(p->actime==0){
                con[cid].pplsum[con[cid].solnum[uid]]--;
                con[cid].solnum[uid]--;
            }
        }
    }

    myfree();

    return 0;
}

void createContest()
{
    int t;
    scanf("%d",&t);
    scanf("%d",&t);
    for( int i = 0 ; i < t ; i++ ){
        scanf("%d",&t);
    }
}
void getRank()
{
    int cid,uid;
    scanf("%d%d",&cid,&uid);
    int solved = con[cid].solnum[uid];
    printf("%d %d %d %d\n",uid, solved, con[cid].pplsum[solved+1]+1, con[cid].pplsum[solved]);
}
void freelist(done* head)
{
    if(!head)   return;
    done* p = head;
    done* q = NULL;
    while(p){
        q = p->next;
        free(p);
        p = q;
    }
}
void myfree()
{
    for( int i = 0 ; i < maxcnt ; ++i ){
        for( int j = 0 ; j < maxcnt ; ++j ){
            freelist(d[i][j]);
        }
    }
}
// void rejudge()
// {
//     int sid;
//     scanf("%d",&sid);

//     if(sb[sid].res==0)  return;
//     sb[sid].res = 1;
//     int cid = sb[sid].uid;
//     int pid = sb[sid].pid;
//     int uid = sb[sid].uid;

//     done* p = d[uid][pid-1000];
//     while(p){
//         if(p->cid==cid) break;
//         p = p->next;
//     }

//     p->actime--;
//     if(p->actime==0){
//         con[cid].pplsum[con[cid].solnum[uid]]--;
//         con[cid].solnum[uid]--;
//     }
// }
// void submission()
// {
//     int sid,cid,pid,uid;                        //读入
//     char result[6];
//     scanf("%d%d%d%d%s",&sid,&cid,&pid,&uid,result);

//     sb[sid].cid = cid;          //创建提交记录
//     sb[sid].pid = pid;
//     sb[sid].uid = uid;
//     sb[sid].res = strcmp(result,"AC")?0:1;

//     if(participated[uid][cid]==0){
//         participated[uid][cid] = 1;
//         con[cid].pplsum[0]++;
//     }

//     done* p = findNode(uid, pid, cid);
//     if(sb[sid].res==1){
//         if(p->actime==0){
//             con[cid].solnum[uid]++;
//             con[cid].pplsum[con[cid].solnum[uid]]++;
//         }
//         p->actime++;
//     }
// }
// done* findNode(int uid, int pid, int cid)
// {
//     done* p = d[uid][pid-1000];
//     int flag = 0;
//     while(p){
//         if(p->cid==cid){
//             flag = 1;
//             break;
//         }
//         p = p->next;
//     }
//     if(flag)    return p;
//     else{
//         done* q = (done*)malloc(sizeof(done));
//         q->cid = cid;
//         q->actime = 0;
//         q->next = d[uid][pid-1000];
//         d[uid][pid-1000] = q;
//         return q;
//     }
// }
