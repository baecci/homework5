#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define MAX_STACK_SIZE 10  //스택의 최대 크기를 10으로 정한다.
#define MAX_EXPRESSION_SIZE 20  //스택의 최대

/* stack 내에서 우선순위는 내림차순, lparen = 0 가장 낮음 */
typedef enum{
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;  //구조체 정의로 연산자의 우선순위를 지정해준다.

char infixExp[MAX_EXPRESSION_SIZE];
char postfixExp[MAX_EXPRESSION_SIZE];
char postfixStack[MAX_STACK_SIZE];
int evalStack[MAX_STACK_SIZE];

int postfixStackTop = -1;  //공백 스택으로 초기화한다.
int evalStackTop = -1;

int evalResult = 0;

void postfixPush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
void evaluation();

int main()
{
	char command;  //입력 받을 커멘드 변수를 선언한다.

	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {  //입력 받은 커멘드에 따라 함수를 호출한다.
		case 'i': case 'I':
			getInfix();  //산수식을 입력 받는다. 
			break;
		case 'p': case 'P':  //입력 받은 산수식을 Postfix로 변환한다.
			toPostfix();
			break;
		case 'e': case 'E':  //계산한다.
			evaluation();
			break;
		case 'd': case 'D':  //디버그해서 현재 Infix, postfix 등을 출력한다.
			debug();
			break;
		case 'r': case 'R':  //초기화 한다.
			reset();
			break;
		case 'q': case 'Q':  //프로그램을 종료한다.
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;


}

void postfixPush(char x)
{
    postfixStack[++postfixStackTop] = x;  //받은 문자를 스택의 탑을 한 칸 증가시킨 후 postfixstack 문자열 변수에 저장한다.
}

char postfixPop()
{
	char x;
    if(postfixStackTop == -1)  //스택이 -1인지 확인하고
        return '\0';
    else {
    	x = postfixStack[postfixStackTop--];  //아니면 현재 top의 위치에 있는 값을 x에 저장하고 스택을 1 내린다.
    }
    return x;
}

void evalPush(int x)
{
    evalStack[++evalStackTop] = x;  //스택을 1올린 후 전달 받은 x를 저장한다.
}

int evalPop()  
{
    if(evalStackTop == -1)  //스택에 아무것도 없다면 -1을 리턴한다.
        return -1;
    else
        return evalStack[evalStackTop--];  //스택을 리턴 후 한 스택 내린다.
}


void getInfix()
{
    printf("Type the expression >>> ");  //입력받은 산수식을 infixExp에 저장한다.
    scanf("%s",infixExp);
}

precedence getToken(char symbol)  //연산자를 전달 받아서, 이를 코드로 변환하여 리턴한다.
{
	switch(symbol) {
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand;
	}
}

precedence getPriority(char x)  
{
	return getToken(x);  //getToken함수로 연산 기호를 우선순위로 바꾼다.
}


void charCat(char* c)  //문자를 받아 postfixExp에 저장합니다
{
	if (postfixExp == '\0')
		strncpy(postfixExp, c, 1);
	else
		strncat(postfixExp, c, 1);
}


void toPostfix()
{
	char *exp = infixExp;  //산수식을 읽기 위해 문자형 포인터로 받는다. 
	char x;  //확인할 문자를 임시 저장한다.

	while(*exp != '\0')  //입력받은 산수식을 문자 단위로 읽어, stack을 이용해 Postfix로 변환한다.
	{
		if(getPriority(*exp) == operand)  //피연산자, 연산기호 등을 골라내어 차례대로 postFixExp에 저장한다. 
		{
			x = *exp;
        	charCat(&x);
		}
        else if(getPriority(*exp) == lparen) {

        	postfixPush(*exp);  
        }
        else if(getPriority(*exp) == rparen)
        {
        	while((x = postfixPop()) != '(') {
        		charCat(&x);
        	}
        }
        else
        {
            while(getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp))
            {
            	x = postfixPop();
            	charCat(&x);
            }
            postfixPush(*exp);
        }
        exp++;  //다음 문자를 읽기 위해 exp를 증가시킨다.
	}

    while(postfixStackTop != -1)  //공백 스택이 아닌 경우, 즉 아직 문자가 남아 있는 경우 남은 문자를 postFixExp에 마저 저장한다.
    {
    	x = postfixPop();
    	charCat(&x);
    }

}

void debug()
{
	printf("\n---DEBUG\n");  //현재 상황을 출력한다.
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++)  
		printf("%c  ", postfixStack[i]);

	printf("\n");

}

void reset()
{
	infixExp[0] = '\0';  //산수식을 전부 초기화한다.
	postfixExp[0] = '\0';

	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';
         
	postfixStackTop = -1;  //공백 스택으로 전부 초기화한다.
	evalStackTop = -1;
	evalResult = 0;
}
void evaluation()  //변환한 postfixExp를 계산한다.
{
	int opr1, opr2, i;

	int length = strlen(postfixExp);  //postfixExp 문자열 길이만큼 문자를 읽을 것이므로 length에 저장한다.
	char symbol;
	evalStackTop = -1;  

	for(i = 0; i < length; i++)  //문자를 읽어가며 피연산자를 먼저 골라내고, 연산기호를 골라내어 피연산자를 계산한다.
	{
		symbol = postfixExp[i];
		if(getToken(symbol) == operand) {
			evalPush(symbol - '0');
		}
		else {
			opr2 = evalPop();
			opr1 = evalPop();
			switch(getToken(symbol)) {
			case plus: evalPush(opr1 + opr2); break;
			case minus: evalPush(opr1 - opr2); break;
			case times: evalPush(opr1 * opr2); break;
			case divide: evalPush(opr1 / opr2); break;
			default: break;
			}
		}
	}
	evalResult = evalPop();
}