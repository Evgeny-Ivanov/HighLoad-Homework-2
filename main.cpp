#include <iostream>
#include <algorithm>


using namespace std;

//0 7 15 31
int const NPAD = 7;

struct Node {
    struct Node *next;
    long int pad[NPAD];
};

#define NEXT16(x) x x x x x x x x x x x x x x x x

#define NEXT64(x) NEXT16(x) NEXT16(x) NEXT16(x) NEXT16(x)

#define NEXT128(x) NEXT64(x) NEXT64(x)

#define NEXT256(x) NEXT128(x) NEXT128(x)

double bypass(Node *begin, int arraySize){
    Node * current = begin;

    //unsigned long long start = __rdtsc();//количество тактов
    struct timespec t1, t2;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&t1);
    for(int i=0;i<arraySize;i++){
        NEXT256(*current;current = current->next;);
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&t2);
    //unsigned long long end = __rdtsc();

    return 1000000000. * (((t2.tv_sec + t2.tv_nsec * 1.e-9)
                           - (t1.tv_sec + t1.tv_nsec * 1.e-9)))/(arraySize*256);//нано секунды
}

void randJoin(Node *arrayBuf[],int arraySize){
    Node * array = *arrayBuf;
    int * arrayIndexs = new int[arraySize]();//массив индексов для соединения
    for(int i=0;i<arraySize;i++){
        arrayIndexs[i] = i;
    }
    random_shuffle(&arrayIndexs[0],&arrayIndexs[arraySize-1]);//перемешиваем

    int startIndex = 0;
    Node * current = &array[startIndex];
    for(int i=0;i<arraySize;i++){
        if(arrayIndexs[i] != startIndex) {
            current->next = &array[arrayIndexs[i]];
            current = current->next;
        }
    }
    array[arrayIndexs[arraySize-1]].next = &array[startIndex];//замкнутый список
    delete[] arrayIndexs;
}

void rowJoin(Node *arrayBuf[],int arraySize){
    Node * array = *arrayBuf;
    for(int i=0;i<arraySize-1;i++){
        array[i].next = &array[i+1];
    }
    array[arraySize-1].next = &array[0];//замкнутый список
}

//14 21
int main(int argc, char *argv[]) {

    for(int N=28;N>9;N--) {
        double sum = 0;
        int count = 1;
        for(int m=0;m<count;m++) {
            int arraySize = pow(2,N)/sizeof(Node);
            Node *array = new Node[arraySize];

            for (int i = 0; i < arraySize; i++) {
                for (int j = 0; j < NPAD; j++) {
                    array[i].pad[j] = rand() % 1000;
                }
            }

            randJoin(&array, arraySize);

            sum += bypass(&array[0], arraySize);
            delete[] array;
        }
        cout << N << ";" << sum/count << "\n";
    }

    return 0;
}