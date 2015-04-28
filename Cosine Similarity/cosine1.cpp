/* 
  Author: Shubham Prasad
  Cosine similarity code.
*/

#include<iostream>
#include<cstdio>
#include<vector>
#include<string>
#include<algorithm>
#include<cmath>
#include<set>
#include<map>
#include<queue>
#include<cstring>
#include<stack>
#include<string>
#include<sys/types.h>
#include<dirent.h>
#include<fstream>
#include<iterator>
using namespace std;

string StringToLower(string);
void listdir(const char *,const char *,int );

int main(int argc, char ** argv) {
	
  char s[100],s1[100];
  cout<<"Enter the path of the folder where documents are kept"<<endl;
  gets(s);
  cout<<"Enter the path of the query document file"<<endl;
  gets(s1);
  DIR *dp;
  dirent *d;
  dp = opendir(s);
  int count=0;
  while((d = readdir(dp)) != NULL){
  	count++;
  }
 
  listdir(s,s1,count);
  cout << endl;
  int x;
  cin>>x;
  return 0;
}

void listdir(const char *dirname,const char *filename,int n) {

  DIR *dp;
  dirent *d;

  int i=0,j,k,l;
  dp = opendir(dirname);
  vector<string> names(n);
  vector<pair<string,float> > pair1[n],pair3[n]; 
  int z=0;
  while((d = readdir(dp)) != NULL)
  {
  
  	if(d->d_name[0]=='.')
  	continue;
  	
  	ifstream file1;
  	string  str1="";//dirname+d->d_name;
  	
  	for(l=0;dirname[l]!='\0';l++){
	  	str1+=dirname[l];
    }
    
  	str1+=(char)92;
  	l++;
  	
    for(j=0;d->d_name[j]!='\0';j++){
  		str1+=d->d_name[j];
  		names[z].push_back(d->d_name[j]);
     }
     
  	z++;
  	char str2[100];
  	
  	for(j=0;j<100;j++){
	  	str2[j]='\0';
    }
	  for(j=0;j<str1.size();j++){
  		str2[j]=str1[j];
  	}
  	
  	file1.open(str2);
	  	map<string, int> m;
	  	string  s1;
	  	int count=0;
	  	int a=0;
	  	while(file1>>s1){
	  		s1=StringToLower(s1);
	  		a++;
	  		count++;
	  		m[s1]++;	
	  	}
	  	
    for( map<string,int> :: iterator it=m.begin();it!=m.end();it++){
  		pair1[i].push_back(make_pair(it->first,(float)(it->second)/(float)count));
  	}
  	i++;
  }
  
  for(i=0;i<n;i++)
  pair3[i]=pair1[i];
  
  for(i=0;i<n;i++){                               
  	 for(int l=0;l<pair1[i].size();l++)
  		{
		int count1=1;
	  	for(int j=0;j<n;j++){
		  	if(i!=j){
				  for(k=0;k<pair1[j].size();k++)
	  				if(pair1[i][l].first==pair1[j][k].first)
	  					{
						  count1++;
						  break;
	  					}	 		
	  		}
  		}
  		pair3[i][l].second=(1+log10((float(n))/(float)count1));   //idf
  		pair1[i][l].second*=(1+log10((float)n/(float)(count1))); //tf-idf
  		}
  }
  
  	ifstream file2;
  	file2.open(filename);
  	
  	map<string,int> m2;
  	string s2;
  	int count3=0,b=0; 
  	
  	while(file2>>s2)
  	{
      s2=StringToLower(s2);
	  b++;
	  m2[s2]++;
	  count3++;
  	}

  	vector<pair<string,float> > pair2,pair4;
  	map<string,int> :: iterator it2;
  	
  	for(it2=m2.begin();it2!=m2.end();it2++){
	  	pair2.push_back(make_pair(it2->first,(float)(it2->second)/(float)count3));  //tf of query
    }
    
    pair4=pair2;
   
    for(i=0;i<pair2.size();i++){
    	for(j=0;j<n;j++)
    		{
			for(k=0;k<pair1[j].size();k++){
				if(pair2[i].first==pair1[j][k].first){
					pair4[i].second=pair3[j][k].second;         //idf of query
				}	
			}
    		}
    }
    
    for(i=0;i<pair2.size();i++){
    	pair4[i].second*=(pair2[i].second);  //tf-idf of query
    }
   
    float val1=0; // vector mod of query
    float val2=0;//dot product
    float val3=0; //vector mod of documents
    
    for(i=0;i<pair4.size();i++){
    	val1+=pair4[i].second*pair4[i].second;
    }
    val1=sqrt(val1);
    
    vector<pair<float,string> > ans(n);
    
    for(i=0;i<n;i++){
    	val2=0.0;
    	val3=0.0;
    	for(k=0;k<pair2.size();k++){
   		     for(j=0;j<pair1[i].size();j++){
	    		if(pair1[i][j].first==pair2[k].first){
		    		val2+=(pair4[k].second*pair1[i][j].second); 
		    		val3+=(pair1[i][j].second*pair1[i][j].second);//vector mod of documents
		    		break;
		    	}
	    	}
	    }
	    val3=sqrt(val3);
	    ans[i].second=names[i];
	    if(val3==0)
	    ans[i].first=0;
	    else
	    ans[i].first=val2/(val3*val1);
    }
    
    sort(ans.rbegin(),ans.rend());
    
    cout<<"The top three documents with most similarity with document ";
    puts(filename);
    cout<<"in descending order are"<<endl;
    
    cout<<"1: "<<ans[0].second<<endl;
    cout<<"2: "<<ans[1].second<<endl;
    cout<<"3: "<<ans[2].second<<endl;
}
string StringToLower(string strToConvert)
	{
   for(unsigned int i=0;i<strToConvert.length();i++)
   		{
     	 strToConvert[i] = tolower(strToConvert[i]);
   		}
   return strToConvert;//return the converted string
}