/**
 * Name: Kevyn Higbee, Preston Roberts
 * 
 * Date: April 23, 2019
 * Assignment: Program 4
 * File: PBuffer Implementation File
 **/

#include "PBuffer.h"

/**
 * Class Constructor - initializes variables
 */
PBuffer::PBuffer()
{
	front = 0;
	rear = -1;
	count = 0;
}

/**
 * isFull - checks if the buffer is full
 * 			@return true : when count >= BUFFER_SIZE
 * 					false : when count < BUFFER_SIZE
 */
bool PBuffer::isFull()
{
	return (count >= BUFFER_SIZE);
}

/**
 * isEmpty - checks if the buffer is empty
 * 			@return true : when count <= 0
 * 					false : when count > 0
 */
bool PBuffer::isEmpty()
{
	return (count <= 0);
}

/**
 * insertItem - inserts an item into the buffer
 * 			@return 0 : successful insertion
 * 					-1 : overflow
 */
int PBuffer::insertItem(bItm itm)
{
	if(isFull())
		return -1;
	else
	{
		count++;
		rear = (rear+1) % BUFFER_SIZE;
		buffer[rear] = itm;

		cout << "Item " << itm << " inserted by producer. ";
		display();

		return 0;
	}
}

/**
 * removeItem - removes front of buffer, sets referenced variable
 * 			@return 0 : successful removal
 * 					-1 : underflow
 */
int PBuffer::removeItem(bItm *itm)
{
	if(isEmpty())
		return -1;
	else
	{
		count--;
		*itm = buffer[front];
		front = (front+1) % BUFFER_SIZE;

		cout << "Item " << *itm << " removed by consumer. ";
		display();

		return 0;
	}
}

/**
 * display - prints the content of the buffer
 */
void PBuffer::display()
{
	cout << "Buffer Content: [ ";
	
	if( isEmpty() )
		cout << "empty ";
	else
	{
		int n = front;

		for(int i = 0; i < count; i++)
		{
			n = (front+i) % BUFFER_SIZE;
			cout << buffer[n] << " ";
		}
	}

	cout << "]" << endl;
}