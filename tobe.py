import random

src = "tobe"
letters = "abcdefghijklmnopqrstuvwxyz"
output = "tobs"
while not output == src:
    output = random.sample(letters,len(src))
print (output)