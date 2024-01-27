#include "Text.h"
#include "UIBuilder.h"

UIBuilder* UIBuilder::AssignEventManager(IEventHandlingService* _eventManager){
	eventManager = _eventManager;
	return this;
}
// UIBuilder* UIBuilder::AssignTextAssembler(const TextAssembler &_textAssembler){
//	TODO
// }
UIBuilder* UIBuilder::AssignEvent(const EventType &_event, std::function<void()> _delegate){
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

UIBuilder* UIBuilder::SetTexture(const Sprite * _sprite){
	sprite = (Sprite *)_sprite;
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

Component* UIBuilder::Build(){

	Component * component = nullptr;

	if(type == BUTTON){
		Button * b = new Button(x, y, width, height);

		IEventListener * listener = b;

		if( eventManager != nullptr){

			for(auto i = events.begin(); i != events.end(); ++i){
				eventManager->Subscribe(i->first, listener);
				b->AssignEvent(i->first, i->second);
			}

		}

		component = b;
	}
	else if(type == CANVAS){
		Canvas * c = new Canvas(x, y, width, height);

		IEventListener * listener = c;

		if( eventManager != nullptr){

			for(auto i = events.begin(); i != events.end(); ++i){
				eventManager->Subscribe(i->first, listener);
				c->AssignEvent(i->first, i->second);
			}

		}

		component = c;
	}
	else if(type == TEXT){
		component = new Text(x, y, width, height, text, textLen);
	}

	if(sprite != nullptr)
		component->SetTexture(sprite);


	component->SetColor(color);

	events.clear();

	return component;
}


IEventHandlingService* UIBuilder::getEventManager(){
	return eventManager;
}
ComponentType UIBuilder::getType(){
	return type;
}
float UIBuilder::getHeight(){
	return height;
}
float UIBuilder::getWidth(){
	return width;
}
float UIBuilder::getX(){
	return x;
}
float UIBuilder::getY(){
	return y;
}
Color UIBuilder::getColor(){
	return color;
}
std::map <EventType, std::function<void()> > UIBuilder::getEvents(){
	return events;
}
char* UIBuilder::getText(){
	return text;
}
int UIBuilder::getTextLen(){
	return textLen;
}

