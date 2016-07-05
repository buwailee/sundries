#include <iostream>
#include <sstream>
#include <fstream>
#include <cctype>
#include <vector>

using namespace std;

bool hashead = false;

string readoneword(fstream &in, char &c){
	string temp("");
	while(in.get(c) && isalnum(c))
		temp.push_back(c);
	return temp;
}

string readwords(fstream &in, char &c){
	string temp("");
	if (c == '{')
		while(in.get(c) && c != '}')
			temp.push_back(c);
	return temp;
}

string filename(const char a[], string b){
	string texname(a), basename("");
	for (auto i : texname){
		basename.push_back(i);
		if (i == '.')
			break;
	}
	return basename+b;
}


stringstream stepone(fstream &in){
	stringstream temp;

	char c;
	int count = 0;
	int chapter = 0;
	int section = 0;
	int subsection = 0;

	while(in.get(c)){
		char d = in.peek();
		if (c == '%')
			while(in.get(c) && c != '\n'){}
		else if (c == '\t')
			in.get(c);
		else if (c == '\\' && isalnum(d)){
			string foo = readoneword(in, c);
			if (foo == "chapter")
				temp << "# " << ++chapter << ". " << readwords(in, c) << ' ';
			else if (foo == "section")
				temp << "## " << chapter << "." << ++section << ". " << readwords(in, c) << ' ';
			else if (foo == "subsection")
				temp << "### " << chapter << "." << section << "." << ++subsection << ". " << readwords(in, c) << ' ';
			// else if (foo == "textit")
			// 	temp << "*" << readwords(in, c) << "* ";
			else if (foo == "pro" || foo == "lem" || foo == "defi" || foo == "theo" || foo == "para")
				temp << '\n' << ++count << ' ';
			else if (foo == "proof")
				temp << "> ";
			else if (foo == "qed")
				temp << '\n';
			else if (foo == "begin"){
				string bar = readwords(in, c);
				if (bar == "pro" || bar == "lem" || bar == "defi" || bar == "theo")
					temp << ++count << ' ';
				else if (bar == "proof")
					temp << "> ";
				else if (bar == "document")
					hashead = true;
				else if (bar == "align*")
					temp << "\\[\\begin{split}";
				else if (bar == "split" || bar == "equation" || bar == "cases")
					temp << "\\begin{" << bar << '}';
			}
			else if (foo == "end"){
				string bar = readwords(in, c);
				if (bar == "split" || bar == "equation" || bar == "cases")
					temp << "\\end{" << bar << '}';
				else if (bar == "align*")
					temp << "\\end{split}\\]";
			}
			else
				temp << '\\' << foo << c;
			in.get(c);
		}
		temp << c;
	}
	return temp;
}

stringstream steptwo(stringstream &in, bool hashead){
	stringstream out;
	char c, d;
	while(in.get(c)){
		d = in.peek();
		if (c == '*' || c == '_')
			out << '\\' << c;
		else if (c == '\\' && (d == '{' || d == '}' || d == '\\'))
			out << '\\' << c;
		else if (c == '\n' && d == '\\'){
			in.get();
			in.get(c);
			if (c == '[' || c == ']'){
				out << "\\\\" << c;
				if (in.peek() == '\n'){
					in.get();
					if (in.peek() == '\n')
						out << '\n';
				}
			}
			else
				out << "\n\\" << c;
		}
		else if (c == '\n' && d == '\n'){
			in.get(c);
			if (in.peek() == '\n')
				out << c;
			else
				out << '\n' << c;
		}
		else
			out << c;
	}
	return out;
}

int main(int argc, char const *argv[]){
	fstream in, out;
	if (argc == 2){
		in.open(argv[1], ios::in);
		out.open(filename(argv[1], "md").c_str(), ios::out);
	}
	else if (argc == 1){
		cout << "no input file" << endl;
		return 0;
	}
	else{
		cout << "more than one file" << endl;
		return 0;
	}

	stringstream temp = stepone(in);
	out << steptwo(temp, hashead).str() << endl;

	in.close();
	out.close();

	return 0;
}