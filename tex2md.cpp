#include <iostream>
#include <vector>
#include "tex2md.h"

using namespace std;

template <typename T> inline std::string readoneword(T &in, char &c){
	string temp("");
	while(in.get(c) && isalnum(c))
		temp.push_back(c);
	return temp;
}

template <typename T> std::string readwords(T &in, char &c){
	string temp("");
	int depth = 1;
	if (c == '{')
		while(in.get(c)){
			if (c == '{') depth++;
			else if (c == '}' && --depth == 0) break;
			temp.push_back(c);
		}
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


stringstream stepone(ifstream &in){
	stringstream temp;

	char c;
	int count = 0;
	ChaSect counter;

	// // read header
	// while(in.get(c)){
	// 	char d = in.peek();
	// 	if (c == '\\' && isalnum(d)){
	// 		string foo = readoneword(in, c);
	// 		if (foo == "newcommand"){
				
	// 		}
	// 	}
	// }

	// read texts
	while(in.get(c)){
		char d = in.peek();
		if (c == '%')
			while(in.get(c) && c != '\n'){}
		else if (c == '\t')
			in.get(c);
		else if (c == '\\' && isalnum(d)){
			string foo = readoneword(in, c);
			if (foo == "chapter")
				temp << "# " << ++counter.chapter << ". " << readwords(in, c) << ' ';
			else if (foo == "section"){
				count = 0;
				temp << "## " << counter.chapter << "." << ++counter.section << ". " << readwords(in, c) << ' ';
			}
			else if (foo == "subsection")
				temp << "### " << counter.chapter << "." << counter.section << "." << ++counter.subsection << ". " << readwords(in, c) << ' ';
			// else if (foo == "textit")
			// 	temp << "*" << readwords(in, c) << "* ";
			else if (foo == "para")
				temp << '\n' << counter.section << ++count << ' ';
			else if (foo == "pro" || foo == "lem" || foo == "defi" || foo == "theo")
				temp << '\n' << foo << ' ' << ++count << ' ';
			else if (foo == "proof")
				temp << "*Proof.* ";
			else if (foo == "qed")
				temp << "<p align = right>Q.E.D.</p>";
			else if (foo == "label"){
				temp << "<span id = \"" << readwords(in, c) << "\"></span>";
			}
			else if (foo == "begin"){
				string bar = readwords(in, c);
				if (bar == "pro" || bar == "lem" || bar == "defi" || bar == "theo")
					temp << ++count << ' ';
				else if (bar == "proof")
					temp << "*Proof.* ";
				else if (bar == "document")
					hashead = true;
				else if (bar == "align*")
					temp << "\\[\\begin{split}";
				// else if (bar == "split" || bar == "equation" || bar == "cases")
				// 	temp << "\\begin{" << bar << '}';
				else
					temp << "\\begin{" << bar << '}';
			}
			else if (foo == "end"){
				string bar = readwords(in, c);
				if (bar == "align*")
					temp << "\\end{split}\\]\n";
				else if (bar == "document");
				// else if (bar == "split" || bar == "equation" || bar == "cases")
				// 	temp << "\\end{" << bar << '}';
				else
					temp << "\\end{" << bar << '}';
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
	bool ismathmode = false;
	char c, d;
	while(in.get(c)&& c != '\n');
	while(in.get(c)){
		d = in.peek();
		if (c == '$'){
			ismathmode = !ismathmode;
			if (d == '$'){
				in.get(c);
				out << "$$";
			}
			else
				out << c;
		}
		else if (c == '_' || (c == '*' && ismathmode))
			out << '\\' << c;
		else if (c == '\\' && (d == '{' || d == '}' || d == '\\'))
			out << '\\' << c;
		else if (c == '\\' && (d == '[' || d == ']')){
			ismathmode = !ismathmode;
			out << "\\\\" << c;
			if (in.peek() == '\n'){
				in.get();
				if (in.peek() == '\n')
					out << '\n';
			}
		}
		else if (c == '\n' && d == '\\'){
			in.get();
			in.get(c);
			if (c == '[' || c == ']'){
				ismathmode = !ismathmode;
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
	ifstream in;
	ofstream out;
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