/*
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH (50)
#define MAX_LINE (1024)
#define FILE_NOT_OPEN (-1)
#define FAILED_MEMORY_ALLOCATION (NULL)

struct _town;
typedef struct _town* TownPosition;
typedef struct _town
{
	char name[MAX_LENGTH];
	int population;
	TownPosition next;
	TownPosition left;
	TownPosition right;
} Town;

struct _country;
typedef struct _country* CountryPosition;
typedef struct _country
{
	char name[MAX_LENGTH];
	CountryPosition next;
	CountryPosition left;
	CountryPosition right;
	TownPosition townRootTree;
	Town townListHead;
} Country;

CountryPosition readAndFillCountries(CountryPosition countryHeadList, CountryPosition countryRootTree);
int createNewCountryFromBuffer(char* countryName, char* countryFile, CountryPosition* countries);
CountryPosition createNewCountry(char* countryName);
TownPosition createNewTown(char* townName, int townPopulation);
int insertSortedNewTownList(TownPosition townHeadList, TownPosition newTown);
int insertTownAfter(TownPosition townCurrent, TownPosition newTown);
TownPosition insertNewTownTree(TownPosition townRootTree, TownPosition newTown);
int insertSortedNewCountryList(CountryPosition townHeadList, CountryPosition newCountry);
int insertCountryAfter(CountryPosition countryCurrent, CountryPosition newCountry);
CountryPosition insertNewCountryTree(CountryPosition countryRootTree, CountryPosition newCountry);
int printCountryList(CountryPosition countryHeadList);
int printCountryTree(CountryPosition countryRootTree);
int printTownList(TownPosition townHeadList);
int printTownTree(TownPosition townRootTree);
int findCountries(CountryPosition countryHeadList, CountryPosition countryRootTree, CountryPosition* countries);
CountryPosition searchCountryTree(CountryPosition countryRootTree, char* countryName);

int main() {							//08:00
	Country countryHeadList = {
		.name = { 0 },
		.next = NULL,
		.left = NULL,
		.right = NULL,
		.townRootTree = NULL,
		.townListHead = {
			.name = { 0 },
			.population = 0,
			.next = NULL,
			.left = NULL,
			.right = NULL,
		}
	};

	CountryPosition countryRootTree = NULL;

	CountryPosition countryCurrentList = NULL;
	CountryPosition countryCurrentTree = NULL;
	CountryPosition countries[] = { NULL, NULL };

	countryRootTree = readAndFillCountries(&countryHeadList, countryRootTree);		//09:00
	printCountryList(&countryHeadList);
	printf("\n\n\n\n");
	printCountryTree(countryRootTree);

	findCountries(&countryHeadList, countryRootTree, countries);

	countryCurrentList = countries[0];
	countryCurrentTree = countries[1];

	printf("\nCountry list: %s", countryCurrentList->name);
	printf("\nCountry tree: %s", countryCurrentTree->name);


	return EXIT_SUCCESS;
}

CountryPosition readAndFillCountries(CountryPosition countryHeadList, CountryPosition countryRootTree) {
	FILE* filePointer = NULL;
	char countryName[MAX_LINE] = { 0 };
	char countryFile[MAX_LINE] = { 0 };
	char nullString[MAX_LINE] = { 0 };
	CountryPosition newCountryList = NULL;
	CountryPosition newCountryTree = NULL;
	CountryPosition countries[] = { NULL, NULL };

	filePointer = fopen("drzave.txt", "r");								//09:22
	if (!filePointer) {
		printf("\033[31mCan't open file!\033[0m\n");
		return FILE_NOT_OPEN;
	}

	while (!feof(filePointer)) {
		fscanf(filePointer, " %s %s", countryName, countryFile);
		createNewCountryFromBuffer(countryName, countryFile, countries);
		newCountryList = countries[0];
		newCountryTree = countries[1];
		insertSortedNewCountryList(countryHeadList, newCountryList);
		countryRootTree = insertNewCountryTree(countryRootTree, newCountryTree);
		strcpy(countryName, nullString);// provjeri treba li
		strcpy(countryFile, nullString);// provjeri treba li
	}

	fclose(filePointer);

	return countryRootTree;
}

int createNewCountryFromBuffer(char* countryName, char* countryFile, CountryPosition* countries) {			//11:15
	char countryLocation[MAX_LENGTH] = { 0 };
	FILE* countryFilePointer = NULL;
	CountryPosition newCountryList = NULL;
	CountryPosition newCountryTree = NULL;
	TownPosition newTownList = NULL;
	TownPosition newTown = NULL;
	char townName[MAX_LENGTH] = { 0 };
	int townPopulation = 0;

	strcpy(countryLocation, countryFile);

	countryFilePointer = fopen(countryLocation, "r");
	if (!countryFilePointer) {
		printf("\033[31mCan't open file!\033[0m\n");
		return FILE_NOT_OPEN;
	}

	newCountryList = createNewCountry(countryName);
	newCountryTree = createNewCountry(countryName);

	while (!feof(countryFilePointer)) {													//13:00
		fscanf(countryFilePointer, " %s %d", townName, &townPopulation);
		newTownList = createNewTown(townName, townPopulation);
		newTown = createNewTown(townName, townPopulation);
		insertSortedNewTownList(&newCountryTree->townListHead, newTownList);
		newCountryList->townRootTree = insertNewTownTree(newCountryList->townRootTree, newTown);
		newTownList = NULL;// provjeri treba li
	}
	countries[0] = newCountryList;
	countries[1] = newCountryTree;

	fclose(countryFilePointer);

	return EXIT_SUCCESS;
}

CountryPosition createNewCountry(char* countryName) {
	CountryPosition country = NULL;
	country = (CountryPosition)malloc(sizeof(Country));
	if (!country) {
		printf("Can't allocate memory!\n");
		return FAILED_MEMORY_ALLOCATION;
	}


	strcpy(country->name, countryName);
	country->next = NULL;
	country->left = NULL;
	country->right = NULL;
	country->townRootTree = NULL;
	//strcpy(country->townHeadList.name, NULL);
	country->townListHead.population = 0;
	country->townListHead.next = NULL;
	country->townListHead.left = NULL;
	country->townListHead.right = NULL;

	return country;
}

TownPosition createNewTown(char* townName, int townPopulation) {
	TownPosition town = NULL;
	town = (TownPosition)malloc(sizeof(Town));
	if (!town) {
		printf("Can't allocate memory!\n");
		return FAILED_MEMORY_ALLOCATION;
	}

	strcpy(town->name, townName);
	town->population = townPopulation;
	town->next = NULL;
	town->left = NULL;
	town->right = NULL;

	return town;
}

int insertSortedNewTownList(TownPosition townHeadList, TownPosition newTown) {
	TownPosition townCurrent = townHeadList;
	while (townCurrent->next != NULL && townCurrent->next->population > newTown->population) {
		townCurrent = townCurrent->next;
	}

	insertTownAfter(townCurrent, newTown);

	return EXIT_SUCCESS;
}

int insertTownAfter(TownPosition townCurrent, TownPosition newTown) {
	newTown->next = townCurrent->next;
	townCurrent->next = newTown;

	return EXIT_SUCCESS;
}

TownPosition insertNewTownTree(TownPosition townRootTree, TownPosition newTown) {
	if (townRootTree == NULL)
		return newTown;

	if (newTown->population > townRootTree->population)//dodaj provjeru po imenima naknadno...
		townRootTree->left = insertNewTownTree(townRootTree->left, newTown);
	else if (newTown->population <= townRootTree->population)
		townRootTree->right = insertNewTownTree(townRootTree->right, newTown);

	return townRootTree;
}

int insertSortedNewCountryList(CountryPosition townHeadList, CountryPosition newCountry) {
	CountryPosition countryCurrent = townHeadList;
	while (countryCurrent->next != NULL && strcmp(countryCurrent->next->name, newCountry->name) < 0) {
		countryCurrent = countryCurrent->next;
	}

	insertCountryAfter(countryCurrent, newCountry);

	return EXIT_SUCCESS;
}

int insertCountryAfter(CountryPosition countryCurrent, CountryPosition newCountry) {
	newCountry->next = countryCurrent->next;
	countryCurrent->next = newCountry;

	return EXIT_SUCCESS;
}

CountryPosition insertNewCountryTree(CountryPosition countryRootTree, CountryPosition newCountry) {
	if (countryRootTree == NULL)
		return newCountry;

	if (strcmp(countryRootTree->name, newCountry->name) > 0)
		countryRootTree->left = insertNewCountryTree(countryRootTree->left, newCountry);
	else if (strcmp(countryRootTree->name, newCountry->name) <= 0)
		countryRootTree->right = insertNewCountryTree(countryRootTree->right, newCountry);

	return countryRootTree;
}

int printCountryList(CountryPosition countryHeadList) {
	CountryPosition currentCountry = countryHeadList->next;
	while (currentCountry != NULL) {
		printf("\n%s", currentCountry->name);
		printTownTree(currentCountry->townRootTree);
		currentCountry = currentCountry->next;
	}
}

int printCountryTree(CountryPosition countryRootTree) {
	if (countryRootTree) {
		printCountryTree(countryRootTree->left);
		printf("\n%s", countryRootTree->name);
		printTownList(&countryRootTree->townListHead);
		printCountryTree(countryRootTree->right);
	}
	return EXIT_SUCCESS;
}

int printTownList(TownPosition townHeadList) {
	TownPosition currentTown = townHeadList->next;
	while (currentTown != NULL) {
		printf("\n\t%s %d", currentTown->name, currentTown->population);
		currentTown = currentTown->next;
	}
}

int printTownTree(TownPosition townRootTree) {
	if (townRootTree) {
		printTownTree(townRootTree->left);
		printf("\n\t%s %d", townRootTree->name, townRootTree->population);
		printTownTree(townRootTree->right);
	}
	return EXIT_SUCCESS;
}

int findCountries(CountryPosition countryHeadList, CountryPosition countryRootTree, CountryPosition* countries) {
	CountryPosition countryCurrentList = countryHeadList->next;
	CountryPosition countryCurrentTree = countryRootTree;
	char countryName[MAX_LENGTH] = { 0 };
	printf("\n\nEnter country name: ");
	scanf(" %s", countryName);

	while (countryCurrentList != NULL && strcmp(countryCurrentList->name, countryName)) {
		countryCurrentList = countryCurrentList->next;
	}

	countryCurrentTree = searchCountryTree(countryRootTree, countryName);

	countries[0] = countryCurrentList;
	countries[1] = countryCurrentTree;

	return EXIT_SUCCESS;
}

CountryPosition searchCountryTree(CountryPosition countryRootTree, char* countryName) {
	if (countryRootTree == NULL || strcmp(countryRootTree->name, countryName) == 0)
		return countryRootTree;

	if (strcmp(countryRootTree->name, countryName) > 0)
		return searchCountryTree(countryRootTree->left, countryName);

	return searchCountryTree(countryRootTree->right, countryName);
}
*/

//10. Napisati program koji cita datoteku drzave.txt u kojoj su zapisani nazivi pojedinih dr�ava.Uz
//ime dr�ave u datoteci se nalazi i ime dodatne datoteke u kojoj se nalaze gradovi pojedine
//dr�ave.Svaka datoteka koja predstavlja dr�avu sadr�i popis gradova u formatu naziv_grada,
//broj_stanovnika.
//a) Potrebno je formirati sortiranu vezanu listu dr�ava po nazivu.Svaki ?vor vezane liste
//sadr�i stablo gradova sortirano po broju stanovnika, zatim po nazivu grada.
//b) Potrebno je formirati stablo dr�ava sortirano po nazivu.Svaki ?vor stabla sadr�i vezanu
//listu gradova sortiranu po broju stanovnika, zatim po nazivu grada.

// a)

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _listCity;
typedef struct _listCity* PositionCity;
typedef struct _listCity {
	char name[50];
	int numCitizens;
	PositionCity next;
} City;

struct _binaryTreeCountry;
typedef struct _binaryTreeCountry* PositionCountry;
typedef struct _binaryTreeCountry {
	char name[50];
	PositionCountry left;
	PositionCountry right;
	PositionCity head; //ovo ce vezat vezanu listu gradova na drzavu
} Country;

PositionCountry readCountryNames(PositionCountry root);
PositionCountry insertCountry(PositionCountry root, char* countryName);
PositionCity insertCity(char* fileName);
int insertIntoList(PositionCity Head, int numOfCitizens, char* city);
PositionCity search(PositionCountry root, char* countryName);
int inorder(PositionCountry root);
int printList(PositionCity Head);

int main() {

	PositionCountry root = NULL;


	root = readCountryNames(root);

	inorder(root);

	return 0;
}

PositionCountry readCountryNames(PositionCountry root) {
	FILE* filePointer = NULL;
	filePointer = fopen("drzave.txt", "r");
	if (!filePointer) {
		printf("Nije moguce otvoriti datoteku drzave\n");
		return -1;
	}

	//triba nan root prvog elementa stabla
	char countryName[50] = "", fileName[50] = "";
	fscanf(filePointer, " %s %s", countryName, fileName);
	root = insertCountry(root, countryName);

	PositionCity head = insertCity(fileName);
	root->head = head;

	while (!feof(filePointer)) {
		char countryName[50] = "", fileName[50] = "";
		fscanf(filePointer, " %s %s", countryName, fileName);
		//printf("%s %s\n", countryName, fileName); //provjera
		insertCountry(root, countryName);

		//vamo je greska ovi currentcountry uvik pokaziva na isto jer insert county uvih vraca root prvog
		PositionCountry currentCountry = insertCountry(root, countryName);

		//sa ovin ispisiva nesto beskonacno
		//PositionCountry currentCountry = search(root, countryName);

		PositionCity head = insertCity(fileName);
		currentCountry->head = head;
	}
	fclose(filePointer);
	return root;
}

PositionCountry insertCountry(PositionCountry root, char* countryName) {
	if (root == NULL) {
		PositionCountry newElement = (PositionCountry)malloc(sizeof(Country));
		if (newElement == NULL) {
			printf("Krivo alocirana memorija\n");
			return -1;
		}
		else {
			strcpy(newElement->name, countryName);
			newElement->left = NULL;
			newElement->right = NULL;
			newElement->head = NULL;
		}
		root = newElement;
	}
	else if (strcmp(countryName, root->name) < 0) {
		root->left = insertCountry(root->left, countryName);
	}
	else if (strcmp(countryName, root->name) > 0) {
		root->right = insertCountry(root->right, countryName);
	}
	return root;
}

PositionCity insertCity(char* fileName) {
	FILE* filePointer = NULL;
	filePointer = fopen(fileName, "r");
	if (!filePointer) {
		printf("Nije moguce otvoriti datoteku drzave\n");
		return -1;
	}

	City Head = { .name = "", .numCitizens = 0, .next = NULL };

	while (!feof(filePointer)) {
		char city[50] = "";
		int numOfCitizens = 0;
		fscanf(filePointer, " %s %d", city, &numOfCitizens);
		//printf("\n %s %d i pozvana funkcija TEST\n", city, numOfCitizens);
		insertIntoList(&Head, numOfCitizens, city);
	}
	fclose(filePointer);
	//printList(&Head);
	return &Head;
}

int insertIntoList(PositionCity Head, int numOfCitizens, char* city) {
	PositionCity newElement = NULL;
	newElement = (PositionCity)malloc(sizeof(City));
	if (!newElement) {
		printf("Kivo alocirana memorija\n");
		return -1;
	}

	strcpy(newElement->name, city);
	newElement->numCitizens = numOfCitizens;
	newElement->next = NULL;

	//sortiranje u listu gradova
	if (Head->next != NULL) {
		while (Head->next != NULL && Head->next->numCitizens > numOfCitizens) {
			Head = Head->next;
		}
		newElement->next = Head->next;
		Head->next = newElement;
	}
	else {
		newElement->next = Head->next;
		Head->next = newElement;
	}
	return 0;
}

PositionCity search(PositionCountry root, char* countryName) {
	if (root == NULL) {
		return NULL;
	}
	else if (strcmp(countryName, root->name) < 0) {
		root->left = search(root->left, countryName);
	}
	else if (strcmp(countryName, root->name) > 0) {
		root->right = search(root->right, countryName);
	}
	else if (strcmp(countryName, root->name) == 0) {
		return root;
	}
	else {
		return root;
	}
}

int inorder(PositionCountry root) {
	if (root != NULL) {
		inorder(root->left);
		printf(" %s", root->name);
		//printList(root->head);
		inorder(root->right);
	}
	return 0;
}

int printList(PositionCity Head) {
	if (Head->next == NULL) {
		printf("Greska nema gradova\n");
		return -1;
	}
	else {
		PositionCity current = Head->next;
		while (current->next != NULL) {
			printf(" %s", current->name);
		}
	}
	return 0;
}
