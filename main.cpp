#include <iostream>
#include <math.h>
#include <algorithm>
#include <time.h>
#include <string>

using namespace std;

//0 7 15 31
int const NPAD = 7;

struct Node {
    struct Node *next;
    long int pad[NPAD];
};


void rowJoin(Node *arrayBuf[],int arraySize){
    Node * array = *arrayBuf;
    for(int i=0;i<arraySize-1;i++){
        array[i].next = &array[i+1];
    }
}

#define NEXT16(x) x x x x x x x x x x x x x x x x

#define NEXT256(x) NEXT16(x) NEXT16(x) NEXT16(x) NEXT16(x) NEXT16(x) NEXT16(x) NEXT16(x) NEXT16(x) NEXT16(x) NEXT16(x) NEXT16(x) NEXT16(x) NEXT16(x) NEXT16(x) NEXT16(x)

void bypass(Node *begin, int arraySize,int N){
    Node * current = begin;
    struct timespec t1, t2;
    int size = arraySize/16;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&t1);
    for(register int i=0;i<size-1;i++){
        NEXT16(current = current->next;current->pad[0]++;);
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&t2);
    cout << N << ";" << 1000000000. *
                                (((t2.tv_sec + t2.tv_nsec * 1.e-9)
                                  - (t1.tv_sec + t1.tv_nsec * 1.e-9)))/arraySize << "\n";
}

void randJoin(Node *arrayBuf[],int arraySize){
    Node * array = *arrayBuf;
    int * arrayIndexs = new int[arraySize]();//массив индексов для соединения
    for(int i=0;i<arraySize;i++){
        arrayIndexs[i] = i;
    }
    random_shuffle(&arrayIndexs[0],&arrayIndexs[arraySize]);//перемешиваем

    int startIndex = 0;
    Node * current = &array[startIndex];
    for(int i=1;i<arraySize-1;i++){//линкуем структуры
        if(arrayIndexs[i] != startIndex) {
            current->next = &array[arrayIndexs[i]];
            current = current->next;
        }
    }
    delete[] arrayIndexs;
}


//14 21
int main(int argc, char *argv[]) {

    //string mode = "rand";

    //if(argc>1){
    //    N = atoi(argv[1]);
    //    mode = argv[2];
    //}

    for(int k=28;k>9;k--) {
        int N = k;
        int arraySize = pow(2, N) / sizeof(Node);
        Node *array = new Node[arraySize];

        for (int i = 0; i < arraySize; i++) {
            for (int j = 0; j < NPAD; j++) {
                array[i].pad[j] = rand() % 1000;
            }
        }

        //if(mode == "rand")
        //    randJoin(&array, arraySize);
        //if(mode == "row")
        //    rowJoin(&array, arraySize);

        rowJoin(&array, arraySize);

        bypass(&array[0], arraySize,N);
        delete[] array;
    }

    return 0;
}