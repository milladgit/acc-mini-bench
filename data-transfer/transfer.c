#include <stdio.h>
#include <omp.h>

#define N 		10000
#define COUNT 	10000
#define OUTLIER 100


double *a, *b, *c, *d;




/*
 * copyin functions
 */
void copyin_async_one() {
	#pragma acc enter data copyin(a[0:N], b[0:N], c[0:N], d[0:N]) async
	#pragma acc wait
}

void copyin_async_more() {
	#pragma acc enter data copyin(a[0:N]) async(1)
	#pragma acc enter data copyin(b[0:N]) async(2)
	#pragma acc enter data copyin(c[0:N]) async(3)
	#pragma acc enter data copyin(d[0:N]) async(4)
	#pragma acc wait
}

void copyin_async_more_nonumber() {
	#pragma acc enter data copyin(a[0:N]) async
	#pragma acc enter data copyin(b[0:N]) async
	#pragma acc enter data copyin(c[0:N]) async
	#pragma acc enter data copyin(d[0:N]) async
	#pragma acc wait
}







/*
 * update self functions
 */
void update_self_async_one() {
	#pragma acc update self(a[0:N], b[0:N], c[0:N], d[0:N]) async
	#pragma acc wait
}

void update_self_async_more() {
	#pragma acc update self(a[0:N]) async(1)
	#pragma acc update self(b[0:N]) async(2)
	#pragma acc update self(c[0:N]) async(3)
	#pragma acc update self(d[0:N]) async(4)
	#pragma acc wait
}

void update_self_async_more_nonumber() {
	#pragma acc update self(a[0:N]) async
	#pragma acc update self(b[0:N]) async
	#pragma acc update self(c[0:N]) async
	#pragma acc update self(d[0:N]) async
	#pragma acc wait
}







/*
 * update device functions
 */
void update_device_async_one() {
	#pragma acc update device(a[0:N], b[0:N], c[0:N], d[0:N]) async
	#pragma acc wait
}

void update_device_async_more() {
	#pragma acc update device(a[0:N]) async(1)
	#pragma acc update device(b[0:N]) async(2)
	#pragma acc update device(c[0:N]) async(3)
	#pragma acc update device(d[0:N]) async(4)
	#pragma acc wait
}

void update_device_async_more_nonumber() {
	#pragma acc update device(a[0:N]) async
	#pragma acc update device(b[0:N]) async
	#pragma acc update device(c[0:N]) async
	#pragma acc update device(d[0:N]) async
	#pragma acc wait
}







/*
 * copyout functions
 */
void copyout_async_one() {
	#pragma acc exit data copyout(a[0:N], b[0:N], c[0:N], d[0:N]) async
	#pragma acc wait
}

void copyout_async_more() {
	#pragma acc exit data copyout(a[0:N]) async(1)
	#pragma acc exit data copyout(b[0:N]) async(2)
	#pragma acc exit data copyout(c[0:N]) async(3)
	#pragma acc exit data copyout(d[0:N]) async(4)
	#pragma acc wait
}

void copyout_async_more_nonumber() {
	#pragma acc exit data copyout(a[0:N]) async
	#pragma acc exit data copyout(b[0:N]) async
	#pragma acc exit data copyout(c[0:N]) async
	#pragma acc exit data copyout(d[0:N]) async
	#pragma acc wait
}







void measure(char *function_name, void (*function1)(void), void (*function2)(void), void (*function3)(void), void (*function4)(void)) {
	int i;

	for(i=0;i<N;i++) {
		a[i] = i;
		b[i] = i+1;
		c[i] = i+2;
		d[i] = i+3;
	}

	double copyin = 0;
	double copyout = 0;
	double update_self = 0;
	double update_device = 0;
	double t;

	for(i=0;i<COUNT;i++) {
		t = omp_get_wtime();
		function1();
		t = omp_get_wtime() - t;
		if(i>OUTLIER && i<COUNT-OUTLIER)
			copyin += t;

		t = omp_get_wtime();
		function2();
		t = omp_get_wtime() - t;
		if(i>OUTLIER && i<COUNT-OUTLIER)
			update_device += t;

		t = omp_get_wtime();
		function3();
		t = omp_get_wtime() - t;
		if(i>OUTLIER && i<COUNT-OUTLIER)
			update_self += t;

		t = omp_get_wtime();
		function4();
		t = omp_get_wtime() - t;
		if(i>OUTLIER && i<COUNT-OUTLIER)
			copyout += t;
	}

	// total exectution time:
	// printf("%s\nCopyIn: %.3fus\nUpdate device: %.3fus\nUpdate self: %.3fus\nCopyOut: %.3fus\n---------\n", function_name, 1E6*copyin, 1E6*update_device, 1E6*update_self, 1E6*copyout);

	// average execution time
	int c = COUNT - 2*OUTLIER;
	printf("%s\nCopyIn: %.3fus\nUpdate device: %.3fus\nUpdate self: %.3fus\nCopyOut: %.3fus\n---------\n", function_name, 1E6*copyin/c, 1E6*update_device/c, 1E6*update_self/c, 1E6*copyout/c);
}


int main() {
	a = (double*) malloc(N * sizeof(double));
	b = (double*) malloc(N * sizeof(double));
	c = (double*) malloc(N * sizeof(double));
	d = (double*) malloc(N * sizeof(double));


	measure("one", copyin_async_one, update_device_async_one, update_self_async_one, copyout_async_one);
	measure("more", copyin_async_more, update_device_async_more, update_self_async_more, copyout_async_more);
	measure("more_nonumber", copyin_async_more_nonumber, update_device_async_more_nonumber, update_self_async_more_nonumber, copyout_async_more_nonumber);

	return 0;
}
