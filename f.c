#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#define NEXT(index,QSIZE)   ((index+1)%QSIZE)  

typedef struct Queue 

{

    int *buf;

    int qsize;

    int front;
    int rear;
    int count;
}Queue;

typedef struct node{
  
  int id;
  int depth;
  int visit;
  int follower;
  int followed;
  int  *from ;
  int *adj;
  
}node;
void InitQueue(Queue *queue,int qsize)
{

    queue->buf = (int *)malloc(sizeof(int)*qsize);
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
void Enqueue(Queue *queue,int data)
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
int Dequeue(Queue *queue)
{
    int re=0;
    if(IsEmpty(queue))
    {
        printf("큐가 비었음\n");
        return re;
    }
    re = queue->buf[queue->front];
    queue->front = NEXT(queue->front,queue->qsize);
    queue->count--;
    return re;
}

int tmp=0;
int max_dis=0;
node **nodelist;
int  nodeid[51028], is_connect[10000], con_ind=0;
int nodelist_index=0, nodelist_max = 30000;
node* newNode(int x, int y){
	node *v  = (node*)malloc(sizeof(node));
	v->id = x;
	v->follower=1;
	v->followed=0;
	v->visit=0;
	v->adj = (int*)malloc(4*1000);
	v->adj[0]=y;
	v->depth =10000;
	nodeid[nodelist_index] = x;
	nodelist[nodelist_index]=v;
	nodelist_index++;
	return v;
}
void newNodeY(int y){
	node *v  = (node*)malloc(sizeof(node));
	v->id = y;
	v->follower = 0;
	v->followed=1;
	v->visit=0;
	v->depth=10000;
	v->adj =(int*)malloc(4*1000);
	nodeid[nodelist_index] = y;
	nodelist[nodelist_index] =v;
	nodelist_index++;
}
node* find(int x){
	for(int i=0; i<nodelist_index; i++){
		if(nodeid[i]==x) return nodelist[i];
	}
	return NULL;
}

void bfs(node* v, int n){
	v->visit = 1;
	v->depth = n;
	Queue q;
	InitQueue(&q,1000);
	Enqueue(&q,v->id);
	while(!IsEmpty(&q)){
		int x = Dequeue(&q);
		for(int i=0; i<find(x)->follower; i++){
			node * cur = find(x);
			if(find(cur->adj[i])->visit==0){
				node* next = find(cur->adj[i]);
				next->depth = cur->depth+1;
				if(max_dis<next->depth) max_dis = next->depth;
				Enqueue(&q,next->id);
				next->visit=1;
			}
		}
	
	}
}
int search(int x){
	for(int i=0; i<con_ind; i++){
		if(is_connect[i]==x){
			printf("%d \n",x);
			return 1;	
		}
	}
	return 0;
}
void push(int x){
	is_connect[con_ind] = x;
	con_ind++;
}
void clear(){
	con_ind=0;
}
void is_con(node* v,int i){
	int sig =0;
	v->visit = 1;
	Queue q;
	InitQueue(&q,1000);
	Enqueue(&q,v->id);
	while(!IsEmpty(&q)){
		int x = Dequeue(&q);
		for(int i=0; i<find(x)->follower; i++){
			node* cur = find(x);
			if(find(cur->adj[i])->visit==0){
				node* next = find(cur->adj[i]);
				if(i!=0)
					sig = search(next->id);
				if(sig==1){
					printf("connected\n");
					return;
				}
				push(next->id);
				Enqueue(&q,next->id);
				next->visit=1;
			}
		}
	
	}

	if(i!=0 && sig==0){ 
		printf("not connected.\n");
		exit(0);}
}


/*
void dijk(node* v){
	v->depth = 0;
	push(v->id);

	while(!empty()){
		int curr;
		do{
			curr = que[q_i];
			pop();
		}while(!empty()&& find(curr)->visit);
		node* cur = find(curr);
		if(cur->visit) break;

		cur->visit =1;

		for(int i=0; i<cur->follower; i++){
		}

	}
}
*/	
void append(int x, int y){


	int sig=0;
	for(int i=0; i<nodelist_index; i++){
		if(nodelist[i]->id==x){
			int n = nodelist[i]->follower;
			int sig_2= 0;
			for(int j=0; j<n; j++){
				if(nodelist[i]->adj[j]==y)
					sig_2=1;
			} 
			if(sig_2==0){
				nodelist[i]->adj[n]=y;
				nodelist[i]->follower++;
				for(int j=0; j<nodelist_index; j++)
					if(nodelist[j]->id==y) sig_2=1;
				if(sig_2==0)
					newNodeY(y);
				else
					find(y)->followed++;
				
			}
			sig = 1;
		}

	}
	if(sig==0)
		newNode(x,y);
	sig =0;
	for(int j=0; j<nodelist_index; j++){
		if(nodelist[j]->id==y)sig=1;	
	}
	if(sig==0){
		newNodeY(y);
	}

}
int main(void) {
	nodelist = (node ** )malloc(sizeof(node*)*nodelist_max);

	for(int i= 0; i<nodelist_max;  i++){
		nodelist[i] = (node*)malloc(sizeof(node));
	}
	
//	printf(" node max num : %d %lu \n",nodelist_max , sizeof(nodelist));
	char *line, *cut;
	line = (char*)malloc(21);
	cut = (char*)malloc(21);
	int x,y;
	FILE *fp;
	fp = fopen("sampled.txt", "rb");
	int c= 0;
	while(feof(fp)==0){
		printf("%d\n",c);
		c++;
		fgets(line,21,fp);
		cut = strtok(line," ");
   		if(cut==NULL)break; 
		x= atoi(cut);
		cut = strtok(NULL, " ");
		if(cut==NULL)break;

		y = atoi(cut);
	       	append(x,y);
	}
  	fclose(fp);
	printf("\n");

	for(int i=0; i<nodelist_index; i++){
		//printf("%d\n", nodelist[i]->id);
		
	//	bfs(nodelist[i],0);
	//	for(int j=0; j<nodelist_index; j++){
	//		nodelist[j]->visit= 0;
	//		nodelist[j]->depth = 0;
	//	}
	}
/*	
	int hist[6]= {0,0,0,0,0,0};
	for(int i=0; i<nodelist_index; i++){
		int now = nodelist[i]->follower;
		if(now<5){
			hist[0]++;
		}
		else if(now>=5 &&now<10)
			hist[1]++;
		else if(now>=10 && now<15)
			hist[2]++;
		else if(now>=15 && now<20)
			hist[3]++;
		else if(now>=20 && now<25)
			hist[4]++;
		else 
			hist[5]++;
	}
	int sum =0;
	for(int i=0; i<6; i++){
		printf("following under %d %d\n",(i+1)*5,hist[i]);
		hist[i]=0;
	}
	for(int i=0; i<nodelist_index; i++){
		int now = nodelist[i]->followed;
		if(now<5)
			hist[0]++;
		else if(now>=5 &&now<10)
			hist[1]++;
		else if(now>=10 && now<15)
			hist[2]++;
		else if(now>=15 && now<20)
			hist[3]++;
		else if(now>=20 && now<25)
			hist[4]++;
		else 
			hist[5]++;
	}
	for(int i=0; i<6; i++){
		printf("followed under %d  %d\n",(i+1)*5,hist[i]);
		sum = sum+hist[i];
		hist[i]=0;
	}
	printf("sum :%d\n",sum);

	push(nodelist[0]->id);
	for(int i=0; i<nodelist_index; i++){
		is_con(nodelist[i],i);
		printf("%d try!\n",i);
		for(int j=0; j<nodelist_index; j++){
				
			nodelist[i]->visit = 0;
		}
		
	}
*/
	return 0;
}
