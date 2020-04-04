#include "GameObject.h"
#include <iostream>
using namespace Engine;
using namespace Math;
using std::cin;
using std::cout;
GameObject::GameObject(const Point2D & i_Position): position(i_Position)
{
	inputCharacter = NULL;
}
GameObject::~GameObject()
{
	free(this->TempName);

}
void GameObject::getName()
{
	do
	{
		inputCharacter = cin.get();
		if (inputCharacter != '\n')
		{
			count++;
			TempName = (char*)realloc(Name, count * sizeof(char));
			if (TempName != nullptr)
			{

				TempName[count - 1] = inputCharacter;
				Name = TempName;
			}
			else {
				free(Name);
				free(TempName);
				exit(1);
			}
		}

	} while (inputCharacter != '\n');
}

void GameObject::print()
{
	for (int i = 0; i < count; i++)
	{
		cout << Name[i];
	}
}
void GameObject::getPositions()
{

	print();
	cout << " is at "<<position<< std::endl;
}
void GameObject::move(const Engine::Math::Point2D & i_Position)
{
	position += i_Position;
}