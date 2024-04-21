#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>





// Максимальная длина слова в словаре
#define MAX_LEN_WORD 80

#define MAX_HASH 13267
#define MAX_CHAIN 1000

struct WordCount {
	char* word;
	unsigned count;
};

// Массив списков
struct WordCount wordsCounts[MAX_HASH] = { {NULL, 0} };



// Вычисление хэша для строки word
int hash(char* word) {
	unsigned long hash_value = 0;
	int i = 0;
	while (word[i] != '\0') {
		hash_value = 31 * hash_value + ((unsigned)(word[i]));
		i++;
	}
	return hash_value % MAX_HASH;
}


/* createDictionary - создает словарь.
Вызывается перед началом использования словаря. */
void createDictionary(void) {
	for (int i = 0; i < MAX_HASH; i++) {
		wordsCounts[i].word = NULL;
		wordsCounts[i].count = 0;
	}
}


/* destroyDictionary - уничтожает словарь.
Вызывается после окончания использования словаря. */
void destroyDictionary(void) {
	for (int i = 0; i < MAX_HASH; i++) {
		if (wordsCounts[i].word) {
			free(wordsCounts[i].word);		
		}
		wordsCounts[i].count = 0;
	}
}


/* addWordToDictionary – добавляет элемент в множество.
Множество – содержит только уникальные слова.
При первом добавлении создается новый элемент со счетчиком равным 1.
При повторной попытке добавить слово, у соответствующего элемента увеличивается счетчик на 1.


Возвращаемые значения:
0 - если не смог добавить элемент (нехватка динамической памяти ИЛИ хэш-таблица (почти) заполнена ).
1 - если слово уже было - счетчик увеличился на 1.
2 - если такого слова в словаре не было - новый элемент создан, счетчик получил значение 1.
*/
int addWordToDictionary(char* word) {
	int hash_value = hash(word);

	int index = hash_value;
	int step = 1;

	while (step <= MAX_CHAIN) {
		
		if (wordsCounts[index].word == NULL) {
			int wordLength = strlen(word) + 1;
			wordsCounts[index].word = (char*)calloc(wordLength, sizeof(char));
			
			if (wordsCounts[index].word) {
				strncpy(wordsCounts[index].word, word, wordLength);
				wordsCounts[index].count = 1;
				return 2;
			}
			else {
				return 0;
			}
			
		} else if (strcmp(wordsCounts[index].word, word) == 0) {
			wordsCounts[index].count++;
			return 1;
		}


		index++;
		if (index >= MAX_HASH) {
			index = 0;
		}

		step++;
	}

	// слишком длинная цепочка (> MAX_CHAIN)  - означает, что таблица заполнена слишком плотно
	return 0;
}


/* printDictionary – выводит в консоль все слова из словаря.
Выводятся в формате
"слово"->сколько_раз_встретилось_в_файле
*/
void printDictionary(void) {

	for (int i = 0; i < MAX_HASH; i++) {
		if (wordsCounts[i].word) {
			printf("\"%s\"->%d\n", wordsCounts[i].word, wordsCounts[i].count);
		}
	}

}

int getNextDelim(FILE* fp, char token[]);
int getNextWord(FILE* fp, char token[]);


/* loadDictionary – загружает словарь из файла.
filename - имя файла, из которого берутся слова.

Возвращаемое значение:
0 - если файл не смогли прочитать.
1 - если успешно загрузили все слова из файла

*/
int loadDictionary(char* filename) {
	// открыть файл
	FILE* fin = fopen( filename, "r");
	if (fin == NULL) {
		// если файл не смогли открыть - сообщаем об этом
		printf("File %s doesn't open!\n", filename);
		return 0;
	}

	createDictionary();
	char token[MAX_LEN_WORD + 1];

	//int cnt = 0;

	// пока не конец файла
	while (!feof(fin)) {
		// пока есть разделитель - берем его
		while (getNextDelim(fin, token)) {
		}
		// если есть слово - берем его
		if (getNextWord(fin, token)) {
			// если не смогли добавить слово в словарь 
			if (!addWordToDictionary(token)) {
				// прекращаем обработку

				printf("EXIT: !addWordToDictionary(token)\n");
				fclose(fin);
				destroyDictionary();
				return 0;
			}
		}

		
		//cnt++;
	}
	// Закрываем файл с текстом
	fclose(fin);
	return 1;
}




int main(int argc, char* argv[]) {
	printf("It's HomeWork 03!\n");

	if (argc < 2) {
		printf("Filename is not found\n");
		return 1;
	}

	//char filename[] = "C:\\Temp\\Otus\\HW3\\text1.txt";
	//	if (loadDictionary(filename)) {
	printf("Starting working with file %s !\n", argv[1]);

	if (loadDictionary(argv[1])) {
		printDictionary();
		destroyDictionary();
		return 0;
	}

	// Не смогли загрузить словарь
	return 2;
}



int isalpha_my(unsigned char ch);



// Возвращает 1 - если  из файла прочитан разделитель.
// В этом случае в token возвращается строка, содержащая 
// этот разделитель.
// Если в файле был не разделитель - возвращает 0.
// В этом случае состояние token неопределено.
int getNextDelim(FILE* fp, char token[])
{
	int ch = getc(fp);
	if (ch == EOF) {
		return 0;
	}
	if (isalpha_my((unsigned char)ch)) {
		ungetc(ch, fp);
		return 0;
	}
	token[0] = (unsigned char)ch;
	token[1] = '\0';
	return 1;

}


// Возвращает 1 - если  из файла прочитано слово.
// В этом случае в token возвращается строка, содержащая 
// это слово. Гарантируется что слово не более MAX_LEN_WORD символов.
// Если в файле не было буквы - возвращает 0.
// В этом случае token содержит пустую строку.
int getNextWord(FILE* fp, char token[])
{
	int i = 0;
	int ch;
	while (((ch = getc(fp)) != EOF) && (i < MAX_LEN_WORD)) {
		if (!isalpha_my((unsigned char)(ch))) {
			break;
		}
		token[i++] = (unsigned char) ch;
	}
	ungetc(ch, fp);
	token[i] = '\0';
	if (i == 0)
		return 0;
	return 1;
}



// Возвращает 0 - если ch - не буква.
// Возвращает 1 - если ch - буква.
// Корректно работает для латинских букв (с кодами < 128)
// И для русских букв из кодировки ANSI
int isalpha_my(unsigned char ch) {

	if (isalpha(ch))
		return 1;

	// ANSI кодировка!!!
	if (ch >= 192 && ch <= 223)
		return 1;
	if (ch >= 224 && ch <= 255)
		return 1;

	return 0;
}

