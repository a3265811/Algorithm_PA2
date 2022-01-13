#include "routing.h"

int main(int argc, char *argv[]){
	router r;
	r.parser(argc,argv);
	r.route();
	r.output(argc,argv);
	return 0;
}
