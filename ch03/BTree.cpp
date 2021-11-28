
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
typedef struct _btree_node_t  
{  
    int num;                      /*�ؼ��ָ���*/ 
    int *key;                     /* �ؼ��֣���ռ�ռ�Ϊ(max+1) - �������1���ռ����ڽ����ռ�ʹ�� */  
    struct _btree_node_t **child;  /* �ӽ�㣺��ռ�ռ�Ϊ��max+2��- �������1���ռ����ڽ����ռ�ʹ�� */   
    struct _btree_node_t *parent;  /* ����� */  
}btree_node_t;  
 
typedef struct  
{  
    int max;                /* ����������ؼ��ָ��� - ��m=max+1 */        
    int min;                /* ���������С�ؼ��ָ��� min=��m/2������ȡ��-1*/         
    int sidx;               /* �������� = (max+1)/2 */        
    btree_node_t *root;     /* B��������ַ */           
}btree_t;  
 
static int btree_merge(btree_t *btree, btree_node_t *node);
static int _btree_merge(btree_t *btree, btree_node_t *left, btree_node_t *right, int mid);
//�ڵ㴴��
static btree_node_t *btree_creat_node(btree_t *btree)  
{  
    btree_node_t *node = NULL;    
    node = (btree_node_t *)calloc(1, sizeof(btree_node_t));  
    if(NULL == node) {  
        fprintf(stderr, "[%s][%d] errmsg:[%d] %s\n", __FILE__, __LINE__, errno, strerror(errno));  
        return NULL;  
    }  
  
    node->num = 0;  //���ؼ��ָ�����ʼ��Ϊ0
  
    /* ����max+1���ռ䣬���ؼ��ָ�������max+1ʱִ�з��Ѳ��� */  
    node->key = (int *)calloc(btree->max+1, sizeof(int));  
    if(NULL == node->key) {  
        free(node), node=NULL;  
        fprintf(stderr, "[%s][%d] errmsg:[%d] %s\n", __FILE__, __LINE__, errno, strerror(errno));  
        return NULL;  
    }  
  
    /* ����max+2���ӽڵ�ռ䣬��Ϊ�ӽڵ�ÿ����һ�Σ����ڵ��һ���ؼ��֣����Ե����ڵ���Ҫ����ʱ���������ӽڵ����Ϊmax+2�� */  
    node->child = (btree_node_t **)calloc(btree->max+2, sizeof(btree_node_t *));  
    if(NULL == node->child) {  
        free(node->key);  
        free(node), node=NULL;  
        fprintf(stderr, "[%s][%d] errmsg:[%d] %s\n", __FILE__, __LINE__, errno, strerror(errno));  
        return NULL;  
    }  
  
    return node;  
}  
 
 
btree_t* btree_creat(int m)  
{  
    btree_t *btree = NULL;  
  
    if(m < 3) { //��������С��3�� 
        fprintf(stderr, "[%s][%d] Parameter 'max' must geater than 2.\n", __FILE__, __LINE__);  
        return NULL;  
    }  
  
    btree = (btree_t *)calloc(1, sizeof(btree_t));  
    if(NULL == btree) {  
        fprintf(stderr, "[%s][%d] errmsg:[%d] %s!\n", __FILE__, __LINE__, errno, strerror(errno));  
        return NULL;  
    }  
  
    btree->max= m - 1;  
    btree->min = m/2;  
    if(0 != m%2) {  
        btree->min++;  
    }  
    btree->min--;  //min=��m/2������ȡ��-1
    btree->sidx = m/2;  //��������
    btree->root = NULL; 
  
    return btree;  
} 
 
//���ѽ��
static int btree_split(btree_t *btree, btree_node_t *node)  
{  
    int idx = 0, total = 0, sidx = btree->sidx;  
    btree_node_t *parent = NULL, *newNode = NULL;   
  
  
    while(node->num > btree->max) {    
        total = node->num;  
  
        newNode = btree_creat_node(btree);  //����һ���½������������sidx+1��ʼ�Ĺؼ���
        if(NULL == newNode) {         
            fprintf(stderr, "[%s][%d] Create node failed!\n", __FILE__, __LINE__);  
            return -1;  
        }  
  
  
        memcpy(newNode->key, node->key + sidx + 1, (total-sidx-1) * sizeof(int));  //��sidx֮��Ĺؼ��ַ����½����
        memcpy(newNode->child, node->child+sidx+1, (total-sidx) * sizeof(btree_node_t *));  //��child[sidx]֮����ӽ��ŵ�newNode->child��
	
 
  
        newNode->num = (total - sidx - 1);  //�����½��Ĺؼ��ָ���
        newNode->parent  = node->parent;    //��node��newNode��Ϊԭnode���ڵ���ӽ��
  
        node->num = sidx;   
		
        parent  = node->parent;  
        if(NULL == parent)  {   //���nodeû�и��ڵ�      
  
            parent = btree_creat_node(btree);  //�½�һ�������Ϊ���ڵ�
            if(NULL == parent) {         
                fprintf(stderr, "[%s][%d] Create root failed!", __FILE__, __LINE__);  
                return -1;  
            }         
  
            btree->root = parent;   //���¸����
			//��node,newNode��Ϊ�½����ڵ���ӽ��
            parent->child[0] = node;  
			parent->child[1] = newNode; 	
            node->parent = parent;   
            newNode->parent = parent;   
  
            parent->key[0] = node->key[sidx];  //�ѷ����������ڵĹؼ��ַŵ��ø��ڵ���
 
            parent->num++;  
        }         
        else {         
			//���nodeԭ�����и��ڵ㣬��ֱ�Ӱ�key[sidx]���뵽���ڵ���
            for(idx=parent->num; idx>0; idx--) {      //�ҵ����ڵ��б�key[sidx]�����С�ؼ��֣���key[sidx]�嵽�ùؼ��ֵ�ǰ��   
                if(node->key[sidx] < parent->key[idx-1]) {   //��parent->key��ÿ���ҵ�һ����key[sidx]��ľ��ٱȽ�ǰһ��      
                    parent->key[idx] = parent->key[idx-1];  //ÿ�ҵ�һ�����ͰѸùؼ��ֵ�ֵ���͸ùؼ����·����Һ��ӽ�������һλ
                    parent->child[idx+1] = parent->child[idx];  
                    continue;  //��������forѭ��
                } 
                break; 
				//ֱ���ҵ���key[sidx]�����Сparent->key������
				//��ʱ��parent->key[idx]��ʾnode->key[sidx]Ҫ����ĵط�,parent->child[idx+1]��newNodeҪ����ĵط�
            }         
  
            parent->key[idx] = node->key[sidx];  //������ѹؼ���
            parent->child[idx+1] = newNode;      //�����½��
            newNode->parent = parent;            
            parent->num++;  
        }         
  
      //  memset(node->key+sidx, 0, (total - sidx) * sizeof(int));  
      //  memset(node->child+sidx+1, 0, (total - sidx) * sizeof(btree_node_t *));   
  
        /* Change node2's child->parent */  
      //  for(idx = 0; idx <= newNode->num; idx++) {  
      //      if(NULL != newNode->child[idx]) {         
      //          newNode->child[idx]->parent = newNode;  
      //      }         
      //  }    
				memset(node->key+sidx, 0, (total - sidx) * sizeof(int));  
        memset(node->child+sidx+1, 0, (total - sidx) * sizeof(btree_node_t *)); 
		//�����Ƹ�newNode���ӽ���е�parentָ��newNode
		for(idx = 0; idx <= newNode->num; idx++) {  
            if(NULL != newNode->child[idx]) {         
                newNode->child[idx]->parent = newNode;  
            }         
        } 
       node = parent;   
    }  
  
    return 0;  
}  
 
static int _btree_insert(btree_t *btree, btree_node_t *node, int key, int idx) 
 //����Ҫ����ؼ��ֵĽ�㣬idx��ʾ���Լ���Ľ���λ�ã�Ҳ����keyҪ�����λ��
{  
    int i = 0;  
  
  
    for(i=node->num; i>idx; i--) {  
        node->key[i] = node->key[i-1];  //����ؼ���
    }  
  
    node->key[idx] = key; 
    node->num++;  
  
    if(node->num > btree->max) {  //���ý��ؼ��ָ����������ؼ��ָ���ʱ��ִ�з��Ѳ���
        return btree_split(btree, node);  
    }  
  
    return 0;  
}  
 
//�������
int btree_insert(btree_t *btree, int key)  
{  
    int idx = 0;  //key����
    btree_node_t *node = btree->root;  
 
    if(NULL == node) {  //���Ϊ�������ʹ�����һ�����
        node = btree_creat_node(btree);  
        if(NULL == node) {  
            fprintf(stderr, "[%s][%d] Create node failed!\n", __FILE__, __LINE__);  
            return -1;  
        }  
        //��һ����㴴���ɹ��󣬲���key
        node->num = 1;   
        node->key[0] = key;  
        node->parent = NULL;  
  
        btree->root = node;  
        return 0;  
    }  
  
  
    while(NULL != node) {  
        for(idx=0; idx < node->num; idx++) {  
            if(key == node->key[idx]) {  //���Ҫ����������Ѿ����ھͲ���Ҫ����
                fprintf(stderr, "[%s][%d] The node is exist!\n", __FILE__, __LINE__);  
                return 0;  
            }  
             else if(key < node->key[idx]) {  //�ҵ���һ�����Լ���Ĺؼ���,û�еĻ��ͼ���idx++
                break;  
            }  
        }  
        //����ý������ӽ�㣬��ʱ��idxҲ��ʾ�ӽ�������
		//�ӽ��������0��ʼ
		//child[idx]<key[idx]<child[idx+1]
        if(NULL != node->child[idx]) {  //�������ؼ����ж�Ӧ���ӽڵ㣬�ͽ�������ӽڵ㣬������ӽڵ���ִ����������
            node = node->child[idx];  
        }  
        else {  
            break;  //ֱ���ҵ�һ�����Լ������û���ӽ��Ĺؼ��֣�ִ�в���������嵽����ؼ��ֵ�ǰ��
        }  
    }  
  
 
    return _btree_insert(btree, node, key, idx);  //ִ�в������
}  
static int _btree_merge(btree_t *btree, btree_node_t *left, btree_node_t *right, int mid)  
{  
    int m = 0;  
    btree_node_t *parent = left->parent;  
  
    left->key[left->num++] = parent->key[mid];  
  
    memcpy(left->key + left->num, right->key, right->num*sizeof(int));  
    memcpy(left->child + left->num, right->child, (right->num+1)*sizeof(btree_node_t *));  
    for(m=0; m<=right->num; m++) {  
        if(NULL != right->child[m]) {  
            right->child[m]->parent = left;  
        }  
    }  
    left->num += right->num;  
  
    for(m=mid; m<parent->num-1; m++) {  
        parent->key[m] = parent->key[m+1];  
        parent->child[m+1] = parent->child[m+2];  
    }  
  
    parent->key[m] = 0;  
    parent->child[m+1] = NULL;  
    parent->num--;  
    free(right);  
  
    /* Check */  
    if(parent->num < btree->min) {  
        return btree_merge(btree, parent);  
    }  
  
    return 0;  
}  
 
static int btree_merge(btree_t *btree, btree_node_t *node)  
{  
    int idx = 0, m = 0, mid = 0;  
    btree_node_t *parent = node->parent, *right = NULL, *left = NULL;  
 
    if(NULL == parent) {  
        if(0 == node->num) {  
            if(NULL != node->child[0]) {  
                btree->root = node->child[0];  
                node->child[0]->parent = NULL;  
            }  
            else {  
                btree->root = NULL;  
            }  
            free(node);  
        }  
        return 0;  
    }  
  
    for(idx=0; idx<=parent->num; idx++) {  
        if(parent->child[idx] == node) {  
            break;  
        }  
    }  
  
    if(idx > parent->num) {  
        fprintf(stderr, "[%s][%d] Didn't find node in parent's children array!\n", __FILE__, __LINE__);  
        return -1;  
    }  
 
    else if(idx == parent->num) {  
        mid = idx - 1;  
        left = parent->child[mid];  
 
        if((node->num + left->num + 1) <= btree->max) {  
            return _btree_merge(btree, left, node, mid);  
        }  
  
 
        for(m=node->num; m>0; m--) {  
            node->key[m] = node->key[m - 1];  
            node->child[m+1] = node->child[m];  
        }  
        node->child[1] = node->child[0];  
  
        node->key[0] = parent->key[mid];  
        node->num++;  
        node->child[0] = left->child[left->num];  
        if(NULL != left->child[left->num]) {  
            left->child[left->num]->parent = node;  
        }  
  
        parent->key[mid] = left->key[left->num - 1];  
        left->key[left->num - 1] = 0;  
        left->child[left->num] = NULL;  
        left->num--;  
        return 0;  
    }  
      
 
    mid = idx;  
    right = parent->child[mid + 1];  
 
    if((node->num + right->num + 1) <= btree->max) {  
        return _btree_merge(btree, node, right, mid);  
    }  
  
    node->key[node->num++] = parent->key[mid];  
    node->child[node->num] = right->child[0];  
    if(NULL != right->child[0]) {  
        right->child[0]->parent = node;  
    }  
  
    parent->key[mid] = right->key[0];  
    for(m=0; m<right->num; m++) {  
        right->key[m] = right->key[m+1];  
        right->child[m] = right->child[m+1];  
    }  
    right->child[m] = NULL;  
    right->num--;  
    return 0;  
}  
 
static int _btree_delete(btree_t *btree, btree_node_t *node, int idx)  
{  
    btree_node_t *orig = node, *child = node->child[idx];  
   
    while(NULL != child) {  
        node = child;  
        child = node->child[child->num];  
    }  
  
    orig->key[idx] = node->key[node->num - 1];  
  
 
    node->key[--node->num] = 0;  
    if(node->num < btree->min) {  
        return btree_merge(btree, node);  
    }  
  
    return 0;  
}  
 
int btree_delete(btree_t *btree, int key)  
{  
    int idx = 0;  
    btree_node_t *node = btree->root;  
  
  
    while(NULL != node) {  
        for(idx=0; idx<node->num; idx++) {  
            if(key == node->key[idx]) {  
                return _btree_delete(btree, node, idx);  
            }  
            else if(key < node->key[idx]) {  
                break;  
            }  
        }  
  
        node = node->child[idx];  
    }  
  
    return 0;  
}  
 
void Inorder(btree_node_t *root,int deep){
    int i,j,k,a=1;
    if(root != NULL)
    {
       if(deep){
        printf("\n");
       }
       for(j = 0;j < deep;j++){
        printf("---");  
       }
       for(i = 0; i <= root->num;i++){
           if(a){
        printf("< %d | ",root->num);
        for( k = 0;k < root->num;k++){
            printf("%d ",root->key[k]);
        }
        a--;
        printf(">");
          }
        Inorder(root->child[i],deep+1);
       }
       printf("\n");
    }
}
 
int main(){
   btree_t *bt;
   int i;
   int a[21]={3,4,44,12,67,98,32,43,24,100,34,55,33,13,25,8,5,41,77,200};
   bt = btree_creat(4);
   for(i = 0;i < 20;i++){
    printf("insert %d: %d\n",i+1,a[i]);
    btree_insert(bt,a[i]);
    Inorder(bt->root,0);
    printf("\n");
   }
 
   for(i = 0;i < 10;i++){
    printf("delete %d: %d\n",i+1,a[i]);
    btree_delete(bt,a[i]);
    Inorder(bt->root,0);
   }
   return 0;
}
