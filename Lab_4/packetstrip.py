import os

with open("packets.dat", "r") as in_file:
    with open("lab04-timeseq.dat", "w") as out_file:
        with open("stripped.dat", "w") as write_file:
            lines = in_file.readlines()

            for line in lines:
                if "192.168.0.66.56735 > yali.mythic-beasts.com.http" in line:
                    continue
                if "Flags [S" in line or "Flags [F" in line:
                    continue
                if ("ack" in line and "seq" not in line):
                    continue
                
                print(line, file=write_file, end="")
                
                print(line[1:16], file=out_file, end=" ")

                seqs = line[ line.find("seq") : line.find(", ack") ]

                print(seqs[ seqs.find(":") + 1 : ], file=out_file)