#include <string>
#include <fstream>
#include <vector>
using namespace std;
#define d 256

string message;
vector<vector<int>> step_values(1001,vector<int>(1,0));
long long int dp[1001] = {};
string dict[1000];
int q = 101, N, dict_size;

void search(string word)
{
    int M = word.length();
    int i, j;
    int p = 0;
    int t = 0;
    int h = 1;

    for (i = 0; i < M-1; i++)
        h = (h*d)%q;

    for (i = 0; i < M; i++) {
        p = (d*p + word[i])%q;
        t = (d*t + message[i])%q;
    }
    for (i = 0; i <= N - M; i++) {
        if ( p == t ) {
            for (j = 0; j < M; j++) {
                if (message[i+j] != word[j])
                    break;
            }
            if (j == M)
                step_values[i].push_back(M);
            }
        if ( i < N-M ) {
            t = (d*(t - message[i]*h) + message[i+M])%q;
            if (t < 0)
                t = (t + q);
        }
    }
}

int main(int argc, char* argv[]) {
    ifstream in(argv[1]);
    ofstream out(argv[2]);
    in>>message;
    N = message.length();
    in>>dict_size;
    for(int i = 0; i < dict_size; i++){
        in>>dict[i];
        search(dict[i]);
    }
    dp[0] = 1;
    for(int i = 1; i <= N; i++){
        for(int j = 0 ; j < step_values[i-1].size(); j++){
            if(step_values[i-1][j]!=0){
                dp[i+step_values[i-1][j]-1] = (dp[i-1]+dp[i+step_values[i-1][j]-1])%1000000007;
            }
        }
    }
    out<<dp[N];
    in.close();
    out.close();
    return 0;
}
