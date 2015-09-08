#ifndef __DNA_H__
#define __DNA_H__

bool EFFICIENT_EXPAND = true;
bool CHUNK_INDEX = true;
bool PRUNE = true;

string DNA_FILENAME = "../../data/1M.txt";
string OUTPUT_FILENAME = "../../results/determined.txt";
int MIN_SUP = 6;
int EDIT_DIS_THRES = 3;
int LEN_THRES = 50;
const string alphabet = "acgt";

void loadConfiguration(int argc, char *argv[])
{
    for (int i = 1; i + 1 < argc; ++ i) {
        string op = argv[i];
        if (op == "-no_exp") {
            EFFICIENT_EXPAND = false;
        } else if (op == "-no_chunk") {
            CHUNK_INDEX = false;
        } else if (op == "-no_prune") {
            PRUNE = false;
        } else if (op == "-o") {
            OUTPUT_FILENAME = argv[i + 1];
            ++ i;
        } else if (op == "-data") {
            DNA_FILENAME = argv[i + 1];
            ++ i;
        } else if (op == "-min_sup") {
            sscanf(argv[i + 1], "%d", &MIN_SUP);
            ++ i;
        } else if (op == "-edit") {
            sscanf(argv[i + 1], "%d", &EDIT_DIS_THRES);
            ++ i;
        } else if (op == "-length") {
            sscanf(argv[i + 1], "%d", &LEN_THRES);
            ++ i;
        }
    }
}

string loadDNASequence()
{
    FILE* in = tryOpen(DNA_FILENAME, "r");
    string dna = "";
    while (getLine(in)) {
        for (size_t i = 0; line[i]; ++ i) {
            line[i] = tolower(line[i]);
            myAssert(alphabet.find(line[i]) != -1, "Unknown Character: " + line[i]);
        }
        dna += line;
    }
    fclose(in);
    return dna;
}

#endif
