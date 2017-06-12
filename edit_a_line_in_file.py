import sys

f=open(sys.argv[1],"r")
line_num = int(sys.argv[2])
replacement = sys.argv[3] + '\n'
lines=[l for l in f]
lines[line_num-1] = replacement
f.close()
f=open(sys.argv[1], "w")
f.writelines(lines)
f.close()
