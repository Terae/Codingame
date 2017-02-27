#include <iostream>
#define O(x)std::cout<<(x);
int main(){int X,Y,A,B;std::cin>>X>>Y>>A>>B;while(1){O(B>Y?(B--,"N"):B<Y?(B++,"S"):"")O(A>X?(A--,"W"):A<X?(A++,"E"):"")O('\n')}}