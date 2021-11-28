#include<stdlib.h>
#include<stdio.h>
struct TreeNode;
typedef struct TreeNode *Position;
typedef struct TreeNode *SearchTree; 
struct TreeNode
{
	int data;
	SearchTree Left;
	SearchTree Right;
};
//建立一棵空树 
SearchTree MakeEmpty (SearchTree T)
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
Position Find(int X,SearchTree T)
{
    if(T==NULL)return NULL;	
    if(X<T->data)
        return Find(X,T->Left);
    else if(X>T->data)
        return Find(X,T->Right);
    else 
        return T;
} 
//寻找最小值 
Position FindMin(SearchTree T)
{
	if(T==NULL) return NULL;
	else if(T->Left==NULL)  return T;
	else return FindMin(T->Left);
}
//寻找最大值 
Position FindMax(SearchTree T)
{
	if(T!=NULL)
	while(T->Right!=NULL)
	    T=T->Right;
	
	return T;
}
//插入节点 
SearchTree Insert(int x,SearchTree T)
{
    if(T==NULL)
    {
	T=(TreeNode*)malloc(sizeof(TreeNode));
	if(T==NULL)
	printf("out of space");
	else
	{
	T->data=x;
	T->Left=NULL;
	T->Right=NULL;
    }
	}
    else if(x<T->data)
    //return Insert(x,T->Left);
    T->Left=Insert(x,T->Left);
	else if(x>T->data)
	//return Insert(x,T->Right);
	T->Right=Insert(x,T->Right);
	
	return T;  
}
//删除结点
SearchTree Delete(int x,SearchTree T)
{
	Position Tmp;
    if(T==NULL) printf("no delete");
	else if(x<T->data)
	    T->Left=Delete(x,T->Left);
	else if(x>T->data)
	    T->Right=Delete(x,T->Right);
	else if(T->Left&&T->Right)
	{
		Tmp=FindMin(T->Right);
		T->data=Tmp->data;
	    T->Right=Delete(T->data,T->Right);
	}
	else
	{
		Tmp=T;
		if(T->Left==NULL)
		    T=T->Right;
		else if(T->Right==NULL)
		    T=T->Left;
		free(Tmp); 
	}
	return T;
}
int Retrieve(Position P)
{
	return P->data;
} 
int main()
{
	SearchTree T;
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
	    for(i=1;i<50;i+=2)
	    if((P=Find(i,T))==NULL||Retrieve(P)!=i)
	    printf("Error at %d\n",i);
	    
	    for(i=0;i<50;i+=2)
	    if((P=Find(i,T))!=NULL)
	    printf("Error at %d\n",i);
	    
	    printf("Min is %d,Max is %d\n",Retrieve(FindMin(T)),Retrieve(FindMax(T)));
	    return 0;
	    
	    
} 
