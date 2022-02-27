#include <string>
#include <iostream>
#include <fstream>

#include <code_generator.h>
#include <arrayChecker.h>

using namespace std;

namespace LA {
	void generate_code(Program p) {
		std::ofstream outputFile;
		//outputFile.open("prog.IR");

		Program newP;
		newP.entryPoint = p.entryPoint;

		//generate error checking
		
		for (auto f : p.functions) {
			auto validator = new arrayChecker(f);
			newP.functions.push_back(validator->validate());
		}

		//generate basic blocks

		//encode values

		std::cout << newP.toString();

		//outputFile.close();
		return;
	}
}