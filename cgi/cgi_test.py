import os
import sys

nb = len(sys.argv)

lst = []
x = 0
y = 0
result = 0
operator = ""

if (nb > 1):
	query = sys.argv[1]
	lst = query.split("&")

for i in lst:
	if i.split("=")[0] == "x":
		x = int(i.split("=")[1])
	if i.split("=")[0] == "y":
		y = int(i.split("=")[1])
	if i.split("=")[0] == "operator":
		operator = i.split("=")[1]

if operator == "+":
	result = x + y
elif operator == "-":
	result = x - y
elif operator == "*":
	result = x * y
elif operator == "/":
	result = x / y

	
print("query: ", x, y, operator)
print("result: ", result)

html_file = open("front-end/www/webserv/calculator.html", "r")
list_of_lines = html_file.readlines()
list_of_lines[31] = "<p class=result>Result: " + str(result) + "</p>"

html_file = open("front-end/www/webserv/calculator.html", "w")
html_file.writelines(list_of_lines)
html_file.close()
