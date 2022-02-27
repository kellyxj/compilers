#include <string>
#include <iostream>
#include <fstream>

#include <code_generator.h>

using namespace std;

namespace LA {
	void generate_code(Program p) {
		std::ofstream outputFile;
		outputFile.open("prog.IR");

		//generate error checking
		Program newP;

		//generate basic blocks

		//encode values

		outputFile << p.toString();

		outputFile.close();
		return;
	}
}