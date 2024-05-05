#include <stdio.h>
#include <stdlib.h> 


//#define EXTENDED_FEATURES


// оригинальные данные из main.asm
long long data[] = { 4, 8, 15, 16, 23, 42 };
char int_format[] = "%ld ";
char empty_str[] = "";


// Код  proc print_int
long long print_int(long long arg1) {
	printf(int_format, arg1);
	fflush(NULL);
	return 0;
}

// Код  proc p
long long p(long long arg1) {
	return arg1 & 0x1;
}

// используемая в main.asm структура данных
struct list_item {
	long long value;
	struct list_item * next;
};

// Код  proc add_element
struct list_item* add_element(long long arg1, struct list_item* arg2) {
	struct list_item * p = malloc(16);
	if (p == NULL) {
		abort();
	}
	p->value = arg1;
	p->next = arg2;
	return p;
}

// Код  proc m
void  map(struct list_item* arg1, long long (*fp2)(long long fp2_arg1)) {

	if (arg1 == NULL) {
		goto outm;		
	}

	fp2(arg1->value);
	map(arg1->next, fp2);

outm:
	return;
}

// Код, аналогичный  proc m, но реализованный не рекурсивно, а итерационно
void  map2(struct list_item* arg1, long long (*fp2)(long long fp2_arg1)) {

	while (arg1 != NULL) {

		fp2(arg1->value);

		arg1 = arg1->next;
			
	}
	return;	
}

// Код  proc f
struct list_item* filter(struct list_item* arg1, struct list_item* arg2, long long (*fp3)(long long fp3_arg1)) {

	if (arg1 == NULL) {
		goto outf;
	}

	if (fp3(arg1->value)) {
		arg2 = add_element(arg1->value, arg2);
	}

	arg2 = filter(arg1->next, arg2, fp3);

outf:
	return arg2;
}

// Код, аналогичный  proc f, но реализованный не рекурсивно, а итерационно
struct list_item* filter2(struct list_item* arg1, struct list_item* arg2, long long (*fp3)(long long fp3_arg1)) {

	while (arg1 != NULL) {

		if (fp3(arg1->value)) {
			arg2 = add_element(arg1->value, arg2);

		}

		arg1 = arg1->next;
	}

	return arg2;
}

//Код, отсутствующий в оригинальном  main.asm файле, но необходимый для очистки (удаления) списков
void delete_list(struct list_item * first) {
	while (first != NULL) {
		struct list_item * del = first;	
		first = first-> next;
		free(del);		
	}
}



// Код  main proc 
int main(void) {
	
	long long data_length = sizeof(data) / sizeof(data[0]);

	struct list_item* rax = NULL;
	long long index = data_length;

	do {
		rax = add_element(data[index - 1], rax);
		index--;
	} while (index != 0);
	
	
	struct list_item* rbx = rax;

	// вывод исходного списка
	map2(rax, print_int);
	puts(empty_str);

	// на иснове исходного списка, создается фильтрованный список (содержит только нечетные значения)
	rax = filter2(rbx, 0, p);
	
#ifdef	EXTENDED_FEATURES
	struct list_item* r12 = rax;
#endif
	
	// вывод отфильтрованного списка
	map2(rax, print_int);
	puts(empty_str);
	
#ifdef	EXTENDED_FEATURES
	// контрольный вывод
	map2(rbx, print_int);
	puts(empty_str);
	map2(r12, print_int);
	puts(empty_str);


	// очистка исходного списка
	delete_list(rbx);
	rbx = NULL;
	
	// очистка отфильтрованного списка
	delete_list(r12);
	r12 = NULL;

		
	// контрольный вывод
	map2(rbx, print_int);
	puts(empty_str);
	map2(r12, print_int);
	puts(empty_str);
#endif

	return 0;
}
