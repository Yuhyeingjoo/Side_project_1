#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#define NEXT(index,QSIZE)   ((index+1)%QSIZE)
#define divSize 1

typedef struct node{

  int id;
  int depth;
  int visit;
  int index;
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
int mat_ind= -1;
int is_connect[10000], con_ind=0;
int nodelist_index=0, nodelist_max = 100;
node* newNodeY(node* p, int y);
node* newNode(int x, int y){
	mat_ind++;
		printf("add: %d %d\n",x,mat_ind);
	int sig=0 ,rest = x%divSize, rest_y = y%divSize;
        node *v  = (node*)malloc(sizeof(node));
	v->index= mat_ind;
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

//	printf("both no\n");
        }

	if(sig==1){

//	printf("x  no y yes\n");
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
	mat_ind++;

		printf("add: %d %d\n",y,mat_ind);
	int rest_y = y%divSize;
        node *v  = (node*)malloc(sizeof(node));
       	v->index = mat_ind;
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

//	printf("x yes y no\n");
		}
		if(sig_2==1){

//	printf("x  yes y yes but coneect firt\n");
				ind->follow[ind->follower] = ind_2;
				ind->follower++;
				ind_2->followee[ind_2->followed] = ind;
				ind_2->followed++;
		}
	}
}
double** arr;
void make_matrix(node* v){
	while(v!=NULL){
		for(int p=0; p<v->follower; p++){
			arr[v->index][v->follow[p]->index]++;
		}
		v=v->next;
	}
	
	for(int i=0; i<= mat_ind; i++){
		int n=0;
		for(int j=0; j<=mat_ind; j++){
			n+=arr[i][j];
		}
		if( n == 0 ) { 
			for(int j = 0 ; j <= mat_ind; j++)  
				arr[i][j] = (0.9)/(mat_ind+1);
		}
		else{
			for(int k=0; k<=mat_ind; k++){
				arr[i][k]=arr[i][k]/n*0.9;
			}
		}
	}

	for(int i = 0 ; i <= mat_ind ; i++) { 
		for(int j = 0 ; j <= mat_ind; j++) {
			arr[i][j] += (0.1)/(mat_ind+1);
		}
	}

	for(int i = 0 ; i <= mat_ind ; i++) { 
		double sum = 0 ;
		for(int j = 0 ; j <= mat_ind; j++){  
			sum += arr[i][j];
		}
		printf("sum; %.2f\n", sum);
	}
	
}
double * mat_mul(double* ier){
	double*result ;
	result = (double*)malloc(sizeof(double)*(mat_ind+1));
	
	for(int i=0; i<=mat_ind; i++){
		double mul_sum=0;
		for(int j=0; j<=mat_ind; j++){
			mul_sum+=arr[j][i]*ier[j];
		}
		result[i] = mul_sum;
	}
	return result;

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
	arr= (double**)malloc(sizeof(double)*(1+mat_ind));
	for(int i=0; i<(mat_ind+1); i++){
		arr[i] = (double*)malloc(sizeof(double)*(1+mat_ind));
	}
	for(int i=0; i<mat_ind; i++){
		for(int j=0; j<mat_ind; j++)
			arr[i][j]=0;
	}
	printf("mat index: %d\n",mat_ind);
	node * u = front[0];
	make_matrix(u);
	node * k =front[0];
	double *ier;
	ier = (double*)malloc(sizeof(double)*(mat_ind+1));
	for(int i=0; i<=mat_ind; i++){
		if(i==0)
			ier[i]=1;
		else
			ier[i]=0;
	}

	for(int i=0; i<20; i++){
		ier = mat_mul(ier);
	}
	double s=0;

	
	
	double ultimate[5], maximum=0;
	int inSSa[5],cur_inSSa;
	for(int i=0; i<5; i++){
		maximum=0;
		for(int j=0; j<=mat_ind; j++){
			int is_contain=0;
			for(int k=0;k<i; k++)
				if(ultimate[k]==ier[j]) is_contain=1;
			if(maximum<ier[j] && is_contain==0){
				maximum = ier[j];
				cur_inSSa = j;
			}
		}
		ultimate[i] = maximum;
		inSSa[i]=cur_inSSa;
	}
	for(int i=0; i<5; i++){
		printf("%.4f ",ultimate[i]);
	}
	for(int i=0; i<5; i++){
		node *z = front[0];
		while(z!=NULL){
			if(inSSa[i]==z->index){
				printf("z->index%d\n",z->id);
				break;
			}
			z=z->next;
		}
	}
	/*	
	while(k!=NULL){
		dd++;
			printf("%d %d \n",k->id, k->index);
		k=k->next;
	
	}


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
*/
}



