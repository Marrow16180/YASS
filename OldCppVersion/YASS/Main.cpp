#include "Menu.hpp"

int main()
{
	try 
	{
		Menu menu{};
		menu.run();
	} 
	catch (const std::runtime_error& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
}