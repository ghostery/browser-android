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
    mydict = {}
    with open("./out_translations.csv", "r") as dictfile:
        reader = csv.reader(dictfile, delimiter=';')
        mydict = {rows[1]:rows[2] for rows in reader}
        print mydict
        
        for filepath in filepaths:
            with open("../../../" + filepath.replace("\n", ""), "r") as file:
                data = file.readlines()

            for i, line in enumerate(data):
                line = line.replace("\n", "")

                try:
                    replace = mydict[line]

                    if(replace == "-") | (replace == "?"):
                        print "skip"
                        continue
                    else:
                        data[i] = replace + "\n"
                        print data[i]
                except Exception, e:
                    print ""
            with open("../../../" + filepath.replace("\n", ""), "w") as file:
                file.writelines(data)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print "Error"
    elif sys.argv[1] == "collect":
        collect()
    elif sys.argv[1] == "replace":
        replace()
    else:
        print "Error"







