Funkcja exec powinna informowac wyo³uj¹cego o b³êdach (np. nieistnieniu wartoœci lub b³êdach przepe³nienia).
Poni¿ej znajduj¹ siê mo¿liwe rozwi¹zania tego problemu. Czêsto mo¿e siê zdarzyæ, ¿e w przypadku b³êdu
do wyniku powinna zostaæ przpisana wartoœ domyœlna. Konieczny zatem jest prosty mechanizm pozwalaj¹cy na 
ob³ugê takiego przypadku. Wersje 1 i 2 s¹ zbyt rozwlek³e wzglêdem autora, stêd wersja 3-cia polegaj¹ca na 
dodatkowym argumencie definiuj¹cym co zwróciæ w przypadku b³êdu.

	// version 1
	auto result = exec<char>(database, "SELECT value FROM table WHERE key = 'children';");
	char value = result.first ? result.second : 0;

	// version 2
	char result;
	try {
		result = exec<char>(database, "SELECT value FROM table WHERE key = 'children';");
	}
	catch (...) {
		result = 0;
	}

	// version 3
	char result = exec<char>(database, 0, "SELECT value FROM table WHERE key = 'children';");

	// version 4
	char result = exec_def<char>(database, "SELECT value FROM table WHERE key = 'children';", 0);

W przypadku wersji 3 domyœlny argument nie mo¿e byæ ostatnim argumentem (interferowa³ by z parametrami bindowania),
st¹d wersja 4 z suffixem _def która explicite ma podany domyœlny argument.