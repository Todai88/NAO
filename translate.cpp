/**
translate.cpp
Written by: Joakim Bajoul Kakaei
Group ViP
**/

#include "translate.h"
#include <alvalue/alvalue.h>
#include <alcommon/alproxy.h>
#include <alcommon/albroker.h>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <string>
#include <alproxies\alspeechrecognitionproxy.h>
using namespace std;
translate::translate(
  boost::shared_ptr<AL::ALBroker> broker,
  const std::string& name) : AL::ALModule(broker, name)
{
  /// This line will display a description of this module on the web page of the robot
  setModuleDescription("This module launch a text to speech command when a "
                       "specific voice command is heard. To start "
                       "this module, call its startRecognition() method in Choregraphe.");

  /// Class functions that need to be bound to be called in Python or by remote processes.
  functionName("startRecognition",
               getName(),
               "This function will launch the ASR engine.");
  BIND_METHOD(translate::startRecognition);

  functionName("stopRecognition",
               getName(),
               "This function will stop the ASR engine.");
  BIND_METHOD(translate::stopRecognition);

  functionName("onSpeechRecognized",
               getName(),
               "Called by ALMemory when a word is recognized.");
  BIND_METHOD(translate::onSpeechRecognized);

  /// Specify a command to which the robot will react.
  
}

translate::~translate()
{
	std::cout<<"Deconstructed!";
	stopRecognition();
	
}
/*
Initiate the code.
*/
void translate::init(){
	std::cout<<"init started!\n";
	std::vector<std::pair<string, float>> test;
	test.push_back(std::make_pair<string, float>("English", 100.0F));
	try{
		read();
		std::cout<<"file read!\n";
		startRecognition(); 
	}
	catch (AL::ALError e)
	{
		
		std::cout << "Broker failed" << std::endl << e.what();
	}
	
		
}
/*
Read the textfile line by line while using '-' as a delimiter and putting the substrings as individual elements in an array.
Then puts all elements in a vector.
*/
void translate::read(){
	ifstream inLine("translate.txt");
	std::string line;
	int i = 0;
	std::string word;
	std::string word2;
	if (inLine.is_open()){
		 while (inLine.good())
								{
		getline(inLine, line);
		word = "";
		word2 = "";
		std::stringstream iss (line);
		getline (iss, word, '-');
		str[i][0] = word;
		getline (iss, word2, '-');		
		str[i][1] = word2;
			
		i++;
		
								
	}std::cout<<str[0][0];
    inLine.close();
  }
	for(int i=0;i<=2;i++){
		for(int j=0;j<=1;j++) {
			words.push_back(str[i][j]);
			std::cout<<"["<<i<<"]["<<j<<"]"<<str[i][j]<<std::endl;			
		}
	}

}
	
/*
Sets up two proxies to ALSpeechRecognition. One to bind all it's methods to fSpeechRecognition.
The second is used to set up the earspeed and whether or not the filter will be used.
Also sets up a proxy to texttospeech to be used later.
*/
void translate::startRecognition()
{
	
	std::cout <<"Started recognition";
  /// Setting up a proxy to ALTextToSpeech
  fTextToSpeech = getParentBroker()->getProxy("ALTextToSpeech");
  /// Setting the language to "English"
  fTextToSpeech->callVoid("setLanguage",std::string("English"));

  /// Setting up a proxy to ALMemory
  fMemory = getParentBroker()->getMemoryProxy();
  /// Setting up a proxy to ALSpeechRecognition
  fSpeechRecognition = getParentBroker()->getProxy("ALSpeechRecognition");
  /// Setting the working language of speech recognition engine
  fSpeechRecognition->callVoid("setLanguage",std::string("English"));
  
  
  /// Setting the word list that should be recognized
 
  AL::ALSpeechRecognitionProxy(asr);

  /// Set earspeed to the slowest and most accurate.
  asr.setParameter("EarSpeed", 0.0);

  std::cout<<asr.getParameter("EarSpeed");

  /// Use the filter to make sure HI-DEF input is used.
  asr.setParameter("EarUseFilter", 1);
  std::cout<<asr.getParameter("EarUseFilter");
  /// Set the vector as the wordlist to be used. 

  fSpeechRecognition->callVoid("setWordListAsVocabulary", words);
  std::cout<<"Wordlist set!";
  /// Launching the speech recognition engine by subscribing to a ALMemory key
  /// that is fed by ALSpeechRecognition. Note that a callback function is specified.
  fMemory->subscribeToEvent("WordRecognized",getName(),"onSpeechRecognized");
}

void translate::stopRecognition()
{
	std::cout<<"Stopped recognition";
  /// Stopping the speech recognition engine by unsubscribing to the ALMemory key
  fMemory->unsubscribeToEvent("WordRecognized",getName());
}


/*
Will be called everytime a word is recognized.
*/
void translate::onSpeechRecognized(const std::string& name,
                                             const AL::ALValue& val,
                                             const std::string& myName)
{
	std::cout<<val;
	std::cout<<"Loop something is happening...!";
  /// Parse the list of words that has been recognized by ALSpeechRecognition
  for(unsigned int i = 0; i < val.getSize()/2 ; ++i)
  {
	std::cout<<"Inside the loop";
    std::cout << "word recognized: " << val[i*2].toString()
              << " with confidence: " << (float)val[i*2+1] << std::endl;

    /// If our "command" has been recognized, start a speech synthesis reaction
    if(wordExists((std::string)val[i*2]) && (float)val[i*2+1] > 0.15)
    {
		std::string translation = translateWord((std::string)val[i*2]);
      fTextToSpeech->callVoid("say",std::string("The translation of" + (std::string)val[i*2] + "is " + translateWord((std::string)val[i*2])));
    }
  }
}
/*
Checks through the vector to see if any instance of the word exist.
*/
bool translate::wordExists(std::string s)
{
	std::cout<<"Checking the vector!";
	return(std::find(words.begin(), words.end(), s) != words.end());
}
/*
Since the word exists, the word will be sent to the array to find its' corresponding translation.
*/
std::string translate::translateWord(std::string word)
{
	std::cout<<"Looking through the array!";
	for(int i=0;i<=2;i++){
		for(int j=0;j<=1;j++){
			if (word == str[i][j]) {
				if (j = 0) {
				return str[i][1];	
				}
					if (j = 1) {
						return str[i][0];
						}
			}
	}
}
}