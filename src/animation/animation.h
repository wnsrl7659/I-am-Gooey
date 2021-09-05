/******************************************************************************/
/*!
\file   animation.h
\author Whitcomb Riley
\par    Email: whitcomb.r\@digipen.edu
\par    Course: Game 200
\brief  header for the animation component

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once
#include <GL\glew.h>
#include "../component/component.h"

class Animation : public Component<Animation>
{
public:
		COMPONENT_NAME(Animation);
		Animation();
		void Init(int rowCount = 1, int colCount = 1, int animCount = 0, int animLength = 0, float speed = 1.0f);
		void Update(float dt);
		void Draw() const;
		const int* operator[](unsigned i) const;
		float GetFrameTimer(void);
		float GetAnimationSpeed(void);
		int GetRowCount(void);
		int GetColCount(void);
		int* GetAnimationSequence(void);
		int* operator[](unsigned i);
		void setSequence(int sequenceNumber);
		void setAnimationSpeed(float secondsBetweenFrame);
		void advanceFrame();
		int getSequenceIndex();
		void clearSequences();
		void setUniforms();
		void print();
		int GetAnimationCount();
		int GetAnimationLength();
		~Animation();
private:

		int* sequences;      //!< Array of texture indexes
		int animationCount;  //!< Number of animations
		int animationLength; //!< Length of each animation
		int rows;            //!< Number of rows in the texture
		int cols;            //!< Number of cols in the texture
		int index;           //!< The index into sequences
		float frameTimer;    //!< Current time before animating (sec)
		float animSpeed;     //!< The time to set back to, after animating (sec)
};
