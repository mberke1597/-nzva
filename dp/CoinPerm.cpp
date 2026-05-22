//Brace your keyboard
//inzva community built algoleague for every algorithm enthusiast hungry for self-improvement and friendly competition. Have fun and good luck!

#include <bits/stdc++.h>
using ll = long long;
const ll MOD = 1000000007;
using namespace std;

ll solve(){
    ll n,k,i,s;
    cin>>n>>k;
    vector<ll> arr(n+1);
    vector<ll> dp(k+1,0);
    for(i = 1 ; i <= n ; i++){
        cin>>arr[i];
    };
    dp[0] = 1;
    
    for(s = 1 ; s<=k ; s++){
        for( i = 1 ; i <=n ; i++){
            if(s >= arr[i]){
                dp[s]+= dp[s-arr[i]];
                if(dp[s] >= MOD) dp[s] -= MOD;
            };
        };
    };
    
    return dp[k]%MOD;
    
};


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout<<solve();
	return 0;
}
