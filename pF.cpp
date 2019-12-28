#include <bits/stdc++.h>

#define pb push_back
#define X first
#define Y second
#define mkp make_pair

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;

const int maxn = 100+5;
int n;
int a[maxn];
int dp[maxn][maxn];

int gcd(int a, int b)
{
  if(a > b) swap(a, b);
  if(a == 0) return b;
  return gcd(b%a, a);
}

int main()
{
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  cout.tie(0);
  while(cin >> n)
  {
    if(n == 0) break;
    for(int i = 0; i < n; i++) cin >> a[i];
    cout << ;
  }

	return 0;
}
