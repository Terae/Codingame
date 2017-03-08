#include <iostream>
int main(){int N,n,i,m=999;std::cin>>N;for(;N>i++;){std::cin>>n;if(abs(n)<abs(m)||(-n==m&&m<0))m=n;}std::cout<<(N>0?m:0)<<"\n";}