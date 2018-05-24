import csv
import fileinput
import sys
import re

filepaths = open("./files.txt", "r")

lines_to_skip = r"^(#)|(<!--)"
strings=[("Mozilla Public", "Mozilla Public"),
        (r"mozilla firefox", "&brandFullName;"),
        (r"([^./_])mozilla", "\\1&vendorShortName;"),
        (r"([^.])firefox", "\\1&brandShortName;")]

mozilla = "mozilla"
firefox = "firefox"

def collect():
    with open('out_strings.csv', 'wb') as csvfile:
        filewriter = csv.writer(csvfile, delimiter='\t')
        filewriter.writerow(['Src', 'Line', 'Orig', 'New string'])

        for filepath in filepaths:
            cleanpath=filepath.replace("\n", "")
            counter=0
            file = open("../../../" + cleanpath, "r")

            for line in file:
                counter = counter + 1
                if not re.match(lines_to_skip, line):
                    line=line.replace("\n", "")
                    orig=line
                    for (exp, repl) in strings:
                        line=re.sub(exp, repl, line, count=0, flags=re.IGNORECASE)
                    if line != orig:
                        filewriter.writerow([cleanpath, counter, orig, line])
                        print str(counter) + " " + line

def replace():
    print "Replacing Strings"

    with open("./out_strings.csv", "r") as csvfile:

        out = csv.reader(csvfile, delimiter='\t')

        #Skips the firt row = CSV Header
        next(out, None)

        for line in out:
        	path = line[0]
        	row = int(line[1])
        	orig = line[2]
        	new = line[3]

        	cleanpath = path.replace("\n", "")

        	with open("../../../" + cleanpath, "r") as file:
        		data = file.readlines()

        		for i, line in enumerate(data):
        			line = line.replace("\n", "")
        			if (i+1) == row:
        				data[i] = new + "\n"

        	with open("../../../" + cleanpath, "w") as file:
        		file.writelines(data)
        		

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print "This script takes one argument: python string_replacer.py [collect | replace]"
    elif sys.argv[1] == "collect":
        collect()
    elif sys.argv[1] == "replace":
        replace()
    else:
        print "Sorry, something went wrong."
