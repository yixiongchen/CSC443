import os
import sys
import subprocess

KB = 1024
MB = 1024 * 1024
sizes = [
    512,
    1 * KB,
    4 * KB,
    8 * KB,
    16 * KB,
    32 * KB,
    1 * MB,
    2 * MB,
    4 * MB
]

filename = sys.argv[1]

for iteration in range(1, 6):
    f1 = open("performance_write_blocks_seq_"+str(iteration)+".txt", "w")
    f2 = open("performance_write_lines_"+str(iteration)+".txt", "w")

    for size in sizes:
        subprocess.run(["write_blocks_seq", filename, str(size)], stdout=f1)
        subprocess.run(["rm", "records.dat"])

    subprocess.run(["write_lines", filename], stdout=f2)
    subprocess.run(["rm", "copy_"+filename])
    
    f1.close()
    f2.close()
    
    print("Finished "+str(iteration)+" iteration.")

