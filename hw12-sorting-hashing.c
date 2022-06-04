/*
 * hw12-sorting-hashing.c
 *
 *  Homework 12: Sorting & Hashing
 *  Department of Computer Science at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ARRAY_SIZE			13	/* prime number */
#define MAX_HASH_TABLE_SIZE 	MAX_ARRAY_SIZE

/* 필요에 따라 함수 추가 가능 */
int initialize(int **a);  //초기화 함수 선언
int freeArray(int *a); //배열에 할당된 메모리 해제 함수 선언
void printArray(int *a); //배열 프린트 함수 선언 

int selectionSort(int *a); //선택정렬함수
int insertionSort(int *a); //정렬에 삽입 
int bubbleSort(int *a); //버블정렬함수
int shellSort(int *a); //셸 정렬 함수
/* recursive function으로 구현 */
int quickSort(int *a, int n); //퀵 정렬 함수


/* hash code generator, key % MAX_HASH_TABLE_SIZE */
int hashCode(int key); 

/* array a에대 한 hash table을 만든다. */
int hashing(int *a, int **ht); 

/* hash table에서 key를 찾아 hash table의 index return */
int search(int *ht, int key); 


int main()
{
	char command; //메뉴선택저장변수
	int *array = NULL; //정수형 포인터 array 선언 후 초기화
	int *hashtable = NULL; //정수형 포인터 hashtable 선언 후 초기화 
	int key = -1; //해싱을 위한 정수형 변수 key 선언 후 초기화
	int index = -1;

	srand(time(NULL)); //난수 생성

	do{
		printf("\n");
		printf("[-----[Gahyun] [2020045070] -----\n");
		printf("----------------------------------------------------------------\n");
		printf("                        Sorting & Hashing                       \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize       = z           Quit             = q\n");
		printf(" Selection Sort   = s           Insertion Sort   = i\n");
		printf(" Bubble Sort      = b           Shell Sort       = l\n");
		printf(" Quick Sort       = k           Print Array      = p\n");
		printf(" Hashing          = h           Search(for Hash) = e\n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initialize(&array); //array 초기화
			break;
		case 'q': case 'Q':
			freeArray(array); //array에 할당된 메모리 해제
			break;
		case 's': case 'S':
			selectionSort(array); //선택정렬
			break;
		case 'i': case 'I':
			insertionSort(array); //배열에 삽입
			break;
		case 'b': case 'B':
			bubbleSort(array); //버블정렬
			break;
		case 'l': case 'L':
			shellSort(array); //셸정렬
			break;
		case 'k': case 'K':
			printf("Quick Sort: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array); //배열 출력 (퀵 정렬 전)
			quickSort(array, MAX_ARRAY_SIZE); //퀵 정렬
			printf("----------------------------------------------------------------\n");
			printArray(array);//배열 출력(퀵 정렬 후)

			break;

		case 'h': case 'H':
			printf("Hashing: \n"); 
			printf("----------------------------------------------------------------\n");
			printArray(array);
			hashing(array, &hashtable); //해싱
			printArray(hashtable); //해싱 결과 출력
			break;

		case 'e': case 'E':
			printf("Your Key = ");
			scanf("%d", &key); //key 값 입력받기
			printArray(hashtable);
			index = search(hashtable, key); //탐색한 주소 index에 저장
			printf("key = %d, index = %d,  hashtable[%d] = %d\n", key, index, index, hashtable[index]); 
			//탐색한 결과 출력하기
			break;

		case 'p': case 'P':
			printArray(array);
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

int initialize(int** a)
{
	int *temp = NULL;

	/* array가 NULL인 경우 메모리 할당 */
	if(*a == NULL) {
		temp = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE); //temp에 정수크기만큼 MAX_ARRAY_SIZE개(13개) 메모리 할당
		*a = temp;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else
		temp = *a; //temp에 a가 가리키는 값 저장

	/* 랜덤값을 배열의 값으로 저장 */
	for(int i = 0; i < MAX_ARRAY_SIZE; i++) //배열의 크기만큼 반복
		temp[i] = rand() % MAX_ARRAY_SIZE; //랜덤값 저장

	return 0;
}

int freeArray(int *a)
{
	if(a != NULL) //array a가 NULL이 아닐 경우(array a가 존재할 경우)
		free(a); //a에 할당된 메모리 해제
	return 0;
}

void printArray(int *a)
{
	if (a == NULL) { //a가 비어있을 경우
		printf("nothing to print.\n");
		return;
	}
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		printf("a[%02d] ", i); 
	printf("\n");
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		printf("%5d ", a[i]); //a에 저장된 값 출력
	printf("\n");
}


int selectionSort(int *a) //선택정렬
{
	int min; //최솟값 저장을 위한 변수
	int minindex; //최솟값 인덱스 저장을 위한 변수
	int i, j;

	printf("Selection Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a); //정렬 전 array a 출력

	for (i = 0; i < MAX_ARRAY_SIZE; i++) //array a만큼 반복
	{
		minindex = i; //minindex에 i 저장
		min = a[i]; //min에 a[i] 저장
		for(j = i+1; j < MAX_ARRAY_SIZE; j++) //array a 만큼 반복
		{
			if (min > a[j]) //min에 저장된 값이 a[j]보다 클 경우 (비교하는 값이 더 작을경우)
			{
				min = a[j]; //min에 a[j]를 저장
				minindex = j; //minindex에 j를 저장
			}
		}
		a[minindex] = a[i]; //a[minindex]에 a[i]를 저장
		a[i] = min; //a[i]에 min 저장
	}

	printf("----------------------------------------------------------------\n");
	printArray(a); //정렬 후 array a 출력
	return 0;
}

int insertionSort(int *a) //삽입
{
	int i, j, t;

	printf("Insertion Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a); //삽입 전 array 출력

	for(i = 1; i < MAX_ARRAY_SIZE; i++)
	{
		t = a[i]; //t에 a[i] 저장
		j = i; //j에 i 저장 
		while (a[j-1] > t && j > 0) //a[j-1]가 t보다 크거나 j가 0보다 클 동안
		{
			a[j] = a[j-1]; //a[j]에 a[j-1] 저장
			j--; //j 감소
		}
		a[j] = t; //a[j]에 t 저장
	}

	printf("----------------------------------------------------------------\n");
	printArray(a); //삽입 후 array 출력

	return 0;
}

int bubbleSort(int *a)
{
	int i, j, t;

	printf("Bubble Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a); //bubble 정렬 전 array a 출력

	for(i = 0; i < MAX_ARRAY_SIZE; i++) 
	{
		for (j = 0; j < MAX_ARRAY_SIZE; j++)
		{
			if (a[j-1] > a[j]) //a[j-1]이 a[j]보다 크면
			{
				//a[j-1]과 a[j]의 값 바꾸기
				t = a[j-1]; //t에 a[j-1]저장
				a[j-1] = a[j]; //a[j-1]에 a[j] 저장
				a[j] = t; //a[j]에 t 저장
			}
		}
	}

	printf("----------------------------------------------------------------\n");
	printArray(a); //정렬 후 array a 출력 

	return 0;
}

int shellSort(int *a)
{
	int i, j, k, h, v;

	printf("Shell Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a); //정렬 전 array a 출력

	for (h = MAX_ARRAY_SIZE/2; h > 0; h /= 2) //h를 원소개수의 1/2로 사용 h가 1이 될때까지 반복
	{
		for (i = 0; i < h; i++) //h의 개수만큼 반복 (단계)
		{
			for(j = i + h; j < MAX_ARRAY_SIZE; j += h) //단계안에서 비교하는 반복
			{
				v = a[j]; //v에 a[j] 저장
				k = j; //k에 j 저장
				while (k > h-1 && a[k-h] > v) //k가 h-1보다 크고, a[k-h]가 v보다 클 동안
				{
					a[k] = a[k-h];//a[k]에 a[k-h]저장
					k -= h; //k에 k-h 저장
				}
				a[k] = v; //a[k]에 v 저장 
			}
		}
	}
	printf("----------------------------------------------------------------\n");
	printArray(a); //정렬 후 array a 출력

	return 0;
}

int quickSort(int *a, int n)
{
	int v, t;
	int i, j;

	if (n > 1) //n이 1보다 클 경우
	{
		v = a[n-1]; //v에 a[n-1]저장
		i = -1; //i 초기화
		j = n - 1; //j에 n-1 저장

		while(1)
		{
			while(a[++i] < v); //왼쪽 부분 집합이 피봇보다 작을 동안
			while(a[--j] > v); //오른쪽 부분 집합 피봇보다 클 동안 

			if (i >= j) break; //i가 j보다 크거나 같아지면 멈춤
			//a[i]와 a[j]값 교환
			t = a[i];
			a[i] = a[j];
			a[j] = t;
		}

		//a[i]와 a[n-1]값 교환
		t = a[i];
		a[i] = a[n-1];
		a[n-1] = t;

		quickSort(a, i); //왼쪽 부분집합 quick 정렬
		quickSort(a+i+1, n-i-1); //오른쪽 부분집합 quick 정렬 
	}


	return 0;
}

int hashCode(int key) {
   return key % MAX_HASH_TABLE_SIZE; //key로 hashtable크기로 나눈 나머지 값 출력 ()
}

int hashing(int *a, int **ht)
{
	int *hashtable = NULL; //포인터 hashtable NULL로 초기화

	/* hash table이 NULL인 경우 메모리 할당 */
	if(*ht == NULL) {
		hashtable = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE); //hashtable에 정수 크기만큼 MAX_ARRAY_SIZE개 메모리 할당
		*ht = hashtable;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else {
		hashtable = *ht;	/* hash table이 NULL이 아닌경우, table 재활용, reset to -1 */
	}

	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		hashtable[i] = -1; //hashtable[i]에 -1로 초기화

	/*
	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++) //hashtable 출력
		printf("hashtable[%d] = %d\n", i, hashtable[i]);
	*/

	int key = -1;
	int hashcode = -1;
	int index = -1;
	for (int i = 0; i < MAX_ARRAY_SIZE; i++) //배열만큼 반복
	{
		key = a[i]; //key에 a[i] 저장
		hashcode = hashCode(key); //생성한 hashtable을 hashcode에 저장
		/*
		printf("key = %d, hashcode = %d, hashtable[%d]=%d\n", key, hashcode, hashcode, hashtable[hashcode]);
		*/
		if (hashtable[hashcode] == -1) //hastable[hashcode]가 -1일 경우
		{
			hashtable[hashcode] = key; //hashtable[hashcode]에 입력받은 key 저장
		} else 	{

			index = hashcode; //index에 hashcode 값 저장

			while(hashtable[index] != -1) //hashtable[index]가 -1이 아닐 동안
			{
				index = (++index) % MAX_HASH_TABLE_SIZE;
				/*
				printf("index = %d\n", index);
				*/
			}
			hashtable[index] = key; //hashtable[index]에 key 저장
		}
	}

	return 0;
}

int search(int *ht, int key) //탐색
{
	int index = hashCode(key); //index값에 생성한 hashtable 저장

	if(ht[index] == key) //ht[index]가 key값과 일치할 경우
		return index; //index값 리턴

	while(ht[++index] != key) //ht[증가한 index]가 key가 아닐 경우
	{
		index = index % MAX_HASH_TABLE_SIZE; //index에 
	}
	return index; //index 값 리턴
}



