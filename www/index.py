#Import modules for CGI handling
import cgi, cgitb

# Create instance of FieldStorage
form = cgi.FieldStorage()

# Get data from fields
first_name = form.getvalue('user')


print ("Status: 200 OK")
print ("Content-type:text/html")
print()
print ("<html>")
print ('<head>')
print ("<title>Hello - Second CGI Program</title>")
print ('</head>')
print ('<body>')
print ("<h2>Hello %s </h2>" % (first_name))
print ('</body>')
print ('</html>')
# while True:
# 	print("KJJJJJJ")