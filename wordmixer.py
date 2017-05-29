import random
import sys

src = open(sys.argv[1], 'r')
dest = open('output.txt', 'w')
for line in src:
	words = line.split()
	for word in words:
	  if len(word) > 3:
		output = word[0] + ''.join(random.sample(word[1:-1],len(word)-2)) + word[-1]
		dest.write(output + ' ')
	  else:
		dest.write(word + ' ')
	  
	dest.write('\n')
	