# TODO
import cs50
import sys
import csv

# Open database
db = cs50.SQL("sqlite:///students.db")

if len(sys.argv) !=2:
    sys.exit("missing command-line argument")

with open(sys.argv[1],newline='') as database:
    reader = csv.reader(database, delimiter=",")
    characters_list=list(reader)[1:]

    for row in characters_list:
        characters_name = row[0].split()
        if len(characters_name)<3:
            db.execute("INSERT INTO students(first, middle, last, house, birth) VALUES(?,?,?,?,?)",characters_name[0],None,characters_name[1],row[1],row[2])
        else:
            db.execute("INSERT INTO students(first, middle, last, house, birth) VALUES(?,?,?,?,?)",characters_name[0],characters_name[1],characters_name[2],row[1],row[2])





# TODO
import cs50
import sys

# Open database
db = cs50.SQL("sqlite:///students.db")

if len(sys.argv) !=2:
    sys.exit("missing command-line argument")

studentes=db.execute("SELECT * FROM students Where (?) ordes by last",sys.argv[1])
print (studentes)