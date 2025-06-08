//#include <iostream>
//#include <string>
//using namespace std;
//
//// Returns true if s1[0..m-1] is a subsequence of s2[0..n-1]
//bool isSubSeqRec(const string& s1, const string& s2, int m, int n)
//{
//    // Base Cases
//    if (m == 0)
//        return true;
//    if (n == 0)
//        return false;
//
//    // If last characters of two strings are matching
//    if (s1[m - 1] == s2[n - 1])
//        return isSubSeqRec(s1, s2, m - 1, n - 1);
//
//    // If last characters are not matching
//    return isSubSeqRec(s1, s2, m, n - 1);
//}
//
//// Wrapper function
//bool isSubSeq(const string& s1, const string& s2)
//{
//    int m = s1.length();
//    int n = s2.length();
//    if (m > n) return false;
//    int j=0;
//    //return isSubSeqRec(s1, s2, m, n);
//    for(int i=0;i<n;i++)
//    {
//    	if(s2[i]==s1[j])
//    		j++;
//    }
//    if(j==m)
//    	return true;
//    return false;
//}
//
//int main()
//{
//    string s1 = "gskrek";
//    string s2 = "geeksforgeeks";
//    isSubSeq(s1, s2) ? cout << "Yes " : cout << "No";
//    return 0;
//}

// C++ Code to check if two Strings are anagrams of
// each other using sorting

#include <algorithm>
#include <iostream>
using namespace std;

// Function to check if two strings are anagrams
bool areAnagrams(string &s1, string &s2) {
    if(s1.length() != s2.length())
    	 return false;
	int Arr[26] = {};
	for(int i =0;i<s1.length();i++)
	{
		Arr[int(s1[i]-'a')]++;
		Arr[int(s2[i]-'a')]--;
	}
	for(int i=0;i<26;i++)
	{
		if(Arr[i]!=0)
			return false;
	}
    return true;
}

int main() {
    string s1 = "geeks";
    string s2 = "kseeeg";
	cout << (areAnagrams(s1, s2) ? "true" : "false") << endl;

    return 0;
}




