#include<stdio.h>
#include<cs50.h>
#include<string.h>
#include<ctype.h>


int check_key(int c,string s);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Command-line argument invalid\n");
        return 1;
    }
    
    int k = strlen(argv[1]);
    
    int b=check_key(k,argv[1]);
    //Function to check if key containing any character that is not an alphabetic character,
    //or not containing each letter exactly once.
    
    string p;
    
    if(b!=0)
    {
        printf("Key is invalid\n");
        return 1;
    }
    else
    {
        string a= "abcdefghijklmnopqrstuvwxyz",c= "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        // Create two strings do compare the letters.
        
        p=get_string("plaintext:");
        int k1=strlen(p);
        
        char d[k1];
        strncpy(d,p,k1);
        //Create a string to copy the plaintext into another string.
        //This is necessary, because if is created a string to write the ciphertext,
        //occurred a bug when de plaintex have the size of 8,9 and 10 or bigger.
        
        for(int i=0;i<k1;i++)
        {
            for(int j=0;j<26;j++)
            {
                if(d[i]==a[j])              
                {
                p[i]=tolower(argv[1][j]);

                }
                else if(d[i]==c[j])
                {
                p[i]=toupper(argv[1][j]);
                }
                else if(ispunct(d[i]))
                {
                p[i]=p[i];
                }
                else if(isblank(d[i]))
                {
                p[i]=' ';
                }

            }
        }
        printf("ciphertext:%s\n",p);
        return 0;
    }
}


int check_key(int c,string s)
{
    int x=0;
    for(int i=0;i<c;i++)
    {
        if(isupper(s[i]) || islower(s[i]))
        {
            for(int j=i+1;s[j]!='\0';j++)
            {
                if(tolower(s[i])==tolower(s[j]))
                {
                    x++;
                }
            }          
        }
        else
        {
            x++;
        }
    }
    if(c!=26)
    {
        x++;
    }
    return x;
}
