//第六题：如何让程序跳入指定地址
/*
#include <stdio.h>
typedef void(*voidFuncPtr)(void);

void test2(void)
{
	voidFuncPtr P1 =NULL;
	P1 = (voidFuncPtr)(0x10000);
	P1();
}

void main()
{
	test2();
}
*/


/*
#include <stdio.h>

void main()
{
	int a = 10;
	int b = 0;
	//const int* p0 = &a; //const修饰的是*p0，值不能变，指向的地址可以变
	//int const* p1 =&a;//const修饰的是*p1，值不能变，指向的地址可以变
	int* const p2 = &a;//const修饰的是p2(指针)，指向的地址不能变
	//p2 = &b;
	//const int* const p3 = &a;//const都修饰到了，指向的值和地址都不能变
	//*p3 =20;
	printf("%d\n",*p2);

}
*/

//第七题：大端模式和小端模式的区别
//小端：低地址存放低位字节
//大端：高地址存放高字节
#include <stdio.h>
void main()
{
	unsigned char* ptr = NULL;
	unsigned int a = 0x1234;
	ptr = (unsigned char*)&a;
	printf("%x\n",*ptr);//地地址存放低字节
	printf("%x\n",*(ptr+1));//高地址存放高字节
}