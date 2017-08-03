# MACFP: Maximal Approximate Consecutive Frequent Pattern Mining under Edit Distance
## Publication

* Jingbo Shang, Jian Peng, and Jiawei Han, "**[MACFP: Maximal Approximate Consecutive Frequent Pattern Mining under Edit Distance](http://shang7.web.engr.illinois.edu/papers/MACFP.pdf)**", In Proceedings of 2016 SIAM International Conference on Data Mining (SDM 2016), 2016

## Requirements

* g++

## Build

MACFP can be easily built by Makefile in the terminal.

```
$ make
```

## Running the code

You could execute the code in the following way:

```
time ./bin/MACFP -data <INPUT_FILE> -min_sup <MIN_SUPPORT> -edit <EDIT_DISTANCE_THRESHOLD> -length <LENGTH_THRESHOLD> -o <OUTPUT_FILENAME>
```

Example:

```
mkdir -p tmp
time ./bin/MACFP -data data/10K.txt -min_sup 4 -edit 2 -length 30 -o tmp/test.txt
```

## Input File

```data/10K.txt``` provides an example of input files. You can break the long string in multiple lines if you want. We only accept the ACGT letters in the current implementation. If you want to use a different alphabet set, please modify the constant ```alphabet``` in the ```src/config/dna.h```.

## Output File

We will output the offsets and all supports for every MACFP in the output file.
