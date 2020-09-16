#include <stdlib.h>
#include <string.h>
#include "bdd.h"

int N;
bdd *ainp;
bdd *binp; 
bdd *co;   
bdd *xout; 
void full_adder(int n){


xout[n] = (ainp[n] ^ binp[n] ^ co[n-1]);
co[n] = ainp[n] & binp[n] |
ainp[n] & co[n-1] |
binp[n] & co[n-1];


}

void half_adder(int n){

xout[n] = ainp[n] ^ binp[n];
co[n] = ainp[n] & binp[n];

}

void build_adder(void)
{
int n,a,b,m;

for (m=0;m<2;m++){
if (m>0){
a=2;
b=4;
}else{
a=0;
b=2;
}
for (n=a ; n<b ; n++)
{
if (n > a)
full_adder(n);
else
half_adder(n);
}

}
ainp[4]=co[1];
binp[4]=xout[2];
half_adder(4);

ainp[5]=co[4];
binp[5]=xout[3];
half_adder(5);
co[6]=co[5] | co[3];

}



int main(int argc, char **argv)
{
using namespace std ;
int method=BDD_REORDER_NONE;
int n;

if(argc < 2 || argc > 3)
{
cout << "usage: adder N R\n";
cout << " N number of bits\n";
cout << " R -> enable reordering if R is win2,win2ite,win3,win3ite,sift,siftite\n";
cout << " in this case 'adder' starts with a worst case ordering\n";
exit(1);
}


N = atoi(argv[1]);

if (N <= 0)
{
cout << "The number of bits must be more than zero\n";
exit(2);
}

if (argc == 3)
{
if (strcmp(argv[2], "win2") == 0)
method = BDD_REORDER_WIN2;
else
if (strcmp(argv[2], "win2ite") == 0)
method = BDD_REORDER_WIN2ITE;
else
if (strcmp(argv[2], "win3") == 0)
method = BDD_REORDER_WIN3;
else
if (strcmp(argv[2], "win3ite") == 0)
method = BDD_REORDER_WIN3ITE;
else
if (strcmp(argv[2], "sift") == 0)
method = BDD_REORDER_SIFT;
else
if (strcmp(argv[2], "siftite") == 0)
method = BDD_REORDER_SIFTITE;
else
if (strcmp(argv[2], "rand") == 0)
method = BDD_REORDER_RANDOM;
}

bdd_init(500,1000);
bdd_setvarnum(2*N);

ainp = new bdd[5];
binp = new bdd[5];
co = new bdd[6];
xout = new bdd[5];

for (n=0 ; n<N ; n++)
{
if (method == BDD_REORDER_NONE)
{
ainp[n] = bdd_ithvar(2*n);
binp[n] = bdd_ithvar(2*n+1);
}
else
{
ainp[n] = bdd_ithvar(n);
binp[n] = bdd_ithvar(N+n);
}
}

for (n=0 ; n<N ; n++)
{
bdd_addvarblock(ainp[n],1);
bdd_addvarblock(binp[n],1);
}
build_adder();

if (method != BDD_REORDER_NONE)
{
cout << "Sizes before reordering:\n";
for (n=0 ; n<N ; n++)
cout << "Out[" << n << "]: " << bdd_nodecount(xout[n]) << " nodes\n";

bdd_reorder(method);
cout << "Sizes after reordering:\n";
}
else
cout << "Sizes:\n";

for (n=0 ; n<N ; n++)
cout << "Out[" << n << "]: " << bdd_nodecount(xout[n]) << " nodes\n";
FILE *fil;
fil=fopen("adder.dot","w");


bdd_fprintdot(fil,co[6]);

fclose(fil);

}



