#include "Text.h"
#include "UIBuilder.h"
	
UIBuilder* UIBuilder::AssignEventManager(IEventHandlingService* _eventManager){
	eventManager = _eventManager;
	return this;
}
// UIBuilder* UIBuilder::AssignTextAssembler(const TextAssembler &_textAssembler){
//	TODO
// }
UIBuilder* UIBuilder::AssignEvent(const EventType &_event,std::function<void()> _delegate){
	/*
	if(type != Button || type != Canvas){
		std::cout<<"Error: it's possible to assing event only to interactive componnents\n";
		//czy tutaj powinien być taki komunikat? czy to jest potrzebne?
		//jak to rozwiącać?
		return this; 
	}
	*/
	events[_event] = _delegate;
	return this;
}
UIBuilder* UIBuilder::SetComponentType(const ComponentType &_type){
	type = _type;
	return this;
}
UIBuilder* UIBuilder::SetDimensions(const float &_width, const float &_height){
	width = _width;
	height = _height;
	return this;
}
UIBuilder* UIBuilder::SetPosition(const float &_x, const float &_y){
	x = _x;
	y = _y;
	return this;
}
UIBuilder* UIBuilder::SetColor(const Color &_color){
	color = _color;
	return this;
}
UIBuilder* UIBuilder::SetText(char* _text, const int &_textLen){
	text = _text;
	textLen = _textLen;
	return this;
}
// UIBuilder* UIBuilder::SetText(const std::string &_text){
//	TODO
// }
Component* UIBuilder::Build(){
	
	Button* newButtonComponent;
	Canvas* newCanvasComponent;
	Text* textComponent;
	if(type == BUTTON){
		newButtonComponent = new Button(x, y, width, height);
		//newButtonComponent->SetEventManager(eventManager);
		//there is no SetEventManager methot in InteractiveComponent
		for(auto i = events.begin(); i != events.end(); ++i){
			newButtonComponent->AssignEvent(i->first, i->second);
		}
		newButtonComponent->SetColor(color);
		return newButtonComponent;
	}
	else if(type == CANVAS){
		newCanvasComponent = new Canvas(x, y, width, height);
		newCanvasComponent->SetEventManager(eventManager);
		//there is no SetEventManager methot in InteractiveComponent
		for(auto i = events.begin(); i != events.end(); ++i){
			newCanvasComponent->AssignEvent(i->first, i->second);
		}
		newCanvasComponent->SetColor(color);
		return newCanvasComponent;
	}
	else if(type == TEXT){
		textComponent = new Text(x, y, width, height, text, textLen);
		textComponent->SetColor(color);
		return textComponent;
	}
	else{
		///czy tu też dodać komunikat że typ jest błędny?
		return NULL;
	}
}
