/*
* Motionizr Module 
* Robert Petre, Joakim Bajoul Kakaei
*
*/
#include "Motionizr.h"
#include <alproxies\alrobotpostureproxy.h>

Motionizr::Motionizr(boost::shared_ptr<AL::ALBroker> broker, const std::string& name):
	ALModule(broker, name),
		mProxy(AL::ALMotionProxy(broker)),
		vibrate(false),
		handMode(false)
{
	setModuleDescription("XploreNao full motion module");

	// Bind methods

	functionName("startMoving", getName(), "Move robot");
	BIND_METHOD(Motionizr::startMoving);

	functionName("stopMoving", getName(), "Stop movement");
	BIND_METHOD(Motionizr::stopMoving);

	functionName("updateMovement", getName(), "update move params");
	BIND_METHOD(Motionizr::updateMovement);

	functionName("moveHead", getName(), "move head to given angle");
	BIND_METHOD(Motionizr::moveHead);

	functionName("resetHead", getName(), "reset head position");
	BIND_METHOD(Motionizr::resetHead);

	functionName("setVibration", getName(), "set vibration bool");
	BIND_METHOD(Motionizr::setVibration);

	functionName("isVibrating", getName(), "get vibration bool");
	BIND_METHOD(Motionizr::isVibrating);

	functionName("isHandMode", getName(), "check if hand is active");
	BIND_METHOD(Motionizr::isHandMode);

	functionName("switchMode", getName(), "switch modes");
	BIND_METHOD(Motionizr::switchMode);

	functionName("moveLeftHand", getName(), "move the left arm");
	BIND_METHOD(Motionizr::moveLeftHand);

	functionName("moveRightHand", getName(), "move the right arm");
	BIND_METHOD(Motionizr::moveRightHand);

	functionName("extendLeft", getName(), "extendLeft");
	BIND_METHOD(Motionizr::extendLeft);

	functionName("contractLeft", getName(), "contract the left elbow");
	BIND_METHOD(Motionizr::contractLeft);

	functionName("extendRight", getName(), "Exend right elbow");
	BIND_METHOD(Motionizr::extendRight);

	functionName("contractRight", getName(), "contract right elbow");
	BIND_METHOD(Motionizr::contractRight);

	functionName("closeHand", getName(), "Close hand");
	BIND_METHOD(Motionizr::contractRight);
}
Motionizr::~Motionizr()
{
	stopMoving();
}

//Init section
void Motionizr::init()
{
	//Reset robot posture on init
	mProxy.stopMove();
	AL::ALRobotPostureProxy ppx(getParentBroker());
	ppx.goToPosture("Stand",0.5f);
	mProxy.moveInit();
	qi::os::sleep(1);
	//Set head stiffness
	mProxy.setStiffnesses(AL::ALValue::array("HeadYaw", "HeadPitch"), AL::ALValue::array(1.0f, 1.0f));
	qi::os::sleep(1);
	std::cout<<"Init done";
	
}

// Custom methods

void Motionizr::startMoving(const float &x, const float &theta)
{
	try
	{
		mProxy.moveToward(x,0.0f,theta);
	}
	catch (AL::ALError e)
	{
		std::cerr<<e.what();
		stopMoving();
	}

}
void Motionizr::stopMoving()
{
	if(mProxy.moveIsActive())
	{
		mProxy.moveToward(0.0f,0.0f,0.0f);
	}
}

void Motionizr::updateMovement()
{
}

void Motionizr::moveHead(const float &yaw, const float &pitch)
{
	vibrate = true;
	AL::ALValue names       = AL::ALValue::array("HeadYaw", "HeadPitch");
	AL::ALValue angles      = AL::ALValue::array(yaw, pitch);
	float fractionMaxSpeed  = 0.2f;
	try 
	{
		mProxy.changeAngles(names, angles, fractionMaxSpeed);
	}
	catch(AL::ALError e)
	{
		std::cerr<<e.what();
	}
}

void Motionizr::resetHead()
{
	 AL::ALValue names       = AL::ALValue::array("HeadYaw", "HeadPitch");
	 if(mProxy.getAngles(names,false).at(0)!=0 || mProxy.getAngles(names,false).at(1)!=0) 
	 {
		 vibrate = false;
		 AL::ALValue angles      = AL::ALValue::array(0.0f, 0.0f);
		float fractionMaxSpeed  = 0.25f;
		try
		{
			mProxy.setAngles(names, angles, fractionMaxSpeed);
		 }
		 catch(AL::ALError e)
		{
			 std::cerr<<e.what();
		}
	 }
}

bool Motionizr::isVibrating()
{
	return vibrate;
}

void Motionizr::setVibration(const bool &value)
{
	vibrate = value;
}

bool Motionizr::isHandMode()
{
	return handMode;
}
int Motionizr::switchMode()
{
	if (handMode) handMode = false;
	else handMode = true;
	return 0;
}

void Motionizr::moveLeftHand(const float &pitch, const float &roll){

		AL::ALValue names       = AL::ALValue::array("LShoulderPitch", "LShoulderRoll"); 
		AL::ALValue angles      = AL::ALValue::array(pitch, roll); // roll:-1.5446 to -0.0349      yaw: -2.0857 to 2.0857

	float fractionMaxSpeed  = 0.2f;
	try 
	{
		mProxy.changeAngles(names, angles, fractionMaxSpeed);
	}
	catch(AL::ALError e)
	{
		std::cerr<<e.what();
	}
	}

void Motionizr::moveRightHand(const float &pitch, const float &roll){

		AL::ALValue names       = AL::ALValue::array("RShoulderPitch", "RShoulderRoll"); 
		AL::ALValue angles      = AL::ALValue::array(pitch, roll); // roll:-1.3265 to 0.3142      yaw: -2.0857 to 2.0857

	float fractionMaxSpeed  = 0.2f;
	try 
	{
		mProxy.changeAngles(names, angles, fractionMaxSpeed);
	}
	catch(AL::ALError e)
	{
		std::cerr<<e.what();
	}
	}

 void Motionizr::extendLeft(){
		const float elbow = 0.2f;
		AL::ALValue names       = AL::ALValue::array("LElbowRoll"); 
		AL::ALValue angles      = AL::ALValue::array(elbow); // roll:-1.3265 to 0.3142      yaw: -2.0857 to 2.0857

	float fractionMaxSpeed  = 0.2f;
	try 
	{
		mProxy.changeAngles(names, angles, fractionMaxSpeed);
	}
	catch(AL::ALError e)
	{
		std::cerr<<e.what();
	}
}
 void Motionizr::contractLeft(){
		const float elbow    = -0.2f;
		AL::ALValue names       = AL::ALValue::array("LElbowRoll"); 
		AL::ALValue angles      = AL::ALValue::array(elbow); // roll:-1.3265 to 0.3142      yaw: -2.0857 to 2.0857

	float fractionMaxSpeed  = 0.2f;
	try 
	{
		mProxy.changeAngles(names, angles, fractionMaxSpeed);
	}
	catch(AL::ALError e)
	{
		std::cerr<<e.what();
	}

}

 void Motionizr::extendRight(){
		const float elbow = 0.2f;
		AL::ALValue names       = AL::ALValue::array("RElbowRoll"); 
		AL::ALValue angles      = AL::ALValue::array(elbow); // roll:-1.3265 to 0.3142      yaw: -2.0857 to 2.0857

	float fractionMaxSpeed  = 0.2f;
	try 
	{
		mProxy.changeAngles(names, angles, fractionMaxSpeed);
	}
	catch(AL::ALError e)
	{
		std::cerr<<e.what();
	}
}

 void Motionizr::contractRight(){
		const float elbow    = -0.2f;
		AL::ALValue names       = AL::ALValue::array("RElbowRoll"); 
		AL::ALValue angles      = AL::ALValue::array(elbow); // roll:-1.3265 to 0.3142      yaw: -2.0857 to 2.0857

	float fractionMaxSpeed  = 0.2f;
	try 
	{
		mProxy.changeAngles(names, angles, fractionMaxSpeed);
	}
	catch(AL::ALError e)
	{
		std::cerr<<e.what();
	}

}
 void Motionizr::closeHand(const std::string& handName){
	mProxy.closeHand(handName);
	}