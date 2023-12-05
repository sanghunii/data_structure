/* stack adt interface header file */

/* 사용자의 편의성을 위해서 모든 전달인자는 포인터로 통일 */
/* 안에 저장된 정보를 다뤄야 하면 Item * */
/* Stack자체(각 노드들) 를 다뤄야 하면 Stack * */
/* ********** First in Last out ********* */

/* ****** Caution ****** */
/*
1. 노드에 입력하고자 하는 정보에 따라 Item의 데이터형 수정해서 사용 요함. (header file)
2. 1과 마찬가지로 GetItem()을 수정해서 사용 요함. (source code file)
*/

#ifndef STACK_H_                        //조건부 컴파일 -> 파일 중복 포함 방지
#define STACK_H_
#include <stdbool.h>



/* 스택의 최대 크기 */
#define SSIZE 10                         //Stacksize




/* 데이터형 정의 */
typedef int Item;                       //Stack의 각 노드가 저장하는 정보(Item)의 데이터형. 구현하고자 하는 프로그램에 맞춰서 적절하게 데이터 타입을 변경해서 사용하면 됨.

typedef struct node {                          //Stack을 구성하는 노드
    Item item;                          //저장된정보
    struct node * prev;                        //이전에 입력된 노드를 가리키는 포인터.
} Node;                              

typedef struct stack {                         //Stack
    Node * head;                      //가장 최근에 생성된 노드 (출력 1순위)          //2023년 11.6 코드 수정 노트 : 변수 이름을 recent보다 head로 하는게 현재 출력 1순위 노드를 나타내는데에 더 효율적이지 않을까? (head node를 가리키니깐 !!)
    int size;                           //Stack의 현재 크기 (<=SSIZE)
} Stack;                //recent == NULL && size == 0  --> stack is empty
//2023년 11.8 --> Stack에서 출력 1순위 노드의 이름을 recent에서 head로 변경함.




/* Stack interface functions prototype*/

/* 연산 : 스택을 초기화 시킨다. */
/* 사전 조건 : pstack은 스택을 가리킨다. (pointer of stack) */
/* 사후 조건 : stack이 비어있는 상태로 초기화 된다. (이때 비어있다는 것은 recent == NULL && size == 0) */
void Initialize(Stack * pstack);

/* 연산 : 스택이 비어 있는지 확인한다  */
/* 사전 조건 : pstack은 스택을 가리킨다. (pointer of stack) */
/* 사후조건 : 비어있으면 true 비어있지 않으면 false리턴 */
bool StackIsEmpty(const Stack * pstack);

/* 연산 : 스택이 가득 차 있는지 확인한다 */
/* 사전 조건 : pstack은 스택을 가리킨다. (pointer of stack) */
/* 사후 조건 : stack이 꽉차있으면 true아니면 false리턴 */
bool StackIsFull(const Stack * pstack);

/* 연산 : 스택에 저장된 정보의 갯수를 리턴한다. */
/* 사전 조건 : pstack은 스택을 가리킨다. (pointer of stack) */
/* 사후 조건 : return size member */
int StackItemCount(const Stack * pstack);

/* 연산 : 노드 생성, 생성한 노드에 원하는 Item입력, Stack에 집어넣음 */
/* 사전 조건 : pstack은 스택을 가리킨다. (pointer of stack), si는 stack item즉 저장할 정보를 가리킨다.*/
/* 사후 조건 : 트리에 항목을 추가하고 성공하면 true 실패하면 false */
bool AddNode(Stack * pstack, Item si);              

/* 연산 : 원하는 정보를 가진 노드를 삭제 */
/* 사전 조건 : pstack은 스택을 가리킨다. (pointer of stack), si는 stack item (삭제하고자 하는 정보) */
/* 사후 조건 : 삭제에 성공하면 true, 실패하면 false 리턴 */
bool DeleteNode(Stack * pstack, Item si);

/* 연산 : 모든 노드 삭제 */
/* 사전 조건 : pstack은 stack을 가리킨다. pstack은 미리 초기화된 스택을 가리킨다.*/
/* 사후 조건 : 삭제에 성공하면 true, 실패하면 false 리턴 */
void DeleteAll(Stack * pstack);                                     //수정된 header file에 맞춰서 source code file 수정해야함.

/* 연산 : 원하는 item이 들어있는 node를 스택에서 찾아서 값을 출력한다. */
/* 사전 조건 : pstack은 미리 초기화된 스택을 가리킨다,si는 stack item  */
/* 사후 조건 : 원하는 정보를 저장된 노드의 값을 끄집어 낸다. (값이 없으면 없다고 알림.) */
void ShowNode(Stack * pstack, Item si);

/* 연산 : 스택에 저장된 정보 전체 출력 */
/* 사전 조건 : pstack은 스택을 가리킨다. (pointer of stack) */
/* 사후 조건 : 저장된 정보 있으면 출력, 없으면 없다고 출력 */
void ShowAll(const Stack * pstack);

/* 연산 : 스택 전체에 어떤 조작을 가한다 */
/* 사전 조건 : pnode는 스택의 recent멤버, 즉 첫번째 전달인자는 Stack의 가장 최근 노드의 주소  */
/*           pfun은 Stack의 모든 항목에 적용할 함수, 함수 시그니쳐는 Traverse()프로토타입 선언에 따른다 */             
/* 사후 조건 : pstack에 어떤 조작을 가한다. */
void Traverse(Node * pnode, void (* pfun)(Item n));



/* 보조 함수 */
/* 연산 : 사용자로부터 저장할 정보(Item)을 입력받는다. */
/* 사전 조건 : */
bool GetItem(Item *);
#endif
/*
void Traverse(Node * pnode, void (*pfun)(int n))
void (*pfun)(int n)     -->  리턴값이 없고 int형을 전달인자로 가지는 함수를 가리키는 포인터 pfun
void *pfun (int n)      --> 범용 포인터를 리턴하고 int형을 전달인자로 가지는 함수 pfun
*/