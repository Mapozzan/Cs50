int main(void)
{
    string text = get_string("Text: ");
    int n=strlen(text);
    int sum_letter=0,sum_word=0,sum_setences=0;
    for(int i=0; i < n; i++)
        {
            if(isalpha(text[i]))
            {
                sum_letter++;
                if(isblank(text[i+1]) || text[i+1]==',' || text[i+1]=='.' || text[i+1]=='!' || text[i+1]=='?')
                {
                    sum_word++;
                }
            }
            else if(text[i]=='.' || text[i]=='!' || text[i]=='?')
            {
                sum_setences++;
            }
        }

       float L= (float) sum_letter/ (float) sum_word*100;
       float S= (float) sum_setences/ (float) sum_word*100;
       float index = 0.0588*L - 0.296*S - 15.8;
       if(round(index)<1)
       {
           printf("Before Grade 1\n");
       }
       else if(round(index)>=16)
       {
           printf("Grade 16+\n");
       }
       else
       {
           printf("Grade %d\n",(int) round(index));
       }
}
