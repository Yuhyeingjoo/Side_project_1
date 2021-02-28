#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#define NEXT(index,QSIZE)   ((index+1)%QSIZE)
#define divSize 1

typedef struct node{

  int id;
  int depth;
  int visit;
  int follower;
  int followed;
  struct node **follow;
  struct node **followee;
  struct node *next;

}node;
typedef struct Queue

{

    node **buf;

    int qsize;

    int front;
    int rear;
    int count;
}Queue;
void InitQueue(Queue *queue,int qsize)
{

    queue->buf = (node **)malloc(sizeof(node*)*qsize);
    queue->qsize = qsize;
    queue->front = queue->rear= 0;
    queue->count = 0;
}
int IsFull(Queue *queue)
{
    return queue->count == queue->qsize;

}
int IsEmpty(Queue *queue)
{
    return queue->count == 0;    //보관 개수가 0이면 빈 상태
}
void Enqueue(Queue *queue,node* data)
{
    if(IsFull(queue))
    {
        printf("큐가 꽉 찼음\n");
        return ;
    }
    queue->buf[queue->rear] = data;
    queue->rear = NEXT(queue->rear,queue->qsize);

    queue->count++;
}


node* Dequeue(Queue *queue)
{
    node* re=0;
    if(IsEmpty(queue))
    {
        printf("큐가 비었음\n");
        return NULL;
    }
    re = queue->buf[queue->front];
    queue->front = NEXT(queue->front,queue->qsize);
    queue->count--;
    return re;
}
node ** front , **flow;
int max_dis=0;
int is_connect[10000], con_ind=0;
int nodelist_index=0, nodelist_max = 100;
node* newNodeY(node* p, int y);
node* newNode(int x, int y){
	int sig=0 ,rest = x%divSize, rest_y = y%divSize;
        node *v  = (node*)malloc(sizeof(node));
        v->id = x;
        v->follower=1;
        v->followed=0;
        v->visit=0;
        v->depth =10000;
        v->follow = (node**)malloc(sizeof(node*)*nodelist_max);
        for(int i=0; i<nodelist_max; i++){
                v->follow[i] = (node*)malloc(sizeof(node));
        }
        v->followee = (node**)malloc(sizeof(node*)*nodelist_max);
        for(int i=0; i<nodelist_max; i++){
                v->followee[i] = (node*)malloc(sizeof(node));
        }
	node* cur = front[rest_y];
		while(cur!=NULL){
			if(cur->id==y){
				sig=1;
				break;
			}
			cur= cur->next;
		}
        if(sig==0){

		if(front[rest]==0x0 || flow[rest]==0x0){
			front[rest] = v;	
			flow[rest] = v;

		}
		else{
			flow[rest]->next= v;
			flow[rest] = v;
		}
                v->follow[0] = newNodeY(v,y);

	printf("both no\n");
        }

	if(sig==1){

	printf("x  no y yes\n");
		if(flow[rest]==0x0 || front[rest]==0x0){
			front[rest]=v;
			flow[rest] = v;
		}
		else{
			flow[rest]->next = v;
			flow[rest] = v;
		}
			v->follow[0]=cur;
			cur->followee[cur->followed] = v;
			cur->followed++;
		

	}
        return v;
}
node* newNodeY(node* p , int y){

	int rest_y = y%divSize;
        node *v  = (node*)malloc(sizeof(node));
        v->id = y;
        v->follower = 0;
        v->followed=1;
        v->visit=0;
        v->depth=10000;
        v->follow = (node**)malloc(sizeof(node*)*nodelist_max);
        for(int i=0; i<nodelist_max; i++){
                v->follow[i] = (node*)malloc(sizeof(node));
        }
        v->followee = (node**)malloc(sizeof(node*)*nodelist_max);
        for(int i=0; i<nodelist_max; i++){
                v->followee[i] = (node*)malloc(sizeof(node));
        }
	if(front[rest_y]==0x0||flow[rest_y]==0x0){
		front[rest_y] = v;
		flow[rest_y] = v;

	}
	else{
		flow[rest_y]->next = v;
		flow[rest_y] = v;
	}
	v->followee[0] = p;
	
	return v;
}


void append(int x, int y){
	int sig=0, rest = x%divSize, rest_y = y%divSize;
	node * ind = front[rest];
	while(ind!=NULL){

		if(ind->id==x){
			sig =1;	
			break;	
		}
		ind= ind->next;
	}

	if(sig==0)
		newNode(x,y);

	if(sig==1){
		int sig_2=0;
		node* ind_2 = front[rest_y];
		while(ind_2!=NULL){
			if(ind_2->id==y){
				sig_2 = 1;
				break;	
				
			}
			ind_2=ind_2->next;
		}
		if(sig_2==0){
			ind->follow[ind->follower] = newNodeY(ind,y);
			ind->follower++;		

	printf("x yes y no\n");
		}
		if(sig_2==1){
			int sig_3=0;
			for(int i=0; i<ind->follower; i++){
				if(ind->follow[i]->id==y){
					sig_3= 1;
					break;	
				}
			}
			if(sig_3==0){

	printf("x  yes y yes but coneect firt\n");
				ind->follow[ind->follower] = ind_2;
				ind->follower++;
				ind_2->followee[ind_2->followed] = ind;
				ind_2->followed++;
			}	
		}
	}
}
int isfriend(node *x, node* next){
	for(int k = 0; k<next->follower; k++){
		if(next->follow[k]==x) return 1;

	}
	return 0;
}
void bfs(node* v){
//	v->depth = n;
	v->visit =1;
	Queue q;
	InitQueue(&q,512);
	Enqueue(&q,v);
//	int count = 1;
	while(!IsEmpty(&q)){
		node* x = Dequeue(&q);
	//	printf("%d ",x->id);

		for(int i=0;i< x->follower; i++){
			if(x->follow[i]->visit==0 &&isfriend(x,x->follow[i])){
			//	count++;
			//	printf("%d\n",count);
				node * next = x->follow[i];
			//	next->depth= x->depth+1;
			//	if(max_dis<next->depth) max_dis = next->depth;
				Enqueue(&q,next);
				next->visit = 1;	
				
			}
		}
	}
//	printf("\n");
			
}
void clear(node* v){
	v->visit =0;
	Queue q;
	InitQueue(&q,210808);
	Enqueue(&q,v);
	while(!IsEmpty(&q)){
		node* x = Dequeue(&q);

		for(int i=0;i< x->follower; i++){
			if(x->follow[i]->visit==1){
				node * next = x->follow[i];
				Enqueue(&q,next);
				next->visit = 0;	
				
			}
		}
	}
			
}
int main(){
	char * line, *cut;
	line = (char*)malloc(21);
	cut= (char*)malloc(21);
	int x,y;
	FILE *fp;
	int count = 0;
	fp= fopen("sampled.txt","rb");
	front = (node**)malloc(sizeof(node*)*divSize);
	flow = (node**)malloc(sizeof(node*)*divSize);
	for(int i=0; i<divSize; i++){
		front[i]= (node*)malloc(sizeof(node));
		flow[i]= (node*)malloc(sizeof(node));
		front[i] = 0x0;
		flow[i] = 0x0;
	}
	while(feof(fp)==0){
		fgets(line,21,fp);
		cut = strtok(line," ");
		if(cut==NULL) break;
		x= atoi(cut);
		cut = strtok(NULL," ");
		if(cut==NULL)break;
		y = atoi(cut);
		printf("%d %d\n",x,y);
		append(x,y);
	}
	fclose(fp);
/*	
	int dd =0, uu=0 ;
	node* c = front[0];
		while(c!=NULL){
			printf("%d\n",dd++);
			bfs(c,0);
			c=c->next;
			node* d = front[0];
			while(d!=NULL){
				d->visit=0;
				d=d->next;
			}
	}
/*	
	printf("%d %d\n",uu,dd);

//	bfs(front,0);
	printf("max: %d\n",max_dis);
/

	printf("%d\n",max_dis);
*/
	int parti =0;
		node* k = front[0];
		while(k!=NULL){

			if(k->visit==0){

				bfs(k);
				printf("ii %d  %d\n",k->id,++parti);	
			}
			k=k->next;	
		}

	
//	node* k = front[1];
	
//	bfs(k,0);
//	printf("max %d  %d\n",max_dis,ddd);
}



