import sys
import re
import pandas as pd

Str = []
sub_list = []

if len(sys.argv) !=3:
    sys.exit("missing command-line argument")

with open(sys.argv[1],'r') as database:
     data_frame = pd.read_csv(database,index_col=0)                   #Creat a data frame with columm name became the index
     STR_List=list(data_frame)                                        # Creat a list with All STR in CSV file
     print(STR_List)

with open(sys.argv[2], 'r') as file:
    txt=file.read()

for i in STR_List:                                                    #Loop for look all the STR on txt file
    match_pattern = re.findall(f"{i}(?:{i})*",txt)

    if not match_pattern:
        Str.append(0)                                                 # .append is a funcion to insert a value in the end of a list
    elif len(match_pattern) == 1:
        back_to_string_match=''.join(match_pattern)                   #Convert the STR element on the list to string
        sub_match_pattern = re.split(f"{i}",back_to_string_match)     #Split the sequence of STR and count how many times has it occurred
        Str.append(len(sub_match_pattern)-1)                          # the -1 is to work correctly
    else:
        sub_list.clear()                                              # clear a auxilair list
        for j in range(len(match_pattern)):                           # loop for check the largest sequence
            back_to_string_match =''.join(match_pattern[j])
            sub_match_pattern = re.split(f'{i}',back_to_string_match)
            sub_list.append(len(sub_match_pattern)-1)
        Str.append(max(sub_list))                                     # insert the maximun value of the auxilia list

match_bool = False
data_frame_2 = data_frame.isin(Str)                                   # creat another data frame with the list of all STR in txt file
for i in range(len(data_frame.index)):                                # loop for compare the DNA on csv file with DNA on txt file
    if (data_frame_2.iloc[i].all()):
        match_bool = True
        match = (data_frame_2.index.values[i])

if match_bool == False:
    print('No match')
else:
    print(match)
