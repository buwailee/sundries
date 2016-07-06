#include <sstream>
#include <fstream>
#include <cctype>

bool hashead = false;

struct ChaSect{
	int chapter = 0;
	int section = 0;
	int subsection = 0;
};

struct LabRef{
	std::string label;
	ChaSect cs;
	int counter;
};

template <typename T> inline std::string readoneword(T &in, char &c);
template <typename T> std::string readwords(T &in, char &c);
std::string filename(const char a[], std::string b);
std::stringstream stepone(std::ifstream &in);
std::stringstream steptwo(std::stringstream &in, bool hashead);