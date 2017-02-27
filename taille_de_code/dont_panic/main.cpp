#include <iostream>
#include <map>
using namespace std;int main(){int a,b,c,d,e,f;cin>>a>>a>>a>>e>>b>>a>>a>>f;map<int,int>L;for(a=0;f>a++;){cin>>d>>c;L[d]=c;}while(1){string D;cin>>f>>c>>D;a=(f==e?b:L[f]);cout<<(((a>c&&D=="LEFT")||(a<c&&D[0]=='R'))?"BLOCK\n":"WAIT\n");}}
