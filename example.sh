make

mkdir -p tmp

time ./bin/MACFP -data data/10K.txt -min_sup 4 -edit 2 -length 30 -o tmp/test.txt
# time ./bin/TDP -data data/10K.txt -min_sup 4 -edit 2 -length 30 -o tmp/test.txt