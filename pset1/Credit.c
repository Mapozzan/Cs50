#include <stdio.h>
#include<cs50.h>
#include <math.h>

int main(void)
{
    int m=0,n=0;
    long x,y,number = get_long("CreditCar Number:");

    // First step of checksum
    x= number/10;
    do
    {
        m=x%10;
        x = x/100;
        m=(m*2);
        if(m>=10)
        {
            n=(m%10)+1+n;
        }
        else if(m>0 && m<10)
            {
                n=n+m;
            }
        }while(x>0);

        //Second step of checksum
        x=number;
        do
        {
            m=x%10;
            n=n+m;
            x=x/100;
        }while(x>0);

        //Check how many digits, number have
       
        x=number;
        m=0;
        do
        {
            x=x/10;
            m=m+1;
        }while(x>0);
        
        
        // Conditions
        if(m==13 && n%10==0)                          // If number have 13 digits is VISA
        {
            x=number/(pow(10,12));                    //Check the first digit of number, if is a 4 then is VISA, if not Invalid
            if(x ==4)
            {
              printf("VISA");
            }
            else
            {
              printf("INVALID");
            }
        }
        else if(m==15 && n%10==0)                                      // If number have 15 digits can be AMERICAN EXPRESS
        {
            x=number/(pow(10,13));                                     //Check the two first digit of number 
            if((x==34 || x==37))                                       // If number star with 34 or 37 is AMERICAN EXPRESS
            {
                printf("AMEX\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else if(m==16 && n%10==0)                                      // If number have 16 digits can be VISA or MASTERCARD                   
        {
            x=number/(pow(10,15));                                     //Check the firs digit of number
            if(x==4)                                                   //If is a 4,is VISA
            {
                printf("VISA\n");
            }
            else if(x==5)                                             //If is a 5, can be MASTERCARD
            { 
                
                y=number/(pow(10,14));                                //Check the second digit of number
                if((y==51 || y==52 || y==53 || y==54 || y==55))       //If is 51,52,53,54 or 55 is MASTERCARD
                {
                    printf("MASTERCARD\n");
                }
                else
                {
                    printf("INVALID\n");
                }
            }
            else                                                      // If number have 16 digits and start with a diffrent number 4 or 5 is INVALID                                                  
            {
                printf("INVALID\n");
            }
        }
        else                                                         // If number is not a creditcard number
        {
            printf("INVALID\n");
        }
}
