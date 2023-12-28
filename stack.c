/* 인터페이스 구현 파일 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"


/* 내부 지원 함수 프로토타입 */
static Node * searching(const Stack *, Item);    //Stack에서 원하는 정보를 가진 노드를 찾아서 그 노드의 주소를 리턴. (입력된 순서 역순으로 하나씩 뒤져봐야함.)
static Node * makenode(Item item);          //새로운 노드를 만들어서 그 노드에 정보(item)을 저장하고 그 노드의 주소를 리턴.
static Node * findparent(const Stack *, Item);    //그 노드를 가리키는 노드를 찾는다.
static bool AddNode(Stack * pstack, Node * new_node);                       
inline static void empty(void);                 //버퍼를 비우는 인라인 함수.

/* ****caution ***** */
/* findparent()를 사용할때는 찾고자 하는 노드가 첫번째 노드가 아님을 가정한다. */





/* ****** 인터페이스 함수 구현 ****** */
void Initialize(Stack * pstack)             //초기화
{
    pstack->head = NULL;
    pstack->size = 0;
}

bool StackIsEmpty(const Stack * pstack)         //비어있는지
{
    return ((pstack->head == NULL && pstack->size == 0) ? true : false);
}

bool StackIsFull(const Stack * pstack)              //꽉찼는지
{
    return pstack->size == SSIZE;
}

int StackItemCount(const Stack * pstack)                //현재 스택에 들어있는 정보 갯수를 리턴한다. 
{
    return pstack->size;
}


bool Push(Stack * pstack, Item si)                                           //bool Push(Stack * pstack, Item si)로 이름변경    
{
    Node * new_node;
    
    if ((new_node = makenode(si)) == NULL)     //여기서 새로운 노드를 만든다.
    {
        fprintf(stderr, "노드 만들기를 실패했습니다.\n");
        return false;
    }
    if (StackIsFull(pstack))
    {
        fprintf(stderr, "스택에 빈자리가 없습니다.\n");
        free(new_node);
        return false;
    }                                            
    if (searching(pstack, si))                   
    {
        fprintf(stderr, "해당 숫자는 이미 저장되어 있습니다.\n");
        free(new_node);
        return false;
    }
    AddNode(pstack, new_node);                                                                                              

    return true;
}

void POP(Stack * pstack)
{
    Node * temp;                        

    if (StackIsEmpty(pstack))
    {
        fpritnf(stderr, "Stack이 비어있습니다.\n");
        return;
    }
    printf("%d\n", pstack->head);
    temp = pstack->head;
    pstack->head = pstack->head->prev;
    free(temp);
}


bool DeleteNode(Stack * pstack, Item si)
{
    Node * t_node;                          //target node
    Node * parent;                          //target node가 중간에 끼어있을때 사용.
    int temp;
    
    if (!(t_node = searching(pstack, si)))
    {
        fprintf(stderr, "해당 정보를 저장하고 있는 노드가 존재하지 않습니다.\n");
        return false;
    }
    /* 해당 정보를 저장하고 있는 노드가 존재함을 확인하면 */
    if (t_node == pstack->head)   //if target node is head node
    {
        pstack->head = t_node->prev;                  
        free(t_node);                                                
        pstack->size--;
    }
    else if (t_node->prev == NULL)              //if target node is tail node
    {
        parent = findparent(pstack, si);        //tail node의 부모노드를 찾는다.
        free(t_node);                           //target node를 메모리에서 해제한다.
        parent->prev = NULL;                    //기존의 tail node의 부모노드를 tail node로 새로 지정.
        pstack->size--;
    }
    else   //target이 중간에 끼어 있으면
    {
        parent = findparent(pstack, si);
        parent->prev = t_node->prev;
        free(t_node);
        pstack->size--;
    }

    return true;
}


void DeleteAll(Stack * pstack)                  //use recursion
{
    Node * pnode = pstack->head;                //Initializing pnode = Stack's head node              
    Node * tnode;                               //target node : 메모리에서 해제시킬 노드.
    
    while (pstack->size != 0)                 //pstack->size == 0이면 모든 노드들이 삭제되었다는 뜻이므로 해당 루프문의 조건문을 이렇게 짰다.
    {
        tnode = pnode;
        pnode = pnode->prev;
        free(tnode);
        pstack->size--;
    } 
}       
/**** recursion을 이용한 DeleteAll()구현에서는 전달인자를 Stack * 으로 다른 함수 인터페이스들과 일관성을 맞추기 힘들다. 
 *      따라서 다른 함수 인터페이스들과의 일관ㅌ을 위해서 recursion이 아닌 다른 방법으로 DeleteAll()을 구현해야한다.
*/

void InStack(Stack * pstack, Item si)
{
    Node * target;

    target = searching(pstack, si);

    if (target == NULL)
        printf("%d는 저장되어 있지 않습니다.\n", si);
    else
        printf("%d는 저장되어 있습니다.\n", si);
}

void ShowAll(const Stack * pstack)
{
    Node * temp;

    temp = pstack->head;
    while (temp)    
    {
        printf("%d \n", temp->item);
        temp = temp->prev;
    }
}

void Traverse(Node * pnode, void (* pfun)(Item n))
{
    if (pnode->prev != NULL)
        Traverse(pnode->prev, pfun);
    (*pfun)(pnode->item);                   //마찬가지로 recursion을 이용해서 맨처음 추가한 노드부터 함수 적용.
}

bool GetItem(Item * si)
{
    puts("저장하고자 하는 정보를 입력하시오:");
    if (scanf("%d", si) == 1)
    {
        empty();
        return true;
    }
    else
    {
        puts("올바르지 않은 입력입니다.");
        empty();
        return false;
    }
}




/* ***** 지원 함수 ***** */
static Node * findparent(const Stack * pstack, Item si)
{
    Node * parent;
    
    parent = pstack->head;
    
    while (parent->prev)     //parent가 마지막 노드가 아니면 계속 루프 진행.
        if (parent->prev->item == si)
            break;
        else
            continue;           //if-else문은 하나의 문장으로 C가 받아들이기 때문에 중괄호 필요 없다 하지만 if문이 while()루프의 몸체라는 사실을 명확히 하기 위해 중괄홀르 씌워주자.         
    

    return parent;              //리턴값이 NULL이면 si를 가리키는 노드를 가리키는 노드가 없음.
}

static Node * searching(const Stack * pstack, Item si)
{
    Node * temp;
     
    temp = pstack->head;
    while (temp != NULL)
        if (temp->item == si)
            break;
        else
        
            temp = temp->prev;          //if - else문은 전체가 하나의 문장으로 카운트 된다.

    return temp;                    
}   //원하는 item이 들어있는 노드를 찾으면 그 노드를 리턴. Item을 못찾았으면 temp의 값은 NULL

static Node * makenode(Item si)             //si : stack item
{
    Node * new_node;
    
    if ((new_node = (Node *)malloc(sizeof(Node))) == NULL)
        return NULL;
    new_node->item = si;

    return new_node;
}

static bool AddNode(Stack * pstack, Node * new_node)
{
    if (StackIsEmpty(pstack))           //추가하는 노드가 Head node.
    {
        puts("첫번째 노드를 성공적으로 추가하였습니다.");
        pstack->head = new_node;
        pstack->size++;
    }
    else
    {
        new_node->prev = pstack->head;            //새로만들어진 노드와 기존 노드를 연결
        pstack->head = new_node;                  //Stack의 recnet멤버가 new_node를 가리키도록 갱신
        pstack->size++;                             //Stack size 갱신
        puts("노드를 성공적으로 만들었습니다.");
    } 
    
}

inline static void empty()
{
    while (getchar() != '\n')
        continue;
}



/* ***** 코드 개선 일지 ***** */
/*
2023년 10/15 : DeleteNode()코드 오류 발견 --> 11.6 오류 수정함
2023년 11/8 : AddNode() 전면 수정 필요함. 

    Item 입력과 AddNode 작업 분리해야함
        1. 전달인자를 2개로 (사용할 스택, 저장할 Item)
        2. Item즉 노드에 저장할 정보를 입력받는 도움함수를 헤더파일과 인터페이스 구현파일에 추가하자  Node * additem(Item) : Item을 입력받아서 새로운 노드를 만든다.
        3. AddNode는 새로 만들어진 노드를 전달인자로 받아서 Stack에 추가하는 역할만 한다.
        
        --> 이렇게 짜야지 stack.h에 있는 Item의 데이터형만 바꿔가면서 여러 프로그램에 적용 가능함. (for utility)
    
2023년 11/10 : Traverse()의 pfun함수포인터 전달인자를 int형에서 Item형으로 바꿈
        ..     다음 입력을 위해 버퍼를 비우는 인라인 함수 empty()추가.

2023년 11/11 : 정보 입력, 노드 생성, 생성한 노드 Stack에 추가. 각 단계를 분리
                get_item() : 저장할 정보를 받음.
                makenode() : 입력받은 정보를 토대로 노드를 만듦.
                AddNode() : Stack에 노드 추가 

2023년 11/11 : DeleteAll() 연산자 오류 발견. 
                1. 다른 연산자와 마찬가지로 Stack의 주소를 전달인자로 받게하여서 다른 연산자들의 전달인자의 데이터형과 일관성을 띄게 한다.
                2. 재귀 과정에 할당된 메모리를 해제함과 동시에 Stack size또한 갱신해야한다.
..     11/12 : DeleteAll() 수정 완료

2023년 12/28 : Stack ADT연산 이름 변경 및 모듈화 수정사안.
                1. 기존의 AddNode()를 Push()로 이름을 바꾸고 AddNode()를 내부지원 함수로 돌린다.
                2. 이후에 POP연산을 구현한다. (맨 위의 것만 POP하는 것과 Stack 전체를 POP하는 연산 둘 다 구현한다.)
                3. line 68 ~ line 79까지 하나로 묶어서 AddNode()내부지원 함수를 새로 만들어서 그걸로 대체
                4. ShowNode()를 InStack()으로 이름변경
                5. fprintf()이용해서 오류가 났을때 어디서 어떤 오류가 발생했는지 standard error stream을 이용해서 디스플레이에 표시하도록 함.
*/




/*
재귀를 이용한 DeleteNode()구현
단, recursion을 이용해서 구현할때는 전달인자를 Stack *이 아닌 Node * 형 이어야 한다. --> 다른 인터페이스 구현 함수들과 통일성이 떨어짐. 그에따라 사용자가 신경써야할 점이 하나 는다.

void DeleteAll(Stack * pstack)                  //use recursion
{
    Node * pnode = pstack->head;                //Initializing pnode = Stack's head node              
    
    if(pnode->prev != NULL)
        DeleteAll(pnode->prev);                 //recursion
    free(pnode);   
    pstack->size--;                             
}   //recursion 이용. --> recursion은 순서의 역전이 필요할때 유용하게 사용할 수 있다.
        //제일 처음 입력된 노드까지 갔다가 재귀를 종료하고 원래의 차원으로 돌아오면서 맨 처음(스택의 맨 밑에 있는 노드)부터 차례대로 메모리를 해제한다.
     recursion을 이용한 DeleteAll()구현에서는 전달인자를 Stack * 으로 다른 함수 인터페이스들과 일관성을 맞추기 힘들다. 
    따라서 다른 함수 인터페이스들과의 일관성을 위해서 recursion이 아닌 다른 방법으로 DeleteAll()을 구현해야한다.
*/
