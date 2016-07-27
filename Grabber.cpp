#include "Grabber.h"
#include <alvision\alvisiondefinitions.h>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <boost\thread.hpp>
#include <pthread.h>
#include <althread\alcriticalsection.h>


VidModule::VidModule(boost::shared_ptr<AL::ALBroker> broker, const std::string& name) : ALModule(broker, name),
	camProxy(AL::ALVideoDeviceProxy(broker)),
	mProx(AL::ALProxy(broker,"Motionizr"))
{
	setModuleDescription("Xplore NAO Video module");

	functionName("stopVid", getName(), "stop vid capture");
	BIND_METHOD(VidModule::stopVid);
	functionName("onRightBumperPressed", getName(), "Method called when the right bumper is pressed. Makes a LED animation.");
	BIND_METHOD(VidModule::onRightBumperPressed);
}

VidModule::~VidModule()
{
	fMemoryProxy.unsubscribeToEvent("onRightBumperPressed", "Bumper");
}

void VidModule::init()
{
	fMemoryProxy = AL::ALMemoryProxy(getParentBroker());
	fState = fMemoryProxy.getData("RightBumperPressed");
	fMemoryProxy.subscribeToEvent("RightBumperPressed", "VidModule",
                                  "onRightBumperPressed");
	pthread_t jim;
	pthread_create(&jim,0,startVid,this);
}
void * VidModule::startVid(void * pArg)
{
	VidModule* lthis = (VidModule*) pArg;
	const std::string clientName = lthis->camProxy.subscribe("test", AL::kQVGA, AL::kBGRColorSpace, 30);
	cv::Mat imgHeader = cv::Mat(cv::Size(320, 240), CV_8UC3);
	cv::namedWindow("NaoStream");

  while ((char) cv::waitKey(30) != 27)
  {
    
    AL::ALValue img = lthis->camProxy.getImageRemote(clientName);

   
    imgHeader.data = (uchar*) img[6].GetBinary();
	
    lthis->camProxy.releaseImage(clientName);
	
   
    cv::imshow("NaoStream", imgHeader);
  }

  
  lthis->camProxy.unsubscribe(clientName);
  return 0;
}

void VidModule::stopVid()
{
	
}
void VidModule::onRightBumperPressed()
{
	
	AL::ALCriticalSection section(fCallbackMutex);
	fState =  fMemoryProxy.getData("RightBumperPressed");
  if (fState  > 0.5f) {
    return;
  }
  mProx.callVoid("setVibration",true);
  
}
