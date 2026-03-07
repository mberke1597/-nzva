//Brace your keyboard
//inzva community built algoleague for every algorithm enthusiast hungry for self-improvement and friendly competition. Have fun and good luck!

#include <bits/stdc++.h>
using ll = long long;
const ll MOD = 1000000007;
using namespace std;

ll solve(){
    ll n,k,i;
    cin>>n>>k;
    vector<ll> arr(n+1);
    vector<vector<ll>> dp(n+1,vector<ll>(k+1,0));
    for(i = 1 ; i <= n ; i++){
        cin>>arr[i];
    };
    for(i = 0 ; i <= n; i++) dp[i][0] = 1;
    
    for(i = 1 ; i<=n ; i++){
        for(ll s = 1 ; s <=k ; s++){
            dp[i][s] = dp[i-1][s];
            if(s >= arr[i]){
                dp[i][s] = (dp[i][s] + dp[i][s -arr[i]])%MOD;
            };
        };
    };
    
    return dp[n][k]%MOD;
    
};


int main() {
    cout<<solve();
	return 0;
}
