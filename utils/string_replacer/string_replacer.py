import csv
import fileinput

filepaths = open("./files.txt", "r")

mozilla = "mozilla"
firefox = "firefox"

counter = 0

#0 = Collect
#1 = Change
mode = 1

if(mode == 0):
	with open('out_strings.csv', 'wb') as csvfile:
		filewriter = csv.writer(csvfile, delimiter='\t')
		filewriter.writerow(['Num', 'String'])

		# array to list the strings seen
		lines_seen = set()

		for filepath in filepaths:
			file = open("../../../" + filepath.replace("\n", ""), "r")

			for line in file:
				# only add strings that have not been added yet
				if line not in lines_seen:
					if (mozilla in line.lower()) | (firefox in line.lower()):
						filewriter.writerow([counter, line.replace("\n", "")])
						print str(counter) + " " + line
						counter = counter + 1
					lines_seen.add(line)
else:
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







