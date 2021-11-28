#include<stdlib.h>
#include<stdio.h>
typedef struct AvlNode
{
	int data;
	struct AvlNode *Left; 
	struct AvlNode *Right;
	int Height; 
}AvlNode,*Position,*AvlTree;
AvlTree MakeEmpty(AvlTree T)
{
	if(T!=NULL)
	{
		MakeEmpty(T->Left);
		MakeEmpty(T->Right);
		free(T);
	}
	return NULL;
}
// 返回指向具有关键字X信息的结点的指针(在二叉排序树中么) 
Position Find(int X,AvlTree T)
{
    if(T==NULL)return NULL;	
    if(X<T->data)
        return Find(X,T->Left);
    else if(X>T->data)
        return Find(X,T->Right);
    else 
        return T;
} 
Position FindMin(AvlTree T)
{
	if(T==NULL) return NULL;
	else if(T->Left==NULL)  return T;
	else return FindMin(T->Left);
}
//寻找最大值 
Position FindMax(AvlTree T)
{
	if(T!=NULL)
	while(T->Right!=NULL)
	    T=T->Right;
	
	return T;
}
int Max(int Lhs,int Rhs){return Lhs>Rhs?Lhs:Rhs;}
int Height(AvlTree T)
{
	if(T==NULL)
	return-1;
	else
	return T->Height;
}
static Position SingleRotateWithLeft(Position K2)
{
    Position K1;
	K1=K2->Left;
	K2->Left=K1->Right;
	K1->Right=K2;
	
	K2->Height=Max(Height(K2->Left),Height(K2->Right))+1;
	K1->Height=Max(Height(K1->Left),K2->Height)+1;	
	return K1;
}
static Position SingleRotateWithRight(Position K1)
{
    Position K2;
	K2=K1->Right;
	K1->Right=K2->Left;
	K2->Left=K1;
	
	K1->Height=Max(Height(K1->Left),Height(K1->Right))+1;
	K2->Height=Max(Height(K2->Right),K1->Height)+1;	
	return K2;
}
static Position DoubleRotateWithLeft(Position K3)
{
	
	K3->Left=SingleRotateWithRight(K3->Left);
	return SingleRotateWithLeft(K3);
}
static Position DoubleRotateWithRight(Position K1)
{
	
	K1->Right=SingleRotateWithLeft(K1->Right);
	return SingleRotateWithRight(K1);
}
//插入 
AvlTree Insert(int x,AvlTree T)
{
	if(T==NULL)
	{
		T=(AvlNode*)malloc(sizeof(AvlNode));
		if(T==NULL)
		     printf("Out of space!!!");
		else{
		T->data=x;
		T->Height=0;
		T->Left=T->Right=NULL;
	    }
	}
	else
	if(x<T->data)
	{
		T->Left=Insert(x,T->Left);
		if(Height(T->Left)-Height(T->Right)==2)
		    if(x<T->Left->data)
		    T=SingleRotateWithLeft(T);
		    else
		    T=DoubleRotateWithLeft(T);
	}
	else
	if(x>T->data)
	{
		T->Right=Insert(x,T->Right);
		if(Height(T->Right)-Height(T->Left)==2)
		   if(x>T->Right->data)
		       T=SingleRotateWithRight(T);
		    else
		       T=DoubleRotateWithRight(T);
    }
    T->Height=Max(Height(T->Left),Height(T->Right))+1;
    return T;	       
		       
		       
	}
	
AvlTree Delete(int x,AvlTree T)
{
    Position temp;
    if(T==NULL) printf("no delete");
    else if(x<T->data)
    {
    	T->Left=Delete(x,T->Left);
    	if(Height(T->Right)-Height(T->Left)==2)
    	{
    		if(Height(T->Right->Right)>Height(T->Right->Left))
    		    T=SingleRotateWithRight(T);
    		else
    		    T=DoubleRotateWithRight(T);
		}
		T->Height=Max(Height(T->Left),Height(T->Right))+1;
	}
	else if(x>T->data)
	{
		T->Right=Delete(x,T->Right);
		if(Height(T->Left)-Height(T->Right)==2)
		{
			if(Height(T->Left->Left)>Height(T->Left->Right))
			    T=SingleRotateWithLeft(T);
			else
			    T=DoubleRotateWithLeft(T);
		}
		T->Height=Max(Height(T->Left),Height(T->Right))+1;
	}
	else if(T->Left&&T->Right)
	{
		temp=FindMin(T->Right);
		T->data=temp->data;
		T->Right=Delete(T->data,T->Right);
		T->Height=Max(Height(T->Left),Height(T->Right))+1;
	}
	else
	{
		temp=T;
		if(T->Left==NULL)
		{
			T=T->Right;
		}
		else if(T->Right==NULL)
		{
			T=T->Left;
		}
		free(temp);
	}
	return T;
} 
int countNodes(AvlTree t) {
	if (t == NULL)
		return 0;
	return countNodes(t->Left) + countNodes(t->Right) + 1;
}
 
int countLeaves(AvlTree t) {
	if (t == NULL)
		return 0;
	else if (t->Left == NULL&&t->Right == NULL)
		return  1;
	else
		return countLeaves(t->Left) + countLeaves(t->Right);
}
 
int countFull(AvlTree t) {
	if (t == NULL)
		return 0;
	else if (t->Left &&t->Right)
		return   countFull(t->Left) + countFull(t->Right) + 1;
	else
		return countFull(t->Left) + countFull(t->Right);
}
int Retrieve(Position P)
{
	return P->data;
} 
int main()
{
    AvlTree T;
	Position P;
	int i,j=0;
	T=MakeEmpty(NULL);
	for(i=0;i<50;i++,j=(j+7)%50)
	   T=Insert(j,T);
	for(i=0;i<50;i++)
	   if((P=Find(i,T))==NULL||Retrieve(P)!=i)
	    printf("Error at %d\n",i);
	for(i=0;i<50;i+=2)
	    T=Delete(i,T);
	printf("Min is %d,Max is %d\n",Retrieve(FindMin(T)),Retrieve(FindMax(T)));
	printf("T中节点个数：%d\n",countNodes(T));
	printf("T中树叶片数：%d\n",countLeaves(T));
	printf("T中满节点个数个数：%d\n",countFull(T));
	return 0;	
	
} 















