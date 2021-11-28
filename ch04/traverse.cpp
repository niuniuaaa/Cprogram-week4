#include <stdio.h>
#include <stdlib.h>
 
// -------- ���� ����-�ֵ� �洢��ʾ�� ------
struct Node{
	char data;
	Node *child;
	Node *brother;
}*CSTree;
 
void PreOrder(Node *head)		// ǰ�����
{
	if(head==NULL)return;
	printf("%c ",head->data);
	PreOrder(head->child);
	PreOrder(head->brother);
}
 
void InOrder(Node *head)		// �������
{
	if(head==NULL)return;
	InOrder(head->child);
	printf("%c ",head->data);
	if(head->child)InOrder(head->child->brother);
}
 
void PostOrder(Node *head)		// �������
{
	if(head==NULL)return;
	PostOrder(head->child);
	printf("%c ",head->data);
	PostOrder(head->brother);
}
 
void AddNode(Node *parent,Node *child)		// ����ӽڵ�
{
	if(parent->child!=NULL)				//���parent ����child�����child���brother
		parent->child->brother=child;
	else parent->child = child;			//����ֱ�����child
}
 
int main()
{
	int NodeNum = 9;
	CSTree = (Node *)malloc(NodeNum*sizeof(Node));
	int i;
	char d = 'A';
	for(i=0;i<NodeNum;i++)			// ��ʼ�����ڵ� dataֵΪ A - I
	{
		CSTree[i].child = NULL;
		CSTree[i].brother = NULL;
		CSTree[i].data = d;
		d++;
	}
 
	AddNode(&CSTree[0],&CSTree[1]);		// A->B
	AddNode(&CSTree[0],&CSTree[2]);		// A->C
	AddNode(&CSTree[1],&CSTree[3]);		// B->D
	AddNode(&CSTree[1],&CSTree[4]);		// B->E
	AddNode(&CSTree[2],&CSTree[5]);		// C->F
	AddNode(&CSTree[3],&CSTree[6]);		// D->G
	AddNode(&CSTree[5],&CSTree[7]);		// F->H
	AddNode(&CSTree[5],&CSTree[8]);		// F->I
	
	printf("ǰ������� ");
	PreOrder(&CSTree[0]);
	printf("\n");
 
	printf("��������� ");
	InOrder(&CSTree[0]);
	printf("\n");
 
	printf("��������� ");
	PostOrder(&CSTree[0]);
	printf("\n");
	return 0;
}
