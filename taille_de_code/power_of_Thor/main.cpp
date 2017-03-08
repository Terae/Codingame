/// Cas global
//#include <iostream>
//int main(){int X,Y,A,B;for(std::cin>>X>>Y>>A>>B;;){std::cout<<(B>Y?B--,"N":B<Y?B++,"S":"")<<(A>X?A--,"W":A<X?A++,"E":"")<<"\n";}}

/// Cas particulier : on connaît déjà la séquence en fonction des 4 possibilités
#include<stdio.h>
int main(){int i,X=getchar()-52;if(X/3)for(;;i++)puts(i<23+X?"W":"SW");for(;;i++)puts(i<14?"S":"E");}