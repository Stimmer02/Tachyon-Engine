#ifndef UIBUILDER_H
#define UIBUILDER_H

#include <functional>
#include <map>
#include "ComponentType.h"
#include "Component.h"
#include "Color.h"
#include "Button.h"
#include "Canvas.h"
#include "EventType.h"
#include "Text.h"
#include "IEventHandlingService.h"
#include "InteractiveComponent.h"
//#include <TextAssembler.h> //TODO

 
class UIBuilder{
private:
	IEventHandlingService* eventManager;
	//TextAssembler textAssembler; //TODO
	ComponentType type;
	float height;
	float width;
	float x;
	float y;
	Color color;
	std::map <EventType, std::function<void()> > events;
	char* text;
	int textLen;
public:
	UIBuilder* AssignEventManager(IEventHandlingService* _eventManager);
	//UIBuilder* AssignTextAssembler(const TextAssembler &_textAssembler);//TODO
	UIBuilder* AssignEvent(const EventType &_event,std::function<void()>);
	UIBuilder* SetComponentType(const ComponentType &_type);
	UIBuilder* SetDimensions(const float &_width, const float &_height);
	UIBuilder* SetPosition(const float &_x, const float &_y);
	UIBuilder* SetColor(const Color &_color);
	UIBuilder* SetText(char* _text, const int &_textLen);
	// UIBuilder* SetText(const std::string &_text); //TODO
	Component* Build();

	//getters added for testing
	IEventHandlingService* getEventManager();
	ComponentType getType();
	float getHeight();
	float getWidth();
	float getX();
	float getY();
	Color getColor();
	std::map <EventType, std::function<void()> > getEvents();
	char* getText();
	int getTextLen();

};

#endif
