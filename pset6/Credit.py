from cs50 import get_int
import math

number = get_int("CreditCar Number:")

x=number/10
n=0

while(x>0):
    m = int(x %10)
    x = int(x/100)
    m = int((m*2))
    if(m>=10):
        n = (m%10)+1+n
    elif(m>0 and m<10):
        n=n+m

x=number
while(x>0):
    m=int(x%10)
    n=n+m
    x=int(x/100)

len_number=len(str(number))
rest_sum=int(n%10)

if(len_number==13 and rest_sum==0):
    x=int(number/(pow(10,12)))
    if(x==4):
        print("VISA")
    else:
        print("INVALID")
elif(len_number==15 and rest_sum==0):
    x=int(number/(pow(10,13)))
    if(x==34 or 37):
        print("AMEX")
    else:
        print("INVALID")
elif(len_number==16 and rest_sum==0):
    x=int(number/(pow(10,15)))
    if(x==4):
        print("VISA")
    elif(x==5):
        x=int(number/(pow(10,14)))
        if x in [51,52,53,54,55]:
            print("MASTERCARD")
        else:
            print("INVALID")
    else:
        print("INVALID")
else:
    print("INVALID")
