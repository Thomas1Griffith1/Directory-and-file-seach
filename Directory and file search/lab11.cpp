#include <stdio.h>
#include <dirent.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <math.h>
#include <string>
#include <string.h>
#include <fstream>

using namespace std;

//make a string all lower case
string toLowerCase(string input)
{
      string output = input;
      for(int ch = 0; ch < input.size(); ch++)
      {
            if(output[ch] <= 'Z' && output[ch]  >= 'A')
            {
                  output[ch] = ( output[ch] - ('Z' - 'z') );
            }

      }
  return output;

}




/*
 *This Function Finds the exact match for the query
 *looking at the file names only and if it is a file name
 *don't look anymore
*/
bool exact_Wording(vector<string> a, string query)
{
      bool foundAsFile = false;

      //loop to go through all the file names in the vector
      for(auto p: a)
      {
            //check to see if the file name is the same as the query
            if(p == query)
            {
                  //print the file name/query and set found as true
                  cout<< query<< endl;
                  cout << endl;
                  foundAsFile = true;
            }
      }


//return if you found a match
return foundAsFile;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////Functions////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////         matching above ||||  misspelled word below       ////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * this function takes a one word query and
 * makes a vector containing a pair of every combination
 * of two or more leters of the word and their number cubed
*/
//make a vector of parsed letters and score for any query word
vector<pair<string,int>> misspellQueryCount(string input)
{
      vector<pair<string,int>> possibleCount;
      int length = input.length();
      //loop that goes through all but the last letter of the string input (query word)
      for(int start = 0; start <= length-2; start++)
      {
            //loop to get all the combinations of letters from the first letter from the first loop
            for(int goTill = 2; goTill <= length-start; goTill++)
            {

                  string word = input;
                  //get at least 2 letter pairs from query (tess -> te, tes, tess, es, ess, ss) adds 1 because vector starts at 0 not 1
                  possibleCount.push_back(make_pair(word.substr (start, goTill) , (pow(goTill, 3)+1)));
            }

      }

      return possibleCount;
}



/*
 * this function takes a one word string that you will compare
 * with the query but turn it into vector form with every combination
 * of two or more letters in the word
*/

//make a vector of parsed letters for a word
vector<string> misspellWordCount(string input)
{

      vector<string> possibleCount;
      int length = input.length();
      //loop that goes through all but the last letter of the string input (word)
      for(int start = 0; start <= length-2; start++)
      {
            //loop to get all the combinations of letters from the first letter from the first loop
            for(int goTill = 2; goTill <= length-start; goTill++)
            {
                  string word = toLowerCase(input);
                  //get at least 2 letter pairs from word (tess -> te, tes, tess, es, ess, ss)
                  possibleCount.push_back(word.substr (start, goTill));
            }

      }

      return possibleCount;
}







/*
 * This function gets the closest or exact match to a one word query
 * going through files then their content BFS. for each word compare the
 * combinations with the query's and save the best to be printed at end
*/
void findMisspelledWord(vector<string> a, vector<pair<string,int>> query)
{
      //create variables to use in function
      pair<string,int> bestScore;
      pair<string,int> original = make_pair("n",-20);
      bestScore = original;



      //go through the file names
      for(auto p: a)
      {
            //make a vector of letters 2 or more of file name
            vector<string> wordVector = misspellWordCount(p);
            int scoreDone = 0;
            //go through the file vector of letters
            for(auto vectW: wordVector)
            {
                        //go through the query vector of letters
                        for(auto vectQ: query)
                        {
                                    //if they have the same letters in a row in vector
                                    if(vectW == vectQ.first)
                                    {
                                                //get the score from the combination and subtract the other letters
                                                int score = (vectQ.second - (p.length() - cbrt(vectQ.second)));
                                                //check if score is better than the the current best score and the set new if it is greater
                                                if(scoreDone <= score)
                                                {
                                                      scoreDone = score+1;
                                                      pair<string,int> newBest = make_pair(p + "\n",scoreDone);
                                                      bestScore = newBest;

                                                }
                                    }

                        }


            }

      }
///////////////////////////////////////////////////////////////////////////////
      //go through each file content
      for(auto p: a)
      {
            //variables set to go through all content in each file
            string contains;
            ifstream inFile;
            string content = "content/";
            inFile.open((content + p));
            //saves all the words in the file
            if (!inFile)
            {
                  cout << "Unable to open file" << endl;
                  exit(1); // terminate with error
            }
            string get;
            while (getline(inFile, get))
            {
                  contains += get + " ";
            }

            inFile.close();

            //used to hold each individual word
            vector<string> wordsInFile;
            contains = toLowerCase(contains);
            stringstream getWords(contains);
            string words;
            //put words from file into vector
            while(getWords >> words)
            {
                  wordsInFile.push_back(words);
            }


            //go through the file content word by word with the vector wordsInFile
            for(auto word: wordsInFile)
            {
                  //make a vector of letters 2 or more of file name
                  vector<string> wordVector = misspellWordCount(word);
                  int scoreDone = 0;
                  //go through the file vector of letters
                  for(auto vectW: wordVector)
                  {
                              //go through the query vector of letters
                              for(auto vectQ: query)
                              {
                                          //if they have the same letters in a row in vector
                                          if(vectW == vectQ.first)
                                          {
                                                      int score = (vectQ.second - (word.length() - cbrt(vectQ.second))-6);
                                                      if(scoreDone < score)
                                                      {
                                                            scoreDone = score;

                                                      }
                                                      if(score >= bestScore.second)
                                                      {
                                                            string whole = p + "\n" + word + "\n";
                                                            pair<string,int> newBest = make_pair(whole,scoreDone);
                                                            bestScore = newBest;
                                                      }

                                          }

                              }

                  }

            }


      }

// a close spelled file name is more important that finding exact match in a random file
if(bestScore.second > 0)
      cout<< bestScore.first  << endl;

}









//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////Functions////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////     misspelled word above ||||  misspelled phrase below       /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
 *This function is used to take a word and
 *get every combination of tow or more letters
 *and push each one on a vector to be
 * return for a query with multiple words
*/
vector<pair<string,int>> QueryParseFromPhrase(string input)
{
      vector<pair<string,int>> possibleCount;
      int length = input.length();
      //loop that goes through all but the last letter of the string input
      for(int start = 0; start <= length-2; start++)
      {

            //loop to get all the combinations of letters from the first letter from the first loop
            for(int goTill = 2; goTill <= length-start; goTill++)
            {

                  string word = input;
                  //get at least 2 letter pairs from query (tess -> te, tes, tess, es, ess, ss)
                  possibleCount.push_back(make_pair(word.substr (start, goTill) , (pow(goTill, 3))));
            }

      }

      return possibleCount;
}






/*
 *this function takes the phrase query and takes each word in a vector
 *then it takes each word from that vector and parse it of every combination of two or
 *more letters and a score on the number of letters cubed
 *into a vector for each using misspellPhraseQueryCount
 * so that it is a vector[word vector(combinations) ]
*/
vector< vector< pair<string,int>> > misspellQueryPhraseCount(string input)
{
      //saves each word in the phrase in a seperate vector within a whole vector
      vector< vector< pair<string,int>> > possibleCount;

      //get each word from the input
      vector<string> wordsInFile;
      stringstream getWords(input);
      string words;
      //put words from query into vector
      while(getWords >> words)
      {
            wordsInFile.push_back(words);
      }

      //for each word in the phrase, get a vector of pair with its score and combo
      for(auto word:wordsInFile)
      {
            vector< pair<string,int>> wordScoreFile = QueryParseFromPhrase(word);
            //push it into the the bigger vector
            possibleCount.push_back(wordScoreFile);
      }

      return possibleCount;
}



//a structure to help keep score from a phrase find
struct scoreKeeper {
      int QPos;
      int selfScore;
} ;


/*
 * This is the function that finds the best phrase match
 * to the query from the list of files in the directory
 * it compares each combo with each query word combo
 * This means that it finds the phrase that has the most in common
 * with the query phrase going through every words in order from the
 * any size as the query
*/
pair<string,int> findBestFilePhrase(vector<string> FileName, vector< vector< pair<string,int>> > QVector)
{

      //variable holds the final and best file relating to query
      pair<string,int> foundBest = make_pair("FileName", -1);

      //vector to use to hold the combination of the best score for each word
      vector<int> FinalScore;
      string fileNameIs = "";
      for(int i = 0; i < QVector.size(); i++)
      {
            FinalScore.push_back(0);
      }

      //holds the best value set to default here for the words in the file's name
      vector<scoreKeeper> totalScore;
      for(int i = 0; i < FileName.size(); i++)
      {

            scoreKeeper Score;
            Score.QPos = 0;
            Score.selfScore = 0;
            totalScore.push_back(Score);
            //makes the file name you are on with each word from the vector
            fileNameIs += FileName[i] + " ";
      }


      //for loop to go through each word in the file name
      for(int FileWordNum = 0; FileWordNum < FileName.size(); FileWordNum++)
      {
            //for each word in the query phrase go through them
            for(int QVpos = 0; QVpos < QVector.size(); QVpos++)
            {

                  //for loop to go through each combination from the query and compare it with the vector combination from the file word
                  for(int QWordCombo = 0; QWordCombo < QVector[QVpos].size(); QWordCombo++)
                  {

                        //get variables for comparing
                        string FileNameWord = FileName[FileWordNum];
                        string QCombo = QVector[QVpos][QWordCombo].first;
                        size_t find = FileNameWord.find(QCombo);
                        //if there is a match
                        if(find!=string::npos)
                        {
                              //if there is a match and it is bigger than what is already saved replace it
                              if(totalScore[FileWordNum].selfScore < QVector[QVpos][QWordCombo].second)
                              {
                                    totalScore[FileWordNum].QPos = QVpos;
                                    totalScore[FileWordNum].selfScore = QVector[QVpos][QWordCombo].second;
                              }

                        }


                  }


            }
      }


      ///get correct math value for final count
      for(auto FWord: totalScore)
      {
            if(FinalScore[FWord.QPos] < FWord.selfScore )
            {
                  FinalScore[FWord.QPos] = FWord.selfScore;
            }
      }

      int Final = 0;
      //add score from the best score on each query word
      for(auto GetScore: FinalScore)
      {
            Final += GetScore;
      }
      foundBest.second = Final;
      foundBest.first = fileNameIs;


      return foundBest;

}



/*
 *this function takes a string of words
 * and makes a vector to hold each word individualy
*/
// get a file name that is a phrase then make a vector
//containing all its words
vector<string> filePhraseToVector(string fileName)
{
      vector<string> fileWords;
      stringstream getWords(fileName);
      string word;

      while(getWords >> word)
      {
            fileWords.push_back(word);
      }

      return fileWords;

}




/*
 * This is the function that finds the best phrase match
 * to the query from the content of a file in the directory
 * it compares each combo with each query word combo
 * This means that it finds the phrase that has the most in common
 * with the query phrase going through every words in order from with the
 * same size as the query one by one[( 0 - 3)-> (1- 4) words in file]
*/
pair<string,int> findBestContentPhrase(string fileName, vector< vector< pair<string,int>> > QVector)
{


      pair<string,int> foundBest = make_pair(fileName+"\n", -1);
      //go through  file content
            string contains;
            ifstream inFile;
            string content = "content/";
            inFile.open((content + fileName));
            if (!inFile)
            {
                  cout << "Unable to open file" << endl;
                  exit(1); // terminate with error
            }
            string get;
            while (getline(inFile, get))
            {
                  contains += get + " ";
            }

            inFile.close();
            contains = toLowerCase(contains);
            //vector of words in the file
            vector<string> wordsInFile;

            int wordSize = QVector.size();
            vector<string> wordsInRightSize;

            stringstream getWords(contains);
            string words;
            //put words from file into vector
            while(getWords >> words)
            {
                  wordsInFile.push_back(words);


            }



            //loop to get the words in file in the correct size of vector of words
            for(int place = wordSize-1; place < wordsInFile.size(); place++)
            {


                  //get the same amount of words in a vector as the query but they will be in reverse order
                  vector<string> wordsInRightSize;
                  for(int count = place-(wordSize-1); count <= place; count++)
                  {

                        wordsInRightSize.push_back(wordsInFile[count]);
                  }


                  pair<string,int> outcome = findBestFilePhrase(wordsInRightSize, QVector);

                  //if new phrase of words is better set it to best
                  if(outcome.second > foundBest.second)
                  {
                        foundBest.first = outcome.first;
                        foundBest.second = outcome.second;
                  }

            }

            foundBest.first = fileName + "\n"+ foundBest.first;
      return foundBest;

}


/*
 * This Function is the master phrase finder for the query
 * It calll all the other functions used to compare the files
 * and the file contents to the query phrase
*/
void findPhrase(vector<string> listOfFiles, vector< vector< pair<string,int>> > QVector)
{

      //find the closest resemblance to query
      pair <string,int> BestOfFiles = make_pair("dummy", -1);
      //go through each file name
      for(auto fileName : listOfFiles)
      {

            vector <string> fileWordVect;
            //get words in the file
            for(auto words : filePhraseToVector(fileName))
            {

                  fileWordVect.push_back(words);
            }
            //get score from the compare of the file name vector and query
            pair <string,int> oneFile = findBestFilePhrase(fileWordVect,QVector);
            if(oneFile.second > BestOfFiles.second)
            {
                  BestOfFiles = oneFile;

            }

            //get the score from the content from the file you are on from the loop
            pair <string,int> oneFileContentBest = findBestContentPhrase(fileName, QVector);
            if(oneFileContentBest.second > BestOfFiles.second)
            {
                  BestOfFiles = oneFileContentBest;



            }

      }

if(BestOfFiles.second > 0)
cout <<  BestOfFiles.first << endl;

}







//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////Functions////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////      misspelled phrase above ||||  main below       ////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main (void)
{
      bool getQueryMore = true;
      while(getQueryMore)
      {
            //get query from user
            string line;
            cout<< "enter Query" << endl;
            getline(cin, line);
            line = toLowerCase(line);
            cout << endl;
            cout << "searching for " << line << endl;
            cout << endl;
            vector<string> listOfFiles;
            DIR *dp;
            struct dirent*ep;

            dp = opendir("content");

            if(dp != NULL)
            {
                  while((ep = readdir(dp)) != NULL)
                  {

                        if(ep->d_name[0] != '.')
                        {
                              string FILE = ep->d_name;
                              listOfFiles.push_back (toLowerCase(FILE));
                        }
                  }
                  closedir(dp);
            }else{
                  perror("couldn't open directory!");
            }


            //exact match for files but does not  check contexts
            bool exact = exact_Wording(listOfFiles, line);

            //if there is not an exact match in the file names do the other tests
            if(!exact)
            {
                  priority_queue<pair<int, string>> orderOfBestFind;
                  //this sees if the query is a phrase of a word
                  int words = 0;
                  for(int at = 0; at < line.length(); at ++)
                  {
                        if(isspace(line[at]) == 1)
                        {
                              words++;
                        }
                  }
                  //if the query is a one word then do word search
                  if(words == 0)
                  {
                        //for one word search

                        vector<pair<string,int>> query = misspellQueryCount(line);
                        findMisspelledWord( listOfFiles, query);

                  }
                  //do phrase search if multiple letters are in the query
                  else{

                        findPhrase(listOfFiles, misspellQueryPhraseCount(line));
                        cout << endl;
                  }

            }


            cout << "type yes if you want to go again" << endl;
            string answer;
            getline(cin, answer);
            answer = toLowerCase(answer);
            if(answer == "yes")
            {
                  getQueryMore = true;
                  cout << endl;
                  cout << endl;
            }
            else{
                  getQueryMore = false;
            }


      }

      return 0;
}
