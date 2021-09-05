/******************************************************************************/
/*!
\file   animation.cpp
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200
\brief  Implementation for animations

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "animation.h"
#include "../graphics/Shader.h"
#include <iostream>
#include "../graphics/DebugWindow.h"
#include "../settings/settings.h"

/*****************************************************************/
/*!
		\brief
				Default constructor
*/
/*****************************************************************/
Animation::Animation()
{
		animationCount = 0;//Default which prevent the delete call in Destructor
}

/*****************************************************************/
/*!
		\brief
				Initializes the animation according to the parameters

		\param rowCount
				The amount of rows to divide the texture by

		\param colCount
				The amount of cols to divide the texture by

		\param animCount
				The amount of seperate animations (aka sequences)

		\param animLength
				The amount of frames per animation (same value for each sequence)

		\param speed
				Determines how long the animation will last (in seconds)
*/
/*****************************************************************/
void Animation::Init(int rowCount, int colCount, int animCount, int animLength, float speed)
{
		index = 0;
		animationLength = animLength + 1;
		animationCount = animCount;
		rows = rowCount;
		cols = colCount;
		animSpeed = speed;
		frameTimer = 0;

		if (animationCount * animationLength)
				sequences = new int[animationCount * animationLength];

		if (!sequences)
		{
				if(Settings::debug)
						printf("crap");
		}

		clearSequences();
}

/*****************************************************************/
/*!
		\brief
				Subtracts the time elapsed from the animation timer, and
				advances a frame if the timer reaches 0.

		\param dt
				time elapsed, in seconds. expected to be .016 or something
*/
/*****************************************************************/
void Animation::Update(float dt)
{
	frameTimer -= dt;
	if (frameTimer <= 0)
	{
		frameTimer = animSpeed;
		advanceFrame();
	}
}

/*****************************************************************/
/*!
		\brief
				Gets the current frame time
		\return
				the frame timer
*/
/*****************************************************************/
float Animation::GetFrameTimer(void)
{
	return frameTimer;
}

/*****************************************************************/
/*!
		\brief
				Gets the row count (the one that divides the texture)
		\return
				the rows
*/
/*****************************************************************/
int Animation::GetRowCount(void)
{
	return rows;
}

/*****************************************************************/
/*!
		\brief
				Gets the column count (the one that divides the texture)
		\return
				the columns
*/
/*****************************************************************/
int Animation::GetColCount(void)
{
	return cols;
}

/*****************************************************************/
/*!
		\brief
				Gets the amount of animation sequences
		\return
				the animation count
*/
/*****************************************************************/
int Animation::GetAnimationCount(void)
{
	return animationCount;
}

/*****************************************************************/
/*!
		\brief
				Gets the length of all sequences
		\return
				the animation length
*/
/*****************************************************************/
int Animation::GetAnimationLength(void)
{
	return animationLength;
}

/*****************************************************************/
/*!
		\brief
				Gets the array of sequences
		\return
				the array of sequences
*/
/*****************************************************************/
int* Animation::GetAnimationSequence(void)
{
	return sequences;
}

/*****************************************************************/
/*!
		\brief
				Gets the animation speed
		\return
				the animation speed 
*/
/*****************************************************************/
float Animation::GetAnimationSpeed(void)
{
	return animSpeed;
}

/*****************************************************************/
/*!
		\brief
				draw from component class
*/
/*****************************************************************/
void Animation::Draw() const
{
		
}

/*****************************************************************/
/*!
		\brief
				sets the animation speed
		\param secondsBetweenFrame
				The seconds between frames..
*/
/*****************************************************************/
void Animation::setAnimationSpeed(float secondsBetweenFrame)
{
	animSpeed = secondsBetweenFrame;
	//frameTimer = animSpeed;
}


/*****************************************************************/
/*!
		\brief
				advances the frame by one, and loops if it reaches an
				invalid frame index (-1).
*/
/*****************************************************************/
void Animation::advanceFrame()
{
		index++;
		if (index > animationCount * animationLength)
				index = 0;
		if (sequences[index] == -1)
		{
				//Reset Row
				int currentRow = index / animationLength;
				index = animationLength * currentRow;
		}
		//std::cout << (int)sequences[index] << std::endl;
}

/*****************************************************************/
/*!
		\brief
				Sets all the data to -1. Part of construction.
*/
/*****************************************************************/
void Animation::clearSequences()
{
		for (int i = 0; i < animationCount * animationLength; i++)
		{
				sequences[i] = -1;
		}
}

/*****************************************************************/
/*!
		\brief
				Given a shader program, sets certain parameters for the
				vertex shader, so that the uv coordinates can break
				up the image and select an index.

		\param shaderProgram
				The shader ID
*/
/*****************************************************************/
void Animation::setUniforms()
{
		/**/
	if (owner)
	{
		Sprite* Spr = owner->GetComponent<Sprite>();
		if (Spr)
		{
			int shaderIndex = Spr->getShaderIndex();
			if (shaderIndex >= 0 && shaderIndex < Shader_Last)
			{
				//shaderIndex = Shader_AnimatedTexture;

				GLint loca = location[shaderIndex].aIndex;
				if (loca != -1)
					glUniform1i(loca, (int)sequences[index]);

				loca = location[shaderIndex].aRow;
				if (loca != -1)
					glUniform1i(loca, rows);

				GLint test = glGetUniformLocation(ShaderArray[shaderIndex], "aColums200");

				loca = location[shaderIndex].aColums;
				if (loca != -1)
					glUniform1i(loca, cols);

			}

		}
		/**/
		/*
		Sprite* Spr = owner->GetComponent<Sprite>();
		if (Spr)
		{
				if (Spr->GetShader() == ShaderArray[Shader_AnimatedTexture])
				{
						GLint loca = location[Shader_AnimatedTexture].aIndex;
						if (loca != -1)
								glUniform1i(loca, (int)sequences[index]);

						loca = location[index].aRow;
						if (loca != -1)
								glUniform1i(loca, rows);

						GLint test = glGetUniformLocation(ShaderArray[index], "aColums200");

						loca = location[index].aColums;
						if (loca != -1)
								glUniform1i(loca, cols);
				}
		}
		/**/
	}
}

/*****************************************************************/
/*!
		\brief
				Prints out the sequence information to the console.
				For debugging purposes.
*/
/*****************************************************************/
void Animation::print()
{		
		//if (Settings::debug)
		//{
		//		std::cout << std::endl << std::endl << "ANIMATION SEQUENCE" << std::endl;
		//		for (int i = 0; i < animationCount * animationLength; i++)
		//		{
		//				if (i % animationLength == 0)
		//						std::cout << std::endl;


		//				std::cout << (int)sequences[i] << " ";
		//		}
		//}
}

/*****************************************************************/
/*!
		\brief
				Sets which animation sequence to use, and resets to the
				start of the sequence. Does not do anything if the 
				sequence is already being used (avoid constantly resetting
				and not animating).

		\param sequenceNumber
				The sequence to use (ranges from 0 to the animationCount)
*/
/*****************************************************************/
void Animation::setSequence(int sequenceNumber)
{
	if (sequenceNumber != index / animationLength)
		index = sequenceNumber * animationLength;
}

/*****************************************************************/
/*!
		\brief
				Gets the texture index from the sequence
*/
/*****************************************************************/
int Animation::getSequenceIndex()
{
		if(animationCount)
				return sequences[index];
		return 0;
}

/*****************************************************************/
/*!
		\brief
				Returns the texture index from the sequence array
		\param i
				the index to use in the sequence array
		\return
				returns the texture index
*/
/*****************************************************************/
const int* Animation::operator[](unsigned i) const
{
	if(i >= 0)
		if (animationCount)
			if(i < (unsigned)animationCount)
				return &sequences[i * animationLength];
}

/*****************************************************************/
/*!
		\brief
				Returns the texture index from the sequence array
		\param i
				the index to use in the sequence array
		\return
				returns the texture index
*/
/*****************************************************************/
int* Animation::operator[](unsigned i)
{
	if(i >= 0)
		if (animationCount)
			if(i < (unsigned)animationCount)
				return &sequences[i * animationLength];
	return 0;
}

/*****************************************************************/
/*!
		\brief
				The destructor. Frees the allocated sequence array.
*/
/*****************************************************************/
Animation::~Animation()
{
		if (animationCount * animationLength)
				delete[] sequences;
}

