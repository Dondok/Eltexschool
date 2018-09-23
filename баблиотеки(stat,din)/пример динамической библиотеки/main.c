#include <stdio.h>
#include <dlfcn.h>

int main(){

	void *ext_library;
	double value=0;
	char * (*ret_test)();

	ext_library = dlopen("libtest.so",RTLD_LAZY);
	if (!ext_library){
		fprintf(stderr,"dlopen() error: %s\n", dlerror());
		return 1;
	};

	ret_test = dlsym(ext_library,"ret_test");	

	printf("Return of ret_test: \"%s\" [%p]\n",(*ret_test)(),(*ret_test)());

	dlclose(ext_library);
};
