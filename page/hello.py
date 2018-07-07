import sys
first = sys.argv[1][sys.argv[1].find('firstname=')+10:sys.argv[1].find('&lastname')]
last =  sys.argv[1][sys.argv[1].find('lastname=')+9:sys.argv[1].find('&file')]
print "<html>HELLO VISITOR", first, last, "<br/>"
print "you send file", sys.argv[1][sys.argv[1].find('file=')+5:], "<br/>", "</html>"

