Funkcja exec powinna informowac wyo�uj�cego o b��dach (np. nieistnieniu warto�ci lub b��dach przepe�nienia).
Poni�ej znajduj� si� mo�liwe rozwi�zania tego problemu. Cz�sto mo�e si� zdarzy�, �e w przypadku b��du
do wyniku powinna zosta� przpisana warto� domy�lna. Konieczny zatem jest prosty mechanizm pozwalaj�cy na 
ob�ug� takiego przypadku. Wersje 1 i 2 s� zbyt rozwlek�e wzgl�dem autora, st�d wersja 3-cia polegaj�ca na 
dodatkowym argumencie definiuj�cym co zwr�ci� w przypadku b��du.

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

W przypadku wersji 3 domy�lny argument nie mo�e by� ostatnim argumentem (interferowa� by z parametrami bindowania),
st�d wersja 4 z suffixem _def kt�ra explicite ma podany domy�lny argument.