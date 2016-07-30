#include <string.h>
#include "search-engine.h"

DictionaryType dictType;
int searches = 0; 
SearchEngine::SearchEngine( int port, DictionaryType dictionaryType):
	MiniHTTPD(port)
{
	// Create dictionary of the indicated type
	int numLines = 0; 
	int count = 0;
	int c;
	char * url; 
	char * desc; 
	int index; 
	dictType = dictionaryType;
	switch (dictionaryType) {
		case ArrayDictionaryType:
			_wordToURLList = new ArrayDictionary();
			break;
		case BinarySearchDictionaryType:
			_wordToURLList = new BinarySearchDictionary();
			break; 
		case HashDictionaryType:
			_wordToURLList = new HashDictionary();
			break;
		case AVLDictionaryType:
			_wordToURLList = new AVLDictionary();
			break;
		default:
			_wordToURLList = NULL;
			break; 


	}
	FILE * file = fopen("url.txt", "r"); 
	while ((c = fgetc(file))!= -1)
		{
			if (c != '\n')
				numLines++; 
		}
	fclose(file); 
	int numberUrls = numLines / 3; 
	URLRecord ** store = new URLRecord*[numberUrls];
	int i = 0; 
	while (i < numberUrls) {
		store[i] = new URLRecord();
		i++;
	}
	int size = 1000;
	 file = fopen("url.txt", "r"); 
	char * nums = new char[size];
	char * temp; 
	
	while (fgets(nums, size, file)) {
		if (strcmp(nums, "\n") !=0) {
			temp = strtok(nums, " "); 
			 index = atoi(temp); 
			temp = strtok(NULL, " "); 
			store[index] -> _url = strdup(temp); 
			fgets(nums, size, file);
			temp = strtok(nums, "\n"); 
			store[index] -> _description = strdup(temp); 
		}
	}
	fclose(file);
	FILE * newfile = fopen("word.txt", "rb"); 
	char * word;  
	while (fgets(nums, size, newfile)) {
		if (strcmp(nums, "\n") != 0) {
			temp = strtok(nums, " ");
			word = strdup(temp); 
			URLRecordList * head = NULL; 
			URLRecordList * prev = NULL; 
			temp = strtok(NULL, " \n");
			while (temp != NULL) {
				 index = atoi(temp);
				if (store[index] -> _url == NULL) 
					continue; 
				URLRecordList * node = new URLRecordList(); 
				if (head == NULL)
					head = node;
				node -> _urlRecord = store[index];
				node -> _next = NULL; 
				if (prev != NULL)
					prev -> _next = node;
				prev = node; 
				temp = strtok(NULL, " \n");

			}
			_wordToURLList -> addRecord(word,  head); 
		}
	}
	fclose(newfile); 
	// Populate dictionary and sort it if necessary
}

	void
SearchEngine::dispatch( FILE * fout, const char * documentRequested)
{
	if (strcmp(documentRequested, "/")==0) {
		// Send initial form
		fprintf(fout, "<TITLE>CS251 Search</TITLE>\r\n");
		fprintf(fout, "<CENTER><H1><em>Boiler Search</em></H1>\n");
		fprintf(fout, "<H2>\n");
		fprintf(fout, "<FORM ACTION=\"search\">\n");
		fprintf(fout, "Search:\n");
		fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
		fprintf(fout, "</H2>\n");
		fprintf(fout, "</FORM></CENTER>\n");
		return;
	}
	char * par = strdup(documentRequested + 13);
	char * temp = par; 
	char ** word = new char*[50];
	int count = 0;
	char * token = strtok(par, "+"); 
	while (token != NULL)
	{
		word[count] = strdup(token); 
		count++;
		token = strtok(NULL, "+");
	}
	char *query = new char[100];
	int i = 0;
	for (i = 0; i < count; i++)
	{
		if (i == 0)
			strcpy(query, word[i]);	
		else {
			strcat(query, " "); 
			strcat(query, word[i]);
		}
		
	}
	fprintf(stderr, "Search for words: \"%s\"\n", query);

	fprintf(fout, "<TITLE>Search Results</TITLE>\r\n");
	fprintf(fout, "<H1> <Center><em>Boiler Search</em></H1>\n");
	fprintf(fout, "<H2> Search Results for \"%s\"</center></H2>\n", query);


	int counter;
	counter = 0;

	int listCount;
	listCount = 0;

	URLRecord **list = new URLRecord*[1000];

	for (i = 0; i < count; i++)
	{
		URLRecordList* data;
		data = (URLRecordList*)_wordToURLList->findRecord(word[i]);

		while (data != NULL)
		{
			int exists = 0;

		
			for (int j = 0; j < listCount; j++)
			{
				if (list[j] == data->_urlRecord)
				{
					exists = 1;
				}
			}

			if (exists == 0)
			{
				list[listCount] = data->_urlRecord;
				listCount++;
			}

			data = data->_next;
		}
	}

	for (i = 0; i < listCount; i++)
	{
		
		for (int j = 0; j < count; j++)
		{
			URLRecordList* data;
			data = (URLRecordList*)_wordToURLList->findRecord(word[j]);

			int exists = 0;
			while (data != NULL)
			{
				if (data->_urlRecord == list[i])
				{
					exists = 1;
				}

				data = data->_next;
			}

			if (exists == 0)
				list[i] = NULL;
		}
	}


	for (i = 0; i < listCount; i++)
	{
		if (list[i] == NULL) continue;

		fprintf(fout, "<h3>%d. <a href=\"%s\">%s</a><h3>\n", counter+1, list[i]->_url, list[i]->_url);
		fprintf(fout, "<blockquote>%s<p></blockquote>\n", list[i]->_description);

		counter++;
	}

	searches++;

	fprintf(fout, "<BR>\n");

	if (dictType == ArrayDictionaryType)
		fprintf(fout, "<h3>Dictionary Used: Array Dictionary</h3>\n");
	else if (dictType == HashDictionaryType)
		fprintf(fout, "<h3>Dictionary Used: Hash Dictionary</h3>\n");
	else if (dictType == AVLDictionaryType)
		fprintf(fout, "<h3>Dictionary Used: AVL Dictionary</h3>\n");
	else if (dictType == BinarySearchDictionaryType)
		fprintf(fout, "<h3>Dictionary Used: Binary Search Dictionary</h3>\n");
	else
		fprintf(fout, "<h3>Dictionary Used: Unknown Dictionary</h3>\n");


	// TODO: The words to search in "documentRequested" are in the form
	// /search?word=a+b+c
	//
	// You need to separate the words before search
	// Search the words in the dictionary and find the URLs that
	// are common for al the words. Then print the URLs and descriptions
	// in HTML. Make the output look nicer.

	// Here the URLs printed are hardwired
/*	
	   const int nurls=2;

	   const char * words = "data structures";

	   const char * urls[] = {
	   "http://www.cs.purdue.edu",
	   "http://www.cs.purdue.edu/homes/cs251"
	   };

	   const char * description[] = {
	   "Computer Science Department. Purdue University.",
	   "CS251 Data Structures"
	   };

	   fprintf( stderr, "Search for words: \"%s\"\n", words);

	   fprintf( fout, "<TITLE>Search Results</TITLE>\r\n");
	   fprintf( fout, "<H1> <Center><em>Boiler Search</em></H1>\n");
	   fprintf( fout, "<H2> Search Results for \"%s\"</center></H2>\n", words );

	   for ( int i = 0; i < nurls; i++ ) {
	   fprintf( fout, "<h3>%d. <a href=\"%s\">%s</a><h3>\n", i+1, urls[i], urls[i] );
	   fprintf( fout, "<blockquote>%s<p></blockquote>\n", description[i] );
	   }
*/	   
	// Add search form at the end
	fprintf(fout, "<HR><H2>\n");
	fprintf(fout, "<FORM ACTION=\"search\">\n");
	fprintf(fout, "Search:\n");
	fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
	fprintf(fout, "</H2>\n");
	fprintf(fout, "</FORM>\n");
}

	void
printUsage()
{
	const char * usage =
		"Usage: search-engine port (array | hash | avl | bsearch)\n"
		"  It starts a search engine at this port using the\n"
		"  data structure indicated. Port has to be larger than 1024.\n";

	fprintf(stderr, "%s", usage);
}

int main(int argc, char ** argv)
{
	if (argc < 3) {
		printUsage();
		return 1;
	}

	// Get port
	int port;
	sscanf( argv[1], "%d", &port);

	// Get DictionaryType
	const char * dictType = argv[2];
	DictionaryType dictionaryType;
	if (!strcmp(dictType, "array")) {
		dictionaryType = ArrayDictionaryType;
	}
	else if (!strcmp(dictType, "hash")) {
		dictionaryType = HashDictionaryType;
	}
	else if (!strcmp(dictType, "avl")) {
		dictionaryType = AVLDictionaryType;
	}
	else if (!strcmp(dictType, "bsearch")) {
		dictionaryType = BinarySearchDictionaryType;
	}
	else {
		printUsage();
		return 0;
	}

	SearchEngine httpd(port, dictionaryType);

	httpd.run();

	return 0;
}
