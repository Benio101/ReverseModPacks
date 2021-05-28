#include <iostream>
#include <filesystem>
#include <fstream>
#include <windows.h>

int main()
{
	// Location of Among Us from Steam's registry. Fallbacks to current (working) dir.
	std::filesystem::path path;

	char steam_path[255]{};
	unsigned long steam_path_size = sizeof(steam_path);
	const auto steam_path_known = !static_cast<bool>(RegGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Valve\\Steam", "InstallPath", RRF_RT_REG_SZ, nullptr, &steam_path, &steam_path_size));

	if (steam_path_known) [[likely]] {
		path = steam_path;
		path /= "steamapps/common/Among Us";
		if (!std::filesystem::is_directory(path)) [[unlikely]]
			goto fallback;
	} else
	fallback:
		path = std::filesystem::current_path();

	auto success = true;
	auto usefulness = false;

	for (const auto& folder : std::vector<std::string>{"BepInEx", "mono", "TownOfImpostors_CustomHats", "TownOfImpostors_Data", "TheOtherHats"}) [[likely]]
		if (std::filesystem::is_directory(path / folder))
			if (std::filesystem::remove_all(path / folder)) [[likely]] {
				usefulness = true;
				std::cout << "Folder \"" + folder + "\" został pomyślnie usunięty." << std::endl;
			} else {
				success = false;
				std::cerr << "Nie udało się usunąć folderu \"" + folder + "\"." << std::endl;
			}
		else
			std::cout << "Folder \"" + folder + "\" nie istnieje." << std::endl;

	for (const auto& folder : std::vector<std::string>{"doorstop_config.ini", "winhttp.dll", "patch.bat", "config-tou.bat", "usun-mody.bat"}) [[likely]]
		if (std::filesystem::is_regular_file(path / folder))
			if (std::filesystem::remove(path / folder)) [[likely]] {
				usefulness = true;
				std::cout << "Plik \"" + folder + "\" został pomyślnie usunięty." << std::endl;
			} else {
				success = false;
				std::cerr << "Nie udało się usunąć pliku \"" + folder + "\"." << std::endl;
			}
		else
			std::cout << "Plik \"" + folder + "\" nie istnieje." << std::endl;

	if (usefulness) [[likely]]
		if (success) [[likely]]
			std::cout << "Wszystkie modyfikacje z folderu Among Us zostały usunięte." << std::endl;
		else
			std::cerr << "Nie udało się usunąć wszystkich modyfikacji z folderu Among Us." << std::endl;
	else
		std::cout << "Nie posiadasz żadnych modyfikacji w folderze Among Us." << std::endl;

	std::cout << "Naciśnij Enter, aby wyjść z programu." << std::endl;
	std::cin.get();
}