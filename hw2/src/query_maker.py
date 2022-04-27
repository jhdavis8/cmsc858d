import random

f = open("query-100-large.fa", "w")
for i in range(0, 100):
    f.write(">" + str(i) + "\n")
    genome = ""
    for i in range(0, random.randrange(90,160)):
        genome += random.choice(["A","C","T","G"])
    f.write(genome + "\n")
f.close()
