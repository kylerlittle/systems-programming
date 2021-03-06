/************* t.c file ********************/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// Global variables
int *FP;
char *ctable = "0123456789abcdef";

// Type declarations
typedef unsigned int u32;

// Function Prototypes
int A(int x, int y);
int B(int x, int y);
int C(int x, int y);
int rpu(u32 x, int BASE);
int print_u32(u32 x, int BASE);
int printu(u32 x);
int prints(char *s);
int printd(int x);
int printx(u32 x);
int printo(u32 x);
int myprintf(char *fmt, ...);


int main(int argc, char *argv[ ], char *env[ ])
{
  printf("***** PART 1 *****\n");
  printf("******************\n");
  
  int a,b,c;
  
  printf("enter main\n");
  
  printf("&argc=%x argv=%x env=%x\n", &argc, argv, env);
  printf("&a=%8x &b=%8x &c=%8x\n", &a, &b, &c);
  
  /* 
   * (1). Write C code to print values of argc and argv[] entries
   */
  printf("***** arguments sent to main *****\n");
  // printf("***** argc: %d *****\n", argc);
  myprintf("***** argc: %d *****\n", argc);
  printf("***** argv: ");
  int i = 0;
  for (i = 0; i < argc; i++) {
    // printf("%s ", argv[i]);
    myprintf("%s ", argv[i]);
  }
  myprintf("*****\n");
  // --- finished printing argc and argv values
  a=1; b=2; c=3;
  A(a,b);

  myprintf("***** PART 2 *****\n");
  myprintf("******************\n");
  
  myprintf("***** testing prints *****\n");
  char* test_str = "helasdfjdslf my name kyler";
  prints(test_str);
  printf(" == %s\n", test_str);
  
  myprintf("***** testing printu *****\n");
  u32 test_u32_1 = 234223, test_u32_2 = 0;
  printu(test_u32_1);
  printf(" == %u\n", test_u32_1);
  printu(test_u32_2);
  printf(" == %u\n", test_u32_2);
  
  myprintf("***** testing printd *****\n");
  int test_int1 = 234223, test_int2 = 0, test_int3 = -12398;
  printd(test_int1);
  printf(" == %d\n", test_int1);
  printd(test_int2);
  printf(" == %d\n", test_int2);
  printd(test_int3);
  printf(" == %d\n", test_int3);
  
  myprintf("***** testing printx *****\n");
  printx(test_u32_1);
  printf(" == 0x%x\n", test_u32_1);
  printx(test_u32_2);
  printf(" == 0x%x\n", test_u32_2);
  
  myprintf("***** testing printo *****\n");
  printo(test_u32_1);
  printf(" == 0%o\n", test_u32_1);
  printo(test_u32_2);
  printf(" == 0%o\n", test_u32_2);
  
  myprintf("***** testing mymyprintf *****\n");
  myprintf("%s == test_str\n", "test_str");
  myprintf("hex: %x oct: %o u32: %u\n", 1, 2, 3);
  myprintf("==\n");
  myprintf("hex: 0x%x oct: 0%o u32: %u\n", 1, 2, 3);
  myprintf("%c %c %c == l o l\n%s == lol\n", 'l','o','l', "lol");
  myprintf("%c%c%c%c == char\n", 'c','h','a','r');
  myprintf("exit main\n");
}

int A(int x, int y)
{
  int d,e,f;
  
  myprintf("enter A\n");
  
  // PRINT ADDRESS OF d, e, f
  d=4; e=5; f=6;
  myprintf("&d=%x &e=%x &f=%x\n", &d, &e, &f);
  myprintf("&d=%x &e=%x &f=%x\n", &d, &e, &f);
  // --- end print address
  
  B(d,e);
  
  myprintf("exit A\n");
}

int B(int x, int y)
{
  int g,h,i;
  
  myprintf("enter B\n");
  
  // PRINT ADDRESS OF g,h,i
  g=7; h=8; i=9;
  printf("&g=%x &h=%x &i=%x\n", &g, &h, &i);
  myprintf("&g=%x &h=%x &i=%x\n", &g, &h, &i);
  // --- end print address
  
  C(g,h);
  
  myprintf("exit B\n");
}

int C(int x, int y)
{
  int u, v, w, i, *p;

  myprintf("enter C\n");
  
  // PRINT ADDRESS OF u,v,w,i,p;
  u=10; v=11; w=12; i=13;
  printf("&u=%x &v=%x &w=%x &i=%x p=%x\n", &u, &v, &w, &i, p);
  myprintf("&u=%x &v=%x &w=%x &i=%x p=%x\n", &u, &v, &w, &i, p);
  // --- end print address

  /*
   * (2). Write C code to print the stack frame link list.
   */
  FP = (int *)getebp();
  myprintf("***** stack frame link list *****\n");
  while (FP) {  // stack fram link list ends with a 0
    myprintf("%x -> ", FP);
    // printf("address: %x\t value: %x\n", FP, *FP);
    FP = *FP;   // since FP points at the next FP, set it to its dereferenced value
  }
  myprintf("%x\n", FP);
  myprintf("*****\n");
  // --- end print stack frame link list

  /*
   * (3). Print the stack contents from p to the frame of main()
   *      YOU MAY JUST PRINT 128 entries of the stack contents.
   */
  p = (int *)&p;  // p now contains the address to itself... 
  myprintf("***** stack contents *****\n");
  int j = 0;
  while (j < 128 && p) {  // stack fram link list ends with a 0, but don't print more than 128 entries
    myprintf("%x (value: %d) -> ", p, *p);
    p++;   // increment pointer to look at next thing on stack
    j++;   // increment j
  }
  myprintf("%x -> ... -> 0\n", p);
  myprintf("*****\n");

  /*
   * (4). On a hard copy of the print out, identify the stack contents
   *      as LOCAL VARIABLES, PARAMETERS, stack frame pointer of each function.
  */
}



// *********************
// PART 2 CODE
// *********************


/*
 * Recursive print unsigned (rpu).
 */
int rpu(u32 x, int BASE)
{
  char c;
  if (x){
    c = ctable[x % BASE];
    rpu(x / BASE, BASE);
    putchar(c);
  }
}

/*
 * Helper function to print an unsigned 32 bit integer in the specified base.
 */
int print_u32(u32 x, int BASE) {
  (x==0)? putchar('0') : rpu(x, BASE);
}

/*
 * Print an unsigned 32 bit integer.
 */
int printu(u32 x)
{
  print_u32(x, 10);  // BASE 10
}

/*
 * Prints character array terminated with null char (a string).
 */
int prints(char *s) {
  while (*s) {  // while s doesn't point at null character
    putchar(*s);
    s++;  // increment pointer
  }
}

/*
 * Prints integer (note that x may be negative).
 */
int printd(int x) {
  if (x < 0) {
    x *= -1;    // negate
    putchar('-');   // print negative sign
  }
  printu((u32)x);
}

/*
 * Prints hexadecimal value for x (will start with "0x").
 */
int printx(u32 x) {
  putchar('0'); putchar('x');   // prefix for hex
  print_u32(x, 16);  // BASE 16
}

/*
 * Prints octal value for x (will start with "0").
 */
int printo(u32 x) {
  putchar('0');   // prefix for octal
  print_u32(x, 8);  // BASE 8
}

/*
 * Same functionality as printf()
 * Supports %c, %s, %u, %d, %o, and %x formatters
 */
int myprintf(char *fmt, ...) {
  char *cp = fmt;  // point at format string
  va_list ap;  // equivalent to *ip -- a ptr to first arg on stack
  va_start(ap, fmt);  // point arg_pointer (ap) to first arg on stack
  int arg;  // declare 32 bits to store the arg
  
  while (*cp) {   // equivalent to while *cp != '\0'
    if (*cp != '%') {
      putchar(*cp);   // spit out any character that isn't a '%'
      if (*cp == '\n') {  // for each '\n', spit out an extra '\r'
	putchar('\r');
      }
    } else {
      ++cp;  // advance char pointer to get char after '%'
      arg = va_arg(ap, int);  // get value of arg on stack
      switch(*cp) {   // switch on the format specifier
      case 'c':
	putchar((char)arg);
	break;
      case 's':
	prints((char *)arg);
	break;
      case 'u':
	printu((u32)arg);
	break;
      case 'd':
	printd(arg);
	break;
      case 'o':
	printo((u32)arg);
	break;
      case 'x':
	printx((u32)arg);
	break;
      default:
	myprintf("Can't handle format specification '%c'\n", *cp);
	break;
      }
    }
    ++cp;
  }
  va_end(ap);  // essentially, set arg pointer to null
}
