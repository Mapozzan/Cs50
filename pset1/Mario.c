#include <stdio.h>
#include<cs50.h>

int get_correct_int(void);
void print_mountain(int n);

int main(void)
{ 
   int n = get_correct_int();
   print_mountain(n);
}



int get_correct_int(void)
{
   int n;
   do
   {
      n=get_int("Enter a Height between 1 and 8:");
   }
   while((n<1) || (n>8));
   return n;
}

void print_mountain(int n)
{
     char  s =' ',h ='#';
     int x=0,y=1,i,j,k;
     do
    {
      for(i=1;i<n;i++)
         {  
           printf("%c",s);
           x=n-i;
         }
         
       for(j=0;j<y;j++) 
         {
              printf("%c",h);
            
         }
        printf("  "); 
       for(k=0;k<y;k++) 
          {
              printf("%c",h);
            
          }
        printf("\n");
        y=y+1;
        n=n-1;
    }while(n>0);
}
